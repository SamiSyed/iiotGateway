/**
  ******************************************************************************
  * @file    stm32wlxx_hal_dma.c
  * @author  MCD Application Team
  * @brief   DMA HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Direct Memory Access (DMA) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and errors functions
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
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) Enable and configure the peripheral to be connected to the DMA Channel
       (except for internal SRAM / FLASH memories: no initialization is
       necessary). Please refer to the Reference manual for connection between
  peripherals and DMA requests.

   (#) For a given Channel, program the required configuration through the
  following parameters: Channel request, Transfer Direction, Source and
  Destination data formats, Circular or Normal mode, Channel Priority level,
  Source and Destination Increment mode using HAL_DMA_Init() function.

       Prior to HAL_DMA_Init the peripheral clock shall be enabled for both DMA
  & DMAMUX thanks to:
      (##) DMA1 or DMA2: __HAL_RCC_DMA1_CLK_ENABLE() or
  __HAL_RCC_DMA2_CLK_ENABLE();
      (##) DMAMUX1:      __HAL_RCC_DMAMUX1_CLK_ENABLE();

   (#) Use HAL_DMA_GetState() function to return the DMA state and
  HAL_DMA_GetError() in case of error detection.

   (#) Use HAL_DMA_Abort() function to abort the current transfer

     -@-   In Memory-to-Memory transfer mode, Circular mode is not allowed.

     *** Polling mode IO operation ***
     =================================
     [..]
       (+) Use HAL_DMA_Start() to start DMA transfer after the configuration of
  Source address and destination address and the Length of data to be
  transferred
       (+) Use HAL_DMA_PollForTransfer() to poll for the end of current
  transfer, in this case a fixed Timeout can be configured by User depending
  from his application.

     *** Interrupt mode IO operation ***
     ===================================
     [..]
       (+) Configure the DMA interrupt priority using HAL_NVIC_SetPriority()
       (+) Enable the DMA IRQ handler using HAL_NVIC_EnableIRQ()
       (+) Use HAL_DMA_Start_IT() to start DMA transfer after the configuration
  of Source address and destination address and the Length of data to be
  transferred. In this case the DMA interrupt is configured
       (+) Use HAL_DMA_IRQHandler() called under DMA_IRQHandler() Interrupt
  subroutine
       (+) At the end of data transfer HAL_DMA_IRQHandler() function is executed
  and user can add his own function to register callbacks with
  HAL_DMA_RegisterCallback().

     *** DMA HAL driver macros list ***
     =============================================
     [..]
       Below the list of macros in DMA HAL driver.

       (+) __HAL_DMA_ENABLE: Enable the specified DMA Channel.
       (+) __HAL_DMA_DISABLE: Disable the specified DMA Channel.
       (+) __HAL_DMA_GET_FLAG: Get the DMA Channel pending flags.
       (+) __HAL_DMA_CLEAR_FLAG: Clear the DMA Channel pending flags.
       (+) __HAL_DMA_ENABLE_IT: Enable the specified DMA Channel interrupts.
       (+) __HAL_DMA_DISABLE_IT: Disable the specified DMA Channel interrupts.
       (+) __HAL_DMA_GET_IT_SOURCE: Check whether the specified DMA Channel
  interrupt is enabled or not.

     [..]
      (@) You can refer to the DMA HAL driver header file for more useful macros

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/** @addtogroup STM32WLxx_HAL_Driver
 * @{
 */

/** @defgroup DMA DMA
 * @brief DMA HAL module driver
 * @{
 */

#ifdef HAL_DMA_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @defgroup DMA_Private_Functions DMA Private Functions
 * @{
 */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                          uint32_t DstAddress, uint32_t DataLength);
static void DMA_CalcDMAMUXChannelBaseAndMask(DMA_HandleTypeDef *hdma);
static void DMA_CalcDMAMUXRequestGenBaseAndMask(DMA_HandleTypeDef *hdma);

/**
 * @}
 */

/* Exported functions
 * ---------------------------------------------------------*/

/** @defgroup DMA_Exported_Functions DMA Exported Functions
 * @{
 */

/** @defgroup DMA_Exported_Functions_Group1 Initialization and de-initialization
functions
 *  @brief   Initialization and de-initialization functions
 *
@verbatim
 ===============================================================================
             ##### Initialization and de-initialization functions  #####
 ===============================================================================
    [..]
    This section provides functions allowing to initialize the DMA Channel
source and destination addresses, incrementation and data sizes, transfer
direction, circular/normal mode selection, memory-to-memory mode selection and
Channel priority value.
    [..]
    The HAL_DMA_Init() function follows the DMA configuration procedures as
described in reference manual.

@endverbatim
  * @{
  */

