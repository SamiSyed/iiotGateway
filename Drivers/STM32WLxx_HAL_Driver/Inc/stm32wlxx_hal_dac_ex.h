/**
 ******************************************************************************
 * @file    stm32wlxx_hal_dac_ex.h
 * @author  MCD Application Team
 * @brief   Header file of DAC HAL Extended module.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WLxx_HAL_DAC_EX_H
#define STM32WLxx_HAL_DAC_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup STM32WLxx_HAL_Driver
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal_def.h"

#if defined(DAC)

/** @addtogroup DACEx
 * @{
 */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  HAL State structures definition
 */

/* Exported constants --------------------------------------------------------*/

/** @defgroup DACEx_Exported_Constants DACEx Exported Constants
 * @{
 */

/** @defgroup DACEx_lfsrunmask_triangleamplitude DACEx lfsrunmask triangle
 * amplitude
 * @{
 */
#define DAC_LFSRUNMASK_BIT0                                                    \
  0x00000000UL /*!< Unmask DAC channel LFSR bit0 for noise wave generation */
#define DAC_LFSRUNMASK_BITS1_0                                                 \
  (DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[1:0] for noise wave        \
                      generation */
#define DAC_LFSRUNMASK_BITS2_0                                                 \
  (DAC_CR_MAMP1_1) /*!< Unmask DAC channel LFSR bit[2:0] for noise wave        \
                      generation */
#define DAC_LFSRUNMASK_BITS3_0                                                 \
  (DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[3:0] for  \
                                       noise wave generation */
#define DAC_LFSRUNMASK_BITS4_0                                                 \
  (DAC_CR_MAMP1_2) /*!< Unmask DAC channel LFSR bit[4:0] for noise wave        \
                      generation */
#define DAC_LFSRUNMASK_BITS5_0                                                 \
  (DAC_CR_MAMP1_2 | DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[5:0] for  \
                                       noise wave generation */
#define DAC_LFSRUNMASK_BITS6_0                                                 \
  (DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1) /*!< Unmask DAC channel LFSR bit[6:0] for  \
                                       noise wave generation */
#define DAC_LFSRUNMASK_BITS7_0                                                 \
  (DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1 |                                           \
   DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[7:0] for noise wave        \
                      generation */
#define DAC_LFSRUNMASK_BITS8_0                                                 \
  (DAC_CR_MAMP1_3) /*!< Unmask DAC channel LFSR bit[8:0] for noise wave        \
                      generation */
#define DAC_LFSRUNMASK_BITS9_0                                                 \
  (DAC_CR_MAMP1_3 | DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[9:0] for  \
                                       noise wave generation */
#define DAC_LFSRUNMASK_BITS10_0                                                \
  (DAC_CR_MAMP1_3 | DAC_CR_MAMP1_1) /*!< Unmask DAC channel LFSR bit[10:0] for \
                                       noise wave generation */
#define DAC_LFSRUNMASK_BITS11_0                                                \
  (DAC_CR_MAMP1_3 | DAC_CR_MAMP1_1 |                                           \
   DAC_CR_MAMP1_0) /*!< Unmask DAC channel LFSR bit[11:0] for noise wave       \
                      generation */
#define DAC_TRIANGLEAMPLITUDE_1                                                \
  0x00000000UL /*!< Select max triangle amplitude of 1 */
#define DAC_TRIANGLEAMPLITUDE_3                                                \
  (DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 3 */
#define DAC_TRIANGLEAMPLITUDE_7                                                \
  (DAC_CR_MAMP1_1) /*!< Select max triangle amplitude of 7 */
#define DAC_TRIANGLEAMPLITUDE_15                                               \
  (DAC_CR_MAMP1_1 | DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 15   \
                                     */
#define DAC_TRIANGLEAMPLITUDE_31                                               \
  (DAC_CR_MAMP1_2) /*!< Select max triangle amplitude of 31 */
#define DAC_TRIANGLEAMPLITUDE_63                                               \
  (DAC_CR_MAMP1_2 | DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 63   \
                                     */
#define DAC_TRIANGLEAMPLITUDE_127                                              \
  (DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1) /*!< Select max triangle amplitude of 127  \
                                     */
#define DAC_TRIANGLEAMPLITUDE_255                                              \
  (DAC_CR_MAMP1_2 | DAC_CR_MAMP1_1 |                                           \
   DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 255 */
#define DAC_TRIANGLEAMPLITUDE_511                                              \
  (DAC_CR_MAMP1_3) /*!< Select max triangle amplitude of 511 */
#define DAC_TRIANGLEAMPLITUDE_1023                                             \
  (DAC_CR_MAMP1_3 |                                                            \
   DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 1023 */
#define DAC_TRIANGLEAMPLITUDE_2047                                             \
  (DAC_CR_MAMP1_3 |                                                            \
   DAC_CR_MAMP1_1) /*!< Select max triangle amplitude of 2047 */
#define DAC_TRIANGLEAMPLITUDE_4095                                             \
  (DAC_CR_MAMP1_3 | DAC_CR_MAMP1_1 |                                           \
   DAC_CR_MAMP1_0) /*!< Select max triangle amplitude of 4095 */

/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/** @defgroup DACEx_Private_Macros DACEx Private Macros
 * @{
 */
#define IS_DAC_TRIGGER(TRIGGER)                                                \
  (((TRIGGER) == DAC_TRIGGER_NONE) || ((TRIGGER) == DAC_TRIGGER_SOFTWARE) ||   \
   ((TRIGGER) == DAC_TRIGGER_T1_TRGO) || ((TRIGGER) == DAC_TRIGGER_T2_TRGO) || \
   ((TRIGGER) == DAC_TRIGGER_LPTIM1_OUT) ||                                    \
   ((TRIGGER) == DAC_TRIGGER_LPTIM2_OUT) ||                                    \
   ((TRIGGER) == DAC_TRIGGER_LPTIM3_OUT) ||                                    \
   ((TRIGGER) == DAC_TRIGGER_EXT_IT9))

#define IS_DAC_SAMPLETIME(TIME) ((TIME) <= 0x000003FFU)

#define IS_DAC_HOLDTIME(TIME) ((TIME) <= 0x000003FFU)

#define IS_DAC_SAMPLEANDHOLD(MODE)                                             \
  (((MODE) == DAC_SAMPLEANDHOLD_DISABLE) ||                                    \
   ((MODE) == DAC_SAMPLEANDHOLD_ENABLE))

#define IS_DAC_TRIMMINGVALUE(TRIMMINGVALUE) ((TRIMMINGVALUE) <= 0x1FU)

#define IS_DAC_NEWTRIMMINGVALUE(TRIMMINGVALUE) ((TRIMMINGVALUE) <= 0x1FU)

#define IS_DAC_CHIP_CONNECTION(CONNECT)                                        \
  (((CONNECT) == DAC_CHIPCONNECT_DISABLE) ||                                   \
   ((CONNECT) == DAC_CHIPCONNECT_ENABLE))

#define IS_DAC_TRIMMING(TRIMMING)                                              \
  (((TRIMMING) == DAC_TRIMMING_FACTORY) || ((TRIMMING) == DAC_TRIMMING_USER))

#define IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(VALUE)                           \
  (((VALUE) == DAC_LFSRUNMASK_BIT0) || ((VALUE) == DAC_LFSRUNMASK_BITS1_0) ||  \
   ((VALUE) == DAC_LFSRUNMASK_BITS2_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS3_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS4_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS5_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS6_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS7_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS8_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS9_0) ||                                      \
   ((VALUE) == DAC_LFSRUNMASK_BITS10_0) ||                                     \
   ((VALUE) == DAC_LFSRUNMASK_BITS11_0) ||                                     \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_1) ||                                     \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_3) ||                                     \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_7) ||                                     \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_15) ||                                    \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_31) ||                                    \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_63) ||                                    \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_127) ||                                   \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_255) ||                                   \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_511) ||                                   \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_1023) ||                                  \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_2047) ||                                  \
   ((VALUE) == DAC_TRIANGLEAMPLITUDE_4095))
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Extended features functions ***********************************************/

