/*!
 * \file      RegionIN865.c
 *
 * \brief     Region implementation for IN865
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 */
/**
 ******************************************************************************
 *
 *          Portions COPYRIGHT 2020 STMicroelectronics
 *
 * @file    RegionIN865.c
 * @author  MCD Application Team
 * @brief   Region implementation for IN865
 ******************************************************************************
 */
#include "RegionIN865.h"
#include "radio.h"

// Definitions
#define CHANNELS_MASK_SIZE 1

#if defined(REGION_IN865)
/*
 * Non-volatile module context.
 */
#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
static RegionNvmDataGroup1_t *RegionNvmGroup1;
static RegionNvmDataGroup2_t *RegionNvmGroup2;
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
// static RegionNvmDataGroup1_t* RegionNvmGroup1; /* Unused for this region */
static RegionNvmDataGroup2_t *RegionNvmGroup2;
static Band_t *RegionBands;
#endif /* REGION_VERSION */

static bool VerifyRfFreq(uint32_t freq) {
  // Check radio driver support
  if (Radio.CheckRfFrequency(freq) == false) {
    return false;
  }

  if ((freq < 865000000) || (freq > 867000000)) {
    return false;
  }
  return true;
}

static TimerTime_t GetTimeOnAir(int8_t datarate, uint16_t pktLen) {
  int8_t phyDr = DataratesIN865[datarate];
  uint32_t bandwidth = RegionCommonGetBandwidth(datarate, BandwidthsIN865);
  TimerTime_t timeOnAir = 0;

  if (datarate == DR_7) { // High Speed FSK channel
    timeOnAir = Radio.TimeOnAir(MODEM_FSK, bandwidth, phyDr * 1000, 0, 5, false,
                                pktLen, true);
  } else {
    timeOnAir = Radio.TimeOnAir(MODEM_LORA, bandwidth, phyDr, 1, 8, false,
                                pktLen, true);
  }
  return timeOnAir;
}
#endif /* REGION_IN865 */

