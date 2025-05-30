/**
  ******************************************************************************
  * @file    stm32wlxx_hal_dac.c
  * @author  MCD Application Team
  * @brief   DAC HAL module driver.
  *         This file provides firmware functions to manage the following
  *         functionalities of the Digital to Analog Converter (DAC) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  *
  *
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
                      ##### DAC Peripheral features #####
  ==============================================================================
    [..]
      *** DAC Channels ***
      ====================
    [..]
    STM32WL devices integrate one 12-bit Digital Analog Converter

    The single converters (i.e. channel1)
    can be used:
      (#) DAC channel1 with DAC_OUT1 (PA10) as output or connected to on-chip
          peripherals (ex. timers, ADC).

      *** DAC Triggers ***
      ====================
    [..]
    Digital to Analog conversion can be non-triggered using DAC_TRIGGER_NONE
    and DAC_OUT1 is available once writing to DHRx register.
    [..]
    Digital to Analog conversion can be triggered by:
      (#) External event: EXTI Line 9 (any GPIOx_PIN_9) using
DAC_TRIGGER_EXT_IT9. The used pin (GPIOx_PIN_9) must be configured in input
mode.

      (#) Timers TRGO: TIM1, TIM2, LPTIM1, LPTIM2, LPTIM3
          (DAC_TRIGGER_T1_TRGO, DAC_TRIGGER_T2_TRGO...)

      (#) Software using DAC_TRIGGER_SOFTWARE

      *** DAC Buffer mode feature ***
      ===============================
      [..]
      Each DAC channel integrates an output buffer that can be used to
      reduce the output impedance, and to drive external loads directly
      without having to add an external operational amplifier.
      To enable, the output buffer use
      sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
      [..]
      (@) Refer to the device datasheet for more details about output
          impedance value with and without output buffer.

      *** GPIO configurations guidelines ***
      =====================
      [..]
      When a DAC channel is used (ex channel1 on PA4) and the other is not
      (ex channel2 on PA5 is configured in Analog and disabled).
      Channel1 may disturb channel2 as coupling effect.
      Note that there is no coupling on channel2 as soon as channel2 is turned
on. Coupling on adjacent channel could be avoided as follows: when unused PA5 is
configured as INPUT PULL-UP or DOWN. PA5 is configured in ANALOG just before it
is turned on.

      *** DAC Sample and Hold feature ***
      ========================
      [..]
      For each converter, 2 modes are supported: normal mode and
      "sample and hold" mode (i.e. low power mode).
      In the sample and hold mode, the DAC core converts data, then holds the
      converted voltage on a capacitor. When not converting, the DAC cores and
      buffer are completely turned off between samples and the DAC output is
      tri-stated, therefore  reducing the overall power consumption. A new
      stabilization period is needed before each new conversion.

      The sample and hold allow setting internal or external voltage @
      low power consumption cost (output value can be at any given rate either
      by CPU or DMA).

      The Sample and hold block and registers uses either LSI & run in
      several power modes: run mode, sleep mode, low power run, low power sleep
      mode & stop1 mode.

      Low power stop1 mode allows only static conversion.

      To enable Sample and Hold mode
      Enable LSI using HAL_RCC_OscConfig with RCC_OSCILLATORTYPE_LSI &
      RCC_LSI_ON parameters.

      Use DAC_InitStructure.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_ENABLE;
         & DAC_ChannelConfTypeDef.DAC_SampleAndHoldConfig.DAC_SampleTime,
           DAC_HoldTime & DAC_RefreshTime;

       *** DAC calibration feature ***
       ===================================
      [..]
       (#)  The 2 converters (channel1 & channel2) provide calibration
capabilities.
       (++) Calibration aims at correcting some offset of output buffer.
       (++) The DAC uses either factory calibration settings OR user defined
           calibration (trimming) settings (i.e. trimming mode).
       (++) The user defined settings can be figured out using self calibration
           handled by HAL_DACEx_SelfCalibrate.
       (++) HAL_DACEx_SelfCalibrate:
       (+++) Runs automatically the calibration.
       (+++) Enables the user trimming mode
       (+++) Updates a structure with trimming values with fresh calibration
            results.
            The user may store the calibration results for larger
            (ex monitoring the trimming as a function of temperature
            for instance)

       *** DAC wave generation feature ***
       ===================================
       [..]
       Both DAC channels can be used to generate
         (#) Noise wave
         (#) Triangle wave

       *** DAC data format ***
       =======================
       [..]
       The DAC data format can be:
         (#) 8-bit right alignment using DAC_ALIGN_8B_R
         (#) 12-bit left alignment using DAC_ALIGN_12B_L
         (#) 12-bit right alignment using DAC_ALIGN_12B_R

       *** DAC data value to voltage correspondence ***
       ================================================
       [..]
       The analog output voltage on each DAC channel pin is determined
       by the following equation:
       [..]
       DAC_OUTx = VREF+ * DOR / 4095
       (+) with  DOR is the Data Output Register
       [..]
          VREF+ is the input voltage reference (refer to the device datasheet)
       [..]
        e.g. To set DAC_OUT1 to 0.7V, use
       (+) Assuming that VREF+ = 3.3V, DAC_OUT1 = (3.3 * 868) / 4095 = 0.7V

       *** DMA requests ***
       =====================
       [..]
       A DMA request can be generated when an external trigger (but not a
software trigger) occurs if DMA1 requests are enabled using HAL_DAC_Start_DMA().
       DMA requests are mapped as following:
      (#) DAC channel1: mapped on DMA request 6 (can be any DMA channel)

     [..]
    (@) For Dual mode and specific signal (Triangle and noise) generation please
        refer to Extended Features Driver description

                      ##### How to use this driver #####
  ==============================================================================
    [..]
      (+) DAC APB clock must be enabled to get write access to DAC
          registers using HAL_DAC_Init()
      (+) Configure DAC_OUTx (DAC_OUT1: PA10) in analog mode.
      (+) Configure the DAC channel using HAL_DAC_ConfigChannel() function.
      (+) Enable the DAC channel using HAL_DAC_Start() or HAL_DAC_Start_DMA()
functions.

     *** Calibration mode IO operation ***
     ======================================
     [..]
       (+) Retrieve the factory trimming (calibration settings) using
HAL_DACEx_GetTrimOffset()
       (+) Run the calibration using HAL_DACEx_SelfCalibrate()
       (+) Update the trimming while DAC running using
HAL_DACEx_SetUserTrimming()

     *** Polling mode IO operation ***
     =================================
     [..]
       (+) Start the DAC peripheral using HAL_DAC_Start()
       (+) To read the DAC last data output value, use the HAL_DAC_GetValue()
function.
       (+) Stop the DAC peripheral using HAL_DAC_Stop()

     *** DMA mode IO operation ***
     ==============================
     [..]
       (+) Start the DAC peripheral using HAL_DAC_Start_DMA(), at this stage the
user specify the length of data to be transferred at each end of conversion
           First issued trigger will start the conversion of the value
previously set by HAL_DAC_SetValue().
       (+) At the middle of data transfer HAL_DAC_ConvHalfCpltCallbackCh1()
           function is executed and user can add his own code by customization
of function pointer HAL_DAC_ConvHalfCpltCallbackCh1()
       (+) At The end of data transfer HAL_DAC_ConvCpltCallbackCh1()
           function is executed and user can add his own code by customization
of function pointer HAL_DAC_ConvCpltCallbackCh1()
       (+) In case of transfer Error, HAL_DAC_ErrorCallbackCh1() function is
executed and user can add his own code by customization of function pointer
HAL_DAC_ErrorCallbackCh1
       (+) In case of DMA underrun, DAC interruption triggers and execute
internal function HAL_DAC_IRQHandler. HAL_DAC_DMAUnderrunCallbackCh1() function
is executed and user can add his own code by customization of function pointer
           HAL_DAC_DMAUnderrunCallbackCh1() and
           add his own code by customization of function pointer
HAL_DAC_ErrorCallbackCh1()
       (+) Stop the DAC peripheral using HAL_DAC_Stop_DMA()

    *** Callback registration ***
    =============================================
    [..]
      The compilation define  USE_HAL_DAC_REGISTER_CALLBACKS when set to 1
      allows the user to configure dynamically the driver callbacks.

    Use Functions @ref HAL_DAC_RegisterCallback() to register a user callback,
      it allows to register following callbacks:
      (+) ConvCpltCallbackCh1     : callback when a half transfer is completed
on Ch1.
      (+) ConvHalfCpltCallbackCh1 : callback when a transfer is completed on
Ch1.
      (+) ErrorCallbackCh1        : callback when an error occurs on Ch1.
      (+) DMAUnderrunCallbackCh1  : callback when an underrun error occurs on
Ch1.
      (+) MspInitCallback         : DAC MspInit.
      (+) MspDeInitCallback       : DAC MspdeInit.
      This function takes as parameters the HAL peripheral handle, the Callback
ID and a pointer to the user callback function.

    Use function @ref HAL_DAC_UnRegisterCallback() to reset a callback to the
default weak (surcharged) function. It allows to reset following callbacks:
      (+) ConvCpltCallbackCh1     : callback when a half transfer is completed
on Ch1.
      (+) ConvHalfCpltCallbackCh1 : callback when a transfer is completed on
Ch1.
      (+) ErrorCallbackCh1        : callback when an error occurs on Ch1.
      (+) DMAUnderrunCallbackCh1  : callback when an underrun error occurs on
Ch1.
      (+) MspInitCallback         : DAC MspInit.
      (+) MspDeInitCallback       : DAC MspdeInit.
      (+) All Callbacks
      This function) takes as parameters the HAL peripheral handle and the
Callback ID.

      By default, after the @ref HAL_DAC_Init and if the state is
HAL_DAC_STATE_RESET all callbacks are reset to the corresponding legacy weak
(surcharged) functions. Exception done for MspInit and MspDeInit callbacks that
are respectively reset to the legacy weak (surcharged) functions in the @ref
HAL_DAC_Init and @ref  HAL_DAC_DeInit only when these callbacks are null (not
registered beforehand). If not, MspInit or MspDeInit are not null, the @ref
HAL_DAC_Init and @ref HAL_DAC_DeInit keep and use the user MspInit/MspDeInit
callbacks (registered beforehand)

      Callbacks can be registered/unregistered in READY state only.
      Exception done for MspInit/MspDeInit callbacks that can be
registered/unregistered in READY or RESET state, thus registered (user)
MspInit/DeInit callbacks can be used during the Init/DeInit. In that case first
register the MspInit/MspDeInit user callbacks using @ref
HAL_DAC_RegisterCallback before calling @ref HAL_DAC_DeInit or @ref HAL_DAC_Init
function.

      When The compilation define USE_HAL_DAC_REGISTER_CALLBACKS is set to 0 or
      not defined, the callback registering feature is not available
      and weak (surcharged) callbacks are used.

     *** DAC HAL driver macros list ***
     =============================================
     [..]
       Below the list of most used macros in DAC HAL driver.

      (+) __HAL_DAC_ENABLE : Enable the DAC peripheral
      (+) __HAL_DAC_DISABLE : Disable the DAC peripheral
      (+) __HAL_DAC_CLEAR_FLAG: Clear the DAC's pending flags
      (+) __HAL_DAC_GET_FLAG: Get the selected DAC's flag status

     [..]
      (@) You can refer to the DAC HAL driver header file for more useful macros

@endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/** @addtogroup STM32WLxx_HAL_Driver
 * @{
 */

