/*!
 * \file      RegionEU868.h
 *
 * \brief     Region definition for EU868
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
 *
 * \author    Johannes Bruder ( STACKFORCE )
 *
 * \defgroup  REGIONEU868 Region EU868
 *            Implementation according to LoRaWAN Specification v1.0.2.
 * \{
 */
/**
 ******************************************************************************
 *
 *          Portions COPYRIGHT 2020 STMicroelectronics
 *
 * @file    RegionEU868.h
 * @author  MCD Application Team
 * @brief   Region definition for EU868
 ******************************************************************************
 */
#ifndef __REGION_EU868_H__
#define __REGION_EU868_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Region.h"

/*!
 * LoRaMac maximum number of channels
 */
#define EU868_MAX_NB_CHANNELS 16

/*!
 * Number of default channels
 */
#define EU868_NUMB_DEFAULT_CHANNELS 3

/*!
 * Number of channels to apply for the CF list
 */
#define EU868_NUMB_CHANNELS_CF_LIST 5

/*!
 * Minimal datarate that can be used by the node
 */
#define EU868_TX_MIN_DATARATE DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU868_TX_MAX_DATARATE DR_7

/*!
 * Minimal datarate that can be used by the node
 */
#define EU868_RX_MIN_DATARATE DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU868_RX_MAX_DATARATE DR_7

/*!
 * Default datarate used by the node
 */
#define EU868_DEFAULT_DATARATE DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define EU868_MIN_RX1_DR_OFFSET 0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define EU868_MAX_RX1_DR_OFFSET 5

/*!
 * Minimal Tx output power that can be used by the node
 */
#define EU868_MIN_TX_POWER TX_POWER_7

/*!
 * Maximal Tx output power that can be used by the node
 */
#define EU868_MAX_TX_POWER TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define EU868_DEFAULT_TX_POWER TX_POWER_0

/*!
 * Default Max EIRP
 */
#define EU868_DEFAULT_MAX_EIRP 16.0f

/*!
 * Default antenna gain
 */
#define EU868_DEFAULT_ANTENNA_GAIN 2.15f

/*!
 * Enabled or disabled the duty cycle
 */
#define EU868_DUTY_CYCLE_ENABLED 1

/*!
 * Maximum RX window duration
 */
#define EU868_MAX_RX_WINDOW 3000

#if (EU868_DEFAULT_DATARATE > DR_5)
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define EU868_RX_WND_2_FREQ 869525000

/*!
 * Second reception window channel datarate definition.
 */
#define EU868_RX_WND_2_DR DR_0

/*!
 * Default uplink dwell time configuration
 */
#define EU868_DEFAULT_UPLINK_DWELL_TIME 0

/*
 * CLASS B
 */
/*!
 * Beacon frequency
 */
#define EU868_BEACON_CHANNEL_FREQ 869525000

/*!
 * Ping slot channel frequency
 */
#define EU868_PING_SLOT_CHANNEL_FREQ 869525000

/*!
 * Payload size of a beacon frame
 */
#define EU868_BEACON_SIZE 17

/*!
 * Size of RFU 1 field
 */
#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
#define EU868_RFU1_SIZE 2
#elif (defined(REGION_VERSION) && (REGION_VERSION == 0x02010001))
#define EU868_RFU1_SIZE 1
#endif /* REGION_VERSION */

/*!
 * Size of RFU 2 field
 */
#define EU868_RFU2_SIZE 0

/*!
 * Datarate of the beacon channel
 */
#define EU868_BEACON_CHANNEL_DR DR_3

/*!
 * Bandwidth of the beacon channel
 */
#define EU868_BEACON_CHANNEL_BW 0

/*!
 * Ping slot channel datarate
 */
#define EU868_PING_SLOT_CHANNEL_DR DR_3

/*!
 * Maximum number of bands
 */
#define EU868_MAX_NB_BANDS 6