PhyParam_t RegionIN865GetPhyParam(GetPhyParams_t *getPhy) {
  PhyParam_t phyParam = {0};

#if defined(REGION_IN865)
  switch (getPhy->Attribute) {
  case PHY_MIN_RX_DR: {
    phyParam.Value = IN865_RX_MIN_DATARATE;
    break;
  }
  case PHY_MIN_TX_DR: {
    phyParam.Value = IN865_TX_MIN_DATARATE;
    break;
  }
  case PHY_DEF_TX_DR: {
    phyParam.Value = IN865_DEFAULT_DATARATE;
    break;
  }
  case PHY_NEXT_LOWER_TX_DR: {
    RegionCommonGetNextLowerTxDrParams_t nextLowerTxDrParams = {
        .CurrentDr = getPhy->Datarate,
        .MaxDr = (int8_t)IN865_TX_MAX_DATARATE,
        .MinDr = (int8_t)IN865_TX_MIN_DATARATE,
        .NbChannels = IN865_MAX_NB_CHANNELS,
        .ChannelsMask = RegionNvmGroup2->ChannelsMask,
        .Channels = RegionNvmGroup2->Channels,
    };
    phyParam.Value = RegionCommonGetNextLowerTxDr(&nextLowerTxDrParams);
    break;
  }
  case PHY_MAX_TX_POWER: {
    phyParam.Value = IN865_MAX_TX_POWER;
    break;
  }
  case PHY_DEF_TX_POWER: {
    phyParam.Value = IN865_DEFAULT_TX_POWER;
    break;
  }
  case PHY_DEF_ADR_ACK_LIMIT: {
    phyParam.Value = REGION_COMMON_DEFAULT_ADR_ACK_LIMIT;
    break;
  }
  case PHY_DEF_ADR_ACK_DELAY: {
    phyParam.Value = REGION_COMMON_DEFAULT_ADR_ACK_DELAY;
    break;
  }
  case PHY_MAX_PAYLOAD: {
    phyParam.Value = MaxPayloadOfDatarateIN865[getPhy->Datarate];
    break;
  }
  case PHY_MAX_PAYLOAD_REPEATER: {
    phyParam.Value = MaxPayloadOfDatarateRepeaterIN865[getPhy->Datarate];
    break;
  }
  case PHY_DUTY_CYCLE: {
    phyParam.Value = IN865_DUTY_CYCLE_ENABLED;
    break;
  }
  case PHY_MAX_RX_WINDOW: {
    phyParam.Value = IN865_MAX_RX_WINDOW;
    break;
  }
  case PHY_RECEIVE_DELAY1: {
    phyParam.Value = REGION_COMMON_DEFAULT_RECEIVE_DELAY1;
    break;
  }
  case PHY_RECEIVE_DELAY2: {
    phyParam.Value = REGION_COMMON_DEFAULT_RECEIVE_DELAY2;
    break;
  }
  case PHY_JOIN_ACCEPT_DELAY1: {
    phyParam.Value = REGION_COMMON_DEFAULT_JOIN_ACCEPT_DELAY1;
    break;
  }
  case PHY_JOIN_ACCEPT_DELAY2: {
    phyParam.Value = REGION_COMMON_DEFAULT_JOIN_ACCEPT_DELAY2;
    break;
  }
#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
  case PHY_MAX_FCNT_GAP: {
    phyParam.Value = REGION_COMMON_DEFAULT_MAX_FCNT_GAP;
    break;
  }
  case PHY_ACK_TIMEOUT: {
    phyParam.Value = (REGION_COMMON_DEFAULT_ACK_TIMEOUT +
                      randr(-REGION_COMMON_DEFAULT_ACK_TIMEOUT_RND,
                            REGION_COMMON_DEFAULT_ACK_TIMEOUT_RND));
    break;
  }
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
  case PHY_RETRANSMIT_TIMEOUT: {
    phyParam.Value = (REGION_COMMON_DEFAULT_RETRANSMIT_TIMEOUT +
                      randr(-REGION_COMMON_DEFAULT_RETRANSMIT_TIMEOUT_RND,
                            REGION_COMMON_DEFAULT_RETRANSMIT_TIMEOUT_RND));
    break;
  }
#endif /* REGION_VERSION */
  case PHY_DEF_DR1_OFFSET: {
    phyParam.Value = REGION_COMMON_DEFAULT_RX1_DR_OFFSET;
    break;
  }
  case PHY_DEF_RX2_FREQUENCY: {
    phyParam.Value = IN865_RX_WND_2_FREQ;
    break;
  }
  case PHY_DEF_RX2_DR: {
    phyParam.Value = IN865_RX_WND_2_DR;
    break;
  }
  case PHY_CHANNELS_MASK: {
    phyParam.ChannelsMask = RegionNvmGroup2->ChannelsMask;
    break;
  }
  case PHY_CHANNELS_DEFAULT_MASK: {
    phyParam.ChannelsMask = RegionNvmGroup2->ChannelsDefaultMask;
    break;
  }
  case PHY_MAX_NB_CHANNELS: {
    phyParam.Value = IN865_MAX_NB_CHANNELS;
    break;
  }
  case PHY_CHANNELS: {
    phyParam.Channels = RegionNvmGroup2->Channels;
    break;
  }
  case PHY_DEF_UPLINK_DWELL_TIME: {
    phyParam.Value = IN865_DEFAULT_UPLINK_DWELL_TIME;
    break;
  }
  case PHY_DEF_DOWNLINK_DWELL_TIME: {
    phyParam.Value = REGION_COMMON_DEFAULT_DOWNLINK_DWELL_TIME;
    break;
  }
  case PHY_DEF_MAX_EIRP: {
    phyParam.fValue = IN865_DEFAULT_MAX_EIRP;
    break;
  }
  case PHY_DEF_ANTENNA_GAIN: {
    phyParam.fValue = IN865_DEFAULT_ANTENNA_GAIN;
    break;
  }
  case PHY_BEACON_CHANNEL_FREQ: {
    phyParam.Value = IN865_BEACON_CHANNEL_FREQ;
    break;
  }
  case PHY_BEACON_FORMAT: {
    phyParam.BeaconFormat.BeaconSize = IN865_BEACON_SIZE;
    phyParam.BeaconFormat.Rfu1Size = IN865_RFU1_SIZE;
    phyParam.BeaconFormat.Rfu2Size = IN865_RFU2_SIZE;
    break;
  }
  case PHY_BEACON_CHANNEL_DR: {
    phyParam.Value = IN865_BEACON_CHANNEL_DR;
    break;
  }
  case PHY_PING_SLOT_CHANNEL_FREQ: {
    phyParam.Value = IN865_PING_SLOT_CHANNEL_FREQ;
    break;
  }
  case PHY_PING_SLOT_CHANNEL_DR: {
    phyParam.Value = IN865_PING_SLOT_CHANNEL_DR;
    break;
  }
  case PHY_SF_FROM_DR: {
    phyParam.Value = DataratesIN865[getPhy->Datarate];
    break;
  }
  case PHY_BW_FROM_DR: {
    phyParam.Value =
        RegionCommonGetBandwidth(getPhy->Datarate, BandwidthsIN865);
    break;
  }
  default: {
    break;
  }
  }

#endif /* REGION_IN865 */
  return phyParam;
}

void RegionIN865SetBandTxDone(SetBandTxDoneParams_t *txDone) {
#if defined(REGION_IN865)
#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
  RegionCommonSetBandTxDone(
      &RegionNvmGroup1->Bands[RegionNvmGroup2->Channels[txDone->Channel].Band],
      txDone->LastTxAirTime, txDone->Joined, txDone->ElapsedTimeSinceStartUp);
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
  RegionCommonSetBandTxDone(
      &RegionBands[RegionNvmGroup2->Channels[txDone->Channel].Band],
      txDone->LastTxAirTime, txDone->Joined, txDone->ElapsedTimeSinceStartUp);
#endif /* REGION_VERSION */
#endif /* REGION_IN865 */
}