#ifdef HAL_DAC_MODULE_ENABLED
#if defined(DAC)

/** @defgroup DAC DAC
 * @brief DAC driver modules
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup DAC_Private_Constants DAC Private Constants
 * @{
 */
#define TIMEOUT_DAC_CALIBCONFIG 1U /* 1   ms        */

/**
 * @}
 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions -------------------------------------------------------*/

/** @defgroup DAC_Exported_Functions DAC Exported Functions
 * @{
 */

/** @defgroup DAC_Exported_Functions_Group1 Initialization and de-initialization
functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
  ==============================================================================
              ##### Initialization and de-initialization functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the DAC.
      (+) De-initialize the DAC.

@endverbatim
  * @{
  */

/**
 * @brief  Initialize the DAC peripheral according to the specified parameters
 *         in the DAC_InitStruct and initialize the associated handle.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_Init(DAC_HandleTypeDef *hdac) {
  /* Check DAC handle */
  if (hdac == NULL) {
    return HAL_ERROR;
  }
  /* Check the parameters */
  assert_param(IS_DAC_ALL_INSTANCE(hdac->Instance));

  if (hdac->State == HAL_DAC_STATE_RESET) {
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
    /* Init the DAC Callback settings */
    hdac->ConvCpltCallbackCh1 = HAL_DAC_ConvCpltCallbackCh1;
    hdac->ConvHalfCpltCallbackCh1 = HAL_DAC_ConvHalfCpltCallbackCh1;
    hdac->ErrorCallbackCh1 = HAL_DAC_ErrorCallbackCh1;
    hdac->DMAUnderrunCallbackCh1 = HAL_DAC_DMAUnderrunCallbackCh1;
    if (hdac->MspInitCallback == NULL) {
      hdac->MspInitCallback = HAL_DAC_MspInit;
    }
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

    /* Allocate lock resource and initialize it */
    hdac->Lock = HAL_UNLOCKED;

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
    /* Init the low level hardware */
    hdac->MspInitCallback(hdac);
#else
    /* Init the low level hardware */
    HAL_DAC_MspInit(hdac);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */
  }

  /* Initialize the DAC state*/
  hdac->State = HAL_DAC_STATE_BUSY;

  /* Set DAC error code to none */
  hdac->ErrorCode = HAL_DAC_ERROR_NONE;

  /* Initialize the DAC state*/
  hdac->State = HAL_DAC_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
 * @brief  Deinitialize the DAC peripheral registers to their default reset
 * values.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_DeInit(DAC_HandleTypeDef *hdac) {
  /* Check DAC handle */
  if (hdac == NULL) {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_DAC_ALL_INSTANCE(hdac->Instance));

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  if (hdac->MspDeInitCallback == NULL) {
    hdac->MspDeInitCallback = HAL_DAC_MspDeInit;
  }
  /* DeInit the low level hardware */
  hdac->MspDeInitCallback(hdac);
#else
  /* DeInit the low level hardware */
  HAL_DAC_MspDeInit(hdac);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

  /* Set DAC error code to none */
  hdac->ErrorCode = HAL_DAC_ERROR_NONE;

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

/**
 * @brief  Initialize the DAC MSP.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
__weak void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DAC_MspInit could be implemented in the user file
   */
}

