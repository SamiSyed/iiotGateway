/**
 ******************************************************************************
 * File Name          : stm32_timer_if.h
 * Description        : This file provides the ll driver for  the time server
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_TIMER_IF_H__
#define STM32_TIMER_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_timer.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*!
 * @brief Initialize the PPP timer
 * @note The timer is based on the PPP
 */
UTIL_TIMER_Status_t PPP_Init(void);

/*!
 * @brief Un-initialize the PPP timer
 * @note The timer is based on the PPP
 */
UTIL_TIMER_Status_t PPP_DeInit(void);

/*!
 * @brief Start the timer
 * @note The timer is set at Reference + timeout
 * @param timeout Duration of the Timer in ticks
 */
UTIL_TIMER_Status_t PPP_StartTimer(uint32_t timeout);

/*!
 * @brief Stop the timer
 * @param none
 * @retval none
 */
UTIL_TIMER_Status_t PPP_StopTimer(void);

/*!
 * @brief Return the minimum timeout the PPP is able to handle
 * @param none
 * @retval minimum value for a timeout
 */
uint32_t PPP_GetMinimumTimeout(void);

/*!
 * @brief Get the PPP timer elapsed time since the last Reference was set
 * @retval PPP Elapsed time in ticks
 */
uint32_t PPP_GetTimerElapsedTime(void);

/*!
 * @brief Get the PPP timer value
 * @retval none
 */
uint32_t PPP_GetTimerValue(void);

/*!
 * @brief Set the PPP timer Reference
 * @retval  Timer Reference Value in  Ticks
 */
uint32_t PPP_SetTimerContext(void);

/*!
 * @brief Get the PPP timer Reference
 * @retval Timer Value in  Ticks
 */
uint32_t PPP_GetTimerContext(void);
/*!
 * @brief PPP IRQ Handler on the PPP timer
 * @param none
 * @retval none
 */
void PPP_IrqHandler(void);

/*!
 * @brief converts time in ms to time in ticks
 * @param [IN] time in milliseconds
 * @retval returns time in ticks
 */
uint32_t PPP_ms2Tick(uint32_t timeMicroSec);

/*!
 * @brief converts time in ticks to time in ms
 * @param [IN] time in ticks
 * @retval returns time in milliseconds
 */
uint32_t PPP_Tick2ms(uint32_t tick);

#ifdef __cplusplus
}
#endif

#endif /* STM32_TIMER_IF_H__ */