void RegionIN865InitDefaults(InitDefaultsParams_t *params) {
#if defined(REGION_IN865)
  Band_t bands[IN865_MAX_NB_BANDS] = {IN865_BAND0};

  switch (params->Type) {
  case INIT_TYPE_DEFAULTS: {
    if ((params->NvmGroup1 == NULL) || (params->NvmGroup2 == NULL)) {
      return;
    }

#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
    RegionNvmGroup1 = (RegionNvmDataGroup1_t *)params->NvmGroup1;
    RegionNvmGroup2 = (RegionNvmDataGroup2_t *)params->NvmGroup2;

    // Initialize bands
    memcpy1((uint8_t *)RegionNvmGroup1->Bands, (uint8_t *)bands,
            sizeof(Band_t) * IN865_MAX_NB_BANDS);
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
    RegionNvmGroup2 = (RegionNvmDataGroup2_t *)params->NvmGroup2;
    RegionBands = (Band_t *)params->Bands;

    // Initialize bands
    memcpy1((uint8_t *)RegionBands, (uint8_t *)bands,
            sizeof(Band_t) * IN865_MAX_NB_BANDS);
#endif /* REGION_VERSION */

    // Default channels
    RegionNvmGroup2->Channels[0] = (ChannelParams_t)IN865_LC1;
    RegionNvmGroup2->Channels[1] = (ChannelParams_t)IN865_LC2;
    RegionNvmGroup2->Channels[2] = (ChannelParams_t)IN865_LC3;

    // Initialize the channels default mask
    RegionNvmGroup2->ChannelsDefaultMask[0] = LC(1) + LC(2) + LC(3);

    // Default ChannelsMask
    RegionCommonChanMaskCopy(RegionNvmGroup2->ChannelsMask,
                             RegionNvmGroup2->ChannelsDefaultMask,
                             CHANNELS_MASK_SIZE);
    break;
  }
  case INIT_TYPE_RESET_TO_DEFAULT_CHANNELS: {
    // Reset Channels Rx1Frequency to default 0
    RegionNvmGroup2->Channels[0].Rx1Frequency = 0;
    RegionNvmGroup2->Channels[1].Rx1Frequency = 0;
    RegionNvmGroup2->Channels[2].Rx1Frequency = 0;
    // Default ChannelsMask
    RegionCommonChanMaskCopy(RegionNvmGroup2->ChannelsMask,
                             RegionNvmGroup2->ChannelsDefaultMask,
                             CHANNELS_MASK_SIZE);
    break;
  }
  case INIT_TYPE_ACTIVATE_DEFAULT_CHANNELS: {
    // Restore channels default mask
    RegionNvmGroup2->ChannelsMask[0] |= RegionNvmGroup2->ChannelsDefaultMask[0];
    break;
  }
  default: {
    break;
  }
  }
#endif /* REGION_IN865 */
}

bool RegionIN865Verify(VerifyParams_t *verify, PhyAttribute_t phyAttribute) {
#if defined(REGION_IN865)
  switch (phyAttribute) {
  case PHY_FREQUENCY: {
    return VerifyRfFreq(verify->Frequency);
  }
  case PHY_TX_DR: {
    if (verify->DatarateParams.Datarate ==
        DR_6) { // DR_6 is not supported by this region
      return false;
    } else {
      return RegionCommonValueInRange(verify->DatarateParams.Datarate,
                                      IN865_TX_MIN_DATARATE,
                                      IN865_TX_MAX_DATARATE);
    }
  }
  case PHY_DEF_TX_DR: {
    return RegionCommonValueInRange(verify->DatarateParams.Datarate, DR_0,
                                    DR_5);
  }
  case PHY_RX_DR: {
    if (verify->DatarateParams.Datarate ==
        DR_6) { // DR_6 is not supported by this region
      return false;
    } else {
      return RegionCommonValueInRange(verify->DatarateParams.Datarate,
                                      IN865_RX_MIN_DATARATE,
                                      IN865_RX_MAX_DATARATE);
    }
  }
  case PHY_DEF_TX_POWER:
  case PHY_TX_POWER: {
    // Remark: switched min and max!
    return RegionCommonValueInRange(verify->TxPower, IN865_MAX_TX_POWER,
                                    IN865_MIN_TX_POWER);
  }
  case PHY_DUTY_CYCLE: {
    return IN865_DUTY_CYCLE_ENABLED;
  }
  default:
    return false;
  }
#else
  return false;
#endif /* REGION_IN865 */
}