/**
 * @brief  DeInitialize the DAC MSP.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
__weak void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DAC_MspDeInit could be implemented in the user file
   */
}

/**
 * @}
 */

/** @defgroup DAC_Exported_Functions_Group2 IO operation functions
  *  @brief    IO operation functions
  *
@verbatim
  ==============================================================================
             ##### IO operation functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Start conversion.
      (+) Stop conversion.
      (+) Start conversion and enable DMA transfer.
      (+) Stop conversion and disable DMA transfer.
      (+) Get result of conversion.

@endverbatim
  * @{
  */

/**
 * @brief  Enables DAC and starts conversion of channel.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @param  Channel The selected DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_CHANNEL_1: DAC Channel1 selected
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef *hdac, uint32_t Channel) {
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  /* Enable the Peripheral */
  __HAL_DAC_ENABLE(hdac, Channel);

  /* Check if software trigger enabled */
  if ((hdac->Instance->CR & (DAC_CR_TEN1 | DAC_CR_TSEL1)) ==
      DAC_TRIGGER_SOFTWARE) {
    /* Enable the selected DAC software conversion */
    SET_BIT(hdac->Instance->SWTRIGR, DAC_SWTRIGR_SWTRIG1);
  }

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return HAL_OK;
}

/**
 * @brief  Disables DAC and stop conversion of channel.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @param  Channel The selected DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_CHANNEL_1: DAC Channel1 selected
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_Stop(DAC_HandleTypeDef *hdac, uint32_t Channel) {
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Disable the Peripheral */
  __HAL_DAC_DISABLE(hdac, Channel);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
 * @brief  Enables DAC and starts conversion of channel.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @param  Channel The selected DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_CHANNEL_1: DAC Channel1 selected
 * @param  pData The source Buffer address.
 * @param  Length The length of data to be transferred from memory to DAC
 * peripheral
 * @param  Alignment Specifies the data alignment for DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
 *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
 *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef *hdac, uint32_t Channel,
                                    const uint32_t *pData, uint32_t Length,
                                    uint32_t Alignment) {
  HAL_StatusTypeDef status;
  uint32_t tmpreg = 0U;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_ALIGN(Alignment));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  if (Channel == DAC_CHANNEL_1) {
    /* Set the DMA transfer complete callback for channel1 */
    hdac->DMA_Handle1->XferCpltCallback = DAC_DMAConvCpltCh1;

    /* Set the DMA half transfer complete callback for channel1 */
    hdac->DMA_Handle1->XferHalfCpltCallback = DAC_DMAHalfConvCpltCh1;

    /* Set the DMA error callback for channel1 */
    hdac->DMA_Handle1->XferErrorCallback = DAC_DMAErrorCh1;

    /* Enable the selected DAC channel1 DMA request */
    SET_BIT(hdac->Instance->CR, DAC_CR_DMAEN1);

    /* Case of use of channel 1 */
    switch (Alignment) {
    case DAC_ALIGN_12B_R:
      /* Get DHR12R1 address */
      tmpreg = (uint32_t)&hdac->Instance->DHR12R1;
      break;
    case DAC_ALIGN_12B_L:
      /* Get DHR12L1 address */
      tmpreg = (uint32_t)&hdac->Instance->DHR12L1;
      break;
    default: /* case DAC_ALIGN_8B_R */
      /* Get DHR8R1 address */
      tmpreg = (uint32_t)&hdac->Instance->DHR8R1;
      break;
    }
  }

  /* Enable the DAC DMA underrun interrupt */
  __HAL_DAC_ENABLE_IT(hdac, DAC_IT_DMAUDR1);

  /* Enable the DMA channel */
  status = HAL_DMA_Start_IT(hdac->DMA_Handle1, (uint32_t)pData, tmpreg, Length);

  /* Process Unlocked */
  __HAL_UNLOCK(hdac);

  if (status == HAL_OK) {
    /* Enable the Peripheral */
    __HAL_DAC_ENABLE(hdac, Channel);
  } else {
    hdac->ErrorCode |= HAL_DAC_ERROR_DMA;
  }

  /* Return function status */
  return status;
}

