/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

/*!
 * \file ldpc_encoder.h
 * \brief Declaration of the LDPC encoder.
 * \author David Gregoratti and Jesus Gomez
 * \date 2020
 *
 * \copyright Software Radio Systems Limited
 *
 */

#ifndef SRSLTE_LDPCENCODER_H
#define SRSLTE_LDPCENCODER_H

#include "srslte/phy/fec/ldpc/base_graph.h"

/*!
 * \brief Types of LDPC encoder.
 */
typedef enum SRSLTE_API {
  SRSLTE_LDPC_ENCODER_C = 0, /*!< \brief Non-optimized encoder. */
#if LV_HAVE_AVX2
  SRSLTE_LDPC_ENCODER_AVX2, /*!< \brief SIMD-optimized encoder. */
#endif                      // LV_HAVE_AVX2
#if LV_HAVE_AVX512
  SRSLTE_LDPC_ENCODER_AVX512, /*!< \brief SIMD-optimized encoder. */
#endif                        // LV_HAVE_AVX512
} srslte_ldpc_encoder_type_t;

/*!
 * \brief Describes an LDPC encoder.
 */
typedef struct SRSLTE_API {
  void*              ptr;   /*!< \brief %Encoder auxiliary registers. */
  srslte_basegraph_t bg;    /*!< \brief Current base graph. */
  uint16_t           ls;    /*!< \brief Current lifting size. */
  uint8_t            bgN;   /*!< \brief Number of variable nodes in the BG. */
  uint16_t           liftN; /*!< \brief Number of variable nodes in the lifted graph. */
  uint8_t            bgM;   /*!< \brief Number of check nodes in the BG. */
  uint16_t           liftM; /*!< \brief Number of check nodes in the lifted graph. */
  uint8_t            bgK;   /*!< \brief Number of "uncoded bits" in the BG. */
  uint16_t           liftK; /*!< \brief Number of uncoded bits in the lifted graph. */
  uint16_t*          pcm;   /*!< \brief Pointer to the parity check matrix (compact form). */
  void (*free)(void*);      /*!< \brief Pointer to a "destructor". */
  /*! \brief Pointer to the encoder function. */
  int (*encode)(void*, const uint8_t*, uint8_t*, uint32_t, uint32_t);
  /*!  \brief Pointer to the encoder for the high-rate region. */
  void (*encode_high_rate)(void*, uint8_t*);
  /*!  \brief Pointer to the encoder for the high-rate region (SIMD-optimized version). */
  void (*encode_high_rate_avx2)(void*);
  /*!  \brief Pointer to the encoder for the high-rate region (SIMD-AVX512-optimized version). */
  void (*encode_high_rate_avx512)(void*);

} srslte_ldpc_encoder_t;

/*!
 * Initializes all the LDPC encoder variables according to the given base graph
 * and lifting size.
 * \param[out] q A pointer to a srslte_ldpc_encoder_t structure.
 * \param[in] type The encoder type.
 * \param[in] bg The desired base graph (BG1 or BG2).
 * \param[in] ls The desired lifting size.
 * \return An integer: 0 if the function executes correctly, -1 otherwise.
 */
SRSLTE_API int
srslte_ldpc_encoder_init(srslte_ldpc_encoder_t* q, srslte_ldpc_encoder_type_t type, srslte_basegraph_t bg, uint16_t ls);

/*!
 * The LDPC encoder "destructor": it frees all the resources allocated to the encoder.
 * \param[in] q A pointer to the dismantled encoder.
 */
SRSLTE_API void srslte_ldpc_encoder_free(srslte_ldpc_encoder_t* q);

/*!
 * Encodes a message into a codeword with the specified encoder.
 * \param[in] q A pointer to the desired encoder.
 * \param[in] input The message to encode.
 * \param[out] output The resulting codeword.
 * \param[in] input_length The number of uncoded bits in the input message.
 * \return An integer: 0 if the function executes correctly, -1 otherwise.
 */
SRSLTE_API int
srslte_ldpc_encoder_encode(srslte_ldpc_encoder_t* q, const uint8_t* input, uint8_t* output, uint32_t input_length);

/*!
 * Encodes a message into a codeword with the specified encoder.
 * \param[in] q A pointer to the desired encoder.
 * \param[in] input The message to encode.
 * \param[out] output The resulting codeword.
 * \param[in] input_length The number of uncoded bits in the input message.
 * \param[in] cdwd_rm_length The codeword length after rate matching.
 * \return An integer: 0 if the function executes correctly, -1 otherwise.
 */
SRSLTE_API int srslte_ldpc_encoder_encode_rm(srslte_ldpc_encoder_t* q,
                                             const uint8_t*         input,
                                             uint8_t*               output,
                                             uint32_t               input_length,
                                             uint32_t               cdwd_rm_length);

#endif // SRSLTE_LDPCENCODER_H
