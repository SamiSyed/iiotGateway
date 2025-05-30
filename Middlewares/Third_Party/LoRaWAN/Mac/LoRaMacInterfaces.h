/*!
 * \file      LoRaMacInterfaces.h
 *
 * \brief     LoRa MAC layer external types definition
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
 * \defgroup  LORAMAC LoRa MAC layer implementation
 *            This module specifies the API implementation of the LoRaMAC layer.
 *            This is a placeholder for a detailed description of the LoRaMac
 *            layer and the supported features.
 * \{
 */
/**
 ******************************************************************************
 *
 *          Portions COPYRIGHT 2020 STMicroelectronics
 *
 * @file    LoRaMacInterfaces.h
 * @author  MCD Application Team
 * @brief   LoRa MAC layer external types definition
 ******************************************************************************
 */
#ifndef __LORAMAC_INTERFACES_H__
#define __LORAMAC_INTERFACES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "LoRaMacClassBNvm.h"
#include "LoRaMacCryptoNvm.h"
#include "LoRaMacTypes.h"
#include "LoRaMacVersion.h"
#include "RegionNvm.h"
#include "lorawan_conf.h"
#include "secure-element-nvm.h"

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF12 - BW125
 * AU915        | SF10 - BW125
 * CN470        | SF12 - BW125
 * CN779        | SF12 - BW125
 * EU433        | SF12 - BW125
 * EU868        | SF12 - BW125
 * IN865        | SF12 - BW125
 * KR920        | SF12 - BW125
 * US915        | SF10 - BW125
 * RU864        | SF12 - BW125
 */
#define DR_0 0

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF11 - BW125
 * AU915        | SF9  - BW125
 * CN470        | SF11 - BW125
 * CN779        | SF11 - BW125
 * EU433        | SF11 - BW125
 * EU868        | SF11 - BW125
 * IN865        | SF11 - BW125
 * KR920        | SF11 - BW125
 * US915        | SF9  - BW125
 * RU864        | SF11 - BW125
 */
#define DR_1 1

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF10 - BW125
 * AU915        | SF8  - BW125
 * CN470        | SF10 - BW125
 * CN779        | SF10 - BW125
 * EU433        | SF10 - BW125
 * EU868        | SF10 - BW125
 * IN865        | SF10 - BW125
 * KR920        | SF10 - BW125
 * US915        | SF8  - BW125
 * RU864        | SF10 - BW125
 */
#define DR_2 2

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF9  - BW125
 * AU915        | SF7  - BW125
 * CN470        | SF9  - BW125
 * CN779        | SF9  - BW125
 * EU433        | SF9  - BW125
 * EU868        | SF9  - BW125
 * IN865        | SF9  - BW125
 * KR920        | SF9  - BW125
 * US915        | SF7  - BW125
 * RU864        | SF9  - BW125
 */
#define DR_3 3

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF8  - BW125
 * AU915        | SF8  - BW500
 * CN470        | SF8  - BW125
 * CN779        | SF8  - BW125
 * EU433        | SF8  - BW125
 * EU868        | SF8  - BW125
 * IN865        | SF8  - BW125
 * KR920        | SF8  - BW125
 * US915        | SF8  - BW500
 * RU864        | SF8  - BW125
 */
#define DR_4 4

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF7  - BW125
 * AU915        | RFU
 * CN470        | SF7  - BW125
 * CN779        | SF7  - BW125
 * EU433        | SF7  - BW125
 * EU868        | SF7  - BW125
 * IN865        | SF7  - BW125
 * KR920        | SF7  - BW125
 * US915        | RFU
 * RU864        | SF7  - BW125
 */
#define DR_5 5

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | SF7  - BW250
 * AU915        | RFU
 * CN470        | SF12 - BW125
 * CN779        | SF7  - BW250
 * EU433        | SF7  - BW250
 * EU868        | SF7  - BW250
 * IN865        | SF7  - BW250
 * KR920        | RFU
 * US915        | RFU
 * RU864        | SF7  - BW250
 */
#define DR_6 6

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | FSK
 * AU915        | RFU
 * CN470        | SF12 - BW125
 * CN779        | FSK
 * EU433        | FSK
 * EU868        | FSK
 * IN865        | FSK
 * KR920        | RFU
 * US915        | RFU
 * RU864        | FSK
 */
#define DR_7 7

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF12 - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF12 - BW500
 * RU864        | RFU
 */
#define DR_8 8

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF11 - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF11 - BW500
 * RU864        | RFU
 */
#define DR_9 9

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF10 - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF10 - BW500
 * RU864        | RFU
 */
#define DR_10 10

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF9  - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF9  - BW500
 * RU864        | RFU
 */
#define DR_11 11

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF8  - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF8  - BW500
 * RU864        | RFU
 */
#define DR_12 12

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | SF7  - BW500
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | SF7  - BW500
 * RU864        | RFU
 */
#define DR_13 13

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | RFU
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | RFU
 * RU864        | RFU
 */
#define DR_14 14

/*!
 * Region       | SF
 * ------------ | :-----:
 * AS923        | RFU
 * AU915        | RFU
 * CN470        | RFU
 * CN779        | RFU
 * EU433        | RFU
 * EU868        | RFU
 * IN865        | RFU
 * KR920        | RFU
 * US915        | RFU
 * RU864        | RFU
 */
#define DR_15 15

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP
 * AU915        | Max EIRP
 * CN470        | Max EIRP
 * CN779        | Max EIRP
 * EU433        | Max EIRP
 * EU868        | Max EIRP
 * IN865        | Max EIRP
 * KR920        | Max EIRP
 * US915        | Max ERP
 * RU864        | Max EIRP
 */
#define TX_POWER_0 0

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 2
 * AU915        | Max EIRP - 2
 * CN470        | Max EIRP - 2
 * CN779        | Max EIRP - 2
 * EU433        | Max EIRP - 2
 * EU868        | Max EIRP - 2
 * IN865        | Max EIRP - 2
 * KR920        | Max EIRP - 2
 * US915        | Max ERP - 2
 * RU864        | Max EIRP - 2
 */
#define TX_POWER_1 1

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 4
 * AU915        | Max EIRP - 4
 * CN470        | Max EIRP - 4
 * CN779        | Max EIRP - 4
 * EU433        | Max EIRP - 4
 * EU868        | Max EIRP - 4
 * IN865        | Max EIRP - 4
 * KR920        | Max EIRP - 4
 * US915        | Max ERP - 4
 * RU864        | Max EIRP - 4
 */
#define TX_POWER_2 2

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 6
 * AU915        | Max EIRP - 6
 * CN470        | Max EIRP - 6
 * CN779        | Max EIRP - 6
 * EU433        | Max EIRP - 6
 * EU868        | Max EIRP - 6
 * IN865        | Max EIRP - 6
 * KR920        | Max EIRP - 6
 * US915        | Max ERP - 6
 * RU864        | Max EIRP - 6
 */
#define TX_POWER_3 3

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 8
 * AU915        | Max EIRP - 8
 * CN470        | Max EIRP - 8
 * CN779        | Max EIRP - 8
 * EU433        | Max EIRP - 8
 * EU868        | Max EIRP - 8
 * IN865        | Max EIRP - 8
 * KR920        | Max EIRP - 8
 * US915        | Max ERP - 8
 * RU864        | Max EIRP - 8
 */
#define TX_POWER_4 4

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 10
 * AU915        | Max EIRP - 10
 * CN470        | Max EIRP - 10
 * CN779        | Max EIRP - 10
 * EU433        | Max EIRP - 10
 * EU868        | Max EIRP - 10
 * IN865        | Max EIRP - 10
 * KR920        | Max EIRP - 10
 * US915        | Max ERP - 10
 * RU864        | Max EIRP - 10
 */
#define TX_POWER_5 5

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 12
 * AU915        | Max EIRP - 12
 * CN470        | Max EIRP - 12
 * CN779        | -
 * EU433        | -
 * EU868        | Max EIRP - 12
 * IN865        | Max EIRP - 12
 * KR920        | Max EIRP - 12
 * US915        | Max ERP - 12
 * RU864        | Max EIRP - 12
 */
#define TX_POWER_6 6

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | Max EIRP - 14
 * AU915        | Max EIRP - 14
 * CN470        | Max EIRP - 14
 * CN779        | -
 * EU433        | -
 * EU868        | Max EIRP - 14
 * IN865        | Max EIRP - 14
 * KR920        | Max EIRP - 14
 * US915        | Max ERP - 14
 * RU864        | Max EIRP - 14
 */
#define TX_POWER_7 7

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 16
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | Max EIRP - 16
 * KR920        | -
 * US915        | Max ERP - 16
 * RU864        | -
 */
#define TX_POWER_8 8

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 18
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | Max EIRP - 18
 * KR920        | -
 * US915        | Max ERP - 18
 * RU864        | -
 */
#define TX_POWER_9 9

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 20
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | Max EIRP - 20
 * KR920        | -
 * US915        | Max ERP - 20
 * RU864        | -
 */
#define TX_POWER_10 10

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 22
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | -
 * KR920        | -
 * US915        | Max ERP - 22
 * RU864        | -
 */
#define TX_POWER_11 11

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 24
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | -
 * KR920        | -
 * US915        | Max ERP - 24
 * RU864        | -
 */
#define TX_POWER_12 12

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 26
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | -
 * KR920        | -
 * US915        | Max ERP - 26
 * RU864        | -
 */
#define TX_POWER_13 13

/*!
 * Region       | dBM
 * ------------ | :-----:
 * AS923        | -
 * AU915        | Max EIRP - 28
 * CN470        | -
 * CN779        | -
 * EU433        | -
 * EU868        | -
 * IN865        | -
 * KR920        | -
 * US915        | Max ERP - 28
 * RU864        | -
 */
#define TX_POWER_14 14

/*!
 * RFU
 */
#define TX_POWER_15 15

/*!
 * End-Device activation type
 */
typedef enum eActivationType {
  /*!
   * None
   */
  ACTIVATION_TYPE_NONE = 0,
  /*!
   * Activation By Personalization (ACTIVATION_TYPE_ABP)
   */
  ACTIVATION_TYPE_ABP = 1,
  /*!
   * Over-The-Air Activation (ACTIVATION_TYPE_OTAA)
   */
  ACTIVATION_TYPE_OTAA = 2,
} ActivationType_t;

/*!
 * LoRaMAC receive window channel parameters
 */
typedef struct sRxChannelParams {
  /*!
   * Frequency in Hz
   */
  uint32_t Frequency;
  /*!
   * Data rate
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * The allowed ranges are region specific. Please refer to \ref DR_0 to \ref
   * DR_15 for details.
   */
  uint8_t Datarate;
} RxChannelParams_t;