/**
 * @brief  Disables DAC and stop conversion of channel.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @param  Channel The selected DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_CHANNEL_1: DAC Channel1 selected
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef *hdac, uint32_t Channel) {
  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Disable the selected DAC channel DMA request */
  hdac->Instance->CR &= ~(DAC_CR_DMAEN1 << (Channel & 0x10UL));

  /* Disable the Peripheral */
  __HAL_DAC_DISABLE(hdac, Channel);

  /* Disable the DMA channel */

  /* Disable the DMA channel */
  (void)HAL_DMA_Abort(hdac->DMA_Handle1);

  /* Disable the DAC DMA underrun interrupt */
  __HAL_DAC_DISABLE_IT(hdac, DAC_IT_DMAUDR1);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
 * @brief  Handles DAC interrupt request
 *         This function uses the interruption of DMA
 *         underrun.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
void HAL_DAC_IRQHandler(DAC_HandleTypeDef *hdac) {
  uint32_t itsource = hdac->Instance->CR;
  uint32_t itflag = hdac->Instance->SR;

  if ((itsource & DAC_IT_DMAUDR1) == DAC_IT_DMAUDR1) {
    /* Check underrun flag of DAC channel 1 */
    if ((itflag & DAC_FLAG_DMAUDR1) == DAC_FLAG_DMAUDR1) {
      /* Change DAC state to error state */
      hdac->State = HAL_DAC_STATE_ERROR;

      /* Set DAC error code to channel1 DMA underrun error */
      SET_BIT(hdac->ErrorCode, HAL_DAC_ERROR_DMAUNDERRUNCH1);

      /* Clear the underrun flag */
      __HAL_DAC_CLEAR_FLAG(hdac, DAC_FLAG_DMAUDR1);

      /* Disable the selected DAC channel1 DMA request */
      __HAL_DAC_DISABLE_IT(hdac, DAC_CR_DMAEN1);

      /* Error callback */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
      hdac->DMAUnderrunCallbackCh1(hdac);
#else
      HAL_DAC_DMAUnderrunCallbackCh1(hdac);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */
    }
  }
}

