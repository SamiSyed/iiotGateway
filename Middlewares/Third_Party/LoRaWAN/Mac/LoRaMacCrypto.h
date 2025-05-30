/*!
 * \file      LoRaMacCrypto.h
 *
 * \brief     LoRa MAC layer cryptographic functionality implementation
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
 * addtogroup LORAMAC
 * \{
 *
 */
#ifndef __LORAMAC_CRYPTO_H__
#define __LORAMAC_CRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "LoRaMacCryptoNvm.h"
#include "LoRaMacMessageTypes.h"
#include "LoRaMacTypes.h"
#include "LoRaMacVersion.h"
#include "utilities.h"

/*!
 * Indicates if a random devnonce must be used or not
 */
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01000300))
#define USE_RANDOM_DEV_NONCE 1
#elif (defined(LORAMAC_VERSION) &&                                             \
       ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
#define USE_RANDOM_DEV_NONCE 0
#endif /* LORAMAC_VERSION */

/*!
 * Indicates if JoinNonce is counter based and requires to be checked on 1.0.x
 * devices \remark Only applies to LoRaWAN 1.0.x when following recommendations
 * provided by "Technical Recommendations for Preventing State Synchronization
 *         Issues around LoRaWAN 1.0.x Join Procedure
 *         https://lora-alliance.org/wp-content/uploads/2020/11/lorawan-1.0.x-join-synch-issues-remedies-v1.0.0.pdf
 */
#if (defined(LORAMAC_VERSION) &&                                               \
     ((LORAMAC_VERSION == 0x01000300) || (LORAMAC_VERSION == 0x01000400)))
#define USE_10X_JOIN_NONCE_COUNTER_CHECK 1
#elif (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01010100))
#define USE_10X_JOIN_NONCE_COUNTER_CHECK 0
#endif /* LORAMAC_VERSION */

/*!
 * Initial value of the frame counters
 */
#define FCNT_DOWN_INITIAL_VALUE 0xFFFFFFFF

/*!
 * LoRaMac Crypto Status
 */
typedef enum eLoRaMacCryptoStatus {
  /*!
   * No error occurred
   */
  LORAMAC_CRYPTO_SUCCESS = 0,
  /*!
   * MIC does not match
   */
  LORAMAC_CRYPTO_FAIL_MIC,
  /*!
   * Address does not match
   */
  LORAMAC_CRYPTO_FAIL_ADDRESS,
  /*!
   * JoinNonce was not greater than previous one.
   */
  LORAMAC_CRYPTO_FAIL_JOIN_NONCE,
  /*!
   * RJcount0 reached 2^16-1
   */
  LORAMAC_CRYPTO_FAIL_RJCOUNT0_OVERFLOW,
  /*!
   * FCNT_ID is not supported
   */
  LORAMAC_CRYPTO_FAIL_FCNT_ID,
  /*!
   * FCntUp/Down check failed (new FCnt is smaller than previous one)
   */
  LORAMAC_CRYPTO_FAIL_FCNT_SMALLER,
  /*!
   * FCntUp/Down check failed (duplicated)
   */
  LORAMAC_CRYPTO_FAIL_FCNT_DUPLICATED,
#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01000300))
  /*!
   * MAX_GAP_FCNT check failed
   */
  LORAMAC_CRYPTO_FAIL_MAX_GAP_FCNT,
#endif /* LORAMAC_VERSION */
  /*!
   * Not allowed parameter value
   */
  LORAMAC_CRYPTO_FAIL_PARAM,
  /*!
   * Null pointer exception
   */
  LORAMAC_CRYPTO_ERROR_NPE,
  /*!
   * Invalid key identifier exception
   */
  LORAMAC_CRYPTO_ERROR_INVALID_KEY_ID,
  /*!
   * Invalid address identifier exception
   */
  LORAMAC_CRYPTO_ERROR_INVALID_ADDR_ID,
  /*!
   * Invalid LoRaWAN specification version
   */
  LORAMAC_CRYPTO_ERROR_INVALID_VERSION,
  /*!
   * Incompatible buffer size
   */
  LORAMAC_CRYPTO_ERROR_BUF_SIZE,
  /*!
   * The secure element reports an error
   */
  LORAMAC_CRYPTO_ERROR_SECURE_ELEMENT_FUNC,
  /*!
   * Error from parser reported
   */
  LORAMAC_CRYPTO_ERROR_PARSER,
  /*!
   * Error from serializer reported
   */
  LORAMAC_CRYPTO_ERROR_SERIALIZER,
  /*!
   * RJcount1 reached 2^16-1 which should never happen
   */
  LORAMAC_CRYPTO_ERROR_RJCOUNT1_OVERFLOW,
  /*!
   * Undefined Error occurred
   */
  LORAMAC_CRYPTO_ERROR,
} LoRaMacCryptoStatus_t;