/*!
 * LoRaMAC receive window enumeration
 */
typedef enum eLoRaMacRxSlot {
  /*!
   * LoRaMAC receive window 1
   */
  RX_SLOT_WIN_1,
  /*!
   * LoRaMAC receive window 2
   */
  RX_SLOT_WIN_2,
  /*!
   * LoRaMAC receive window 2 for class c - continuous listening
   */
  RX_SLOT_WIN_CLASS_C,
  /*!
   * LoRaMAC class c multicast downlink
   */
  RX_SLOT_WIN_CLASS_C_MULTICAST,
  /*!
   * LoRaMAC class b ping slot window
   */
  RX_SLOT_WIN_CLASS_B_PING_SLOT,
  /*!
   * LoRaMAC class b multicast slot window
   */
  RX_SLOT_WIN_CLASS_B_MULTICAST_SLOT,
  /*!
   * LoRaMAC no active receive window
   */
  RX_SLOT_NONE,
} LoRaMacRxSlot_t;

/*!
 * Global MAC layer parameters
 */
typedef struct sLoRaMacParams {
  /*!
   * System overall timing error in milliseconds.
   * [-SystemMaxRxError : +SystemMaxRxError]
   * Default: +/-10 ms
   */
  uint32_t SystemMaxRxError;
  /*!
   * Minimum required number of symbols to detect an Rx frame
   * Default: 6 symbols
   */
  uint8_t MinRxSymbols;
  /*!
   * LoRaMac maximum time a reception window stays open
   */
  uint32_t MaxRxWindow;
  /*!
   * Receive delay 1
   */
  uint32_t ReceiveDelay1;
  /*!
   * Receive delay 2
   */
  uint32_t ReceiveDelay2;
  /*!
   * Join accept delay 1
   */
  uint32_t JoinAcceptDelay1;
  /*!
   * Join accept delay 1
   */
  uint32_t JoinAcceptDelay2;
  /*!
   * Number of uplink messages repetitions [1:15]
   */
  uint8_t ChannelsNbTrans;
  /*!
   * Datarate offset between uplink and downlink on first window
   */
  uint8_t Rx1DrOffset;
  /*!
   * LoRaMAC 2nd reception window settings
   */
  RxChannelParams_t Rx2Channel;
  /*!
   * LoRaMAC continuous reception window settings
   */
  RxChannelParams_t RxCChannel;
  /*!
   * Uplink dwell time configuration. 0: No limit, 1: 400ms
   */
  uint8_t UplinkDwellTime;
  /*!
   * Downlink dwell time configuration. 0: No limit, 1: 400ms
   */
  uint8_t DownlinkDwellTime;
  /*!
   * Maximum possible EIRP
   */
  float MaxEirp;
  /*!
   * Antenna gain of the node
   */
  float AntennaGain;
  /*!
   * Limit of uplinks without any downlink response before the ADRACKReq bit
   * will be set.
   */
  uint16_t AdrAckLimit;
  /*!
   * Limit of uplinks without any downlink response after a the first frame
   * with set ADRACKReq bit before the trying to regain the connectivity.
   */
  uint16_t AdrAckDelay;
  /*!
   * Indicates if the node supports repeaters
   */
  bool RepeaterSupport;
  /*!
   * Default response timeout for class b and class c confirmed downlink frames
   * in milli seconds.
   */
  uint32_t RxBCTimeout;
} LoRaMacParams_t;

/*!
 * Global RX layer status
 */
typedef struct sLoRaMacRxStatus {
  /*
   * Rssi of the received packet
   */
  int16_t Rssi;
  /*
   * Snr of the received packet
   */
  int8_t Snr;
  /*
   * Holds the current rx window slot
   */
  LoRaMacRxSlot_t RxSlot;
} LoRaMacRxStatus_t;

/*!
 * LoRaMAC data structure for a PingSlotInfoReq \ref MLME_PING_SLOT_INFO
 *
 * LoRaWAN Specification
 */
typedef union uPingSlotInfo {
  /*!
   * Parameter for byte access
   */
  uint8_t Value;
  /*!
   * Structure containing the parameters for the PingSlotInfoReq
   */
  struct sInfoFields {
    /*!
     * Periodicity = 0: ping slot every second
     * Periodicity = 7: ping slot every 128 seconds
     */
    uint8_t Periodicity : 3;
    /*!
     * RFU
     */
    uint8_t RFU : 5;
  } Fields;
} PingSlotInfo_t;

/*!
 * LoRaMAC data structure for the \ref MLME_BEACON MLME-Indication
 *
 * LoRaWAN Specification
 */
typedef struct sBeaconInfo {
  /*!
   * Timestamp in seconds since 00:00:00, Sunday 6th of January 1980
   * (start of the GPS epoch) modulo 2^32
   */
  SysTime_t Time;
  /*!
   * Frequency
   */
  uint32_t Frequency;
  /*!
   * Datarate
   */
  uint8_t Datarate;
  /*!
   * RSSI
   */
  int16_t Rssi;
  /*!
   * SNR
   */
  int8_t Snr;
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  /*!
   * Param
   * |  Bits | 7:2 |  1:0 |
   * |-------|-----|------|
   * | Param | RFU | Prec |
   *
   * Prec field is used to interpret the precision of beacon's transmit time
   * as 10^(-6+prec) and the default value is 0.
   * RFU will be set to Zero and Prec can take values between 0:3.
   */
  uint8_t Param;
#endif /* LORAMAC_VERSION */
  /*!
   * Data structure for the gateway specific part. The
   * content of the values may differ for each gateway
   */
  struct sGwSpecific {
    /*!
     * Info descriptor - can differ for each gateway
     */
    uint8_t InfoDesc;
    /*!
     * Info - can differ for each gateway
     */
    uint8_t Info[6];
  } GwSpecific;
} BeaconInfo_t;

/*!
 * Enumeration containing the status of the operation of a MAC service
 */
typedef enum eLoRaMacEventInfoStatus {
  /*!
   * Service performed successfully
   */
  LORAMAC_EVENT_INFO_STATUS_OK = 0,
  /*!
   * An error occurred during the execution of the service
   */
  LORAMAC_EVENT_INFO_STATUS_ERROR,
  /*!
   * A Tx timeout occurred
   */
  LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT,
  /*!
   * An Rx timeout occurred on receive window 1
   */
  LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT,
  /*!
   * An Rx timeout occurred on receive window 2
   */
  LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT,
  /*!
   * An Rx error occurred on receive window 1
   */
  LORAMAC_EVENT_INFO_STATUS_RX1_ERROR,
  /*!
   * An Rx error occurred on receive window 2
   */
  LORAMAC_EVENT_INFO_STATUS_RX2_ERROR,
  /*!
   * An error occurred in the join procedure
   */
  LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL,
  /*!
   * A frame with an invalid downlink counter was received. The
   * downlink counter of the frame was equal to the local copy
   * of the downlink counter of the node.
   */
  LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED,
  /*!
   * The MAC could not retransmit a frame since the MAC decreased the datarate.
   * The payload size is not applicable for the datarate.
   */
  LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR,
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01000300))
  /*!
   * The node has lost MAX_FCNT_GAP or more frames.
   */
  LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS,
#endif /* LORAMAC_VERSION */
  /*!
   * An address error occurred
   */
  LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL,
  /*!
   * Message integrity check failure
   */
  LORAMAC_EVENT_INFO_STATUS_MIC_FAIL,
  /*!
   * Not used
   */
  LORAMAC_EVENT_INFO_STATUS_MULTICAST_FAIL,
  /*!
   * The node has received a beacon. Acquisition is no longer pending
   */
  LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED,
  /*!
   * The node has not received a beacon for at least
   * CLASSB_MAX_BEACON_LESS_PERIOD
   */
  LORAMAC_EVENT_INFO_STATUS_BEACON_LOST,
  /*!
   * The node has not received a beacon after the CLASSB_BEACON_INTERVAL
   */
  LORAMAC_EVENT_INFO_STATUS_BEACON_NOT_FOUND,
} LoRaMacEventInfoStatus_t;

/*!
 * LoRaMac tx/rx operation state
 */
typedef union eLoRaMacFlags_t {
  /*!
   * Byte-access to the bits
   */
  uint8_t Value;
  /*!
   * Structure containing single access to bits
   */
  struct sMacFlagBits {
    /*!
     * MCPS-Req pending
     */
    uint8_t McpsReq : 1;
    /*!
     * MCPS-Ind pending
     */
    uint8_t McpsInd : 1;
    /*!
     * MLME-Req pending
     */
    uint8_t MlmeReq : 1;
    /*!
     * MLME-Ind pending
     */
    uint8_t MlmeInd : 1;
    /*!
     * MAC cycle done
     */
    uint8_t MacDone : 1;
    /*!
     * Indicate if a NVM handling is required
     */
    uint8_t NvmHandle : 1;
  } Bits;
} LoRaMacFlags_t;

/*!
 * LoRaMAC region enumeration
 */
typedef enum eLoRaMacRegion {
  /*!
   * AS band on 923MHz
   */
  LORAMAC_REGION_AS923,
  /*!
   * Australian band on 915MHz
   */
  LORAMAC_REGION_AU915,
  /*!
   * Chinese band on 470MHz
   */
  LORAMAC_REGION_CN470,
  /*!
   * Chinese band on 779MHz
   */
  LORAMAC_REGION_CN779,
  /*!
   * European band on 433MHz
   */
  LORAMAC_REGION_EU433,
  /*!
   * European band on 868MHz
   */
  LORAMAC_REGION_EU868,
  /*!
   * South korean band on 920MHz
   */
  LORAMAC_REGION_KR920,
  /*!
   * India band on 865MHz
   */
  LORAMAC_REGION_IN865,
  /*!
   * North american band on 915MHz
   */
  LORAMAC_REGION_US915,
  /*!
   * Russia band on 864MHz
   */
  LORAMAC_REGION_RU864,
} LoRaMacRegion_t;