/**
 * @brief  Set the specified data holding register value for DAC channel.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @param  Channel The selected DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_CHANNEL_1: DAC Channel1 selected
 * @param  Alignment Specifies the data alignment.
 *          This parameter can be one of the following values:
 *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
 *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
 *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
 * @param  Data Data to be loaded in the selected data holding register.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef *hdac, uint32_t Channel,
                                   uint32_t Alignment, uint32_t Data) {
  __IO uint32_t tmp = 0UL;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));
  assert_param(IS_DAC_ALIGN(Alignment));
  assert_param(IS_DAC_DATA(Data));

  tmp = (uint32_t)hdac->Instance;
  if (Channel == DAC_CHANNEL_1) {
    tmp += DAC_DHR12R1_ALIGNMENT(Alignment);
  }

  /* Set the DAC channel selected data holding register */
  *(__IO uint32_t *)tmp = Data;

  /* Return function status */
  return HAL_OK;
}

/**
 * @brief  Conversion complete callback in non-blocking mode for Channel1
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
__weak void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DAC_ConvCpltCallbackCh1 could be implemented in the user
     file
   */
}

/**
 * @brief  Conversion half DMA transfer callback in non-blocking mode for
 * Channel1
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
__weak void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DAC_ConvHalfCpltCallbackCh1 could be implemented in the user
     file
   */
}

/**
 * @brief  Error DAC callback for Channel1.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
__weak void HAL_DAC_ErrorCallbackCh1(DAC_HandleTypeDef *hdac) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DAC_ErrorCallbackCh1 could be implemented in the user file
   */
}

/**
 * @brief  DMA underrun DAC callback for channel1.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
__weak void HAL_DAC_DMAUnderrunCallbackCh1(DAC_HandleTypeDef *hdac) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdac);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DAC_DMAUnderrunCallbackCh1 could be implemented in the user
     file
   */
}

/**
 * @}
 */

/** @defgroup DAC_Exported_Functions_Group3 Peripheral Control functions
  *  @brief    Peripheral Control functions
  *
@verbatim
  ==============================================================================
             ##### Peripheral Control functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure channels.
      (+) Set the specified data holding register value for DAC channel.

@endverbatim
  * @{
  */

/**
 * @brief  Returns the last data output value of the selected DAC channel.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @param  Channel The selected DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_CHANNEL_1: DAC Channel1 selected
 * @retval The selected DAC channel data output value.
 */
uint32_t HAL_DAC_GetValue(const DAC_HandleTypeDef *hdac, uint32_t Channel) {
  uint32_t result;

  /* Check the parameters */
  assert_param(IS_DAC_CHANNEL(Channel));

  result = hdac->Instance->DOR1;

  /* Returns the DAC channel data output register value */
  return result;
}

