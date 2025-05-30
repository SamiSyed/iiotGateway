/**
 ******************************************************************************
 * @file    stm32wlxx_hal_dma_ex.h
 * @author  MCD Application Team
 * @brief   Header file of DMA HAL extension module.
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
#ifndef STM32WLxx_HAL_DMA_EX_H
#define STM32WLxx_HAL_DMA_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal_def.h"
#include "stm32wlxx_ll_dmamux.h"

/** @addtogroup STM32WLxx_HAL_Driver
 * @{
 */

/** @addtogroup DMAEx
 * @{
 */

/* Exported types ------------------------------------------------------------*/
/** @defgroup DMAEx_Exported_Types DMAEx Exported Types
 * @{
 */

/**
 * @brief  HAL DMAMUX Synchronization configuration structure definition
 */
typedef struct {
  uint32_t SyncSignalID; /*!< Specifies the synchronization signal gating the
                           DMA request in periodic mode.
                           This parameter can be a value of @ref
                           DMAEx_DMAMUX_SyncSignalID_selection */

  uint32_t SyncPolarity; /*!< Specifies the polarity of the signal on which the
                           DMA request is synchronized.
                           This parameter can be a value of @ref
                           DMAEx_DMAMUX_SyncPolarity_selection */

  FunctionalState
      SyncEnable; /*!< Specifies if the synchronization shall be enabled or
                    disabled
                    This parameter can take the value ENABLE or DISABLE */

  FunctionalState
      EventEnable; /*!< Specifies if an event shall be generated once the
                     RequestNumber is reached.
                     This parameter can take the value ENABLE or DISABLE */

  uint32_t
      RequestNumber; /*!< Specifies the number of DMA request that will be
                       authorized after a sync event This parameter must be a
                       number between Min_Data = 1 and Max_Data = 32 */

} HAL_DMA_MuxSyncConfigTypeDef;

/**
 * @brief  HAL DMAMUX request generator parameters structure definition
 */
typedef struct {
  uint32_t SignalID; /*!< Specifies the ID of the signal used for DMAMUX request
                       generator This parameter can be a value of @ref
                       DMAEx_DMAMUX_SignalGeneratorID_selection */

  uint32_t Polarity; /*!< Specifies the polarity of the signal on which the
                       request is generated. This parameter can be a value of
                       @ref DMAEx_DMAMUX_RequestGeneneratorPolarity_selection */

  uint32_t
      RequestNumber; /*!< Specifies the number of DMA request that will be
                       generated after a signal event This parameter must be a
                       number between Min_Data = 1 and Max_Data = 32 */

} HAL_DMA_MuxRequestGeneratorConfigTypeDef;

/**
 * @}
 */

/* Exported constants --------------------------------------------------------*/
/** @defgroup DMAEx_Exported_Constants DMAEx Exported Constants
 * @{
 */

/** @defgroup DMAEx_DMAMUX_SyncSignalID_selection DMAMUX SyncSignalID selection
 * @{
 */
#define HAL_DMAMUX1_SYNC_EXTI0                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE0 /*!< Synchronization signal from EXTI Line0  */
#define HAL_DMAMUX1_SYNC_EXTI1                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE1 /*!< Synchronization signal from EXTI Line1  */
#define HAL_DMAMUX1_SYNC_EXTI2                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE2 /*!< Synchronization signal from EXTI Line2  */
#define HAL_DMAMUX1_SYNC_EXTI3                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE3 /*!< Synchronization signal from EXTI Line3  */
#define HAL_DMAMUX1_SYNC_EXTI4                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE4 /*!< Synchronization signal from EXTI Line4  */
#define HAL_DMAMUX1_SYNC_EXTI5                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE5 /*!< Synchronization signal from EXTI Line5  */
#define HAL_DMAMUX1_SYNC_EXTI6                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE6 /*!< Synchronization signal from EXTI Line6  */
#define HAL_DMAMUX1_SYNC_EXTI7                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE7 /*!< Synchronization signal from EXTI Line7  */
#define HAL_DMAMUX1_SYNC_EXTI8                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE8 /*!< Synchronization signal from EXTI Line8  */
#define HAL_DMAMUX1_SYNC_EXTI9                                                 \
  LL_DMAMUX_SYNC_EXTI_LINE9 /*!< Synchronization signal from EXTI Line9  */