/** @addtogroup DACEx_Exported_Functions
 * @{
 */

/** @addtogroup DACEx_Exported_Functions_Group2
 * @{
 */
/* IO operation functions *****************************************************/

HAL_StatusTypeDef HAL_DACEx_TriangleWaveGenerate(DAC_HandleTypeDef *hdac,
                                                 uint32_t Channel,
                                                 uint32_t Amplitude);
HAL_StatusTypeDef HAL_DACEx_NoiseWaveGenerate(DAC_HandleTypeDef *hdac,
                                              uint32_t Channel,
                                              uint32_t Amplitude);

/**
 * @}
 */

/** @addtogroup DACEx_Exported_Functions_Group3
 * @{
 */
/* Peripheral Control functions ***********************************************/

HAL_StatusTypeDef HAL_DACEx_SelfCalibrate(DAC_HandleTypeDef *hdac,
                                          DAC_ChannelConfTypeDef *sConfig,
                                          uint32_t Channel);
HAL_StatusTypeDef HAL_DACEx_SetUserTrimming(DAC_HandleTypeDef *hdac,
                                            DAC_ChannelConfTypeDef *sConfig,
                                            uint32_t Channel,
                                            uint32_t NewTrimmingValue);
uint32_t HAL_DACEx_GetTrimOffset(const DAC_HandleTypeDef *hdac,
                                 uint32_t Channel);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif /* DAC */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* STM32WLxx_HAL_DAC_EX_H */