void RegionIN865ApplyCFList(ApplyCFListParams_t *applyCFList) {
#if defined(REGION_IN865)
  ChannelParams_t newChannel;
  ChannelAddParams_t channelAdd;
  ChannelRemoveParams_t channelRemove;

  // Setup default datarate range
  newChannel.DrRange.Value = (DR_5 << 4) | DR_0;

  // Size of the optional CF list
  if (applyCFList->Size != 16) {
    return;
  }

  // Last byte CFListType must be 0 to indicate the CFList contains a list of
  // frequencies
  if (applyCFList->Payload[15] != 0) {
    return;
  }

  // Last byte is RFU, don't take it into account
  for (uint8_t i = 0, chanIdx = IN865_NUMB_DEFAULT_CHANNELS;
       chanIdx < IN865_MAX_NB_CHANNELS; i += 3, chanIdx++) {
    if (chanIdx < (IN865_NUMB_CHANNELS_CF_LIST + IN865_NUMB_DEFAULT_CHANNELS)) {
      // Channel frequency
      newChannel.Frequency = (uint32_t)applyCFList->Payload[i];
      newChannel.Frequency |= ((uint32_t)applyCFList->Payload[i + 1] << 8);
      newChannel.Frequency |= ((uint32_t)applyCFList->Payload[i + 2] << 16);
      newChannel.Frequency *= 100;

      // Initialize alternative frequency to 0
      newChannel.Rx1Frequency = 0;
    } else {
      newChannel.Frequency = 0;
      newChannel.DrRange.Value = 0;
      newChannel.Rx1Frequency = 0;
    }

    if (newChannel.Frequency != 0) {
      channelAdd.NewChannel = &newChannel;
      channelAdd.ChannelId = chanIdx;

      // Try to add all channels
      RegionIN865ChannelAdd(&channelAdd);
    } else {
      channelRemove.ChannelId = chanIdx;

      RegionIN865ChannelsRemove(&channelRemove);
    }
  }
#endif /* REGION_IN865 */
}

bool RegionIN865ChanMaskSet(ChanMaskSetParams_t *chanMaskSet) {
#if defined(REGION_IN865)
  switch (chanMaskSet->ChannelsMaskType) {
  case CHANNELS_MASK: {
    RegionCommonChanMaskCopy(RegionNvmGroup2->ChannelsMask,
                             chanMaskSet->ChannelsMaskIn, 1);
    break;
  }
  case CHANNELS_DEFAULT_MASK: {
    RegionCommonChanMaskCopy(RegionNvmGroup2->ChannelsDefaultMask,
                             chanMaskSet->ChannelsMaskIn, 1);
    break;
  }
  default:
    return false;
  }
  return true;
#else
  return false;
#endif /* REGION_IN865 */
}

void RegionIN865ComputeRxWindowParameters(int8_t datarate, uint8_t minRxSymbols,
                                          uint32_t rxError,
                                          RxConfigParams_t *rxConfigParams) {
#if defined(REGION_IN865)
  uint32_t tSymbolInUs = 0;

  // Get the datarate, perform a boundary check
  rxConfigParams->Datarate = MIN(datarate, IN865_RX_MAX_DATARATE);
  rxConfigParams->Bandwidth =
      RegionCommonGetBandwidth(rxConfigParams->Datarate, BandwidthsIN865);

  if (rxConfigParams->Datarate == DR_7) { // FSK
    tSymbolInUs = RegionCommonComputeSymbolTimeFsk(
        DataratesIN865[rxConfigParams->Datarate]);
  } else { // LoRa
    tSymbolInUs = RegionCommonComputeSymbolTimeLoRa(
        DataratesIN865[rxConfigParams->Datarate],
        BandwidthsIN865[rxConfigParams->Datarate]);
  }

  RegionCommonComputeRxWindowParameters(
      tSymbolInUs, minRxSymbols, rxError, Radio.GetWakeupTime(),
      &rxConfigParams->WindowTimeout, &rxConfigParams->WindowOffset);
#endif /* REGION_IN865 */
}

bool RegionIN865RxConfig(RxConfigParams_t *rxConfig, int8_t *datarate) {
#if defined(REGION_IN865)
  RadioModems_t modem;
  int8_t dr = rxConfig->Datarate;
  uint8_t maxPayload = 0;
  int8_t phyDr = 0;
  uint32_t frequency = rxConfig->Frequency;

  if (Radio.GetStatus() != RF_IDLE) {
    return false;
  }

  if (rxConfig->RxSlot == RX_SLOT_WIN_1) {
    // Apply window 1 frequency
    frequency = RegionNvmGroup2->Channels[rxConfig->Channel].Frequency;
    // Apply the alternative RX 1 window frequency, if it is available
    if (RegionNvmGroup2->Channels[rxConfig->Channel].Rx1Frequency != 0) {
      frequency = RegionNvmGroup2->Channels[rxConfig->Channel].Rx1Frequency;
    }
  }

  // Read the physical datarate from the datarates table
  phyDr = DataratesIN865[dr];

  Radio.SetChannel(frequency);

  // Radio configuration
  if (dr == DR_7) {
    modem = MODEM_FSK;
    Radio.SetRxConfig(modem, 50000, phyDr * 1000, 0, 83333, 5,
                      rxConfig->WindowTimeout, false, 0, true, 0, 0, false,
                      rxConfig->RxContinuous);
  } else {
    modem = MODEM_LORA;
    Radio.SetRxConfig(modem, rxConfig->Bandwidth, phyDr, 1, 0, 8,
                      rxConfig->WindowTimeout, false, 0, false, 0, 0, true,
                      rxConfig->RxContinuous);
  }

  if (rxConfig->RepeaterSupport == true) {
    maxPayload = MaxPayloadOfDatarateRepeaterIN865[dr];
  } else {
    maxPayload = MaxPayloadOfDatarateIN865[dr];
  }
  Radio.SetMaxPayloadLength(modem,
                            maxPayload + LORAMAC_FRAME_PAYLOAD_OVERHEAD_SIZE);

  RegionCommonRxConfigPrint(rxConfig->RxSlot, frequency, dr);

  *datarate = (uint8_t)dr;
  return true;
#else
  return false;
#endif /* REGION_IN865 */
}