/**
 * @brief Initialize the DMA according to the specified
 *        parameters in the DMA_InitTypeDef and initialize the associated
 * handle.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma) {
  /* Check the DMA handle allocation */
  if (hdma == NULL) {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
  assert_param(IS_DMA_DIRECTION(hdma->Init.Direction));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(hdma->Init.PeriphInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(hdma->Init.MemInc));
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(hdma->Init.PeriphDataAlignment));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(hdma->Init.MemDataAlignment));
  assert_param(IS_DMA_MODE(hdma->Init.Mode));
  assert_param(IS_DMA_PRIORITY(hdma->Init.Priority));

  assert_param(IS_DMA_ALL_REQUEST(hdma->Init.Request));

  /* Compute the channel index */
  if ((uint32_t)(hdma->Instance) < (uint32_t)(DMA2_Channel1)) {
    /* DMA1 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) /
                          ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1))
                         << 2U;
    hdma->DmaBaseAddress = DMA1;
  } else {
    /* DMA2 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA2_Channel1) /
                          ((uint32_t)DMA2_Channel2 - (uint32_t)DMA2_Channel1))
                         << 2U;
    hdma->DmaBaseAddress = DMA2;
  }

  /* Change DMA peripheral state */
  hdma->State = HAL_DMA_STATE_BUSY;

  /* Clear PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR and MEM2MEM bits */
  CLEAR_BIT(hdma->Instance->CCR,
            (DMA_CCR_PL | DMA_CCR_MSIZE | DMA_CCR_PSIZE | DMA_CCR_MINC |
             DMA_CCR_PINC | DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_MEM2MEM));

  /* Set the DMA Channel configuration */
  SET_BIT(hdma->Instance->CCR,
          (hdma->Init.Direction | hdma->Init.PeriphInc | hdma->Init.MemInc |
           hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |
           hdma->Init.Mode | hdma->Init.Priority));

  /* Initialize parameters for DMAMUX channel :
     DMAmuxChannel, DMAmuxChannelStatus and DMAmuxChannelStatusMask
  */
  DMA_CalcDMAMUXChannelBaseAndMask(hdma);

  if (hdma->Init.Direction == DMA_MEMORY_TO_MEMORY) {
    /* if memory to memory force the request to 0*/
    hdma->Init.Request = DMA_REQUEST_MEM2MEM;
  }

  /* Set peripheral request  to DMAMUX channel */
  hdma->DMAmuxChannel->CCR = (hdma->Init.Request & DMAMUX_CxCR_DMAREQ_ID);

  /* Clear the DMAMUX synchro overrun flag */
  hdma->DMAmuxChannelStatus->CFR = hdma->DMAmuxChannelStatusMask;

  if (((hdma->Init.Request > 0UL) &&
       (hdma->Init.Request <= DMA_REQUEST_GENERATOR3))) {
    /* Initialize parameters for DMAMUX request generator :
       DMAmuxRequestGen, DMAmuxRequestGenStatus and DMAmuxRequestGenStatusMask
    */
    DMA_CalcDMAMUXRequestGenBaseAndMask(hdma);

    /* Reset the DMAMUX request generator register*/
    hdma->DMAmuxRequestGen->RGCR = 0U;

    /* Clear the DMAMUX request generator overrun flag */
    hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;
  } else {
    hdma->DMAmuxRequestGen = NULL;
    hdma->DMAmuxRequestGenStatus = NULL;
    hdma->DMAmuxRequestGenStatusMask = 0U;
  }

  /* Initialize the error code */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;

  /* Initialize the DMA state*/
  hdma->State = HAL_DMA_STATE_READY;

  /* Release Lock */
  __HAL_UNLOCK(hdma);

  return HAL_OK;
}

/**
 * @brief DeInitialize the DMA peripheral.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma) {
  /* Check the DMA handle allocation */
  if (NULL == hdma) {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));

  /* Disable the selected DMA Channelx */
  __HAL_DMA_DISABLE(hdma);

  /* Compute the channel index */
  if ((uint32_t)(hdma->Instance) < (uint32_t)(DMA2_Channel1)) {
    /* DMA1 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) /
                          ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1))
                         << 2U;
    hdma->DmaBaseAddress = DMA1;
  } else {
    /* DMA2 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA2_Channel1) /
                          ((uint32_t)DMA2_Channel2 - (uint32_t)DMA2_Channel1))
                         << 2U;
    hdma->DmaBaseAddress = DMA2;
  }

  /* Reset DMA Channel control register */
  hdma->Instance->CCR = 0U;

  /* Clear all flags */
  hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hdma->ChannelIndex & 0x1CU));

  /* Initialize parameters for DMAMUX channel :
     DMAmuxChannel, DMAmuxChannelStatus and DMAmuxChannelStatusMask */

  DMA_CalcDMAMUXChannelBaseAndMask(hdma);

  /* Reset the DMAMUX channel that corresponds to the DMA channel */
  hdma->DMAmuxChannel->CCR = 0U;

  /* Clear the DMAMUX synchro overrun flag */
  hdma->DMAmuxChannelStatus->CFR = hdma->DMAmuxChannelStatusMask;

  /* Reset Request generator parameters if any */
  if (((hdma->Init.Request > 0UL) &&
       (hdma->Init.Request <= DMA_REQUEST_GENERATOR3))) {
    /* Initialize parameters for DMAMUX request generator :
       DMAmuxRequestGen, DMAmuxRequestGenStatus and DMAmuxRequestGenStatusMask
    */
    DMA_CalcDMAMUXRequestGenBaseAndMask(hdma);

    /* Reset the DMAMUX request generator register*/
    hdma->DMAmuxRequestGen->RGCR = 0U;

    /* Clear the DMAMUX request generator overrun flag */
    hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;
  }

  hdma->DMAmuxRequestGen = NULL;
  hdma->DMAmuxRequestGenStatus = NULL;
  hdma->DMAmuxRequestGenStatusMask = 0U;

  /* Clean callbacks */
  hdma->XferCpltCallback = NULL;
  hdma->XferHalfCpltCallback = NULL;
  hdma->XferErrorCallback = NULL;
  hdma->XferAbortCallback = NULL;

  /* Initialize the error code */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;

  /* Initialize the DMA state */
  hdma->State = HAL_DMA_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(hdma);

  return HAL_OK;
}