#define HAL_DMAMUX1_SYNC_EXTI10                                                \
  LL_DMAMUX_SYNC_EXTI_LINE10 /*!< Synchronization signal from EXTI Line10 */
#define HAL_DMAMUX1_SYNC_EXTI11                                                \
  LL_DMAMUX_SYNC_EXTI_LINE11 /*!< Synchronization signal from EXTI Line11 */
#define HAL_DMAMUX1_SYNC_EXTI12                                                \
  LL_DMAMUX_SYNC_EXTI_LINE12 /*!< Synchronization signal from EXTI Line12 */
#define HAL_DMAMUX1_SYNC_EXTI13                                                \
  LL_DMAMUX_SYNC_EXTI_LINE13 /*!< Synchronization signal from EXTI Line13 */
#define HAL_DMAMUX1_SYNC_EXTI14                                                \
  LL_DMAMUX_SYNC_EXTI_LINE14 /*!< Synchronization signal from EXTI Line14 */
#define HAL_DMAMUX1_SYNC_EXTI15                                                \
  LL_DMAMUX_SYNC_EXTI_LINE15 /*!< Synchronization signal from EXTI Line15 */
#define HAL_DMAMUX1_SYNC_DMAMUX1_CH0_EVT                                       \
  LL_DMAMUX_SYNC_DMAMUX_CH0 /*!< Synchronization signal from DMAMUX channel0   \
                               Event */
#define HAL_DMAMUX1_SYNC_DMAMUX1_CH1_EVT                                       \
  LL_DMAMUX_SYNC_DMAMUX_CH1 /*!< Synchronization signal from DMAMUX channel1   \
                               Event */
#define HAL_DMAMUX1_SYNC_LPTIM1_OUT                                            \
  LL_DMAMUX_SYNC_LPTIM1_OUT /*!< Synchronization signal from LPTIM1 Output */
#define HAL_DMAMUX1_SYNC_LPTIM2_OUT                                            \
  LL_DMAMUX_SYNC_LPTIM2_OUT /*!< Synchronization signal from LPTIM2 Output */
#define HAL_DMAMUX1_SYNC_LPTIM3_OUT                                            \
  LL_DMAMUX_SYNC_LPTIM3_OUT /*!< Synchronization signal from LPTIM3 Output */

#define HAL_DMAMUX1_MAX_SYNC HAL_DMAMUX1_SYNC_LPTIM3_OUT
/**
 * @}
 */

/** @defgroup DMAEx_DMAMUX_SyncPolarity_selection DMAMUX SyncPolarity selection
 * @{
 */
#define HAL_DMAMUX_SYNC_NO_EVENT                                               \
  LL_DMAMUX_SYNC_NO_EVENT /*!< block synchronization events */
#define HAL_DMAMUX_SYNC_RISING                                                 \
  LL_DMAMUX_SYNC_POL_RISING /*!< synchronize with rising edge events */
#define HAL_DMAMUX_SYNC_FALLING                                                \
  LL_DMAMUX_SYNC_POL_FALLING /*!< synchronize with falling edge events */
#define HAL_DMAMUX_SYNC_RISING_FALLING                                         \
  LL_DMAMUX_SYNC_POL_RISING_FALLING /*!< synchronize with rising and falling   \
                                       edge events    */

/**
 * @}
 */

/** @defgroup DMAEx_DMAMUX_SignalGeneratorID_selection DMAMUX SignalGeneratorID
 * selection
 * @{
 */