bool RegionIN865TxConfig(TxConfigParams_t *txConfig, int8_t *txPower,
                         TimerTime_t *txTimeOnAir) {
#if defined(REGION_IN865)
  RadioModems_t modem;
  int8_t phyDr = DataratesIN865[txConfig->Datarate];
#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
  int8_t txPowerLimited = RegionCommonLimitTxPower(
      txConfig->TxPower,
      RegionNvmGroup1->Bands[RegionNvmGroup2->Channels[txConfig->Channel].Band]
          .TxMaxPower);
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
  int8_t txPowerLimited = RegionCommonLimitTxPower(
      txConfig->TxPower,
      RegionBands[RegionNvmGroup2->Channels[txConfig->Channel].Band]
          .TxMaxPower);
#endif /* REGION_VERSION */
  uint32_t bandwidth =
      RegionCommonGetBandwidth(txConfig->Datarate, BandwidthsIN865);
  int8_t phyTxPower = 0;

  // Calculate physical TX power
  phyTxPower = RegionCommonComputeTxPower(txPowerLimited, txConfig->MaxEirp,
                                          txConfig->AntennaGain);

  // Setup the radio frequency
  Radio.SetChannel(RegionNvmGroup2->Channels[txConfig->Channel].Frequency);

  if (txConfig->Datarate == DR_7) { // High Speed FSK channel
    modem = MODEM_FSK;
    Radio.SetTxConfig(modem, phyTxPower, 25000, bandwidth, phyDr * 1000, 0, 5,
                      false, true, 0, 0, false, 4000);
  } else {
    modem = MODEM_LORA;
    Radio.SetTxConfig(modem, phyTxPower, 0, bandwidth, phyDr, 1, 8, false, true,
                      0, 0, false, 4000);
  }
  RegionCommonTxConfigPrint(
      RegionNvmGroup2->Channels[txConfig->Channel].Frequency,
      txConfig->Datarate);

  // Update time-on-air
  *txTimeOnAir = GetTimeOnAir(txConfig->Datarate, txConfig->PktLen);

  // Setup maximum payload length of the radio driver
  Radio.SetMaxPayloadLength(modem, txConfig->PktLen);

  *txPower = txPowerLimited;
  return true;
#else
  return false;
#endif /* REGION_IN865 */
}