/**
 * @}
 */

/** @defgroup DMA_Exported_Functions_Group2 Input and Output operation functions
 *  @brief   Input and Output operation functions
 *
@verbatim
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the source, destination address and data length and Start
DMA transfer
      (+) Configure the source, destination address and data length and
          Start DMA transfer with interrupt
      (+) Abort DMA transfer
      (+) Poll for transfer complete
      (+) Handle DMA interrupt request
      (+) Register and Unregister DMA callbacks

@endverbatim
  * @{
  */

/**
 * @brief Start the DMA Transfer.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param SrcAddress The source memory Buffer address
 * @param DstAddress The destination memory Buffer address
 * @param DataLength The length of data to be transferred from source to
 * destination
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                                uint32_t DstAddress, uint32_t DataLength) {
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));

  /* Process locked */
  __HAL_LOCK(hdma);

  if (hdma->State == HAL_DMA_STATE_READY) {
    /* Change DMA peripheral state */
    hdma->State = HAL_DMA_STATE_BUSY;

    /* Initialize the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;

    /* Disable the peripheral */
    __HAL_DMA_DISABLE(hdma);

    /* Configure the source, destination address and the data length & clear
     * flags*/
    DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);

    /* Enable the Peripheral */
    __HAL_DMA_ENABLE(hdma);
  } else {
    /* Change the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_BUSY;

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);

    /* Return error status */
    status = HAL_ERROR;
  }

  return status;
}

/**
 * @brief Start the DMA Transfer with interrupt enabled.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param SrcAddress The source memory Buffer address
 * @param DstAddress The destination memory Buffer address
 * @param DataLength The length of data to be transferred from source to
 * destination
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                                   uint32_t DstAddress, uint32_t DataLength) {
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));

  /* Process locked */
  __HAL_LOCK(hdma);

  if (hdma->State == HAL_DMA_STATE_READY) {
    /* Change DMA peripheral state */
    hdma->State = HAL_DMA_STATE_BUSY;
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;

    /* Disable the peripheral */
    __HAL_DMA_DISABLE(hdma);

    /* Configure the source, destination address and the data length & clear
     * flags*/
    DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);

    /* Enable the transfer complete interrupt */
    /* Enable the transfer Error interrupt */
    if (NULL != hdma->XferHalfCpltCallback) {
      /* Enable the Half transfer complete interrupt as well */
      __HAL_DMA_ENABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));
    } else {
      __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
      __HAL_DMA_ENABLE_IT(hdma, (DMA_IT_TC | DMA_IT_TE));
    }

    /* Check if DMAMUX Synchronization is enabled*/
    if ((hdma->DMAmuxChannel->CCR & DMAMUX_CxCR_SE) != 0U) {
      /* Enable DMAMUX sync overrun IT*/
      hdma->DMAmuxChannel->CCR |= DMAMUX_CxCR_SOIE;
    }

    if (hdma->DMAmuxRequestGen != NULL) {
      /* if using DMAMUX request generator, enable the DMAMUX request generator
       * overrun IT*/
      /* enable the request gen overrun IT*/
      hdma->DMAmuxRequestGen->RGCR |= DMAMUX_RGxCR_OIE;
    }

    /* Enable the Peripheral */
    __HAL_DMA_ENABLE(hdma);
  } else {
    /* Change the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_BUSY;

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);

    /* Return error status */
    status = HAL_ERROR;
  }

  return status;
}