typedef struct sLoRaMacNvmDataGroup1 {
  /*!
   * Counts the number of missed ADR acknowledgements
   */
  uint32_t AdrAckCounter;
  /*!
   * Last transmission time.
   */
  TimerTime_t LastTxDoneTime;
  /*!
   * Aggregated time off.
   */
  TimerTime_t AggregatedTimeOff;
  /*!
   * Last received Message integrity Code (MIC)
   */
  uint32_t LastRxMic;
  /*!
   * Channels TX power
   */
  int8_t ChannelsTxPower;
  /*!
   * Channels data rate
   */
  int8_t ChannelsDatarate;
  /*!
   * If the server has sent a FRAME_TYPE_DATA_CONFIRMED_DOWN this variable
   * indicates if the ACK bit must be set for the next transmission
   */
  bool SrvAckRequested;
  /*!
   * Counts the number if uplinks to know when the next Rejoin request type 0 is
   * required. ( If requested by the server through RejoinParamSetupReq MAC
   * command )
   */
  uint32_t Rejoin0UplinksCounter;
  /*!
   * Counter of Rejoin Request of retries.
   * ( If requested by the server through ForceRejoinReq MAC command )
   */
  uint8_t ForceRejoinRetriesCounter;
  /*!
   * Counts the number of uplinks containing a RekeyInd MAC command to know
   * when the end device should reverted to join state because it didn't
   * received a RekeyConf.
   */
  uint16_t RekeyIndUplinksCounter;
  /*!
   * CRC32 value of the MacGroup1 data structure.
   */
  uint32_t Crc32;
} LoRaMacNvmDataGroup1_t;

typedef struct sLoRaMacNvmDataGroup2 {
  /*!
   * LoRaMac region.
   */
  LoRaMacRegion_t Region;
  /*!
   * LoRaMac parameters
   */
  LoRaMacParams_t MacParams;
  /*!
   * LoRaMac default parameters
   */
  LoRaMacParams_t MacParamsDefaults;
  /*!
   * Channels TX power
   */
  int8_t ChannelsTxPowerDefault;
  /*!
   * Channels data rate
   */
  int8_t ChannelsDatarateDefault;
  /*!
   * Network ID ( 3 bytes )
   */
  uint32_t NetID;
  /*!
   * Mote Address
   */
  uint32_t DevAddr;
  /*!
   * Multicast channel list
   */
  MulticastCtx_t MulticastChannelList[LORAMAC_MAX_MC_CTX];
  /*!
   * Actual device class
   */
  DeviceClass_t DeviceClass;
  /*!
   * Indicates if the node is connected to
   * a private or public network
   */
  bool PublicNetwork;
  /*!
   * LoRaMac ADR control status
   */
  bool AdrCtrlOn;
  /*!
   * Maximum duty cycle
   * \remark Possibility to shutdown the device.
   */
  uint8_t MaxDCycle;
  /*!
   * Enables/Disables duty cycle management (Test only)
   */
  bool DutyCycleOn;
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  /*!
   * Set to true, if the datarate was increased
   * with a link adr request.
   */
  bool ChannelsDatarateChangedLinkAdrReq;
  /*!
   * The stack will set this variable to true, if a downlink has been received.
   */
  bool DownlinkReceived;
  /*!
   * Enables/disable FPort 224 processing (certification port)
   */
  bool IsCertPortOn;
#endif /* LORAMAC_VERSION */
  /*!
   * Aggregated duty cycle management
   */
  uint16_t AggregatedDCycle;
  /*!
   * Stores the time at LoRaMac initialization.
   *
   * \remark Used for the BACKOFF_DC computation.
   */
  SysTime_t InitializationTime;
  /*!
   * Current LoRaWAN Version
   */
  Version_t Version;
  /*!
   * End-Device network activation
   */
  ActivationType_t NetworkActivation;
  /*!
   * Number of uplinks without Rejoin request type 0.
   * ( If requested by the server through RejoinParamSetupReq MAC command )
   * When it's set to 0, Rejoin0UplinksCounter won't be incremented
   */
  uint32_t Rejoin0UplinksLimit;
  /*!
   * The total number of times the device will retry the Rejoin Request.
   * ( If requested by the server through ForceRejoinReq MAC command )
   */
  uint8_t ForceRejoinMaxRetries;
  /*!
   * Rejoin Request Type
   * ( If requested by the server through ForceRejoinReq MAC command )
   */
  uint8_t ForceRejoinType;
  /*!
   * Time in seconds between cyclic transmission of Type 0 Rejoin requests.
   */
  uint32_t Rejoin0CycleInSec;
  /*!
   * Time in seconds between cyclic transmission of Type 1 Rejoin requests.
   */
  uint32_t Rejoin1CycleInSec;
  /*!
   * Indicates if a Rejoin request was sent and no join-accept or any downlink
   * has been received yet.
   */
  bool IsRejoinAcceptPending;
  /*!
   * Indicates if a reqjoin request 0 is in the queue to send.
   */
  bool IsRejoin0RequestQueued;
  /*!
   * Indicates if a reqjoin request 1 is in the queue to send.
   */
  bool IsRejoin1RequestQueued;
  /*!
   * Indicates if a reqjoin request 2 is in the queue to send.
   */
  bool IsRejoin2RequestQueued;
  /*!
   * CRC32 value of the MacGroup2 data structure.
   */
  uint32_t Crc32;
} LoRaMacNvmDataGroup2_t;

/*!
 * LoRaMAC data structure for non-volatile memory (NVM).
 * This structure contains data which must be stored in NVM.
 */
typedef struct sLoRaMacNvmData {
  /*!
   * Parameters related to the crypto layer. Change with every TX/RX
   * procedure.
   */
  LoRaMacCryptoNvmData_t Crypto;
  /*!
   * Parameters related to the MAC which change with high probability after
   * every TX/RX procedure.
   */
  LoRaMacNvmDataGroup1_t MacGroup1;
  /*!
   * Parameters related to the MAC which do not change very likely with every
   * TX/RX procedure.
   */
  LoRaMacNvmDataGroup2_t MacGroup2;
  /*!
   * Parameters related to the secure-element.
   */
  SecureElementNvmData_t SecureElement;
  /*!
   * Parameters related to the regional implementation which change with high
   * probability after every TX/RX procedure.
   */
  RegionNvmDataGroup1_t RegionGroup1;
  /*!
   * Parameters related to the regional implementation which do not change
   * very likely with every TX/RX procedure.
   */
  RegionNvmDataGroup2_t RegionGroup2;
  /*!
   * Parameters related to class b.
   */
  LoRaMacClassBNvmData_t ClassB;
} LoRaMacNvmData_t;

/*!
 *
 * \brief   LoRaMAC data services
 *
 * \details The following table list the primitives which are supported by the
 *          specific MAC data service:
 *
 * Name                  | Request | Indication | Response | Confirm
 * --------------------- | :-----: | :--------: | :------: | :-----:
 * \ref MCPS_UNCONFIRMED | YES     | YES        | NO       | YES
 * \ref MCPS_CONFIRMED   | YES     | YES        | NO       | YES
 * \ref MCPS_MULTICAST   | NO      | YES        | NO       | NO
 * \ref MCPS_PROPRIETARY | YES     | YES        | NO       | YES
 *
 * The following table provides links to the function implementations of the
 * related MCPS primitives:
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MCPS-Request     | \ref LoRaMacMlmeRequest
 * MCPS-Confirm     | MacMcpsConfirm in \ref LoRaMacPrimitives_t
 * MCPS-Indication  | MacMcpsIndication in \ref LoRaMacPrimitives_t
 */
typedef enum eMcps {
  /*!
   * Unconfirmed LoRaMAC frame
   */
  MCPS_UNCONFIRMED,
  /*!
   * Confirmed LoRaMAC frame
   */
  MCPS_CONFIRMED,
  /*!
   * Multicast LoRaMAC frame
   */
  MCPS_MULTICAST,
  /*!
   * Proprietary frame
   */
  MCPS_PROPRIETARY,
} Mcps_t;

/*!
 * Structure which defines return parameters for requests.
 */
typedef struct sRequestReturnParam {
  /*!
   * This value reports the time in milliseconds which
   * an application must wait before its possible to send
   * the next uplink.
   */
  TimerTime_t DutyCycleWaitTime;
} RequestReturnParam_t;

/*!
 * LoRaMAC MCPS-Request for an unconfirmed frame
 */
typedef struct sMcpsReqUnconfirmed {
  /*!
   * Frame port field. Must be set if the payload is not empty. Use the
   * application specific frame port values: [1...223]
   *
   * LoRaWAN Specification V1.0.2, chapter 4.3.2
   */
  uint8_t fPort;
  /*!
   * Pointer to the buffer of the frame payload
   */
  void *fBuffer;
  /*!
   * Size of the frame payload
   */
  uint16_t fBufferSize;
  /*!
   * Uplink datarate, if ADR is off
   */
  int8_t Datarate;
} McpsReqUnconfirmed_t;

/*!
 * LoRaMAC MCPS-Request for a confirmed frame
 */
typedef struct sMcpsReqConfirmed {
  /*!
   * Frame port field. Must be set if the payload is not empty. Use the
   * application specific frame port values: [1...223]
   *
   * LoRaWAN Specification V1.0.2, chapter 4.3.2
   */
  uint8_t fPort;
  /*!
   * Pointer to the buffer of the frame payload
   */
  void *fBuffer;
  /*!
   * Size of the frame payload
   */
  uint16_t fBufferSize;
  /*!
   * Uplink datarate, if ADR is off
   */
  int8_t Datarate;
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01000300))
  /*!
   * Number of trials to transmit the frame, if the LoRaMAC layer did not
   * receive an acknowledgment. The MAC performs a datarate adaptation,
   * according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
   * to the following table:
   *
   * Transmission nb | Data Rate
   * ----------------|-----------
   * 1 (first)       | DR
   * 2               | DR
   * 3               | max(DR-1,0)
   * 4               | max(DR-1,0)
   * 5               | max(DR-2,0)
   * 6               | max(DR-2,0)
   * 7               | max(DR-3,0)
   * 8               | max(DR-3,0)
   *
   * Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
   * the datarate, in case the LoRaMAC layer did not receive an acknowledgment
   */
  uint8_t NbTrials;
#endif /* LORAMAC_VERSION */
} McpsReqConfirmed_t;

/*!
 * LoRaMAC MCPS-Request for a proprietary frame
 */
typedef struct sMcpsReqProprietary {
  /*!
   * Pointer to the buffer of the frame payload
   */
  void *fBuffer;
  /*!
   * Size of the frame payload
   */
  uint16_t fBufferSize;
  /*!
   * Uplink datarate, if ADR is off
   */
  int8_t Datarate;
} McpsReqProprietary_t;

/*!
 * LoRaMAC MCPS-Request structure
 */
typedef struct sMcpsReq {
  /*!
   * MCPS-Request type
   */
  Mcps_t Type;

  /*!
   * MCPS-Request parameters
   */
  union uMcpsParam {
    /*!
     * MCPS-Request parameters for an unconfirmed frame
     */
    McpsReqUnconfirmed_t Unconfirmed;
    /*!
     * MCPS-Request parameters for a confirmed frame
     */
    McpsReqConfirmed_t Confirmed;
    /*!
     * MCPS-Request parameters for a proprietary frame
     */
    McpsReqProprietary_t Proprietary;
  } Req;

  /*!
   * MCPS-Request return parameters
   */
  RequestReturnParam_t ReqReturn;
} McpsReq_t;