/*!
 * Band 0 definition
 * Band = { DutyCycle, TxMaxPower, LastBandUpdateTime, LastMaxCreditAssignTime,
 * TimeCredits, MaxTimeCredits, ReadyForTransmission }
 */
#define EU868_BAND0                                                            \
  { 100, EU868_MAX_TX_POWER, 0, 0, 0, 0, 0 } //  1.0 %

/*!
 * Band 1 definition
 * Band = { DutyCycle, TxMaxPower, LastBandUpdateTime, LastMaxCreditAssignTime,
 * TimeCredits, MaxTimeCredits, ReadyForTransmission }
 */
#define EU868_BAND1                                                            \
  { 100, EU868_MAX_TX_POWER, 0, 0, 0, 0, 0 } //  1.0 %

/*!
 * Band 2 definition
 * Band = { DutyCycle, TxMaxPower, LastBandUpdateTime, LastMaxCreditAssignTime,
 * TimeCredits, MaxTimeCredits, ReadyForTransmission }
 */
#define EU868_BAND2                                                            \
  { 1000, EU868_MAX_TX_POWER, 0, 0, 0, 0, 0 } //  0.1 %

/*!
 * Band 3 definition
 * Band = { DutyCycle, TxMaxPower, LastBandUpdateTime, LastMaxCreditAssignTime,
 * TimeCredits, MaxTimeCredits, ReadyForTransmission }
 */
#define EU868_BAND3                                                            \
  { 10, EU868_MAX_TX_POWER, 0, 0, 0, 0, 0 } // 10.0 %

/*!
 * Band 4 definition
 * Band = { DutyCycle, TxMaxPower, LastBandUpdateTime, LastMaxCreditAssignTime,
 * TimeCredits, MaxTimeCredits, ReadyForTransmission }
 */
#define EU868_BAND4                                                            \
  { 100, EU868_MAX_TX_POWER, 0, 0, 0, 0, 0 } //  1.0 %

/*!
 * Band 5 definition
 * Band = { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff,
 *          DutyCycleTimePeriod, MaxAllowedTimeOnAir, AggregatedTimeOnAir,
 * StartTimeOfPeriod }
 */
#define EU868_BAND5                                                            \
  { 1000, EU868_MAX_TX_POWER, 0, 0, 0, 0, 0 } //  0.1 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin )
 * }, Band }
 */
#define EU868_LC1                                                              \
  { 868100000, 0, {((DR_5 << 4) | DR_0)}, 1 }

/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin )
 * }, Band }
 */
#define EU868_LC2                                                              \
  { 868300000, 0, {((DR_5 << 4) | DR_0)}, 1 }

/*!
 * LoRaMac default channel 3
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin )
 * }, Band }
 */
#define EU868_LC3                                                              \
  { 868500000, 0, {((DR_5 << 4) | DR_0)}, 1 }

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define EU868_JOIN_CHANNELS (uint16_t)(LC(1) | LC(2) | LC(3))

/*!
 * Data rates table definition
 */
static const uint8_t DataratesEU868[] = {12, 11, 10, 9, 8, 7, 7, 50};

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t BandwidthsEU868[] = {125000, 125000, 125000, 125000,
                                           125000, 125000, 250000, 0};

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with
 * repeater.
 */
static const uint8_t MaxPayloadOfDatarateEU868[] = {51,  51,  51,  115,
                                                    242, 242, 242, 242};

/*!
 * Maximum payload with respect to the datarate index. Can operate with
 * repeater.
 */
static const uint8_t MaxPayloadOfDatarateRepeaterEU868[] = {51,  51,  51,  115,
                                                            222, 222, 222, 222};

/*!
 * \brief The function gets a value of a specific phy attribute.
 *
 * \param [in] getPhy Pointer to the function parameters.
 *
 * \retval Returns a structure containing the PHY parameter.
 */
PhyParam_t RegionEU868GetPhyParam(GetPhyParams_t *getPhy);