/**
 * @brief Abort the DMA Transfer.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma) {
  /* Check the DMA peripheral handle */
  if (NULL == hdma) {
    return HAL_ERROR;
  }

  /* Check the DMA peripheral state */
  if (hdma->State != HAL_DMA_STATE_BUSY) {
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);

    return HAL_ERROR;
  } else {
    /* Disable DMA IT */
    __HAL_DMA_DISABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));

    /* disable the DMAMUX sync overrun IT*/
    hdma->DMAmuxChannel->CCR &= ~DMAMUX_CxCR_SOIE;

    /* Disable the channel */
    __HAL_DMA_DISABLE(hdma);

    /* Clear all flags */
    hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hdma->ChannelIndex & 0x1CU));

    /* Clear the DMAMUX synchro overrun flag */
    hdma->DMAmuxChannelStatus->CFR = hdma->DMAmuxChannelStatusMask;

    if (hdma->DMAmuxRequestGen != NULL) {
      /* if using DMAMUX request generator, disable the DMAMUX request generator
       * overrun IT*/
      /* disable the request gen overrun IT*/
      hdma->DMAmuxRequestGen->RGCR &= ~DMAMUX_RGxCR_OIE;

      /* Clear the DMAMUX request generator overrun flag */
      hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;
    }

    /* Change the DMA state */
    hdma->State = HAL_DMA_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);
  }

  return HAL_OK;
}

/**
 * @brief Aborts the DMA Transfer in Interrupt mode.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma) {
  HAL_StatusTypeDef status = HAL_OK;

  if (hdma->State != HAL_DMA_STATE_BUSY) {
    /* no transfer ongoing */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;

    status = HAL_ERROR;
  } else {
    /* Disable DMA IT */
    __HAL_DMA_DISABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));

    /* Disable the channel */
    __HAL_DMA_DISABLE(hdma);

    /* disable the DMAMUX sync overrun IT*/
    hdma->DMAmuxChannel->CCR &= ~DMAMUX_CxCR_SOIE;

    /* Clear all flags */
    hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hdma->ChannelIndex & 0x1CU));

    /* Clear the DMAMUX synchro overrun flag */
    hdma->DMAmuxChannelStatus->CFR = hdma->DMAmuxChannelStatusMask;

    if (hdma->DMAmuxRequestGen != NULL) {
      /* if using DMAMUX request generator, disable the DMAMUX request generator
       * overrun IT*/
      /* disable the request gen overrun IT*/
      hdma->DMAmuxRequestGen->RGCR &= ~DMAMUX_RGxCR_OIE;

      /* Clear the DMAMUX request generator overrun flag */
      hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;
    }

    /* Change the DMA state */
    hdma->State = HAL_DMA_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);

    /* Call User Abort callback */
    if (hdma->XferAbortCallback != NULL) {
      hdma->XferAbortCallback(hdma);
    }
  }
  return status;
}

/**
 * @brief Polling for transfer complete.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param CompleteLevel Specifies the DMA level complete.
 * @param Timeout Timeout duration.
 * @retval HAL status
 */
HAL_StatusTypeDef
HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma,
                        HAL_DMA_LevelCompleteTypeDef CompleteLevel,
                        uint32_t Timeout) {
  uint32_t temp;
  uint32_t tickstart;

  if (hdma->State != HAL_DMA_STATE_BUSY) {
    /* no transfer ongoing */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
    __HAL_UNLOCK(hdma);
    return HAL_ERROR;
  }

  /* Polling mode not supported in circular mode */
  if ((hdma->Instance->CCR & DMA_CCR_CIRC) != 0U) {
    hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;
    return HAL_ERROR;
  }

  /* Get the level transfer complete flag */
  if (HAL_DMA_FULL_TRANSFER == CompleteLevel) {
    /* Transfer Complete flag */
    temp = DMA_FLAG_TC1 << (hdma->ChannelIndex & 0x1CU);
  } else {
    /* Half Transfer Complete flag */
    temp = DMA_FLAG_HT1 << (hdma->ChannelIndex & 0x1CU);
  }

  /* Get tick */
  tickstart = HAL_GetTick();

  while ((hdma->DmaBaseAddress->ISR & temp) == 0U) {
    if ((hdma->DmaBaseAddress->ISR &
         (DMA_FLAG_TE1 << (hdma->ChannelIndex & 0x1CU))) != 0U) {
      /* When a DMA transfer error occurs */
      /* A hardware clear of its EN bits is performed */
      /* Clear all flags */
      hdma->DmaBaseAddress->IFCR =
          (DMA_ISR_GIF1 << (hdma->ChannelIndex & 0x1CU));

      /* Update error code */
      hdma->ErrorCode = HAL_DMA_ERROR_TE;

      /* Change the DMA state */
      hdma->State = HAL_DMA_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdma);

      return HAL_ERROR;
    }
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY) {
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U)) {
        /* Update error code */
        hdma->ErrorCode = HAL_DMA_ERROR_TIMEOUT;

        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hdma);

        return HAL_ERROR;
      }
    }
  }

  /*Check for DMAMUX Request generator (if used) overrun status */
  if (hdma->DMAmuxRequestGen != NULL) {
    /* if using DMAMUX request generator Check for DMAMUX request generator
     * overrun */
    if ((hdma->DMAmuxRequestGenStatus->RGSR &
         hdma->DMAmuxRequestGenStatusMask) != 0U) {
      /* Disable the request gen overrun interrupt */
      hdma->DMAmuxRequestGen->RGCR |= DMAMUX_RGxCR_OIE;

      /* Clear the DMAMUX request generator overrun flag */
      hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;

      /* Update error code */
      hdma->ErrorCode |= HAL_DMA_ERROR_REQGEN;
    }
  }

  /* Check for DMAMUX Synchronization overrun */
  if ((hdma->DMAmuxChannelStatus->CSR & hdma->DMAmuxChannelStatusMask) != 0U) {
    /* Clear the DMAMUX synchro overrun flag */
    hdma->DMAmuxChannelStatus->CFR = hdma->DMAmuxChannelStatusMask;

    /* Update error code */
    hdma->ErrorCode |= HAL_DMA_ERROR_SYNC;
  }

  if (HAL_DMA_FULL_TRANSFER == CompleteLevel) {
    /* Clear the transfer complete flag */
    hdma->DmaBaseAddress->IFCR = (DMA_FLAG_TC1 << (hdma->ChannelIndex & 0x1CU));

    /* Process unlocked */
    __HAL_UNLOCK(hdma);

    /* The selected Channelx EN bit is cleared (DMA is disabled and
    all transfers are complete) */
    hdma->State = HAL_DMA_STATE_READY;
  } else {
    /* Clear the half transfer complete flag */
    hdma->DmaBaseAddress->IFCR = (DMA_FLAG_HT1 << (hdma->ChannelIndex & 0x1CU));
  }

  return HAL_OK;
}

