/**
  ******************************************************************************
  * @file    stm32wlxx_hal_timebase_tim_template.c
  * @author  MCD Application Team
  * @brief   HAL time base based on the hardware TIM Template.
  *
  *          This file overrides the native HAL time base functions (defined as
  *          weak) the TIM time base:
  *           + Initializes the TIM peripheral generate a Period elapsed Event
  *             each 1ms when uwTickFreq is set to default value, else 10 ms or
  *             100 ms, depending of above global variable value.
  *           + HAL_IncTick is called inside HAL_TIM_PeriodElapsedCallback
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
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    This file must be copied to the application folder and modified as follows:
    (#) Rename it to 'stm32wlxx_hal_timebase_tim.c'
    (#) Add this file and the TIM HAL drivers to your project and uncomment
       HAL_TIM_MODULE_ENABLED define in stm32wlxx_hal_conf.h

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/** @addtogroup STM32WLxx_HAL_Driver
 * @{
 */

/** @addtogroup HAL_TimeBase_TIM
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef TimHandle = {.Init = {0}};

/* Private function prototypes -----------------------------------------------*/
void TIM2_IRQHandler(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  This function configures the TIM2 as a time base source.
 *         The time source is configured to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called  automatically at the beginning of program
 *         after reset by HAL_Init() or at any time when clock is configured,
 *         by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
  RCC_ClkInitTypeDef clkconfig;
  uint32_t uwTimclock;
  uint32_t uwAPB1Prescaler = 0U;
  uint32_t uwPrescalerValue = 0U;
  uint32_t pFLatency;
  HAL_StatusTypeDef status = HAL_OK;

  /* Check uwTickFreq for MisraC 2012 (even if uwTickFreq is a enum type that
   * don't take the value zero)*/
  if ((uint32_t)uwTickFreq != 0U) {
    /* Enable TIM2 clock */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    /* Compute TIM2 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1) {
      uwTimclock = HAL_RCC_GetPCLK1Freq();
    } else {
      uwTimclock = 2 * HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM2 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM2 */
    TimHandle.Instance = TIM2;

    /* Initialize TIMx peripheral as follow:
    + Period = [(TIM2CLK/uwTickFreq) - 1]. to have a (1/uwTickFreq) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    TimHandle.Init.Period = (1000000U / (1000U / (uint32_t)uwTickFreq)) - 1U;
    TimHandle.Init.Prescaler = uwPrescalerValue;
    TimHandle.Init.ClockDivision = 0U;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0U;
    if (HAL_TIM_Base_Init(&TimHandle) == HAL_OK) {
      /* Start the TIM time Base generation in interrupt mode */
      if (HAL_TIM_Base_Start_IT(&TimHandle) == HAL_OK) {
        /* Enable the TIM2 global Interrupt */
        HAL_NVIC_EnableIRQ(TIM2_IRQn);

        /* Configure the SysTick IRQ priority */
        if (TickPriority < (1UL << __NVIC_PRIO_BITS)) {
          /*Configure the TIM2 IRQ priority */
          HAL_NVIC_SetPriority(TIM2_IRQn, TickPriority, 0U);
          uwTickPrio = TickPriority;
        } else {
          status = HAL_ERROR;
        }
      } else {
        status = HAL_ERROR;
      }
    } else {
      status = HAL_ERROR;
    }
  } else {
    status = HAL_ERROR;
  }

  /* Return function status */
  return status;
}

/**
 * @brief  Suspend Tick increment.
 * @note   Disable the tick increment by disabling TIM2 update interrupt.
 * @retval None
 */
void HAL_SuspendTick(void) {
  /* Disable TIM2 update Interrupt */
  __HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling TIM2 update interrupt.
 * @retval None
 */
void HAL_ResumeTick(void) {
  /* Enable TIM2 Update interrupt */
  __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) { HAL_IncTick(); }

/**
 * @brief  This function handles TIM interrupt request.
 * @retval None
 */
void TIM2_IRQHandler(void) { HAL_TIM_IRQHandler(&TimHandle); }

/**
 * @}
 */

/**
 * @}
 */