/*!
 * \brief Updates the last TX done parameters of the current channel.
 *
 * \param [in] txDone Pointer to the function parameters.
 */
void RegionEU868SetBandTxDone(SetBandTxDoneParams_t *txDone);

/*!
 * \brief Initializes the channels masks and the channels.
 *
 * \param [in] params Sets the initialization type.
 */
void RegionEU868InitDefaults(InitDefaultsParams_t *params);

/*!
 * \brief Verifies a parameter.
 *
 * \param [in] verify Pointer to the function parameters.
 *
 * \param [in] phyAttribute Sets the initialization type.
 *
 * \retval Returns true, if the parameter is valid.
 */
bool RegionEU868Verify(VerifyParams_t *verify, PhyAttribute_t phyAttribute);

/*!
 * \brief The function parses the input buffer and sets up the channels of the
 *        CF list.
 *
 * \param [in] applyCFList Pointer to the function parameters.
 */
void RegionEU868ApplyCFList(ApplyCFListParams_t *applyCFList);

/*!
 * \brief Sets a channels mask.
 *
 * \param [in] chanMaskSet Pointer to the function parameters.
 *
 * \retval Returns true, if the channels mask could be set.
 */
bool RegionEU868ChanMaskSet(ChanMaskSetParams_t *chanMaskSet);

/*!
 * Computes the Rx window timeout and offset.
 *
 * \param [in] datarate     Rx window datarate index to be used
 *
 * \param [in] minRxSymbols Minimum required number of symbols to detect an Rx
 * frame.
 *
 * \param [in] rxError      System maximum timing error of the receiver. In
 * milliseconds The receiver will turn on in a [-rxError : +rxError] ms interval
 * around RxOffset
 *
 * \param [out] rxConfigParams Returns updated WindowTimeout and WindowOffset
 * fields.
 */
void RegionEU868ComputeRxWindowParameters(int8_t datarate, uint8_t minRxSymbols,
                                          uint32_t rxError,
                                          RxConfigParams_t *rxConfigParams);

/*!
 * \brief Configuration of the RX windows.
 *
 * \param [in] rxConfig Pointer to the function parameters.
 *
 * \param [out] datarate The datarate index which was set.
 *
 * \retval Returns true, if the configuration was applied successfully.
 */
bool RegionEU868RxConfig(RxConfigParams_t *rxConfig, int8_t *datarate);

/*!
 * \brief TX configuration.
 *
 * \param [in] txConfig Pointer to the function parameters.
 *
 * \param [out] txPower The tx power index which was set.
 *
 * \param [out] txTimeOnAir The time-on-air of the frame.
 *
 * \retval Returns true, if the configuration was applied successfully.
 */
bool RegionEU868TxConfig(TxConfigParams_t *txConfig, int8_t *txPower,
                         TimerTime_t *txTimeOnAir);

/*!
 * \brief The function processes a Link ADR Request.
 *
 * \param [in] linkAdrReq Pointer to the function parameters.
 *
 * \param [out] drOut The datarate which was applied.
 *
 * \param [out] txPowOut The TX power which was applied.
 *
 * \param [out] nbRepOut The number of repetitions to apply.
 *
 * \param [out] nbBytesParsed The number bytes which were parsed.
 *
 * \retval Returns the status of the operation, according to the LoRaMAC
 * specification.
 */
uint8_t RegionEU868LinkAdrReq(LinkAdrReqParams_t *linkAdrReq, int8_t *drOut,
                              int8_t *txPowOut, uint8_t *nbRepOut,
                              uint8_t *nbBytesParsed);

/*!
 * \brief The function processes a RX Parameter Setup Request.
 *
 * \param [in] rxParamSetupReq Pointer to the function parameters.
 *
 * \retval Returns the status of the operation, according to the LoRaMAC
 * specification.
 */
uint8_t RegionEU868RxParamSetupReq(RxParamSetupReqParams_t *rxParamSetupReq);