/**
 * @brief Handle DMA interrupt request.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval None
 */
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma) {
  uint32_t flag_it = hdma->DmaBaseAddress->ISR;
  uint32_t source_it = hdma->Instance->CCR;

  /* Half Transfer Complete Interrupt management ******************************/
  if (((flag_it & (DMA_FLAG_HT1 << (hdma->ChannelIndex & 0x1CU))) != 0U) &&
      ((source_it & DMA_IT_HT) != 0U)) {
    /* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
    if ((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U) {
      /* Disable the half transfer interrupt */
      __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
    }
    /* Clear the half transfer complete flag */
    hdma->DmaBaseAddress->IFCR = DMA_ISR_HTIF1 << (hdma->ChannelIndex & 0x1CU);

    /* DMA peripheral state is not updated in Half Transfer */
    /* but in Transfer Complete case */

    if (hdma->XferHalfCpltCallback != NULL) {
      /* Half transfer callback */
      hdma->XferHalfCpltCallback(hdma);
    }
  }

  /* Transfer Complete Interrupt management ***********************************/
  else if ((0U != (flag_it & (DMA_FLAG_TC1 << (hdma->ChannelIndex & 0x1CU)))) &&
           (0U != (source_it & DMA_IT_TC))) {
    if ((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U) {
      /* Disable the transfer complete and error interrupt */
      __HAL_DMA_DISABLE_IT(hdma, DMA_IT_TE | DMA_IT_TC);

      /* Change the DMA state */
      hdma->State = HAL_DMA_STATE_READY;
    }
    /* Clear the transfer complete flag */
    __HAL_DMA_CLEAR_FLAG(hdma, (DMA_FLAG_TC1 << (hdma->ChannelIndex & 0x1CU)));

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);

    if (hdma->XferCpltCallback != NULL) {
      /* Transfer complete callback */
      hdma->XferCpltCallback(hdma);
    }
  }

  /* Transfer Error Interrupt management **************************************/
  else if (((flag_it & (DMA_FLAG_TE1 << (hdma->ChannelIndex & 0x1CU))) != 0U) &&
           ((source_it & DMA_IT_TE) != 0U)) {
    /* When a DMA transfer error occurs */
    /* A hardware clear of its EN bits is performed */
    /* Disable ALL DMA IT */
    __HAL_DMA_DISABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));

    /* Clear all flags */
    hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hdma->ChannelIndex & 0x1CU));

    /* Update error code */
    hdma->ErrorCode = HAL_DMA_ERROR_TE;

    /* Change the DMA state */
    hdma->State = HAL_DMA_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hdma);

    if (hdma->XferErrorCallback != NULL) {
      /* Transfer error callback */
      hdma->XferErrorCallback(hdma);
    }
  } else {
    /* Nothing To Do */
  }
  return;
}

/**
 * @brief Register callbacks
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param CallbackID User Callback identifier
 *                   a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
 * @param pCallback Pointer to private callback function which has pointer to
 *                  a DMA_HandleTypeDef structure as parameter.
 * @retval HAL status
 */