/*!
 * LoRaMAC MCPS-Confirm
 */
typedef struct sMcpsConfirm {
  /*!
   * Holds the previously performed MCPS-Request
   */
  Mcps_t McpsRequest;
  /*!
   * Status of the operation
   */
  LoRaMacEventInfoStatus_t Status;
  /*!
   * Uplink datarate
   */
  uint8_t Datarate;
  /*!
   * Transmission power
   */
  int8_t TxPower;
  /*!
   * Set if an acknowledgement was received
   */
  bool AckReceived;
  /*!
   * Provides the number of retransmissions
   */
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01000300))
  uint8_t NbRetries;
#elif (defined(LORAMAC_VERSION) &&                                             \
       ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  uint8_t NbTrans;
#endif /* LORAMAC_VERSION */
  /*!
   * The transmission time on air of the frame
   */
  TimerTime_t TxTimeOnAir;
  /*!
   * The uplink counter value related to the frame
   */
  uint32_t UpLinkCounter;
  /*!
   * The uplink channel related to the frame
   */
  uint32_t Channel;
} McpsConfirm_t;

/*!
 * LoRaMAC MCPS-Indication primitive
 */
typedef struct sMcpsIndication {
  /*!
   * MCPS-Indication type
   */
  Mcps_t McpsIndication;
  /*!
   * Status of the operation
   */
  LoRaMacEventInfoStatus_t Status;
  /*!
   * Multicast
   */
  uint8_t Multicast;
  /*!
   * Application port
   */
  uint8_t Port;
  /*!
   * Downlink datarate
   */
  uint8_t RxDatarate;
  /*!
   * Frame pending status
   */
  uint8_t IsUplinkTxPending;
  /*!
   * Pointer to the received data stream
   */
  uint8_t *Buffer;
  /*!
   * Size of the received data stream
   */
  uint8_t BufferSize;
  /*!
   * Indicates, if data is available
   */
  bool RxData;
  /*!
   * Set if an acknowledgement was received
   */
  bool AckReceived;
  /*!
   * The downlink counter value for the received frame
   */
  uint32_t DownLinkCounter;
  /*!
   * The device address of the frame
   */
  uint32_t DevAddress;
  /*!
   * Set if a DeviceTimeAns MAC command was received.
   */
  bool DeviceTimeAnsReceived;
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  /*!
   * Response timeout for a class b or c device when a
   * confirmed downlink has been received. In all other
   * cases this variable is 0.
   */
  TimerTime_t ResponseTimeout;
#endif /* LORAMAC_VERSION */
} McpsIndication_t;

/*!
 * \brief LoRaMAC management services
 *
 * \details The following table list the primitives which are supported by the
 *          specific MAC management service:
 *
 * Name                         | Request | Indication | Response | Confirm
 * ---------------------------- | :-----: | :--------: | :------: | :-----:
 * \ref MLME_JOIN               | YES     | NO         | NO       | YES
 * \ref MLME_REJOIN_0           | YES     | NO         | NO       | YES
 * \ref MLME_REJOIN_1           | YES     | NO         | NO       | YES
 * \ref MLME_REJOIN_2           | YES     | NO         | NO       | YES
 * \ref MLME_LINK_CHECK         | YES     | NO         | NO       | YES
 * \ref MLME_TXCW               | YES     | NO         | NO       | YES
 * \ref MLME_DERIVE_MC_KE_KEY   | YES     | NO         | NO       | YES
 * \ref MLME_DERIVE_MC_KEY_PAIR | YES     | NO         | NO       | YES
 * \ref MLME_REVERT_JOIN        | NO      | YES        | NO       | NO
 *
 * The following table provides links to the function implementations of the
 * related MLME primitives.
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MLME-Request     | \ref LoRaMacMlmeRequest
 * MLME-Confirm     | MacMlmeConfirm in \ref LoRaMacPrimitives_t
 * MLME-Indication  | MacMlmeIndication in \ref LoRaMacPrimitives_t
 */
typedef enum eMlme {
  /*!
   * An unknown MLME service
   */
  MLME_UNKNOWN,
  /*!
   * Initiates the Over-the-Air activation
   *
   * LoRaWAN Specification V1.0.2, chapter 6.2
   */
  MLME_JOIN,
  /*!
   * Initiates sending a ReJoin-request type 0
   *
   * LoRaWAN Specification V1.1.0, chapter 6.2.4.1
   */
  MLME_REJOIN_0,
  /*!
   * Initiates sending a ReJoin-request type 1
   *
   * LoRaWAN Specification V1.1.0, chapter 6.2.4.2
   */
  MLME_REJOIN_1,
  /*!
   * Initiates sending a ReJoin-request type 2
   *
   * LoRaWAN Specification V1.1.0, chapter 6.2.4.2
   */
  MLME_REJOIN_2,
  /*!
   * LinkCheckReq - Connectivity validation
   *
   * LoRaWAN Specification V1.0.2, chapter 5, table 4
   */
  MLME_LINK_CHECK,
  /*!
   * Sets Tx continuous wave mode
   *
   * LoRaWAN end-device certification
   */
  MLME_TXCW,
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01000300))
  /*!
   * Sets Tx continuous wave mode (new LoRa-Alliance CC definition)
   *
   * LoRaWAN end-device certification
   */
  MLME_TXCW_1,
#endif /* LORAMAC_VERSION */
  /*!
   * Derives the McKEKey from the AppKey or NwkKey.
   */
  MLME_DERIVE_MC_KE_KEY,
  /*!
   * Derives a Multicast group key pair ( McAppSKey, McNwkSKey ) from McKey
   */
  MLME_DERIVE_MC_KEY_PAIR,
  /*!
   * Initiates a DeviceTimeReq
   *
   * LoRaWAN end-device certification
   */
  MLME_DEVICE_TIME,
  /*!
   * The MAC uses this MLME primitive to indicate a beacon reception
   * status.
   *
   * LoRaWAN end-device certification
   */
  MLME_BEACON,
  /*!
   * Initiate a beacon acquisition. The MAC will search for a beacon.
   * It will search for XX_BEACON_INTERVAL milliseconds.
   *
   * LoRaWAN end-device certification
   */
  MLME_BEACON_ACQUISITION,
  /*!
   * Initiates a PingSlotInfoReq
   *
   * LoRaWAN end-device certification
   */
  MLME_PING_SLOT_INFO,
  /*!
   * Initiates a BeaconTimingReq
   *
   * LoRaWAN end-device certification
   */
  MLME_BEACON_TIMING,
  /*!
   * Primitive which indicates that the beacon has been lost
   *
   * \remark The upper layer is required to switch the device class to ClassA
   *
   * LoRaWAN end-device certification
   */
  MLME_BEACON_LOST,
  /*!
   *
   * Indicates that the device hasn't received a RekeyConf and it reverts to the
   * join state.
   *
   * \remark The upper layer is required to trigger the Join process again.
   */
  MLME_REVERT_JOIN,
} Mlme_t;

/*!
 * LoRaMAC MLME-Request for the join service
 */
typedef struct sMlmeReqJoin {
  /*!
   * LoRaWAN Network End-Device Activation ( ACTIVATION_TYPE_NONE,
   * ACTIVATION_TYPE_ABP or ACTIVATION_TYPE_OTAA )
   *
   * Related MIB type: \ref MIB_NETWORK_ACTIVATION
   */
  ActivationType_t NetworkActivation;
  /*!
   * Datarate used for join request.
   */
  uint8_t Datarate;
  /*!
   * Channels TX power
   */
  int8_t TxPower;
} MlmeReqJoin_t;

/*!
 * LoRaMAC MLME-Request for Tx continuous wave mode
 */
typedef struct sMlmeReqTxCw {
  /*!
   * Time in seconds while the radio is kept in continuous wave mode
   */
  uint16_t Timeout;
  /*!
   * RF frequency to set (Only used with new way)
   */
  uint32_t Frequency;
  /*!
   * RF output power to set (Only used with new way)
   */
  int8_t Power;
} MlmeReqTxCw_t;

/*!
 * LoRaMAC MLME-Request for the ping slot info service
 */
typedef struct sMlmeReqPingSlotInfo {
  PingSlotInfo_t PingSlot;
} MlmeReqPingSlotInfo_t;

/*!
 * LoRaMAC MLME-Request to derive the McKEKey from the AppKey or NwkKey
 */
typedef struct sMlmeReqDeriveMcKEKey {
  /*!
   *  Key identifier of the root key to use to perform the derivation ( NwkKey
   * or AppKey )
   */
  KeyIdentifier_t KeyID;
  /*!
   * Nonce value ( nonce <= 15)
   */
  uint16_t Nonce;
  /*!
   * DevEUI Value
   */
  uint8_t *DevEUI;
} MlmeReqDeriveMcKEKey_t;

/*!
 * LoRaMAC MLME-Request to derive a Multicast group key pair ( McAppSKey,
 * McNwkSKey ) from McKey
 */
typedef struct sMlmeReqDeriveMcSessionKeyPair {
  /*!
   *  Address identifier to select the multicast group
   */
  AddressIdentifier_t GroupID;
} MlmeReqDeriveMcSessionKeyPair_t;

/*!
 * LoRaMAC MLME-Request structure
 */
typedef struct sMlmeReq {
  /*!
   * MLME-Request type
   */
  Mlme_t Type;

  /*!
   * MLME-Request parameters
   */
  union uMlmeParam {
    /*!
     * MLME-Request parameters for a join request
     */
    MlmeReqJoin_t Join;
    /*!
     * MLME-Request parameters for Tx continuous mode request
     */
    MlmeReqTxCw_t TxCw;
    /*!
     * MLME-Request parameters for a ping slot info request
     */
    MlmeReqPingSlotInfo_t PingSlotInfo;
    /*!
     * MLME-Request to derive the McKEKey from the AppKey or NwkKey
     */
    MlmeReqDeriveMcKEKey_t DeriveMcKEKey;
    /*!
     * MLME-Request to derive a Multicast group key pair ( McAppSKey, McNwkSKey
     * ) from McKey
     */
    MlmeReqDeriveMcSessionKeyPair_t DeriveMcSessionKeyPair;
  } Req;

  /*!
   * MLME-Request return parameters
   */
  RequestReturnParam_t ReqReturn;
} MlmeReq_t;

/*!
 * LoRaMAC MLME-Confirm primitive
 */