/*!
 * \brief The function processes a Channel Request.
 *
 * \param [in] newChannelReq Pointer to the function parameters.
 *
 * \retval Returns the status of the operation, according to the LoRaMAC
 * specification.
 */
int8_t RegionEU868NewChannelReq(NewChannelReqParams_t *newChannelReq);

/*!
 * \brief The function processes a TX ParamSetup Request.
 *
 * \param [in] txParamSetupReq Pointer to the function parameters.
 *
 * \retval Returns the status of the operation, according to the LoRaMAC
 * specification. Returns -1, if the functionality is not implemented. In this
 * case, the end node shall not process the command.
 */
int8_t RegionEU868TxParamSetupReq(TxParamSetupReqParams_t *txParamSetupReq);

/*!
 * \brief The function processes a DlChannel Request.
 *
 * \param [in] dlChannelReq Pointer to the function parameters.
 *
 * \retval Returns the status of the operation, according to the LoRaMAC
 * specification.
 */
int8_t RegionEU868DlChannelReq(DlChannelReqParams_t *dlChannelReq);

/*!
 * \brief Alternates the datarate of the channel for the join request.
 *
 * \param [in] currentDr Current datarate.
 *
 * \param [in] type Alternation type.
 *
 * \retval Datarate to apply.
 */
int8_t RegionEU868AlternateDr(int8_t currentDr, AlternateDrType_t type);

/*!
 * \brief Searches and set the next random available channel
 *
 * \param [in] nextChanParams pointer of selected channel parameters
 *
 * \param [out] channel Next channel to use for TX.
 *
 * \param [out] time Time to wait for the next transmission according to the
 * duty cycle.
 *
 * \param [out] aggregatedTimeOff Updates the aggregated time off.
 *
 * \retval Function status [1: OK, 0: Unable to find a channel on the current
 * datarate]
 */
LoRaMacStatus_t RegionEU868NextChannel(NextChanParams_t *nextChanParams,
                                       uint8_t *channel, TimerTime_t *time,
                                       TimerTime_t *aggregatedTimeOff);

/*!
 * \brief Adds a channel.
 *
 * \param [in] channelAdd Pointer to the function parameters.
 *
 * \retval Status of the operation.
 */
LoRaMacStatus_t RegionEU868ChannelAdd(ChannelAddParams_t *channelAdd);

/*!
 * \brief Removes a channel.
 *
 * \param [in] channelRemove Pointer to the function parameters.
 *
 * \retval Returns true, if the channel was removed successfully.
 */
bool RegionEU868ChannelsRemove(ChannelRemoveParams_t *channelRemove);

#if (defined(REGION_VERSION) && (REGION_VERSION == 0x01010003))
/*!
 * \brief Sets the radio into continuous wave mode.
 *
 * \param [IN] continuousWave Pointer to the function parameters.
 */
void RegionEU868SetContinuousWave(ContinuousWaveParams_t *continuousWave);
#endif /* REGION_VERSION */

/*!
 * \brief Computes new datarate according to the given offset
 *
 * \param [in] downlinkDwellTime Downlink dwell time configuration. 0: No limit,
 * 1: 400ms
 *
 * \param [in] dr Current datarate
 *
 * \param [in] drOffset Offset to be applied
 *
 * \retval newDr Computed datarate.
 */
uint8_t RegionEU868ApplyDrOffset(uint8_t downlinkDwellTime, int8_t dr,
                                 int8_t drOffset);

/*!
 * \brief Sets the radio into beacon reception mode
 *
 * \param [in] rxBeaconSetup Pointer to the function parameters
 *
 * \param [out] outDr Datarate used to receive the beacon
 */
void RegionEU868RxBeaconSetup(RxBeaconSetup_t *rxBeaconSetup, uint8_t *outDr);

/*! \} defgroup REGIONEU868 */

#ifdef __cplusplus
}
#endif

#endif // __REGION_EU868_H__
