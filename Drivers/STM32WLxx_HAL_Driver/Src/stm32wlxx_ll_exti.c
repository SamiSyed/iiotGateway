/**
 ******************************************************************************
 * @file    stm32wlxx_ll_exti.c
 * @author  MCD Application Team
 * @brief   EXTI LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_ll_exti.h"
#ifdef USE_FULL_ASSERT
#include "stm32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

/** @addtogroup STM32WLxx_LL_Driver
 * @{
 */

#if defined(EXTI)

/** @defgroup EXTI_LL EXTI
 * @{
 */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup EXTI_LL_Private_Macros
 * @{
 */

#define IS_LL_EXTI_LINE_0_31(__VALUE__)                                        \
  (((__VALUE__) & ~LL_EXTI_LINE_ALL_0_31) == 0x00000000U)
#define IS_LL_EXTI_LINE_32_63(__VALUE__)                                       \
  (((__VALUE__) & ~LL_EXTI_LINE_ALL_32_63) == 0x00000000U)

#define IS_LL_EXTI_MODE(__VALUE__)                                             \
  (((__VALUE__) == LL_EXTI_MODE_IT) || ((__VALUE__) == LL_EXTI_MODE_EVENT) ||  \
   ((__VALUE__) == LL_EXTI_MODE_IT_EVENT))

#define IS_LL_EXTI_TRIGGER(__VALUE__)                                          \
  (((__VALUE__) == LL_EXTI_TRIGGER_NONE) ||                                    \
   ((__VALUE__) == LL_EXTI_TRIGGER_RISING) ||                                  \
   ((__VALUE__) == LL_EXTI_TRIGGER_FALLING) ||                                 \
   ((__VALUE__) == LL_EXTI_TRIGGER_RISING_FALLING))

/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup EXTI_LL_Exported_Functions
 * @{
 */

/** @addtogroup EXTI_LL_EF_Init
 * @{
 */

/**
 * @brief  De-initialize the EXTI registers to their default reset values.
 * @retval An ErrorStatus enumeration value:
 *          - SUCCESS: EXTI registers are de-initialized
 *          - ERROR: not applicable
 */
ErrorStatus LL_EXTI_DeInit(void) {
  /* Rising Trigger selection register set to default reset values */
  LL_EXTI_WriteReg(RTSR1, 0x00000000U);

  /* Falling Trigger selection register set to default reset values */
  LL_EXTI_WriteReg(FTSR1, 0x00000000U);

  /* Software interrupt event register set to default reset values */
  LL_EXTI_WriteReg(SWIER1, 0x00000000U);

  /* Pending register set to default reset values */
  LL_EXTI_WriteReg(PR1, 0xFFFFFFFFU);

  /* Rising Trigger selection register 2 set to default reset values */
  LL_EXTI_WriteReg(RTSR2, 0x00000000U);

  /* Falling Trigger selection register 2 set to default reset values */
  LL_EXTI_WriteReg(FTSR2, 0x00000000U);

  /* Software interrupt event register 2 set to default reset values */
  LL_EXTI_WriteReg(SWIER2, 0x00000000U);

  /* Pending register 2 set to default reset values */
  LL_EXTI_WriteReg(PR2, 0xFFFFFFFFU);

  /* Interrupt mask register set to default reset values */
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
  LL_EXTI_WriteReg(C2IMR1, 0x00000000U);
#else
  LL_EXTI_WriteReg(IMR1, 0x00000000U);
#endif /* DUAL_CORE && CORE_CM0PLUS */

  /* Event mask register set to default reset values */
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
  LL_EXTI_WriteReg(C2EMR1, 0x00000000U);
#else
  LL_EXTI_WriteReg(EMR1, 0x00000000U);
#endif /* DUAL_CORE && CORE_CM0PLUS */

  /* Interrupt mask register 2 set to default reset values */
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
  LL_EXTI_WriteReg(C2IMR2, 0x00000000U);
#else
  LL_EXTI_WriteReg(IMR2, 0x00000000U);
#endif /* DUAL_CORE && CORE_CM0PLUS */

  /* Event mask register 2 set to default reset values */
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
  LL_EXTI_WriteReg(C2EMR2, 0x00000000U);
#else
  LL_EXTI_WriteReg(EMR2, 0x00000000U);
#endif /* DUAL_CORE && CORE_CM0PLUS */

  return SUCCESS;
}

/**
 * @brief  Initialize the EXTI registers according to the specified parameters
 * in EXTI_InitStruct.
 * @param  EXTI_InitStruct pointer to a @ref LL_EXTI_InitTypeDef structure.
 * @retval An ErrorStatus enumeration value:
 *          - SUCCESS: EXTI registers are initialized
 *          - ERROR: not applicable
 */
ErrorStatus LL_EXTI_Init(LL_EXTI_InitTypeDef *EXTI_InitStruct) {
  ErrorStatus status = SUCCESS;
  /* Check the parameters */
  assert_param(IS_LL_EXTI_LINE_0_31(EXTI_InitStruct->Line_0_31));
  assert_param(IS_LL_EXTI_LINE_32_63(EXTI_InitStruct->Line_32_63));
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->LineCommand));
  assert_param(IS_LL_EXTI_MODE(EXTI_InitStruct->Mode));

  /* ENABLE LineCommand */
  if (EXTI_InitStruct->LineCommand != DISABLE) {
    assert_param(IS_LL_EXTI_TRIGGER(EXTI_InitStruct->Trigger));

    /* Configure EXTI Lines in range from 0 to 31 */
    if (EXTI_InitStruct->Line_0_31 != LL_EXTI_LINE_NONE) {
      switch (EXTI_InitStruct->Mode) {
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
      case LL_EXTI_MODE_IT:
        /* First Disable Event on provided Lines */
        LL_C2_EXTI_DisableEvent_0_31(EXTI_InitStruct->Line_0_31);
        /* Then Enable IT on provided Lines */
        LL_C2_EXTI_EnableIT_0_31(EXTI_InitStruct->Line_0_31);
        break;
      case LL_EXTI_MODE_EVENT:
        /* First Disable IT on provided Lines */
        LL_C2_EXTI_DisableIT_0_31(EXTI_InitStruct->Line_0_31);
        /* Then Enable Event on provided Lines */
        LL_C2_EXTI_EnableEvent_0_31(EXTI_InitStruct->Line_0_31);
        break;
      case LL_EXTI_MODE_IT_EVENT:
        /* Directly Enable IT & Event on provided Lines */
        LL_C2_EXTI_EnableIT_0_31(EXTI_InitStruct->Line_0_31);
        LL_C2_EXTI_EnableEvent_0_31(EXTI_InitStruct->Line_0_31);
        break;
      default:
        status = ERROR;
        break;
#else
      case LL_EXTI_MODE_IT:
        /* First Disable Event on provided Lines */
        LL_EXTI_DisableEvent_0_31(EXTI_InitStruct->Line_0_31);
        /* Then Enable IT on provided Lines */
        LL_EXTI_EnableIT_0_31(EXTI_InitStruct->Line_0_31);
        break;
      case LL_EXTI_MODE_EVENT:
        /* First Disable IT on provided Lines */
        LL_EXTI_DisableIT_0_31(EXTI_InitStruct->Line_0_31);
        /* Then Enable Event on provided Lines */
        LL_EXTI_EnableEvent_0_31(EXTI_InitStruct->Line_0_31);
        break;
      case LL_EXTI_MODE_IT_EVENT:
        /* Directly Enable IT & Event on provided Lines */
        LL_EXTI_EnableIT_0_31(EXTI_InitStruct->Line_0_31);
        LL_EXTI_EnableEvent_0_31(EXTI_InitStruct->Line_0_31);
        break;
      default:
        status = ERROR;
        break;
#endif /* DUAL_CORE && CORE_CM0PLUS */
      }
      if (EXTI_InitStruct->Trigger != LL_EXTI_TRIGGER_NONE) {
        switch (EXTI_InitStruct->Trigger) {
        case LL_EXTI_TRIGGER_RISING:
          /* First Disable Falling Trigger on provided Lines */
          LL_EXTI_DisableFallingTrig_0_31(EXTI_InitStruct->Line_0_31);
          /* Then Enable Rising Trigger on provided Lines */
          LL_EXTI_EnableRisingTrig_0_31(EXTI_InitStruct->Line_0_31);
          break;
        case LL_EXTI_TRIGGER_FALLING:
          /* First Disable Rising Trigger on provided Lines */
          LL_EXTI_DisableRisingTrig_0_31(EXTI_InitStruct->Line_0_31);
          /* Then Enable Falling Trigger on provided Lines */
          LL_EXTI_EnableFallingTrig_0_31(EXTI_InitStruct->Line_0_31);
          break;
        case LL_EXTI_TRIGGER_RISING_FALLING:
          LL_EXTI_EnableRisingTrig_0_31(EXTI_InitStruct->Line_0_31);
          LL_EXTI_EnableFallingTrig_0_31(EXTI_InitStruct->Line_0_31);
          break;
        default:
          status = ERROR;
          break;
        }
      }
    }
    /* Configure EXTI Lines in range from 32 to 63 */
    if (EXTI_InitStruct->Line_32_63 != LL_EXTI_LINE_NONE) {
      switch (EXTI_InitStruct->Mode) {
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
      case LL_EXTI_MODE_IT:
        /* First Disable Event on provided Lines */
        LL_C2_EXTI_DisableEvent_32_63(EXTI_InitStruct->Line_32_63);
        /* Then Enable IT on provided Lines */
        LL_C2_EXTI_EnableIT_32_63(EXTI_InitStruct->Line_32_63);
        break;
      case LL_EXTI_MODE_EVENT:
        /* First Disable IT on provided Lines */
        LL_C2_EXTI_DisableIT_32_63(EXTI_InitStruct->Line_32_63);
        /* Then Enable Event on provided Lines */
        LL_C2_EXTI_EnableEvent_32_63(EXTI_InitStruct->Line_32_63);
        break;
      case LL_EXTI_MODE_IT_EVENT:
        /* Directly Enable IT & Event on provided Lines */
        LL_C2_EXTI_EnableIT_32_63(EXTI_InitStruct->Line_32_63);
        LL_C2_EXTI_EnableEvent_32_63(EXTI_InitStruct->Line_32_63);
        break;
      default:
        status = ERROR;
        break;
#else
      case LL_EXTI_MODE_IT:
        /* First Disable Event on provided Lines */
        LL_EXTI_DisableEvent_32_63(EXTI_InitStruct->Line_32_63);
        /* Then Enable IT on provided Lines */
        LL_EXTI_EnableIT_32_63(EXTI_InitStruct->Line_32_63);
        break;
      case LL_EXTI_MODE_EVENT:
        /* First Disable IT on provided Lines */
        LL_EXTI_DisableIT_32_63(EXTI_InitStruct->Line_32_63);
        /* Then Enable Event on provided Lines */
        LL_EXTI_EnableEvent_32_63(EXTI_InitStruct->Line_32_63);
        break;
      case LL_EXTI_MODE_IT_EVENT:
        /* Directly Enable IT & Event on provided Lines */
        LL_EXTI_EnableIT_32_63(EXTI_InitStruct->Line_32_63);
        LL_EXTI_EnableEvent_32_63(EXTI_InitStruct->Line_32_63);
        break;
      default:
        status = ERROR;
        break;
#endif /* DUAL_CORE && CORE_CM0PLUS */
      }
      if (EXTI_InitStruct->Trigger != LL_EXTI_TRIGGER_NONE) {
        switch (EXTI_InitStruct->Trigger) {
        case LL_EXTI_TRIGGER_RISING:
          /* First Disable Falling Trigger on provided Lines */
          LL_EXTI_DisableFallingTrig_32_63(EXTI_InitStruct->Line_32_63);
          /* Then Enable IT on provided Lines */
          LL_EXTI_EnableRisingTrig_32_63(EXTI_InitStruct->Line_32_63);
          break;
        case LL_EXTI_TRIGGER_FALLING:
          /* First Disable Rising Trigger on provided Lines */
          LL_EXTI_DisableRisingTrig_32_63(EXTI_InitStruct->Line_32_63);
          /* Then Enable Falling Trigger on provided Lines */
          LL_EXTI_EnableFallingTrig_32_63(EXTI_InitStruct->Line_32_63);
          break;
        case LL_EXTI_TRIGGER_RISING_FALLING:
          LL_EXTI_EnableRisingTrig_32_63(EXTI_InitStruct->Line_32_63);
          LL_EXTI_EnableFallingTrig_32_63(EXTI_InitStruct->Line_32_63);
          break;
        default:
          status = ERROR;
          break;
        }
      }
    }
  }
  /* DISABLE LineCommand */
  else {
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
    /* De-configure EXTI Lines in range from 0 to 31 */
    LL_C2_EXTI_DisableIT_0_31(EXTI_InitStruct->Line_0_31);
    LL_C2_EXTI_DisableEvent_0_31(EXTI_InitStruct->Line_0_31);
    /* De-configure EXTI Lines in range from 32 to 63 */
    LL_C2_EXTI_DisableIT_32_63(EXTI_InitStruct->Line_32_63);
    LL_C2_EXTI_DisableEvent_32_63(EXTI_InitStruct->Line_32_63);
#else
    /* De-configure EXTI Lines in range from 0 to 31 */
    LL_EXTI_DisableIT_0_31(EXTI_InitStruct->Line_0_31);
    LL_EXTI_DisableEvent_0_31(EXTI_InitStruct->Line_0_31);
    /* De-configure EXTI Lines in range from 32 to 63 */
    LL_EXTI_DisableIT_32_63(EXTI_InitStruct->Line_32_63);
    LL_EXTI_DisableEvent_32_63(EXTI_InitStruct->Line_32_63);
#endif /* DUAL_CORE && CORE_CM0PLUS */
  }
  return status;
}

/**
 * @brief  Set each @ref LL_EXTI_InitTypeDef field to default value.
 * @param  EXTI_InitStruct Pointer to a @ref LL_EXTI_InitTypeDef structure.
 * @retval None
 */
void LL_EXTI_StructInit(LL_EXTI_InitTypeDef *EXTI_InitStruct) {
  EXTI_InitStruct->Line_0_31 = LL_EXTI_LINE_NONE;
  EXTI_InitStruct->Line_32_63 = LL_EXTI_LINE_NONE;
  EXTI_InitStruct->LineCommand = DISABLE;
  EXTI_InitStruct->Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct->Trigger = LL_EXTI_TRIGGER_FALLING;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif /* defined (EXTI) */

/**
 * @}
 */

#endif /* USE_FULL_LL_DRIVER */
