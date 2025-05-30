/*
 ---------------------------------------------------------------------------
 Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.

 LICENSE TERMS

 The redistribution and use of this software (with or without changes)
 is allowed without the payment of fees or royalties provided that:

  1. source code distributions include the above copyright notice, this
     list of conditions and the following disclaimer;

  2. binary distributions include the above copyright notice, this list
     of conditions and the following disclaimer in their documentation;

  3. the name of the copyright holder is not used to endorse products
     built using this software without specific written permission.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue 09/09/2006

 This is an AES implementation that uses only 8-bit byte operations on the
 cipher state.
 */

#ifndef LORAWAN_AES_H
#define LORAWAN_AES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if 1
#define AES_ENC_PREKEYED /* AES encryption with a precomputed key schedule  */
#endif
#if 0
#define AES_DEC_PREKEYED /* AES decryption with a precomputed key schedule  */
#endif
#if 0
#define AES_ENC_128_OTFK /* AES encryption with 'on the fly' 128 bit keying */
#endif
#if 0
#define AES_DEC_128_OTFK /* AES decryption with 'on the fly' 128 bit keying */
#endif
#if 0
#define AES_ENC_256_OTFK /* AES encryption with 'on the fly' 256 bit keying */
#endif
#if 0
#define AES_DEC_256_OTFK /* AES decryption with 'on the fly' 256 bit keying */
#endif

#define N_ROW 4
#define N_COL 4
#define N_BLOCK (N_ROW * N_COL)
#define N_MAX_ROUNDS 14

typedef uint8_t return_type;

/*  Warning: The key length for 256 bit keys overflows a byte
    (see comment below)
*/

typedef uint8_t length_type;

typedef struct {
  uint8_t ksch[(N_MAX_ROUNDS + 1) * N_BLOCK];
  uint8_t rnd;
} lorawan_aes_context;

/*  The following calls are for a precomputed key schedule

    NOTE: If the length_type used for the key length is an
    unsigned 8-bit character, a key length of 256 bits must
    be entered as a length in bytes (valid inputs are hence
    128, 192, 16, 24 and 32).
*/

#if defined(AES_ENC_PREKEYED) || defined(AES_DEC_PREKEYED)

return_type lorawan_aes_set_key(const uint8_t key[], length_type keylen,
                                lorawan_aes_context ctx[1]);
#endif

#if defined(AES_ENC_PREKEYED)

return_type lorawan_aes_encrypt(const uint8_t in[N_BLOCK], uint8_t out[N_BLOCK],
                                const lorawan_aes_context ctx[1]);

return_type lorawan_aes_cbc_encrypt(const uint8_t *in, uint8_t *out,
                                    int32_t n_block, uint8_t iv[N_BLOCK],
                                    const lorawan_aes_context ctx[1]);
#endif

#if defined(AES_DEC_PREKEYED)

return_type lorawan_aes_decrypt(const uint8_t in[N_BLOCK], uint8_t out[N_BLOCK],
                                const lorawan_aes_context ctx[1]);

return_type lorawan_aes_cbc_decrypt(const uint8_t *in, uint8_t *out,
                                    int32_t n_block, uint8_t iv[N_BLOCK],
                                    const lorawan_aes_context ctx[1]);
#endif

/*  The following calls are for 'on the fly' keying.  In this case the
    encryption and decryption keys are different.

    The encryption subroutines take a key in an array of bytes in
    key[L] where L is 16, 24 or 32 bytes for key lengths of 128,
    192, and 256 bits respectively.  They then encrypts the input
    data, in[] with this key and put the reult in the output array
    out[].  In addition, the second key array, o_key[L], is used
    to output the key that is needed by the decryption subroutine
    to reverse the encryption operation.  The two key arrays can
    be the same array but in this case the original key will be
    overwritten.

    In the same way, the decryption subroutines output keys that
    can be used to reverse their effect when used for encryption.

    Only 128 and 256 bit keys are supported in these 'on the fly'
    modes.
*/

#if defined(AES_ENC_128_OTFK)
void lorawan_aes_encrypt_128(const uint8_t in[N_BLOCK], uint8_t out[N_BLOCK],
                             const uint8_t key[N_BLOCK],
                             uint8_t o_key[N_BLOCK]);
#endif

#if defined(AES_DEC_128_OTFK)
void lorawan_aes_decrypt_128(const uint8_t in[N_BLOCK], uint8_t out[N_BLOCK],
                             const uint8_t key[N_BLOCK],
                             uint8_t o_key[N_BLOCK]);
#endif

#if defined(AES_ENC_256_OTFK)
void lorawan_aes_encrypt_256(const uint8_t in[N_BLOCK], uint8_t out[N_BLOCK],
                             const uint8_t key[2 * N_BLOCK],
                             uint8_t o_key[2 * N_BLOCK]);
#endif

#if defined(AES_DEC_256_OTFK)
void lorawan_aes_decrypt_256(const uint8_t in[N_BLOCK], uint8_t out[N_BLOCK],
                             const uint8_t key[2 * N_BLOCK],
                             uint8_t o_key[2 * N_BLOCK]);
#endif

#ifdef __cplusplus
}
#endif

#endif