uint8_t RegionIN865LinkAdrReq(LinkAdrReqParams_t *linkAdrReq, int8_t *drOut,
                              int8_t *txPowOut, uint8_t *nbRepOut,
                              uint8_t *nbBytesParsed) {
  uint8_t status = 0x07;
#if defined(REGION_IN865)
  RegionCommonLinkAdrParams_t linkAdrParams = {0};
  uint8_t nextIndex = 0;
  uint8_t bytesProcessed = 0;
  uint16_t chMask = 0;
  GetPhyParams_t getPhy;
  PhyParam_t phyParam;
  RegionCommonLinkAdrReqVerifyParams_t linkAdrVerifyParams;

  while (bytesProcessed < linkAdrReq->PayloadSize) {
    // Get ADR request parameters
    nextIndex = RegionCommonParseLinkAdrReq(
        &(linkAdrReq->Payload[bytesProcessed]), &linkAdrParams);

    if (nextIndex == 0)
      break; // break loop, since no more request has been found

    // Update bytes processed
    bytesProcessed += nextIndex;

    // Revert status, as we only check the last ADR request for the channel mask
    // KO
    status = 0x07;

    // Setup temporary channels mask
    chMask = linkAdrParams.ChMask;

    // Verify channels mask
    if ((linkAdrParams.ChMaskCtrl == 0) && (chMask == 0)) {
      status &= 0xFE; // Channel mask KO
    } else if (((linkAdrParams.ChMaskCtrl >= 1) &&
                (linkAdrParams.ChMaskCtrl <= 5)) ||
               (linkAdrParams.ChMaskCtrl >= 7)) {
      // RFU
      status &= 0xFE; // Channel mask KO
    } else {
      for (uint8_t i = 0; i < IN865_MAX_NB_CHANNELS; i++) {
        if (linkAdrParams.ChMaskCtrl == 6) {
          if (RegionNvmGroup2->Channels[i].Frequency != 0) {
            chMask |= 1 << i;
          }
        } else {
          if (((chMask & (1 << i)) != 0) &&
              (RegionNvmGroup2->Channels[i].Frequency ==
               0)) {        // Trying to enable an undefined channel
            status &= 0xFE; // Channel mask KO
          }
        }
      }
    }
  }

  if (linkAdrParams.Datarate != DR_6) {
    // Get the minimum possible datarate
    getPhy.Attribute = PHY_MIN_TX_DR;
    getPhy.UplinkDwellTime = linkAdrReq->UplinkDwellTime;
    phyParam = RegionIN865GetPhyParam(&getPhy);

    linkAdrVerifyParams.Status = status;
    linkAdrVerifyParams.AdrEnabled = linkAdrReq->AdrEnabled;
    linkAdrVerifyParams.Datarate = linkAdrParams.Datarate;
    linkAdrVerifyParams.TxPower = linkAdrParams.TxPower;
    linkAdrVerifyParams.NbRep = linkAdrParams.NbRep;
    linkAdrVerifyParams.CurrentDatarate = linkAdrReq->CurrentDatarate;
    linkAdrVerifyParams.CurrentTxPower = linkAdrReq->CurrentTxPower;
    linkAdrVerifyParams.CurrentNbRep = linkAdrReq->CurrentNbRep;
    linkAdrVerifyParams.NbChannels = IN865_MAX_NB_CHANNELS;
    linkAdrVerifyParams.ChannelsMask = &chMask;
    linkAdrVerifyParams.MinDatarate = (int8_t)phyParam.Value;
    linkAdrVerifyParams.MaxDatarate = IN865_TX_MAX_DATARATE;
    linkAdrVerifyParams.Channels = RegionNvmGroup2->Channels;
    linkAdrVerifyParams.MinTxPower = IN865_MIN_TX_POWER;
    linkAdrVerifyParams.MaxTxPower = IN865_MAX_TX_POWER;
    linkAdrVerifyParams.Version = linkAdrReq->Version;

    // Verify the parameters and update, if necessary
    status = RegionCommonLinkAdrReqVerifyParams(
        &linkAdrVerifyParams, &linkAdrParams.Datarate, &linkAdrParams.TxPower,
        &linkAdrParams.NbRep);
  } else {          // DR_6 is not supported by this region
    status &= 0xFD; // Datarate KO
  }

  // Update channelsMask if everything is correct
  if (status == 0x07) {
    // Set the channels mask to a default value
    memset1((uint8_t *)RegionNvmGroup2->ChannelsMask, 0,
            sizeof(RegionNvmGroup2->ChannelsMask));
    // Update the channels mask
    RegionNvmGroup2->ChannelsMask[0] = chMask;
  }

  // Update status variables
  *drOut = linkAdrParams.Datarate;
  *txPowOut = linkAdrParams.TxPower;
  *nbRepOut = linkAdrParams.NbRep;
  *nbBytesParsed = bytesProcessed;

#endif /* REGION_IN865 */
  return status;
}

uint8_t RegionIN865RxParamSetupReq(RxParamSetupReqParams_t *rxParamSetupReq) {
  uint8_t status = 0x07;
#if defined(REGION_IN865)

  // Verify radio frequency
  if (VerifyRfFreq(rxParamSetupReq->Frequency) == false) {
    status &= 0xFE; // Channel frequency KO
  }

  // Verify datarate
  if ((RegionCommonValueInRange(rxParamSetupReq->Datarate,
                                IN865_RX_MIN_DATARATE,
                                IN865_RX_MAX_DATARATE) == false) ||
      // DR_6 is not supported by this region
      (rxParamSetupReq->Datarate == DR_6)) {
    status &= 0xFD; // Datarate KO
  }

  // Verify datarate offset
  if (RegionCommonValueInRange(rxParamSetupReq->DrOffset,
                               IN865_MIN_RX1_DR_OFFSET,
                               IN865_MAX_RX1_DR_OFFSET) == false) {
    status &= 0xFB; // Rx1DrOffset range KO
  }

#endif /* REGION_IN865 */
  return status;
}

int8_t RegionIN865NewChannelReq(NewChannelReqParams_t *newChannelReq) {
  uint8_t status = 0x03;
  ChannelAddParams_t channelAdd;
  ChannelRemoveParams_t channelRemove;

  if (newChannelReq->NewChannel->Frequency == 0) {
    channelRemove.ChannelId = newChannelReq->ChannelId;

    // Remove
    if (RegionIN865ChannelsRemove(&channelRemove) == false) {
      status &= 0xFC;
    }
  } else {
    channelAdd.NewChannel = newChannelReq->NewChannel;
    channelAdd.ChannelId = newChannelReq->ChannelId;

    switch (RegionIN865ChannelAdd(&channelAdd)) {
    case LORAMAC_STATUS_OK: {
      break;
    }
    case LORAMAC_STATUS_FREQUENCY_INVALID: {
      status &= 0xFE;
      break;
    }
    case LORAMAC_STATUS_DATARATE_INVALID: {
      status &= 0xFD;
      break;
    }
    case LORAMAC_STATUS_FREQ_AND_DR_INVALID: {
      status &= 0xFC;
      break;
    }
    default: {
      status &= 0xFC;
      break;
    }
    }
  }

  return status;
}

int8_t RegionIN865TxParamSetupReq(TxParamSetupReqParams_t *txParamSetupReq) {
  // Do not accept the request
  return -1;
}