HAL_StatusTypeDef
HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma,
                         HAL_DMA_CallbackIDTypeDef CallbackID,
                         void (*pCallback)(DMA_HandleTypeDef *_hdma)) {
  HAL_StatusTypeDef status = HAL_OK;

  /* Process locked */
  __HAL_LOCK(hdma);

  if (hdma->State == HAL_DMA_STATE_READY) {
    switch (CallbackID) {
    case HAL_DMA_XFER_CPLT_CB_ID:
      hdma->XferCpltCallback = pCallback;
      break;

    case HAL_DMA_XFER_HALFCPLT_CB_ID:
      hdma->XferHalfCpltCallback = pCallback;
      break;

    case HAL_DMA_XFER_ERROR_CB_ID:
      hdma->XferErrorCallback = pCallback;
      break;

    case HAL_DMA_XFER_ABORT_CB_ID:
      hdma->XferAbortCallback = pCallback;
      break;

    default:
      status = HAL_ERROR;
      break;
    }
  } else {
    status = HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hdma);

  return status;
}

/**
 * @brief UnRegister callbacks
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param CallbackID User Callback identifier
 *                   a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
 * @retval HAL status
 */
HAL_StatusTypeDef
HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma,
                           HAL_DMA_CallbackIDTypeDef CallbackID) {
  HAL_StatusTypeDef status = HAL_OK;

  /* Process locked */
  __HAL_LOCK(hdma);

  if (hdma->State == HAL_DMA_STATE_READY) {
    switch (CallbackID) {
    case HAL_DMA_XFER_CPLT_CB_ID:
      hdma->XferCpltCallback = NULL;
      break;

    case HAL_DMA_XFER_HALFCPLT_CB_ID:
      hdma->XferHalfCpltCallback = NULL;
      break;

    case HAL_DMA_XFER_ERROR_CB_ID:
      hdma->XferErrorCallback = NULL;
      break;

    case HAL_DMA_XFER_ABORT_CB_ID:
      hdma->XferAbortCallback = NULL;
      break;

    case HAL_DMA_XFER_ALL_CB_ID:
      hdma->XferCpltCallback = NULL;
      hdma->XferHalfCpltCallback = NULL;
      hdma->XferErrorCallback = NULL;
      hdma->XferAbortCallback = NULL;
      break;

    default:
      status = HAL_ERROR;
      break;
    }
  } else {
    status = HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hdma);

  return status;
}

/**
 * @}
 */

/** @defgroup DMA_Exported_Functions_Group3 Peripheral State and Errors
functions
 *  @brief    Peripheral State and Errors functions
 *
@verbatim
 ===============================================================================
            ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Check the DMA state
      (+) Get error code

@endverbatim
  * @{
  */

/**
 * @brief Return the DMA handle state.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval HAL state
 */
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma) {
  /* Return DMA handle state */
  return hdma->State;
}

/**
 * @brief Return the DMA error code.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval DMA Error Code
 */
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma) {
  /* Return the DMA error code */
  return hdma->ErrorCode;
}

/**
 * @}
 */

/**
 * @}
 */

#if defined(DMA_CCR_SECM) && defined(DMA_CCR_PRIV)
/** @defgroup DMA_Exported_Functions_Group4 Attributes management functions
 *  @brief    Attributes management functions
 *
@verbatim
 ===============================================================================
            ##### Attributes management functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Configure the DMA channel(s) privilege and non-privilege attributes
      (+) Configure the DMA channel(s) secure and non-secure attributes from
          secure world when the system implements the security (ESE=1)
      (+) Get the DMA channel(s) attributes

@endverbatim
  * @{
  */

/**
 * @brief Configure the DMA channel attribute(s).
 * @note  Available attributes are security and privilege protection.
 *        Each field can be set independently. Not allowed configurations
 *        are not taken into account & HAL_ERROR returned.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param ChannelAttributes specifies the DMA channel secure/privilege
 * attributes. This parameter can be a one or a combination of @ref
 * DMA_Channel_Attributes
 * @retval HAL Status
 */