#define HAL_DMAMUX1_REQ_GEN_EXTI0                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE0 /*!< Request signal generation from EXTI Line0  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI1                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE1 /*!< Request signal generation from EXTI Line1  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI2                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE2 /*!< Request signal generation from EXTI Line2  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI3                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE3 /*!< Request signal generation from EXTI Line3  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI4                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE4 /*!< Request signal generation from EXTI Line4  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI5                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE5 /*!< Request signal generation from EXTI Line5  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI6                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE6 /*!< Request signal generation from EXTI Line6  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI7                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE7 /*!< Request signal generation from EXTI Line7  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI8                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE8 /*!< Request signal generation from EXTI Line8  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI9                                              \
  LL_DMAMUX_REQ_GEN_EXTI_LINE9 /*!< Request signal generation from EXTI Line9  \
                                */
#define HAL_DMAMUX1_REQ_GEN_EXTI10                                             \
  LL_DMAMUX_REQ_GEN_EXTI_LINE10 /*!< Request signal generation from EXTI       \
                                   Line10 */
#define HAL_DMAMUX1_REQ_GEN_EXTI11                                             \
  LL_DMAMUX_REQ_GEN_EXTI_LINE11 /*!< Request signal generation from EXTI       \
                                   Line11 */
#define HAL_DMAMUX1_REQ_GEN_EXTI12                                             \
  LL_DMAMUX_REQ_GEN_EXTI_LINE12 /*!< Request signal generation from EXTI       \
                                   Line12 */
#define HAL_DMAMUX1_REQ_GEN_EXTI13                                             \
  LL_DMAMUX_REQ_GEN_EXTI_LINE13 /*!< Request signal generation from EXTI       \
                                   Line13 */
#define HAL_DMAMUX1_REQ_GEN_EXTI14                                             \
  LL_DMAMUX_REQ_GEN_EXTI_LINE14 /*!< Request signal generation from EXTI       \
                                   Line14 */
#define HAL_DMAMUX1_REQ_GEN_EXTI15                                             \
  LL_DMAMUX_REQ_GEN_EXTI_LINE15 /*!< Request signal generation from EXTI       \
                                   Line15 */
#define HAL_DMAMUX1_REQ_GEN_DMAMUX1_CH0_EVT                                    \
  LL_DMAMUX_REQ_GEN_DMAMUX_CH0 /*!< Request signal generation from DMAMUX      \
                                  channel0 Event */
#define HAL_DMAMUX1_REQ_GEN_DMAMUX1_CH1_EVT                                    \
  LL_DMAMUX_REQ_GEN_DMAMUX_CH1 /*!< Request signal generation from DMAMUX      \
                                  channel1 Event */
#define HAL_DMAMUX1_REQ_GEN_LPTIM1_OUT                                         \
  LL_DMAMUX_REQ_GEN_LPTIM1_OUT /*!< Request signal generation from LPTIM1      \
                                  Output */
#define HAL_DMAMUX1_REQ_GEN_LPTIM2_OUT                                         \
  LL_DMAMUX_REQ_GEN_LPTIM2_OUT /*!< Request signal generation from LPTIM2      \
                                  Output */
#define HAL_DMAMUX1_REQ_GEN_LPTIM3_OUT                                         \
  LL_DMAMUX_REQ_GEN_LPTIM3_OUT /*!< Request signal generation from LPTIM3      \
                                  Output */

#define HAL_DMAMUX1_MAX_REQ_GEN HAL_DMAMUX1_REQ_GEN_LPTIM3_OUT
/**
 * @}
 */

/** @defgroup DMAEx_DMAMUX_RequestGeneneratorPolarity_selection DMAMUX
 * RequestGeneneratorPolarity selection
 * @{
 */
#define HAL_DMAMUX_REQ_GEN_NO_EVENT                                            \
  LL_DMAMUX_REQ_GEN_NO_EVENT /*!< block request generator events */
#define HAL_DMAMUX_REQ_GEN_RISING                                              \
  LL_DMAMUX_REQ_GEN_POL_RISING /*!< generate request on rising edge events */
#define HAL_DMAMUX_REQ_GEN_FALLING                                             \
  LL_DMAMUX_REQ_GEN_POL_FALLING /*!< generate request on falling edge events   \
                                 */
#define HAL_DMAMUX_REQ_GEN_RISING_FALLING                                      \
  LL_DMAMUX_REQ_GEN_POL_RISING_FALLING /*!< generate request on rising and     \
                                          falling edge events */