/*!
 * Signature of callback function to be called by the LoRaMac Crypto module when
 * the non-volatile context have to be stored. It is also possible to save the
 * entire crypto module context.
 *
 */
typedef void (*LoRaMacCryptoNvmEvent)(void);

/*!
 * Initialization of LoRaMac Crypto module
 * It sets initial values of volatile variables and assigns the non-volatile
 * context.
 *
 * \param [in]    nvm                 - Pointer to the non-volatile memory data
 *                                      structure.
 * \retval                            - Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoInit(LoRaMacCryptoNvmData_t *nvm);

/*!
 * Sets the LoRaWAN specification version to be used.
 *
 * \warning This function should be used for ABP only. In case of OTA the
 * version will be set automatically.
 *
 * \param [in]    version             - LoRaWAN specification version to be
 * used.
 *
 * \retval                            - Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoSetLrWanVersion(Version_t version);

#if (defined(LORAMAC_VERSION) && (LORAMAC_VERSION == 0x01000300))
/*!
 * Returns updated fCntID downlink counter value.
 *
 * \param[IN]     fCntID         - Frame counter identifier
 * \param[IN]     maxFcntGap     - Maximum allowed frame counter difference
 * (only necessary for L2 LW1.0.x) \param[IN]     frameFcnt      - Received
 * frame counter (used to update current counter value) \param[OUT] currentDown
 * - Current downlink counter value \retval - Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoGetFCntDown(FCntIdentifier_t fCntID,
                                               uint16_t maxFCntGap,
                                               uint32_t frameFcnt,
                                               uint32_t *currentDown);
#elif (defined(LORAMAC_VERSION) &&                                             \
       ((LORAMAC_VERSION == 0x01000400) || (LORAMAC_VERSION == 0x01010100)))
/*!
 * Returns updated fCntID downlink counter value.
 *
 * \param [in]    fCntID         - Frame counter identifier
 * \param [in]    frameFcnt      - Received frame counter (used to update
 * current counter value) \param [out]   currentDown    - Current downlink
 * counter value \retval                       - Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoGetFCntDown(FCntIdentifier_t fCntID,
                                               uint32_t frameFcnt,
                                               uint32_t *currentDown);
#endif /* LORAMAC_VERSION */

/*!
 * Returns updated fCntUp uplink counter value.
 *
 * \param [in]    currentUp      - Uplink counter value
 * \retval                       - Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoGetFCntUp(uint32_t *currentUp);

/*!
 * Computes next RJcount0 or RJcount1 counter value.
 *
 * \param[IN]     fCntID          - Frame counter identifier
 * \param[OUT]    rJcount         - RJcount value
 *
 * \retval                        - Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoGetRJcount(FCntIdentifier_t fCntID,
                                              uint16_t *rJcount);

/*!
 * Provides multicast context.
 *
 * \param [in]    multicastList - Pointer to the multicast context list
 *
 * \retval                      - Status of the operation
 */
LoRaMacCryptoStatus_t
LoRaMacCryptoSetMulticastReference(MulticastCtx_t *multicastList);

/*!
 * Sets a key
 *
 * \param [in]    keyID          - Key identifier
 * \param [in]    key            - Key value (16 byte), if its a multicast key
 * it must be encrypted with McKEKey \retval                       - Status of
 * the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoSetKey(KeyIdentifier_t keyID, uint8_t *key);

/*!
 * Prepares the join-request message.
 * It computes the mic and add it to the message.
 *
 * \param [in,out] macMsg        - Join-request message object
 * \retval                       - Status of the operation
 */
LoRaMacCryptoStatus_t
LoRaMacCryptoPrepareJoinRequest(LoRaMacMessageJoinRequest_t *macMsg);