HAL_StatusTypeDef HAL_DMA_ConfigChannelAttributes(DMA_HandleTypeDef *hdma,
                                                  uint32_t ChannelAttributes) {
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t ccr;

#if defined(CORE_CM0PLUS)
  uint32_t ccr_SECM;
#endif /* CORE_CM0PLUS */

  /* Check the DMA peripheral handle */
  if (hdma == NULL) {
    status = HAL_ERROR;
    return status;
  }

  /* Check the parameters */
  assert_param(IS_DMA_ATTRIBUTES(ChannelAttributes));

  /* Read CCR register */
  ccr = READ_REG(hdma->Instance->CCR);

  /* Apply any requested privilege/non-privilege attributes */
  if ((ChannelAttributes & DMA_CHANNEL_ATTR_PRIV_MASK) != 0U) {
    if ((ChannelAttributes & DMA_CCR_PRIV) != 0U) {
      SET_BIT(ccr, DMA_CCR_PRIV);
    } else {
      CLEAR_BIT(ccr, DMA_CCR_PRIV);
    }
  }

#if defined(CORE_CM0PLUS)
  /* Channel */
  /* Check what is the current SECM status */
  if ((hdma->Instance->CCR & DMA_CCR_SECM) == DMA_CCR_SECM) {
    /* Channel is currently secure */
    ccr_SECM = DMA_CCR_SECM;
  } else {
    /* Channel is currently non-secure */
    ccr_SECM = 0U;
  }

  if ((ChannelAttributes & DMA_CHANNEL_ATTR_SEC_MASK) != 0U) {
    if ((ChannelAttributes & DMA_CCR_SECM) != 0U) {
      SET_BIT(ccr, DMA_CCR_SECM);
      /* Channel changed to secure */
      ccr_SECM = DMA_CCR_SECM;
    } else {
      CLEAR_BIT(ccr, DMA_CCR_SECM);
      /* Channel changed to non-secure */
      ccr_SECM = 0U;
    }
  }

  /* Channel source */
  if ((ChannelAttributes & DMA_CHANNEL_ATTR_SEC_SRC_MASK) != 0U) {
    /* Configure Source security attributes */
    if ((ChannelAttributes & DMA_CCR_SSEC) != 0x0U) {
      /* SSEC can only be set if channel is secure */
      /* Otherwise configuration is not taken into account */
      if (ccr_SECM == 0U) {
        status = HAL_ERROR;
      } else {
        SET_BIT(ccr, DMA_CCR_SSEC);
      }
    } else {
      CLEAR_BIT(ccr, DMA_CCR_SSEC);
    }
  }

  /* Channel destination */
  if ((ChannelAttributes & DMA_CHANNEL_ATTR_SEC_DEST_MASK) != 0U) {
    /* Configure Destination security attributes */
    if ((ChannelAttributes & DMA_CCR_DSEC) != 0U) {
      if (ccr_SECM == 0U) {
        /* DSEC can only be set if channel is secure */
        /* Destination channel is non secure */
        status = HAL_ERROR;
      } else {
        SET_BIT(ccr, DMA_CCR_DSEC);
      }
    } else {
      CLEAR_BIT(ccr, DMA_CCR_DSEC);
    }
  }

#endif /* CORE_CM0PLUS */

  /* Update CCR Register: PRIV, SECM, SCEC, DSEC bits */
  WRITE_REG(hdma->Instance->CCR, ccr);

  return status;
}

/**
 * @brief Get the attribute of a DMA channel.
 * @note  Secure and non-secure attributes are only available from secure state
 *        when the system implements the security (ESE=1)
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param ChannelAttributes pointer to return the attributes.
 * @retval HAL Status.
 */
HAL_StatusTypeDef
HAL_DMA_GetConfigChannelAttributes(DMA_HandleTypeDef *hdma,
                                   uint32_t *ChannelAttributes) {
  uint32_t read_attributes;
  uint32_t attributes;

  /* Check the DMA peripheral handle and pointer to returned value */
  if ((hdma == NULL) || (ChannelAttributes == NULL)) {
    return HAL_ERROR;
  }

#if defined(CORE_CM0PLUS)
  /* Get secure or non-secure attributes */
  read_attributes =
      READ_BIT(hdma->Instance->CCR,
               DMA_CCR_PRIV | DMA_CCR_SECM | DMA_CCR_SSEC | DMA_CCR_DSEC);

  /* Get privilege attributes */
  attributes = ((read_attributes & DMA_CCR_PRIV) == 0U) ? DMA_CHANNEL_NPRIV
                                                        : DMA_CHANNEL_PRIV;

  /* Get security attributes */
  attributes |= ((read_attributes & DMA_CCR_SECM) == 0U) ? DMA_CHANNEL_NSEC
                                                         : DMA_CHANNEL_SEC;

  /* Get security attributes of the source */
  attributes |= ((read_attributes & DMA_CCR_SSEC) == 0U) ? DMA_CHANNEL_SRC_NSEC
                                                         : DMA_CHANNEL_SRC_SEC;

  /* Get security attributes of the destination */
  attributes |= ((read_attributes & DMA_CCR_DSEC) == 0U) ? DMA_CHANNEL_DEST_NSEC
                                                         : DMA_CHANNEL_DEST_SEC;

#else

  /* Get secure or non-secure attributes */
  read_attributes = READ_BIT(hdma->Instance->CCR, DMA_CCR_PRIV | DMA_CCR_SECM);

  /* Get privilege attributes */
  attributes = ((read_attributes & DMA_CCR_PRIV) == 0U) ? DMA_CHANNEL_NPRIV
                                                        : DMA_CHANNEL_PRIV;

  /* Get security attributes */
  attributes |= ((read_attributes & DMA_CCR_SECM) == 0U) ? DMA_CHANNEL_NSEC
                                                         : DMA_CHANNEL_SEC;
#endif /* CORE_CM0PLUS */

  /* return value */
  *ChannelAttributes = attributes;

  return HAL_OK;
}
#endif /* DMA_SECURE_SWITCH */
/** @addtogroup DMA_Private_Functions
 * @{
 */