int8_t RegionIN865DlChannelReq(DlChannelReqParams_t *dlChannelReq) {
  uint8_t status = 0x03;

#if defined(REGION_IN865)
  if (dlChannelReq->ChannelId >= (CHANNELS_MASK_SIZE * 16)) {
    return 0;
  }

  // Verify if the frequency is supported
  if (VerifyRfFreq(dlChannelReq->Rx1Frequency) == false) {
    status &= 0xFE;
  }

  // Verify if an uplink frequency exists
  if (RegionNvmGroup2->Channels[dlChannelReq->ChannelId].Frequency == 0) {
    status &= 0xFD;
  }

  // Apply Rx1 frequency, if the status is OK
  if (status == 0x03) {
    RegionNvmGroup2->Channels[dlChannelReq->ChannelId].Rx1Frequency =
        dlChannelReq->Rx1Frequency;
  }

#endif /* REGION_IN865 */
  return status;
}

int8_t RegionIN865AlternateDr(int8_t currentDr, AlternateDrType_t type) {
#if defined(REGION_IN865)
  return currentDr;
#else
  return -1;
#endif /* REGION_IN865 */
}

LoRaMacStatus_t RegionIN865NextChannel(NextChanParams_t *nextChanParams,
                                       uint8_t *channel, TimerTime_t *time,
                                       TimerTime_t *aggregatedTimeOff) {
#if defined(REGION_IN865)
  uint8_t nbEnabledChannels = 0;
  uint8_t nbRestrictedChannels = 0;
  uint8_t enabledChannels[IN865_MAX_NB_CHANNELS] = {0};
  RegionCommonIdentifyChannelsParam_t identifyChannelsParam;
  RegionCommonCountNbOfEnabledChannelsParams_t countChannelsParams;
  LoRaMacStatus_t status = LORAMAC_STATUS_NO_CHANNEL_FOUND;
  uint16_t joinChannels = IN865_JOIN_CHANNELS;

  if (RegionCommonCountChannels(RegionNvmGroup2->ChannelsMask, 0, 1) ==
      0) { // Reactivate default channels
    RegionNvmGroup2->ChannelsMask[0] |= LC(1) + LC(2) + LC(3);
  }

  // Search how many channels are enabled
  countChannelsParams.Joined = nextChanParams->Joined;
  countChannelsParams.Datarate = nextChanParams->Datarate;
  countChannelsParams.ChannelsMask = RegionNvmGroup2->ChannelsMask;
  countChannelsParams.Channels = RegionNvmGroup2->Channels;
#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
  countChannelsParams.Bands = RegionNvmGroup1->Bands;
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
  countChannelsParams.Bands = RegionBands;
#endif /* REGION_VERSION */
  countChannelsParams.MaxNbChannels = IN865_MAX_NB_CHANNELS;
  countChannelsParams.JoinChannels = &joinChannels;

  identifyChannelsParam.AggrTimeOff = nextChanParams->AggrTimeOff;
  identifyChannelsParam.LastAggrTx = nextChanParams->LastAggrTx;
  identifyChannelsParam.DutyCycleEnabled = nextChanParams->DutyCycleEnabled;
  identifyChannelsParam.MaxBands = IN865_MAX_NB_BANDS;

  identifyChannelsParam.ElapsedTimeSinceStartUp =
      nextChanParams->ElapsedTimeSinceStartUp;
  identifyChannelsParam.LastTxIsJoinRequest =
      nextChanParams->LastTxIsJoinRequest;
  identifyChannelsParam.ExpectedTimeOnAir =
      GetTimeOnAir(nextChanParams->Datarate, nextChanParams->PktLen);

  identifyChannelsParam.CountNbOfEnabledChannelsParam = &countChannelsParams;

  status = RegionCommonIdentifyChannels(
      &identifyChannelsParam, aggregatedTimeOff, enabledChannels,
      &nbEnabledChannels, &nbRestrictedChannels, time);

  if (status == LORAMAC_STATUS_OK) {
    // We found a valid channel
    *channel = enabledChannels[randr(0, nbEnabledChannels - 1)];
  } else if (status == LORAMAC_STATUS_NO_CHANNEL_FOUND) {
    // Datarate not supported by any channel, restore defaults
    RegionNvmGroup2->ChannelsMask[0] |= LC(1) + LC(2) + LC(3);
  }
  return status;
#else
  return LORAMAC_STATUS_NO_CHANNEL_FOUND;
#endif /* REGION_IN865 */
}