/**
 * @brief  Configures the selected DAC channel.
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @param  sConfig DAC configuration structure.
 * @param  Channel The selected DAC channel.
 *          This parameter can be one of the following values:
 *            @arg DAC_CHANNEL_1: DAC Channel1 selected
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_DAC_ConfigChannel(DAC_HandleTypeDef *hdac,
                                        const DAC_ChannelConfTypeDef *sConfig,
                                        uint32_t Channel) {
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpreg1;
  uint32_t tmpreg2;
  uint32_t tickstart;

  /* Check the DAC parameters */
  assert_param(IS_DAC_TRIGGER(sConfig->DAC_Trigger));
  assert_param(IS_DAC_OUTPUT_BUFFER_STATE(sConfig->DAC_OutputBuffer));
  assert_param(IS_DAC_CHIP_CONNECTION(sConfig->DAC_ConnectOnChipPeripheral));
  assert_param(IS_DAC_TRIMMING(sConfig->DAC_UserTrimming));
  if ((sConfig->DAC_UserTrimming) == DAC_TRIMMING_USER) {
    assert_param(IS_DAC_TRIMMINGVALUE(sConfig->DAC_TrimmingValue));
  }
  assert_param(IS_DAC_SAMPLEANDHOLD(sConfig->DAC_SampleAndHold));
  if ((sConfig->DAC_SampleAndHold) == DAC_SAMPLEANDHOLD_ENABLE) {
    assert_param(
        IS_DAC_SAMPLETIME(sConfig->DAC_SampleAndHoldConfig.DAC_SampleTime));
    assert_param(
        IS_DAC_HOLDTIME(sConfig->DAC_SampleAndHoldConfig.DAC_HoldTime));
    assert_param(
        IS_DAC_REFRESHTIME(sConfig->DAC_SampleAndHoldConfig.DAC_RefreshTime));
  }
  assert_param(IS_DAC_CHANNEL(Channel));

  /* Process locked */
  __HAL_LOCK(hdac);

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_BUSY;

  /* Sample and hold configuration */
  if (sConfig->DAC_SampleAndHold == DAC_SAMPLEANDHOLD_ENABLE) {
    /* Get timeout */
    tickstart = HAL_GetTick();

    /* SHSR1 can be written when BWST1 is cleared */
    while (((hdac->Instance->SR) & DAC_SR_BWST1) != 0UL) {
      /* Check for the Timeout */
      if ((HAL_GetTick() - tickstart) > TIMEOUT_DAC_CALIBCONFIG) {
        /* New check to avoid false timeout detection in case of preemption */
        if (((hdac->Instance->SR) & DAC_SR_BWST1) != 0UL) {
          /* Update error code */
          SET_BIT(hdac->ErrorCode, HAL_DAC_ERROR_TIMEOUT);

          /* Change the DMA state */
          hdac->State = HAL_DAC_STATE_TIMEOUT;

          return HAL_TIMEOUT;
        }
      }
    }
    hdac->Instance->SHSR1 = sConfig->DAC_SampleAndHoldConfig.DAC_SampleTime;

    /* HoldTime */
    MODIFY_REG(hdac->Instance->SHHR, DAC_SHHR_THOLD1 << (Channel & 0x10UL),
               (sConfig->DAC_SampleAndHoldConfig.DAC_HoldTime)
                   << (Channel & 0x10UL));
    /* RefreshTime */
    MODIFY_REG(hdac->Instance->SHRR, DAC_SHRR_TREFRESH1 << (Channel & 0x10UL),
               (sConfig->DAC_SampleAndHoldConfig.DAC_RefreshTime)
                   << (Channel & 0x10UL));
  }

  if (sConfig->DAC_UserTrimming == DAC_TRIMMING_USER)
  /* USER TRIMMING */
  {
    /* Get the DAC CCR value */
    tmpreg1 = hdac->Instance->CCR;
    /* Clear trimming value */
    tmpreg1 &= ~(((uint32_t)(DAC_CCR_OTRIM1)) << (Channel & 0x10UL));
    /* Configure for the selected trimming offset */
    tmpreg2 = sConfig->DAC_TrimmingValue;
    /* Calculate CCR register value depending on DAC_Channel */
    tmpreg1 |= tmpreg2 << (Channel & 0x10UL);
    /* Write to DAC CCR */
    hdac->Instance->CCR = tmpreg1;
  }
  /* else factory trimming is used (factory setting are available at reset)*/
  /* SW Nothing has nothing to do */

  /* Get the DAC MCR value */
  tmpreg1 = hdac->Instance->MCR;
  /* Clear DAC_MCR_MODEx bits */
  tmpreg1 &= ~(((uint32_t)(DAC_MCR_MODE1)) << (Channel & 0x10UL));
  /* Configure for the selected DAC channel: mode, buffer output & on chip
   * peripheral connect */
  tmpreg2 = (sConfig->DAC_SampleAndHold | sConfig->DAC_OutputBuffer |
             sConfig->DAC_ConnectOnChipPeripheral);
  /* Calculate MCR register value depending on DAC_Channel */
  tmpreg1 |= tmpreg2 << (Channel & 0x10UL);
  /* Write to DAC MCR */
  hdac->Instance->MCR = tmpreg1;

  /* DAC in normal operating mode hence clear DAC_CR_CENx bit */
  CLEAR_BIT(hdac->Instance->CR, DAC_CR_CEN1 << (Channel & 0x10UL));

  /* Get the DAC CR value */
  tmpreg1 = hdac->Instance->CR;
  /* Clear TENx, TSELx, WAVEx and MAMPx bits */
  tmpreg1 &=
      ~(((uint32_t)(DAC_CR_MAMP1 | DAC_CR_WAVE1 | DAC_CR_TSEL1 | DAC_CR_TEN1))
        << (Channel & 0x10UL));
  /* Configure for the selected DAC channel: trigger */
  /* Set TSELx and TENx bits according to DAC_Trigger value */
  tmpreg2 = sConfig->DAC_Trigger;
  /* Calculate CR register value depending on DAC_Channel */
  tmpreg1 |= tmpreg2 << (Channel & 0x10UL);
  /* Write to DAC CR */
  hdac->Instance->CR = tmpreg1;
  /* Disable wave generation */
  CLEAR_BIT(hdac->Instance->CR, (DAC_CR_WAVE1 << (Channel & 0x10UL)));

  /* Change DAC state */
  hdac->State = HAL_DAC_STATE_READY;

  /* Process unlocked */
  __HAL_UNLOCK(hdac);

  /* Return function status */
  return status;
}

/**
 * @}
 */

/** @defgroup DAC_Exported_Functions_Group4 Peripheral State and Errors
functions
  *  @brief   Peripheral State and Errors functions
  *
@verbatim
  ==============================================================================
            ##### Peripheral State and Errors functions #####
  ==============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Check the DAC state.
      (+) Check the DAC Errors.

@endverbatim
  * @{
  */

/**
 * @brief  return the DAC handle state
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval HAL state
 */
HAL_DAC_StateTypeDef HAL_DAC_GetState(const DAC_HandleTypeDef *hdac) {
  /* Return DAC handle state */
  return hdac->State;
}

/**
 * @brief  Return the DAC error code
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval DAC Error Code
 */