typedef struct sMlmeConfirm {
  /*!
   * Holds the previously performed MLME-Request
   */
  Mlme_t MlmeRequest;
  /*!
   * Status of the operation
   */
  LoRaMacEventInfoStatus_t Status;
  /*!
   * The transmission time on air of the frame
   */
  TimerTime_t TxTimeOnAir;
  /*!
   * Demodulation margin. Contains the link margin [dB] of the last
   * successfully received LinkCheckReq
   */
  uint8_t DemodMargin;
  /*!
   * Number of gateways which received the last LinkCheckReq
   */
  uint8_t NbGateways;
  /*!
   * Provides the number of retransmissions
   */
  uint8_t NbRetries;
  /*!
   * The delay which we have received through the
   * BeaconTimingAns
   */
  TimerTime_t BeaconTimingDelay;
  /*!
   * The channel of the next beacon
   */
  uint8_t BeaconTimingChannel;
} MlmeConfirm_t;

/*!
 * LoRaMAC MLME-Indication primitive
 */
typedef struct sMlmeIndication {
  /*!
   * MLME-Indication type
   */
  Mlme_t MlmeIndication;
  /*!
   * Status of the operation
   */
  LoRaMacEventInfoStatus_t Status;
  /*!
   * Downlink datarate
   */
  uint8_t RxDatarate;
  /*!
   * The downlink counter value for the received frame
   */
  uint32_t DownLinkCounter;
  /*!
   * Beacon information. Only valid for \ref MLME_BEACON,
   * status \ref LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED
   */
  BeaconInfo_t BeaconInfo;
} MlmeIndication_t;

/*!
 * LoRa Mac Information Base (MIB)
 *
 * The following table lists the MIB parameters and the related attributes:
 *
 * Attribute                                     | Get | Set
 * ----------------------------------------------| :-: | :-:
 * \ref MIB_DEVICE_CLASS                         | YES | YES
 * \ref MIB_NETWORK_ACTIVATION                   | YES | YES
 * \ref MIB_DEV_EUI                              | YES | YES
 * \ref MIB_JOIN_EUI                             | YES | YES
 * \ref MIB_ADR                                  | YES | YES
 * \ref MIB_NET_ID                               | YES | YES
 * \ref MIB_DEV_ADDR                             | YES | YES
 * \ref MIB_APP_KEY                              | NO  | YES
 * \ref MIB_NWK_KEY                              | NO  | YES
 * \ref MIB_J_S_INT_KEY                          | NO  | YES
 * \ref MIB_J_S_ENC_KEY                          | NO  | YES
 * \ref MIB_F_NWK_S_INT_KEY                      | NO  | YES
 * \ref MIB_S_NWK_S_INT_KEY                      | NO  | YES
 * \ref MIB_NWK_S_ENC_KEY                        | NO  | YES
 * \ref MIB_APP_S_KEY                            | NO  | YES
 * \ref MIB_MC_KE_KEY                            | NO  | YES
 * \ref MIB_MC_KEY_0                             | NO  | YES
 * \ref MIB_MC_APP_S_KEY_0                       | NO  | YES
 * \ref MIB_MC_NWK_S_KEY_0                       | NO  | YES
 * \ref MIB_MC_KEY_1                             | NO  | YES
 * \ref MIB_MC_APP_S_KEY_1                       | NO  | YES
 * \ref MIB_MC_NWK_S_KEY_1                       | NO  | YES
 * \ref MIB_MC_KEY_2                             | NO  | YES
 * \ref MIB_MC_APP_S_KEY_2                       | NO  | YES
 * \ref MIB_MC_NWK_S_KEY_2                       | NO  | YES
 * \ref MIB_MC_KEY_3                             | NO  | YES
 * \ref MIB_MC_APP_S_KEY_3                       | NO  | YES
 * \ref MIB_MC_NWK_S_KEY_3                       | NO  | YES
 * \ref MIB_PUBLIC_NETWORK                       | YES | YES
 * \ref MIB_REPEATER_SUPPORT                     | YES | YES
 * \ref MIB_CHANNELS                             | YES | NO
 * \ref MIB_RX2_CHANNEL                          | YES | YES
 * \ref MIB_RX2_DFAULT_CHANNEL                   | YES | YES
 * \ref MIB_RXC_CHANNEL                          | YES | YES
 * \ref MIB_RXC_DFAULT_CHANNEL                   | YES | YES
 * \ref MIB_CHANNELS_MASK                        | YES | YES
 * \ref MIB_CHANNELS_DEFAULT_MASK                | YES | YES
 * \ref MIB_CHANNELS_NB_TRANS                    | YES | YES
 * \ref MIB_MAX_RX_WINDOW_DURATION               | YES | YES
 * \ref MIB_RECEIVE_DELAY_1                      | YES | YES
 * \ref MIB_RECEIVE_DELAY_2                      | YES | YES
 * \ref MIB_JOIN_ACCEPT_DELAY_1                  | YES | YES
 * \ref MIB_JOIN_ACCEPT_DELAY_2                  | YES | YES
 * \ref MIB_CHANNELS_DATARATE                    | YES | YES
 * \ref MIB_CHANNELS_MIN_TX_DATARATE             | YES | NO
 * \ref MIB_CHANNELS_DEFAULT_DATARATE            | YES | YES
 * \ref MIB_CHANNELS_TX_POWER                    | YES | YES
 * \ref MIB_CHANNELS_DEFAULT_TX_POWER            | YES | YES
 * \ref MIB_SYSTEM_MAX_RX_ERROR                  | YES | YES
 * \ref MIB_MIN_RX_SYMBOLS                       | YES | YES
 * \ref MIB_BEACON_INTERVAL                      | YES | YES
 * \ref MIB_BEACON_RESERVED                      | YES | YES
 * \ref MIB_BEACON_GUARD                         | YES | YES
 * \ref MIB_BEACON_WINDOW                        | YES | YES
 * \ref MIB_BEACON_WINDOW_SLOTS                  | YES | YES
 * \ref MIB_PING_SLOT_WINDOW                     | YES | YES
 * \ref MIB_BEACON_SYMBOL_TO_DEFAULT             | YES | YES
 * \ref MIB_BEACON_SYMBOL_TO_EXPANSION_MAX       | YES | YES
 * \ref MIB_PING_SLOT_SYMBOL_TO_EXPANSION_MAX    | YES | YES
 * \ref MIB_BEACON_SYMBOL_TO_EXPANSION_FACTOR    | YES | YES
 * \ref MIB_PING_SLOT_SYMBOL_TO_EXPANSION_FACTOR | YES | YES
 * \ref MIB_MAX_BEACON_LESS_PERIOD               | YES | YES
 * \ref MIB_ANTENNA_GAIN                         | YES | YES
 * \ref MIB_DEFAULT_ANTENNA_GAIN                 | YES | YES
 * \ref MIB_NVM_CTXS                             | YES | YES
 * \ref MIB_NVM_BKP_CTXS                         | YES | YES
 * \ref MIB_ABP_LORAWAN_VERSION                  | NO  | YES
 * \ref MIB_LORAWAN_VERSION                      | YES | NO
 * \ref MIB_IS_CERT_FPORT_ON                     | YES | YES
 * \ref MIB_BEACON_STATE                         | YES | NO
 * \ref MIB_REJOIN_0_CYCLE                       | YES | YES
 * \ref MIB_REJOIN_1_CYCLE                       | YES | YES
 * \ref MIB_ADR_ACK_LIMIT                        | YES | YES
 * \ref MIB_ADR_ACK_DELAY                        | YES | YES
 * \ref MIB_ADR_ACK_DEFAULT_LIMIT                | YES | YES
 * \ref MIB_ADR_ACK_DEFAULT_DELAY                | YES | YES
 * \ref MIB_RSSI_FREE_THRESHOLD                  | YES | YES
 * \ref MIB_CARRIER_SENSE_TIME                   | YES | YES
 *
 * The following table provides links to the function implementations of the
 * related MIB primitives:
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MIB-Set          | \ref LoRaMacMibSetRequestConfirm
 * MIB-Get          | \ref LoRaMacMibGetRequestConfirm
 */
typedef enum eMib {
  /*!
   * LoRaWAN device class
   *
   * LoRaWAN Specification V1.0.2
   */
  MIB_DEVICE_CLASS,
  /*!
   * LoRaWAN Network End-Device Activation
   *
   * LoRaWAN Specification V1.0.2
   */
  MIB_NETWORK_ACTIVATION,
  /*!
   * LoRaWAN device EUI
   *
   * LoRaWAN Specification V1.0.2
   */
  MIB_DEV_EUI,
  /*!
   * LoRaWAN join EUI
   *
   * LoRaWAN Specification V1.0.2
   */
  MIB_JOIN_EUI,
  /*!
   * Adaptive data rate
   *
   * LoRaWAN Specification V1.0.2, chapter 4.3.1.1
   *
   * [true: ADR enabled, false: ADR disabled]
   */
  MIB_ADR,
  /*!
   * Network identifier
   *
   * LoRaWAN Specification V1.0.2, chapter 6.1.1
   */
  MIB_NET_ID,
  /*!
   * End-device address
   *
   * LoRaWAN Specification V1.0.2, chapter 6.1.1
   */
  MIB_DEV_ADDR,
  /*!
   * Application root key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.1.3
   */
  MIB_APP_KEY,
  /*!
   * Network root key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.1.3
   */
  MIB_NWK_KEY,
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01010100))
  /*!
   * Join session integrity key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.1.4
   */
  MIB_J_S_INT_KEY,
  /*!
   * Join session encryption key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.1.4
   */
  MIB_J_S_ENC_KEY,
  /*!
   * Forwarding Network session integrity key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.2.2
   */
  MIB_F_NWK_S_INT_KEY,
  /*!
   * Serving Network session integrity key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.2.3
   */
  MIB_S_NWK_S_INT_KEY,
  /*!
   * Network session encryption key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.2.4
   */
  MIB_NWK_S_ENC_KEY,
#else
  /*!
   * Network session key
   */
  MIB_NWK_S_KEY,