/*!
 * Prepares a rejoin-request type 1 message.
 * It computes the mic and add it to the message.
 *
 * \param [in,out] macMsg        - Rejoin message object
 * \retval                       - Status of the operation
 */
LoRaMacCryptoStatus_t
LoRaMacCryptoPrepareReJoinType1(LoRaMacMessageReJoinType1_t *macMsg);

/*!
 * Prepares a rejoin-request type 0 or 2 message.
 * It computes the mic and add it to the message.
 *
 * \param [in,out] macMsg        - Rejoin message object
 * \retval                       - Status of the operation
 */
LoRaMacCryptoStatus_t
LoRaMacCryptoPrepareReJoinType0or2(LoRaMacMessageReJoinType0or2_t *macMsg);

/*!
 * Handles the join-accept message.
 * It decrypts the message, verifies the MIC and if successful derives the
 * session keys.
 *
 * \param [in]    joinReqType    - Type of last join-request or rejoin which
 * triggered the join-accept response \param [in]    joinEUI        - Join
 * server EUI (8 byte) \param [in,out] macMsg        - Join-accept message
 * object \retval                       - Status of the operation
 */
LoRaMacCryptoStatus_t
LoRaMacCryptoHandleJoinAccept(JoinReqIdentifier_t joinReqType, uint8_t *joinEUI,
                              LoRaMacMessageJoinAccept_t *macMsg);

/*!
 * Secures a message (encryption + integrity).
 *
 * \param [in]    fCntUp          - Uplink sequence counter
 * \param [in]    txDr            - Data rate used for the transmission
 * \param [in]    txCh            - Index of the channel used for the
 * transmission \param [in,out] macMsg         - Data message object \retval -
 * Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoSecureMessage(uint32_t fCntUp, uint8_t txDr,
                                                 uint8_t txCh,
                                                 LoRaMacMessageData_t *macMsg);

/*!
 * Unsecures a message (decryption + integrity verification).
 *
 * \param [in]    addrID          - Address identifier
 * \param [in]    address         - Address
 * \param [in]    fCntID          - Frame counter identifier
 * \param [in]    fCntDown        - Downlink sequence counter
 * \param [in,out] macMsg         - Data message object
 * \retval                        - Status of the operation
 */
LoRaMacCryptoStatus_t
LoRaMacCryptoUnsecureMessage(AddressIdentifier_t addrID, uint32_t address,
                             FCntIdentifier_t fCntID, uint32_t fCntDown,
                             LoRaMacMessageData_t *macMsg);

LoRaMacCryptoStatus_t
LoRaMacCryptoComputeDataBlock(uint8_t *buffer, uint32_t size,
                              uint16_t sessionCnt, uint8_t fragIndex,
                              uint32_t descriptor, uint32_t *cmac);

/*!
 * Derives the LifeTime keys
 *
 * McRootKey
 * 1.0.x: aes128_encrypt(AppKey, 0x00 | pad16)
 * 1.1.x: aes128_encrypt(AppKey, 0x20 | pad16)
 *
 * McKEKey = aes128_encrypt(McRootKey , 0x00 | pad16)
 *
 * DataBlockIntKey = aes128_encrypt(AppKey , 0x30 | pad16)
 *
 * \param [in]    versionMinor    - LoRaWAN specification minor version to be
 * used (only used for McRootKey) \param [in]    keyID           - Key
 * identifier \retval                        - Status of the operation
 */
LoRaMacCryptoStatus_t LoRaMacCryptoDeriveLifeTimeKey(uint8_t versionMinor,
                                                     KeyIdentifier_t keyID);

/*!
 * Derives a Multicast group key pair ( McAppSKey, McNwkSKey ) from McKey
 *
 * McAppSKey = aes128_encrypt(McKey, 0x01 | McAddr | pad16)
 * McNwkSKey = aes128_encrypt(McKey, 0x02 | McAddr | pad16)
 *
 * \param [in]    addrID          - Address identifier to select the multicast
 * group \param [in]    mcAddr          - Multicast group address (4 bytes)
 * \retval                        - Status of the operation
 */
LoRaMacCryptoStatus_t
LoRaMacCryptoDeriveMcSessionKeyPair(AddressIdentifier_t addrID,
                                    uint32_t mcAddr);

/*! \} addtogroup LORAMAC */

#ifdef __cplusplus
}
#endif

#endif // __LORAMAC_CRYPTO_H__