uint32_t HAL_DAC_GetError(const DAC_HandleTypeDef *hdac) {
  return hdac->ErrorCode;
}

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup DAC_Exported_Functions
 * @{
 */

/** @addtogroup DAC_Exported_Functions_Group1
 * @{
 */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User DAC Callback
 *         To be used instead of the weak (surcharged) predefined callback
 * @note   The HAL_DAC_RegisterCallback() may be called before HAL_DAC_Init() in
 * HAL_DAC_STATE_RESET to register callbacks for HAL_DAC_MSPINIT_CB_ID and
 * HAL_DAC_MSPDEINIT_CB_ID
 * @param  hdac DAC handle
 * @param  CallbackID ID of the callback to be registered
 *         This parameter can be one of the following values:
 *          @arg @ref HAL_DAC_ERROR_INVALID_CALLBACK   DAC Error Callback ID
 *          @arg @ref HAL_DAC_CH1_COMPLETE_CB_ID       DAC CH1 Complete Callback
 * ID
 *          @arg @ref HAL_DAC_CH1_HALF_COMPLETE_CB_ID  DAC CH1 Half Complete
 * Callback ID
 *          @arg @ref HAL_DAC_CH1_ERROR_ID             DAC CH1 Error Callback ID
 *          @arg @ref HAL_DAC_CH1_UNDERRUN_CB_ID       DAC CH1 UnderRun Callback
 * ID
 *          @arg @ref HAL_DAC_MSPINIT_CB_ID            DAC MSP Init Callback ID
 *          @arg @ref HAL_DAC_MSPDEINIT_CB_ID          DAC MSP DeInit Callback
 * ID
 *
 * @param  pCallback pointer to the Callback function
 * @retval status
 */
HAL_StatusTypeDef HAL_DAC_RegisterCallback(DAC_HandleTypeDef *hdac,
                                           HAL_DAC_CallbackIDTypeDef CallbackID,
                                           pDAC_CallbackTypeDef pCallback) {
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL) {
    /* Update the error code */
    hdac->ErrorCode |= HAL_DAC_ERROR_INVALID_CALLBACK;
    return HAL_ERROR;
  }

  if (hdac->State == HAL_DAC_STATE_READY) {
    switch (CallbackID) {
    case HAL_DAC_CH1_COMPLETE_CB_ID:
      hdac->ConvCpltCallbackCh1 = pCallback;
      break;
    case HAL_DAC_CH1_HALF_COMPLETE_CB_ID:
      hdac->ConvHalfCpltCallbackCh1 = pCallback;
      break;
    case HAL_DAC_CH1_ERROR_ID:
      hdac->ErrorCallbackCh1 = pCallback;
      break;
    case HAL_DAC_CH1_UNDERRUN_CB_ID:
      hdac->DMAUnderrunCallbackCh1 = pCallback;
      break;
    case HAL_DAC_MSPINIT_CB_ID:
      hdac->MspInitCallback = pCallback;
      break;
    case HAL_DAC_MSPDEINIT_CB_ID:
      hdac->MspDeInitCallback = pCallback;
      break;
    default:
      /* Update the error code */
      hdac->ErrorCode |= HAL_DAC_ERROR_INVALID_CALLBACK;
      /* update return status */
      status = HAL_ERROR;
      break;
    }
  } else if (hdac->State == HAL_DAC_STATE_RESET) {
    switch (CallbackID) {
    case HAL_DAC_MSPINIT_CB_ID:
      hdac->MspInitCallback = pCallback;
      break;
    case HAL_DAC_MSPDEINIT_CB_ID:
      hdac->MspDeInitCallback = pCallback;
      break;
    default:
      /* Update the error code */
      hdac->ErrorCode |= HAL_DAC_ERROR_INVALID_CALLBACK;
      /* update return status */
      status = HAL_ERROR;
      break;
    }
  } else {
    /* Update the error code */
    hdac->ErrorCode |= HAL_DAC_ERROR_INVALID_CALLBACK;
    /* update return status */
    status = HAL_ERROR;
  }

  return status;
}

/**
 * @brief  Unregister a User DAC Callback
 *         DAC Callback is redirected to the weak (surcharged) predefined
 * callback
 * @note   The HAL_DAC_UnRegisterCallback() may be called before HAL_DAC_Init()
 * in HAL_DAC_STATE_RESET to un-register callbacks for HAL_DAC_MSPINIT_CB_ID and
 * HAL_DAC_MSPDEINIT_CB_ID
 * @param  hdac DAC handle
 * @param  CallbackID ID of the callback to be unregistered
 *         This parameter can be one of the following values:
 *          @arg @ref HAL_DAC_CH1_COMPLETE_CB_ID          DAC CH1 transfer
 * Complete Callback ID
 *          @arg @ref HAL_DAC_CH1_HALF_COMPLETE_CB_ID     DAC CH1 Half Complete
 * Callback ID
 *          @arg @ref HAL_DAC_CH1_ERROR_ID                DAC CH1 Error Callback
 * ID
 *          @arg @ref HAL_DAC_CH1_UNDERRUN_CB_ID          DAC CH1 UnderRun
 * Callback ID
 *          @arg @ref HAL_DAC_MSPINIT_CB_ID               DAC MSP Init Callback
 * ID
 *          @arg @ref HAL_DAC_MSPDEINIT_CB_ID             DAC MSP DeInit
 * Callback ID
 *          @arg @ref HAL_DAC_ALL_CB_ID                   DAC All callbacks
 * @retval status
 */