#endif /* LORAMAC_VERSION */
  /*!
   * Application session key
   *
   * LoRaWAN Specification V1.1.0, chapter 6.1.1.3
   */
  MIB_APP_S_KEY,
  /*!
   * Multicast key encryption key
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_KE_KEY,
#if (LORAMAC_MAX_MC_CTX > 0)
  /*!
   * Multicast root key index 0
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_KEY_0,
  /*!
   * Multicast Application session key index 0
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_APP_S_KEY_0,
  /*!
   * Multicast Network session key index 0
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_NWK_S_KEY_0,
#endif /* LORAMAC_MAX_MC_CTX > 0 */
#if (LORAMAC_MAX_MC_CTX > 1)
  /*!
   * Multicast root key index 1
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_KEY_1,
  /*!
   * Multicast Application session key index 1
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_APP_S_KEY_1,
  /*!
   * Multicast Network session key index 1
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_NWK_S_KEY_1,
#endif /* LORAMAC_MAX_MC_CTX > 1 */
#if (LORAMAC_MAX_MC_CTX > 2)
  /*!
   * Multicast root key index 2
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_KEY_2,
  /*!
   * Multicast Application session key index 2
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_APP_S_KEY_2,
  /*!
   * Multicast Network session key index 2
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_NWK_S_KEY_2,
#endif /* LORAMAC_MAX_MC_CTX > 2 */
#if (LORAMAC_MAX_MC_CTX > 3)
  /*!
   * Multicast root key index 3
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_KEY_3,
  /*!
   * Multicast Application session key index 3
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_APP_S_KEY_3,
  /*!
   * Multicast Network session key index 3
   *
   * LoRaWAN - Secure element specification v1
   */
  MIB_MC_NWK_S_KEY_3,
#endif /* LORAMAC_MAX_MC_CTX > 3 */
  /*!
   * Set the network type to public or private
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * [true: public network, false: private network]
   */
  MIB_PUBLIC_NETWORK,
  /*!
   * Support the operation with repeaters
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * [true: repeater support enabled, false: repeater support disabled]
   */
  MIB_REPEATER_SUPPORT,
  /*!
   * Communication channels. A get request will return a
   * pointer which references the first entry of the channel list. The
   * list is of size LORA_MAX_NB_CHANNELS
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   */
  MIB_CHANNELS,
  /*!
   * Set receive window 2 channel
   *
   * LoRaWAN Specification V1.0.2, chapter 3.3.1
   */
  MIB_RX2_CHANNEL,
  /*!
   * Set receive window 2 channel
   *
   * LoRaWAN Specification V1.0.2, chapter 3.3.2
   */
  MIB_RX2_DEFAULT_CHANNEL,
  /*!
   * Set receive window C channel
   *
   * LoRaWAN Specification V1.0.2, chapter 3.3.1
   */
  MIB_RXC_CHANNEL,
  /*!
   * Set receive window C channel
   *
   * LoRaWAN Specification V1.0.2, chapter 3.3.2
   */
  MIB_RXC_DEFAULT_CHANNEL,
  /*!
   * LoRaWAN channels mask
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   */
  MIB_CHANNELS_MASK,
  /*!
   * LoRaWAN default channels mask
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   */
  MIB_CHANNELS_DEFAULT_MASK,
  /*!
   * Set the number of repetitions on a channel
   *
   * LoRaWAN Specification V1.0.2, chapter 5.2, V1.1.0, chapter 5.3
   */
  MIB_CHANNELS_NB_TRANS,
  /*!
   * Maximum receive window duration in [ms]
   *
   * LoRaWAN Specification V1.0.2, chapter 3.3.3
   */
  MIB_MAX_RX_WINDOW_DURATION,
  /*!
   * Receive delay 1 in [ms]
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   */
  MIB_RECEIVE_DELAY_1,
  /*!
   * Receive delay 2 in [ms]
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   */
  MIB_RECEIVE_DELAY_2,
  /*!
   * Join accept delay 1 in [ms]
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   */
  MIB_JOIN_ACCEPT_DELAY_1,
  /*!
   * Join accept delay 2 in [ms]
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   */
  MIB_JOIN_ACCEPT_DELAY_2,
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  /*!
   * Minimum Data rate of a channel
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * The possible values are region specific. Please refer to \ref DR_0 to \ref
   * DR_15 for details.
   */
  MIB_CHANNELS_MIN_TX_DATARATE,
#endif /* LORAMAC_VERSION */
  /*!
   * Default Data rate of a channel
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * The allowed ranges are region specific. Please refer to \ref DR_0 to \ref
   * DR_15 for details.
   */
  MIB_CHANNELS_DEFAULT_DATARATE,
  /*!
   * Data rate of a channel
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * The allowed ranges are region specific. Please refer to \ref DR_0 to \ref
   * DR_15 for details.
   */
  MIB_CHANNELS_DATARATE,
  /*!
   * Transmission power of a channel
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * The allowed ranges are region specific. Please refer to \ref TX_POWER_0 to
   * \ref TX_POWER_15 for details.
   */
  MIB_CHANNELS_TX_POWER,
  /*!
   * Transmission power of a channel
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * The allowed ranges are region specific. Please refer to \ref TX_POWER_0 to
   * \ref TX_POWER_15 for details.
   */
  MIB_CHANNELS_DEFAULT_TX_POWER,
  /*!
   * System overall timing error in milliseconds.
   * [-SystemMaxRxError : +SystemMaxRxError]
   * Default: +/-10 ms
   */
  MIB_SYSTEM_MAX_RX_ERROR,
  /*!
   * Minimum required number of symbols to detect an Rx frame
   * Default: 6 symbols
   */
  MIB_MIN_RX_SYMBOLS,
  /*!
   * Antenna gain of the node. Default value is region specific.
   * The antenna gain is used to calculate the TX power of the node.
   * The formula is:
   * radioTxPower = ( int8_t )floor( maxEirp - antennaGain )
   *
   * \remark The antenna gain value is referenced to the isotropic antenna.
   *         The value is in dBi.
   *         MIB_ANTENNA_GAIN[dBi] = measuredAntennaGain[dBd] + 2.15
   */
  MIB_ANTENNA_GAIN,
  /*!
   * Default antenna gain of the node. Default value is region specific.
   * The antenna gain is used to calculate the TX power of the node.
   * The formula is:
   * radioTxPower = ( int8_t )floor( maxEirp - antennaGain )
   *
   * \remark The antenna gain value is referenced to the isotropic antenna.
   *         The value is in dBi.
   *         MIB_DEFAULT_ANTENNA_GAIN[dBi] = measuredAntennaGain[dBd] + 2.15
   */
  MIB_DEFAULT_ANTENNA_GAIN,
  /*!
   * Structure holding pointers to internal contexts and its size
   */
  MIB_NVM_CTXS,
  /*!
   * Structure holding pointers to internal backup contexts and its size
   */
  MIB_NVM_BKP_CTXS,
  /*!
   * LoRaWAN MAC layer operating version when activated by ABP.
   */
  MIB_ABP_LORAWAN_VERSION,
  /*!
   * LoRaWAN MAC and regional parameter version.
   */
  MIB_LORAWAN_VERSION,
  /*!
   * Time between periodic transmission of a Type 0 Rejoin request.
   */
  MIB_REJOIN_0_CYCLE,
  /*!
   * Time between periodic transmission of a Type 1 Rejoin request.
   */
  MIB_REJOIN_1_CYCLE,
  /*!
   * Beacon interval in ms
   */
  MIB_BEACON_INTERVAL,
  /*!
   * Beacon reserved time in ms
   */
  MIB_BEACON_RESERVED,
  /*!
   * Beacon guard time in ms
   */
  MIB_BEACON_GUARD,
  /*!
   * Beacon window time in ms
   */
  MIB_BEACON_WINDOW,
  /*!
   * Beacon window time in number of slots
   */
  MIB_BEACON_WINDOW_SLOTS,
  /*!
   * Ping slot length time in ms
   */
  MIB_PING_SLOT_WINDOW,
  /*!
   * Default symbol timeout for beacons and ping slot windows
   */
  MIB_BEACON_SYMBOL_TO_DEFAULT,
  /*!
   * Maximum symbol timeout for beacons
   */
  MIB_BEACON_SYMBOL_TO_EXPANSION_MAX,
  /*!
   * Maximum symbol timeout for ping slots
   */
  MIB_PING_SLOT_SYMBOL_TO_EXPANSION_MAX,
  /*!
   * Symbol expansion value for beacon windows in case of beacon
   * loss in symbols
   */
  MIB_BEACON_SYMBOL_TO_EXPANSION_FACTOR,
  /*!
   * Symbol expansion value for ping slot windows in case of beacon
   * loss in symbols
   */
  MIB_PING_SLOT_SYMBOL_TO_EXPANSION_FACTOR,
  /*!
   * Maximum allowed beacon less time in ms
   */
  MIB_MAX_BEACON_LESS_PERIOD,
  /*!
   * Ping slot data rate
   *
   * LoRaWAN Regional Parameters V1.0.2rB
   *
   * The allowed ranges are region specific. Please refer to \ref DR_0 to \ref
   * DR_15 for details.
   */
  MIB_PING_SLOT_DATARATE,
  /*!
   * Default response timeout for class b and class c confirmed downlink frames
   * in milli seconds.
   */
  MIB_RXB_C_TIMEOUT,
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  /*!
   * LoRaWAN certification FPort handling state (ON/OFF)
   */
  MIB_IS_CERT_FPORT_ON,
#endif /* LORAMAC_VERSION */
  /*!
   * ADR ack limit value
   */
  MIB_ADR_ACK_LIMIT,
  /*!
   * ADR ack delay value
   */
  MIB_ADR_ACK_DELAY,
  /*!
   * ADR ack default limit value
   */
  MIB_ADR_ACK_DEFAULT_LIMIT,
  /*!
   * ADR ack default delay value
   */
  MIB_ADR_ACK_DEFAULT_DELAY,
  /*!
   * RSSI free channel threshold value (KR920 and AS923 only)
   */
  MIB_RSSI_FREE_THRESHOLD,
  /*!
   * Carrier sense time value (KR920 and AS923 only)
   */
  MIB_CARRIER_SENSE_TIME,
  /*!
   * Beacon state
   */
  MIB_BEACON_STATE,
} Mib_t;

/*!
 * LoRaMAC MIB parameters
 */