LoRaMacStatus_t RegionIN865ChannelAdd(ChannelAddParams_t *channelAdd) {
#if defined(REGION_IN865)
  bool drInvalid = false;
  bool freqInvalid = false;
  uint8_t id = channelAdd->ChannelId;

  if (id < IN865_NUMB_DEFAULT_CHANNELS) {
    return LORAMAC_STATUS_FREQ_AND_DR_INVALID;
  }

  if (id >= IN865_MAX_NB_CHANNELS) {
    return LORAMAC_STATUS_PARAMETER_INVALID;
  }

  // Validate the datarate range
  if (RegionCommonValueInRange(channelAdd->NewChannel->DrRange.Fields.Min,
                               IN865_TX_MIN_DATARATE,
                               IN865_TX_MAX_DATARATE) == false) {
    drInvalid = true;
  }
  if (RegionCommonValueInRange(channelAdd->NewChannel->DrRange.Fields.Max,
                               IN865_TX_MIN_DATARATE,
                               IN865_TX_MAX_DATARATE) == false) {
    drInvalid = true;
  }
  if (channelAdd->NewChannel->DrRange.Fields.Min >
      channelAdd->NewChannel->DrRange.Fields.Max) {
    drInvalid = true;
  }

  // Check frequency
  if (freqInvalid == false) {
    if (VerifyRfFreq(channelAdd->NewChannel->Frequency) == false) {
      freqInvalid = true;
    }
  }

  // Check status
  if ((drInvalid == true) && (freqInvalid == true)) {
    return LORAMAC_STATUS_FREQ_AND_DR_INVALID;
  }
  if (drInvalid == true) {
    return LORAMAC_STATUS_DATARATE_INVALID;
  }
  if (freqInvalid == true) {
    return LORAMAC_STATUS_FREQUENCY_INVALID;
  }

  memcpy1((uint8_t *)&(RegionNvmGroup2->Channels[id]),
          (uint8_t *)channelAdd->NewChannel,
          sizeof(RegionNvmGroup2->Channels[id]));
  RegionNvmGroup2->Channels[id].Band = 0;
  RegionNvmGroup2->ChannelsMask[0] |= (1 << id);
  return LORAMAC_STATUS_OK;
#else
  return LORAMAC_STATUS_NO_CHANNEL_FOUND;
#endif /* REGION_IN865 */
}

bool RegionIN865ChannelsRemove(ChannelRemoveParams_t *channelRemove) {
#if defined(REGION_IN865)
  uint8_t id = channelRemove->ChannelId;

  if (id < IN865_NUMB_DEFAULT_CHANNELS) {
    return false;
  }

  // Remove the channel from the list of channels
  RegionNvmGroup2->Channels[id] = (ChannelParams_t){0, 0, {0}, 0};

  return RegionCommonChanDisable(RegionNvmGroup2->ChannelsMask, id,
                                 IN865_MAX_NB_CHANNELS);
#else
  return false;
#endif /* REGION_IN865 */
}

#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
void RegionIN865SetContinuousWave(ContinuousWaveParams_t *continuousWave) {
#if defined(REGION_IN865)
  int8_t txPowerLimited = RegionCommonLimitTxPower(
      continuousWave->TxPower,
      RegionNvmGroup1
          ->Bands[RegionNvmGroup2->Channels[continuousWave->Channel].Band]
          .TxMaxPower);
  int8_t phyTxPower = 0;
  uint32_t frequency =
      RegionNvmGroup2->Channels[continuousWave->Channel].Frequency;

  // Calculate physical TX power
  phyTxPower = RegionCommonComputeTxPower(
      txPowerLimited, continuousWave->MaxEirp, continuousWave->AntennaGain);

  Radio.SetTxContinuousWave(frequency, phyTxPower, continuousWave->Timeout);
#endif /* REGION_IN865 */
}

uint8_t RegionIN865ApplyDrOffset(uint8_t downlinkDwellTime, int8_t dr,
                                 int8_t drOffset) {
#if defined(REGION_IN865)
  // Apply offset formula
  return MIN(DR_5, MAX(DR_0, dr - EffectiveRx1DrOffsetIN865[drOffset]));
#else
  return 0;
#endif /* REGION_IN865 */
}
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
uint8_t RegionIN865ApplyDrOffset(uint8_t downlinkDwellTime, int8_t dr,
                                 int8_t drOffset) {
#if defined(REGION_IN865)
  int8_t datarate = EffectiveRx1DrOffsetIN865[dr][drOffset];

  if ((datarate < 0) || (dr == DR_6)) {
    datarate = DR_0;
  }
  return datarate;
#else
  return 0;
#endif /* REGION_IN865 */
}
#endif /* REGION_VERSION */

void RegionIN865RxBeaconSetup(RxBeaconSetup_t *rxBeaconSetup, uint8_t *outDr) {
#if defined(REGION_IN865)
  RegionCommonRxBeaconSetupParams_t regionCommonRxBeaconSetup;

  regionCommonRxBeaconSetup.Datarates = DataratesIN865;
  regionCommonRxBeaconSetup.Frequency = rxBeaconSetup->Frequency;
  regionCommonRxBeaconSetup.BeaconSize = IN865_BEACON_SIZE;
  regionCommonRxBeaconSetup.BeaconDatarate = IN865_BEACON_CHANNEL_DR;
  regionCommonRxBeaconSetup.BeaconChannelBW = IN865_BEACON_CHANNEL_BW;
  regionCommonRxBeaconSetup.RxTime = rxBeaconSetup->RxTime;
  regionCommonRxBeaconSetup.SymbolTimeout = rxBeaconSetup->SymbolTimeout;

  RegionCommonRxBeaconSetup(&regionCommonRxBeaconSetup);

  // Store downlink datarate
  *outDr = IN865_BEACON_CHANNEL_DR;
#endif /* REGION_IN865 */
}