/**
 * @brief Sets the DMA Transfer parameter.
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @param SrcAddress The source memory Buffer address
 * @param DstAddress The destination memory Buffer address
 * @param DataLength The length of data to be transferred from source to
 * destination
 * @retval HAL status
 */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                          uint32_t DstAddress, uint32_t DataLength) {
  /* Clear the DMAMUX synchro overrun flag */
  hdma->DMAmuxChannelStatus->CFR = hdma->DMAmuxChannelStatusMask;

  if (hdma->DMAmuxRequestGen != NULL) {
    /* Clear the DMAMUX request generator overrun flag */
    hdma->DMAmuxRequestGenStatus->RGCFR = hdma->DMAmuxRequestGenStatusMask;
  }

  /* Clear all flags */
  hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hdma->ChannelIndex & 0x1CU));

  /* Configure DMA Channel data length */
  hdma->Instance->CNDTR = DataLength;

  /* Memory to Peripheral */
  if ((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH) {
    /* Configure DMA Channel destination address */
    hdma->Instance->CPAR = DstAddress;

    /* Configure DMA Channel source address */
    hdma->Instance->CMAR = SrcAddress;
  }
  /* Peripheral to Memory */
  else {
    /* Configure DMA Channel source address */
    hdma->Instance->CPAR = SrcAddress;

    /* Configure DMA Channel destination address */
    hdma->Instance->CMAR = DstAddress;
  }
}

/**
 * @brief Updates the DMA handle with the DMAMUX  channel and status mask
 * depending on channel number
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval None
 */
static void DMA_CalcDMAMUXChannelBaseAndMask(DMA_HandleTypeDef *hdma) {
  uint32_t channel_number;

  /* check if instance is not outside the DMA channel range */
  if ((uint32_t)hdma->Instance < (uint32_t)DMA2_Channel1) {
    /* DMA1 */
    /* Associate a DMA Channel to a DMAMUX channel */
    hdma->DMAmuxChannel = (DMAMUX1_Channel0 + (hdma->ChannelIndex >> 2U));

    /* Prepare channel_number used for DMAmuxChannelStatusMask computation */
    channel_number = (((uint32_t)hdma->Instance & 0xFFU) - 8U) / 20U;
  } else {
    /* DMA2 */
    /* Associate a DMA Channel to a DMAMUX channel */
    hdma->DMAmuxChannel = (DMAMUX1_Channel7 + (hdma->ChannelIndex >> 2U));

    /* Prepare channel_number used for DMAmuxChannelStatusMask computation */
    channel_number = (((((uint32_t)hdma->Instance & 0xFFU) - 8U) / 20U) + 7U);
  }

  /* Initialize the field DMAmuxChannelStatus to DMAMUX1_ChannelStatus base */
  hdma->DMAmuxChannelStatus = DMAMUX1_ChannelStatus;

  /* Initialize the field DMAmuxChannelStatusMask with the corresponding index
   * of the DMAMUX channel selected for the current ChannelIndex */
  hdma->DMAmuxChannelStatusMask = 1UL << (channel_number & 0x1FU);
}

/**
 * @brief Updates the DMA handle with the DMAMUX  request generator params
 * @param hdma Pointer to a DMA_HandleTypeDef structure that contains
 *             the configuration information for the specified DMA Channel.
 * @retval None
 */

static void DMA_CalcDMAMUXRequestGenBaseAndMask(DMA_HandleTypeDef *hdma) {
  uint32_t request = hdma->Init.Request & DMAMUX_CxCR_DMAREQ_ID;

  /* DMA Channels are connected to DMAMUX1 request generator blocks*/
  hdma->DMAmuxRequestGen = (DMAMUX_RequestGen_TypeDef *)((
      uint32_t)(((uint32_t)DMAMUX1_RequestGenerator0) + ((request - 1U) * 4U)));

  hdma->DMAmuxRequestGenStatus = DMAMUX1_RequestGenStatus;

  /* here "Request" is either DMA_REQUEST_GENERATOR0 to DMA_REQUEST_GENERATOR3,
   * i.e. <= 4*/
  hdma->DMAmuxRequestGenStatusMask = 1UL << ((request - 1U) & 0x3U);
}

/**
 * @}
 */

/**
 * @}
 */

#endif /* HAL_DMA_MODULE_ENABLED */
/**
 * @}
 */

/**
 * @}
 */