typedef union uMibParam {
  /*!
   * LoRaWAN device class
   *
   * Related MIB type: \ref MIB_DEVICE_CLASS
   */
  DeviceClass_t Class;
  /*!
   * LoRaWAN Network End-Device Activation ( ACTIVATION_TYPE_NONE,
   * ACTIVATION_TYPE_ABP or OTTA )
   *
   * Related MIB type: \ref MIB_NETWORK_ACTIVATION
   */
  ActivationType_t NetworkActivation;
  /*!
   * LoRaWAN device EUI
   *
   * Related MIB type: \ref MIB_DEV_EUI
   */
  uint8_t *DevEui;
  /*!
   * LoRaWAN Join server EUI
   *
   * Related MIB type: \ref MIB_JOIN_EUI
   */
  uint8_t *JoinEui;
  /*!
   * Activation state of ADR
   *
   * Related MIB type: \ref MIB_ADR
   */
  bool AdrEnable;
  /*!
   * Network identifier
   *
   * Related MIB type: \ref MIB_NET_ID
   */
  uint32_t NetID;
  /*!
   * End-device address
   *
   * Related MIB type: \ref MIB_DEV_ADDR
   */
  uint32_t DevAddr;
  /*!
   * Application root key
   *
   * Related MIB type: \ref MIB_APP_KEY
   */
  uint8_t *AppKey;
  /*!
   * Network root key
   *
   * Related MIB type: \ref MIB_NWK_KEY
   */
  uint8_t *NwkKey;
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01010100))
  /*!
   * Join session integrity key
   *
   * Related MIB type: \ref MIB_J_S_INT_KEY
   */
  uint8_t *JSIntKey;
  /*!
   * Join session encryption key
   *
   * Related MIB type: \ref MIB_J_S_ENC_KEY
   */
  uint8_t *JSEncKey;
  /*!
   * Forwarding Network session integrity key
   *
   * Related MIB type: \ref MIB_F_NWK_S_INT_KEY
   */
  uint8_t *FNwkSIntKey;
  /*!
   * Serving Network session integrity key
   *
   * Related MIB type: \ref MIB_S_NWK_S_INT_KEY
   */
  uint8_t *SNwkSIntKey;
  /*!
   * Network session encryption key
   *
   * Related MIB type: \ref MIB_NWK_S_ENC_KEY
   */
  uint8_t *NwkSEncKey;
#else
  /*!
   * Network session key
   *
   * Related MIB type: \ref MIB_NWK_S_KEY
   */
  uint8_t *NwkSKey;
#endif /* LORAMAC_VERSION */
  /*!
   * Application session key
   *
   * Related MIB type: \ref MIB_APP_S_KEY
   */
  uint8_t *AppSKey;
  /*!
   * Multicast key encryption key
   *
   * Related MIB type: \ref MIB_MC_KE_KEY
   */
  uint8_t *McKEKey;
  /*!
   * Multicast root key index 0
   *
   * Related MIB type: \ref MIB_MC_KEY_0
   */
  uint8_t *McKey0;
  /*!
   * Multicast Application session key index 0
   *
   * Related MIB type: \ref MIB_MC_APP_S_KEY_0
   */
  uint8_t *McAppSKey0;
  /*!
   * Multicast Network session key index 0
   *
   * Related MIB type: \ref MIB_MC_NWK_S_KEY_0
   */
  uint8_t *McNwkSKey0;
  /*!
   * Multicast root key index 0
   *
   * Related MIB type: \ref MIB_MC_KEY_0
   */
  uint8_t *McKey1;
  /*!
   * Multicast Application session key index 1
   *
   * Related MIB type: \ref MIB_MC_APP_S_KEY_1
   */
  uint8_t *McAppSKey1;
  /*!
   * Multicast Network session key index 1
   *
   * Related MIB type: \ref MIB_MC_NWK_S_KEY_1
   */
  uint8_t *McNwkSKey1;
  /*!
   * Multicast root key index 2
   *
   * Related MIB type: \ref MIB_MC_KEY_2
   */
  uint8_t *McKey2;
  /*!
   * Multicast Application session key index 2
   *
   * Related MIB type: \ref MIB_MC_APP_S_KEY_2
   */
  uint8_t *McAppSKey2;
  /*!
   * Multicast Network session key index 2
   *
   * Related MIB type: \ref MIB_MC_NWK_S_KEY_2
   */
  uint8_t *McNwkSKey2;
  /*!
   * Multicast root key index 2
   *
   * Related MIB type: \ref MIB_MC_KEY_2
   */
  uint8_t *McKey3;
  /*!
   * Multicast Application session key index 2
   *
   * Related MIB type: \ref MIB_MC_APP_S_KEY_2
   */
  uint8_t *McAppSKey3;
  /*!
   * Multicast Network session key index 2
   *
   * Related MIB type: \ref MIB_MC_NWK_S_KEY_2
   */
  uint8_t *McNwkSKey3;
  /*!
   * Enable or disable a public network
   *
   * Related MIB type: \ref MIB_PUBLIC_NETWORK
   */
  bool EnablePublicNetwork;
  /*!
   * Enable or disable repeater support
   *
   * Related MIB type: \ref MIB_REPEATER_SUPPORT
   */
  bool EnableRepeaterSupport;
  /*!
   * LoRaWAN Channel
   *
   * Related MIB type: \ref MIB_CHANNELS
   */
  ChannelParams_t *ChannelList;
  /*!
   * Channel for the receive window 2
   *
   * Related MIB type: \ref MIB_RX2_CHANNEL
   */
  RxChannelParams_t Rx2Channel;
  /*!
   * Channel for the receive window 2
   *
   * Related MIB type: \ref MIB_RX2_DEFAULT_CHANNEL
   */
  RxChannelParams_t Rx2DefaultChannel;
  /*!
   * Channel for the receive window C
   *
   * Related MIB type: \ref MIB_RXC_CHANNEL
   */
  RxChannelParams_t RxCChannel;
  /*!
   * Channel for the receive window C
   *
   * Related MIB type: \ref MIB_RXC_DEFAULT_CHANNEL
   */
  RxChannelParams_t RxCDefaultChannel;
  /*!
   * Channel mask
   *
   * Related MIB type: \ref MIB_CHANNELS_MASK
   */
  uint16_t *ChannelsMask;
  /*!
   * Default channel mask
   *
   * Related MIB type: \ref MIB_CHANNELS_DEFAULT_MASK
   */
  uint16_t *ChannelsDefaultMask;
  /*!
   * Number of frame repetitions
   *
   * Related MIB type: \ref MIB_CHANNELS_NB_TRANS
   */
  uint8_t ChannelsNbTrans;
  /*!
   * Maximum receive window duration
   *
   * Related MIB type: \ref MIB_MAX_RX_WINDOW_DURATION
   */
  uint32_t MaxRxWindow;
  /*!
   * Receive delay 1
   *
   * Related MIB type: \ref MIB_RECEIVE_DELAY_1
   */
  uint32_t ReceiveDelay1;
  /*!
   * Receive delay 2
   *
   * Related MIB type: \ref MIB_RECEIVE_DELAY_2
   */
  uint32_t ReceiveDelay2;
  /*!
   * Join accept delay 1
   *
   * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_1
   */
  uint32_t JoinAcceptDelay1;
  /*!
   * Join accept delay 2
   *
   * Related MIB type: \ref MIB_JOIN_ACCEPT_DELAY_2
   */
  uint32_t JoinAcceptDelay2;
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  /*!
   * Channels minimum tx data rate
   *
   * Related MIB type: \ref MIB_CHANNELS_MIN_TX_DATARATE
   */
  int8_t ChannelsMinTxDatarate;
#endif /* LORAMAC_VERSION */
  /*!
   * Channels data rate
   *
   * Related MIB type: \ref MIB_CHANNELS_DEFAULT_DATARATE
   */
  int8_t ChannelsDefaultDatarate;
  /*!
   * Channels data rate
   *
   * Related MIB type: \ref MIB_CHANNELS_DATARATE
   */
  int8_t ChannelsDatarate;
  /*!
   * Channels TX power
   *
   * Related MIB type: \ref MIB_CHANNELS_DEFAULT_TX_POWER
   */
  int8_t ChannelsDefaultTxPower;
  /*!
   * Channels TX power
   *
   * Related MIB type: \ref MIB_CHANNELS_TX_POWER
   */
  int8_t ChannelsTxPower;
  /*!
   * Multicast channels
   *
   * Related MIB type: \ref MIB_MULTICAST_CHANNEL
   */
  McChannelParams_t MulticastChannel;
  /*!
   * System overall timing error in milliseconds.
   *
   * Related MIB type: \ref MIB_SYSTEM_MAX_RX_ERROR
   */
  uint32_t SystemMaxRxError;
  /*!
   * Minimum required number of symbols to detect an Rx frame
   *
   * Related MIB type: \ref MIB_MIN_RX_SYMBOLS
   */
  uint8_t MinRxSymbols;
  /*!
   * Antenna gain
   *
   * Related MIB type: \ref MIB_ANTENNA_GAIN
   */
  float AntennaGain;
  /*!
   * Default antenna gain
   *
   * Related MIB type: \ref MIB_DEFAULT_ANTENNA_GAIN
   */
  float DefaultAntennaGain;
  /*!
   * Returns a pointer to the structure holding all data which shall be stored
   * in the NVM.
   *
   * Related MIB type: \ref MIB_NVM_CTXS
   */
  void *Contexts;
  /*!
   * Returns a pointer to the backup structure holding all data which shall be
   * stored in the NVM.
   *
   * Related MIB type: \ref MIB_NVM_CTXS
   */
  void *BackupContexts;
  /*!
   * LoRaWAN MAC layer operating version when activated by ABP.
   *
   * Related MIB type: \ref MIB_ABP_LORAWAN_VERSION
   */
  Version_t AbpLrWanVersion;
  /*!
   * LoRaWAN MAC regional parameter version.
   *
   * Related MIB type: \ref MIB_LORAWAN_VERSION
   */
  struct sLrWanVersion {
    Version_t LoRaWan;
    Version_t LoRaWanRegion;
  } LrWanVersion;
  /*!
   * Time in seconds between cyclic transmission of Type 0 Rejoin requests.
   */
  uint32_t Rejoin0CycleInSec;
  /*!
   * Time in seconds between cyclic transmission of Type 1 Rejoin requests.
   */
  uint32_t Rejoin1CycleInSec;
  /*!
   * Time in seconds between cyclic transmission of Type 2 Rejoin requests.
   */
  uint32_t Rejoin2CycleInSec;
  /*!
   * Beacon interval in ms
   *
   * Related MIB type: \ref MIB_BEACON_INTERVAL
   */
  uint32_t BeaconInterval;
  /*!
   * Beacon reserved time in ms
   *
   * Related MIB type: \ref MIB_BEACON_RESERVED
   */
  uint32_t BeaconReserved;
  /*!
   * Beacon guard time in ms
   *
   * Related MIB type: \ref MIB_BEACON_GUARD
   */
  uint32_t BeaconGuard;
  /*!
   * Beacon window time in ms
   *
   * Related MIB type: \ref MIB_BEACON_WINDOW
   */
  uint32_t BeaconWindow;
  /*!
   * Beacon window time in number of slots
   *
   * Related MIB type: \ref MIB_BEACON_WINDOW_SLOTS
   */
  uint32_t BeaconWindowSlots;
  /*!
   * Ping slot length time in ms
   *
   * Related MIB type: \ref MIB_PING_SLOT_WINDOW
   */
  uint32_t PingSlotWindow;
  /*!
   * Default symbol timeout for beacons and ping slot windows
   *
   * Related MIB type: \ref MIB_BEACON_SYMBOL_TO_DEFAULT
   */
  uint32_t BeaconSymbolToDefault;
  /*!
   * Maximum symbol timeout for beacons
   *
   * Related MIB type: \ref MIB_BEACON_SYMBOL_TO_EXPANSION_MAX
   */
  uint32_t BeaconSymbolToExpansionMax;
  /*!
   * Maximum symbol timeout for ping slots
   *
   * Related MIB type: \ref MIB_PING_SLOT_SYMBOL_TO_EXPANSION_MAX
   */
  uint32_t PingSlotSymbolToExpansionMax;
  /*!
   * Symbol expansion value for beacon windows in case of beacon
   * loss in symbols
   *
   * Related MIB type: \ref MIB_BEACON_SYMBOL_TO_EXPANSION_FACTOR
   */
  uint32_t BeaconSymbolToExpansionFactor;
  /*!
   * Symbol expansion value for ping slot windows in case of beacon
   * loss in symbols
   *
   * Related MIB type: \ref MIB_PING_SLOT_SYMBOL_TO_EXPANSION_FACTOR
   */
  uint32_t PingSlotSymbolToExpansionFactor;
  /*!
   * Maximum allowed beacon less time in ms
   *
   * Related MIB type: \ref MIB_MAX_BEACON_LESS_PERIOD
   */
  uint32_t MaxBeaconLessPeriod;
  /*!
   * Ping slots data rate
   *
   * Related MIB type: \ref MIB_PING_SLOT_DATARATE
   */
  int8_t PingSlotDatarate;
  /*!
   * Default response timeout for class b and class c confirmed
   *
   * Related MIB type: \ref MIB_RXB_C_TIMEOUT
   */
  uint32_t RxBCTimeout;
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
  /*!
   * LoRaWAN certification FPort handling state (ON/OFF)
   *
   * Related MIB type: \ref MIB_IS_CERT_FPORT_ON
   */
  bool IsCertPortOn;