HAL_StatusTypeDef
HAL_DAC_UnRegisterCallback(DAC_HandleTypeDef *hdac,
                           HAL_DAC_CallbackIDTypeDef CallbackID) {
  HAL_StatusTypeDef status = HAL_OK;

  if (hdac->State == HAL_DAC_STATE_READY) {
    switch (CallbackID) {
    case HAL_DAC_CH1_COMPLETE_CB_ID:
      hdac->ConvCpltCallbackCh1 = HAL_DAC_ConvCpltCallbackCh1;
      break;
    case HAL_DAC_CH1_HALF_COMPLETE_CB_ID:
      hdac->ConvHalfCpltCallbackCh1 = HAL_DAC_ConvHalfCpltCallbackCh1;
      break;
    case HAL_DAC_CH1_ERROR_ID:
      hdac->ErrorCallbackCh1 = HAL_DAC_ErrorCallbackCh1;
      break;
    case HAL_DAC_CH1_UNDERRUN_CB_ID:
      hdac->DMAUnderrunCallbackCh1 = HAL_DAC_DMAUnderrunCallbackCh1;
      break;
    case HAL_DAC_MSPINIT_CB_ID:
      hdac->MspInitCallback = HAL_DAC_MspInit;
      break;
    case HAL_DAC_MSPDEINIT_CB_ID:
      hdac->MspDeInitCallback = HAL_DAC_MspDeInit;
      break;
    case HAL_DAC_ALL_CB_ID:
      hdac->ConvCpltCallbackCh1 = HAL_DAC_ConvCpltCallbackCh1;
      hdac->ConvHalfCpltCallbackCh1 = HAL_DAC_ConvHalfCpltCallbackCh1;
      hdac->ErrorCallbackCh1 = HAL_DAC_ErrorCallbackCh1;
      hdac->DMAUnderrunCallbackCh1 = HAL_DAC_DMAUnderrunCallbackCh1;
      hdac->MspInitCallback = HAL_DAC_MspInit;
      hdac->MspDeInitCallback = HAL_DAC_MspDeInit;
      break;
    default:
      /* Update the error code */
      hdac->ErrorCode |= HAL_DAC_ERROR_INVALID_CALLBACK;
      /* update return status */
      status = HAL_ERROR;
      break;
    }
  } else if (hdac->State == HAL_DAC_STATE_RESET) {
    switch (CallbackID) {
    case HAL_DAC_MSPINIT_CB_ID:
      hdac->MspInitCallback = HAL_DAC_MspInit;
      break;
    case HAL_DAC_MSPDEINIT_CB_ID:
      hdac->MspDeInitCallback = HAL_DAC_MspDeInit;
      break;
    default:
      /* Update the error code */
      hdac->ErrorCode |= HAL_DAC_ERROR_INVALID_CALLBACK;
      /* update return status */
      status = HAL_ERROR;
      break;
    }
  } else {
    /* Update the error code */
    hdac->ErrorCode |= HAL_DAC_ERROR_INVALID_CALLBACK;
    /* update return status */
    status = HAL_ERROR;
  }

  return status;
}
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup DAC_Private_Functions
 * @{
 */

/**
 * @brief  DMA conversion complete callback.
 * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
 *                the configuration information for the specified DMA module.
 * @retval None
 */
void DAC_DMAConvCpltCh1(DMA_HandleTypeDef *hdma) {
  DAC_HandleTypeDef *hdac =
      (DAC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  hdac->ConvCpltCallbackCh1(hdac);
#else
  HAL_DAC_ConvCpltCallbackCh1(hdac);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

  hdac->State = HAL_DAC_STATE_READY;
}

/**
 * @brief  DMA half transfer complete callback.
 * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
 *                the configuration information for the specified DMA module.
 * @retval None
 */
void DAC_DMAHalfConvCpltCh1(DMA_HandleTypeDef *hdma) {
  DAC_HandleTypeDef *hdac =
      (DAC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;
  /* Conversion complete callback */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  hdac->ConvHalfCpltCallbackCh1(hdac);
#else
  HAL_DAC_ConvHalfCpltCallbackCh1(hdac);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */
}

/**
 * @brief  DMA error callback
 * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
 *                the configuration information for the specified DMA module.
 * @retval None
 */
void DAC_DMAErrorCh1(DMA_HandleTypeDef *hdma) {
  DAC_HandleTypeDef *hdac =
      (DAC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* Set DAC error code to DMA error */
  hdac->ErrorCode |= HAL_DAC_ERROR_DMA;

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  hdac->ErrorCallbackCh1(hdac);
#else
  HAL_DAC_ErrorCallbackCh1(hdac);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

  hdac->State = HAL_DAC_STATE_READY;
}

/**
 * @}
 */

/**
 * @}
 */

#endif /* DAC */

#endif /* HAL_DAC_MODULE_ENABLED */
/**
 * @}
 */