/**
 * @}
 */

/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup DMAEx_Exported_Functions
 * @{
 */

/* IO operation functions *****************************************************/
/** @addtogroup DMAEx_Exported_Functions_Group1
 * @{
 */

/* ------------------------- REQUEST -----------------------------------------*/
HAL_StatusTypeDef HAL_DMAEx_ConfigMuxRequestGenerator(
    DMA_HandleTypeDef *hdma,
    HAL_DMA_MuxRequestGeneratorConfigTypeDef *pRequestGeneratorConfig);
HAL_StatusTypeDef HAL_DMAEx_EnableMuxRequestGenerator(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMAEx_DisableMuxRequestGenerator(DMA_HandleTypeDef *hdma);
/* -------------------------------------------------------------------------- */

/* ------------------------- SYNCHRO -----------------------------------------*/
HAL_StatusTypeDef
HAL_DMAEx_ConfigMuxSync(DMA_HandleTypeDef *hdma,
                        HAL_DMA_MuxSyncConfigTypeDef *pSyncConfig);
/* -------------------------------------------------------------------------- */

void HAL_DMAEx_MUX_IRQHandler(DMA_HandleTypeDef *hdma);

/**
 * @}
 */

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/** @defgroup DMAEx_Private_Macros DMAEx Private Macros
 * @brief    DMAEx private macros
 * @{
 */

#define IS_DMAMUX_SYNC_SIGNAL_ID(SIGNAL_ID)                                    \
  (((SIGNAL_ID) == HAL_DMAMUX1_SYNC_EXTI0) ||                                  \
   (((SIGNAL_ID) >= HAL_DMAMUX1_SYNC_EXTI1) &&                                 \
    ((SIGNAL_ID) <= HAL_DMAMUX1_MAX_SYNC)))

#define IS_DMAMUX_SYNC_REQUEST_NUMBER(REQUEST_NUMBER)                          \
  (((REQUEST_NUMBER) > 0U) && ((REQUEST_NUMBER) <= 32U))

#define IS_DMAMUX_SYNC_POLARITY(POLARITY)                                      \
  (((POLARITY) == HAL_DMAMUX_SYNC_NO_EVENT) ||                                 \
   ((POLARITY) == HAL_DMAMUX_SYNC_RISING) ||                                   \
   ((POLARITY) == HAL_DMAMUX_SYNC_FALLING) ||                                  \
   ((POLARITY) == HAL_DMAMUX_SYNC_RISING_FALLING))

#define IS_DMAMUX_SYNC_STATE(SYNC) (((SYNC) == DISABLE) || ((SYNC) == ENABLE))

#define IS_DMAMUX_SYNC_EVENT(EVENT)                                            \
  (((EVENT) == DISABLE) || ((EVENT) == ENABLE))

#define IS_DMAMUX_REQUEST_GEN_SIGNAL_ID(SIGNAL_ID)                             \
  (((SIGNAL_ID) == HAL_DMAMUX1_REQ_GEN_EXTI0) ||                               \
   (((SIGNAL_ID) >= HAL_DMAMUX1_REQ_GEN_EXTI1) &&                              \
    ((SIGNAL_ID) <= HAL_DMAMUX1_MAX_REQ_GEN)))

#define IS_DMAMUX_REQUEST_GEN_REQUEST_NUMBER(REQUEST_NUMBER)                   \
  (((REQUEST_NUMBER) > 0U) && ((REQUEST_NUMBER) <= 32U))

#define IS_DMAMUX_REQUEST_GEN_POLARITY(POLARITY)                               \
  (((POLARITY) == HAL_DMAMUX_REQ_GEN_NO_EVENT) ||                              \
   ((POLARITY) == HAL_DMAMUX_REQ_GEN_RISING) ||                                \
   ((POLARITY) == HAL_DMAMUX_REQ_GEN_FALLING) ||                               \
   ((POLARITY) == HAL_DMAMUX_REQ_GEN_RISING_FALLING))

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* STM32WLxx_HAL_DMA_EX_H */