#endif /* LORAMAC_VERSION */
  /*!
   * ADR ack limit value
   *
   * Related MIB types: \ref MIB_ADR_ACK_LIMIT, MIB_ADR_ACK_DEFAULT_LIMIT
   */
  uint16_t AdrAckLimit;
  /*!
   * ADR ack delay value
   *
   * Related MIB types: \ref MIB_ADR_ACK_DELAY, MIB_ADR_ACK_DEFAULT_DELAY
   */
  uint16_t AdrAckDelay;
  /*!
   * RSSI free channel threshold (KR920 and AS923 only)
   *
   * Related MIB type: \ref MIB_RSSI_FREE_THRESHOLD
   */
  int16_t RssiFreeThreshold;
  /*!
   * Carrier sense time (KR920 and AS923 only)
   *
   * Related MIB type: \ref MIB_CARRIER_SENSE_TIME
   */
  uint32_t CarrierSenseTime;
  /*!
   * State of the beaconing mechanism
   *
   * Related MIB type: \ref MIB_BEACON_STATE
   */
  BeaconState_t BeaconState;
} MibParam_t;

/*!
 * LoRaMAC MIB-RequestConfirm structure
 */
typedef struct eMibRequestConfirm {
  /*!
   * MIB-Request type
   */
  Mib_t Type;

  /*!
   * MLME-RequestConfirm parameters
   */
  MibParam_t Param;
} MibRequestConfirm_t;

/*!
 * LoRaMAC tx information
 */
typedef struct sLoRaMacTxInfo {
  /*!
   * Size of the application data payload which can be transmitted.
   */
  uint8_t MaxPossibleApplicationDataSize;
  /*!
   * The current maximum possible payload size without MAC commands
   * which is dependent on the current datarate.
   */
  uint8_t CurrentPossiblePayloadSize;
} LoRaMacTxInfo_t;

/*!
 * LoRaMAC Status
 */
typedef enum eLoRaMacStatus {
  /*!
   * Service started successfully
   */
  LORAMAC_STATUS_OK,
  /*!
   * Service not started - LoRaMAC is busy
   */
  LORAMAC_STATUS_BUSY,
  /*!
   * Service unknown
   */
  LORAMAC_STATUS_SERVICE_UNKNOWN,
  /*!
   * Service not started - invalid parameter
   */
  LORAMAC_STATUS_PARAMETER_INVALID,
  /*!
   * Service not started - invalid frequency
   */
  LORAMAC_STATUS_FREQUENCY_INVALID,
  /*!
   * Service not started - invalid datarate
   */
  LORAMAC_STATUS_DATARATE_INVALID,
  /*!
   * Service not started - invalid frequency and datarate
   */
  LORAMAC_STATUS_FREQ_AND_DR_INVALID,
  /*!
   * Service not started - the device is not in a LoRaWAN
   */
  LORAMAC_STATUS_NO_NETWORK_JOINED,
  /*!
   * Service not started - payload length error
   */
  LORAMAC_STATUS_LENGTH_ERROR,
  /*!
   * Service not started - the specified region is not supported
   * or not activated with preprocessor definitions.
   */
  LORAMAC_STATUS_REGION_NOT_SUPPORTED,
  /*!
   * The application data was not transmitted
   * because prioritized pending MAC commands had to be sent.
   */
  LORAMAC_STATUS_SKIPPED_APP_DATA,
  /*!
   * An MCPS or MLME request can return this status. In this case,
   * the MAC cannot send the frame, as the duty cycle limits all
   * available bands. When a request returns this value, the
   * variable "DutyCycleWaitTime" in "ReqReturn" of the input
   * parameters contains the remaining time to wait. If the
   * value is constant and does not change, the expected time
   * on air for this frame is exceeding the maximum permitted
   * time according to the duty cycle time period, defined
   * in Region.h, DUTY_CYCLE_TIME_PERIOD. By default this time
   * is 1 hour, and a band with 1% duty cycle is then allowed
   * to use an air time of 36 seconds.
   */
  LORAMAC_STATUS_DUTYCYCLE_RESTRICTED,
  /*!
   * No active channel is available to send a request
   */
  LORAMAC_STATUS_NO_CHANNEL_FOUND,
  /*!
   * Even if one or more channels are available according to the channel plan,
   * no free channel was found during the LBT procedure
   */
  LORAMAC_STATUS_NO_FREE_CHANNEL_FOUND,
  /*!
   * Next request cannot be sent because the RX window is required by the beacon
   */
  LORAMAC_STATUS_BUSY_BEACON_RESERVED_TIME,
  /*!
   * Next request cannot be sent because the RX window is required by the ping
   * slot
   */
  LORAMAC_STATUS_BUSY_PING_SLOT_WINDOW_TIME,
  /*!
   * Next beacon will be received during the next uplink
   */
  LORAMAC_STATUS_BUSY_UPLINK_COLLISION,
  /*!
   * An error in the cryptographic module is occurred
   */
  LORAMAC_STATUS_CRYPTO_ERROR,
  /*!
   * An error in the frame counter handler module is occurred
   */
  LORAMAC_STATUS_FCNT_HANDLER_ERROR,
  /*!
   * An error in the MAC command module is occurred
   */
  LORAMAC_STATUS_MAC_COMMAD_ERROR,
  /*!
   * An error in the Class B module is occurred
   */
  LORAMAC_STATUS_CLASS_B_ERROR,
  /*!
   * An error in the Confirm Queue module is occurred
   */
  LORAMAC_STATUS_CONFIRM_QUEUE_ERROR,
  /*!
   * The multicast group doesn't exist
   */
  LORAMAC_STATUS_MC_GROUP_UNDEFINED,
  /*!
   * NVM Data inconsistent
   */
  LORAMAC_STATUS_NVM_DATA_INCONSISTENT,
  /*!
   * Undefined error occurred
   */
  LORAMAC_STATUS_ERROR
} LoRaMacStatus_t;

/*!
 * LoRaMAC events structure
 * Used to notify upper layers of MAC events
 */
typedef struct sLoRaMacPrimitives {
  /*!
   * \brief   MCPS-Confirm primitive
   *
   * \param   [out] MCPS-Confirm parameters
   */
  void (*MacMcpsConfirm)(McpsConfirm_t *McpsConfirm);
  /*!
   * \brief   MCPS-Indication primitive
   *
   * \param   [out] MCPS-Indication parameters
   */
  void (*MacMcpsIndication)(McpsIndication_t *McpsIndication,
                            LoRaMacRxStatus_t *RxStatus);
  /*!
   * \brief   MLME-Confirm primitive
   *
   * \param   [out] MLME-Confirm parameters
   */
  void (*MacMlmeConfirm)(MlmeConfirm_t *MlmeConfirm);
  /*!
   * \brief   MLME-Indication primitive
   *
   * \param   [out] MLME-Indication parameters
   */
  void (*MacMlmeIndication)(MlmeIndication_t *MlmeIndication,
                            LoRaMacRxStatus_t *RxStatus);
} LoRaMacPrimitives_t;

/*!
 * LoRaMAC callback structure
 */
typedef struct sLoRaMacCallback {
  /*!
   * \brief   Measures the battery level
   *
   * \retval  Battery level [0: node is connected to an external
   *          power source, 1..254: battery level, where 1 is the minimum
   *          and 254 is the maximum value, 255: the node was not able
   *          to measure the battery level]
   */
  uint8_t (*GetBatteryLevel)(void);
  /*!
   * \brief   Measures the temperature level
   *
   * \retval  Temperature level
   */
  int16_t (*GetTemperatureLevel)(void);
  /*!
   * \brief   Get the board 64 bits unique ID
   *
   * \param   [out] id unique
   */
  void (*GetUniqueId)(uint8_t *id);
  /*!
   * \brief   Get the 64 bits unique Device address
   *
   * \param   [out] id unique
   */
  void (*GetDevAddress)(uint32_t *devAddr);
  /*!
   * \brief   Will be called when an attribute has changed in one of the
   * context.
   *
   * \param   notifyFlags Bitmap that contains the modules which changed.
   *                      Refer to \ref LoRaMacNvmData_t.
   */
  void (*NvmDataChange)(uint16_t notifyFlags);
  /*!
   *\brief    Will be called each time a Radio IRQ is handled by the MAC
   *          layer.
   *
   *\warning  Runs in a IRQ context. Should only change variables state.
   */
  void (*MacProcessNotify)(void);
} LoRaMacCallback_t;

/*! \} defgroup LORAMAC */

#ifdef __cplusplus
}
#endif

#endif // __LORAMAC_INTERFACES_H__
