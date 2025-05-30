/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    sys_app.h
 * @author  MCD Application Team
 * @brief   Function prototypes for sys_app.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_APP_H__
#define __SYS_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32_adv_trace.h"
#include "sys_conf.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported defines ----------------------------------------------------------*/
/* USER CODE BEGIN ED */

/* USER CODE END ED */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
#define APP_PPRINTF(...)                                                       \
  do {                                                                         \
  } while (UTIL_ADV_TRACE_OK !=                                                \
           UTIL_ADV_TRACE_COND_FSend(VLEVEL_ALWAYS, T_REG_OFF, TS_OFF,         \
                                     __VA_ARGS__)) /* Polling Mode */
#define APP_TPRINTF(...)                                                       \
  do {                                                                         \
    {                                                                          \
      UTIL_ADV_TRACE_COND_FSend(VLEVEL_ALWAYS, T_REG_OFF, TS_ON, __VA_ARGS__); \
    }                                                                          \
  } while (0); /* with timestamp */
#define APP_PRINTF(...)                                                        \
  do {                                                                         \
    {                                                                          \
      UTIL_ADV_TRACE_COND_FSend(VLEVEL_ALWAYS, T_REG_OFF, TS_OFF,              \
                                __VA_ARGS__);                                  \
    }                                                                          \
  } while (0);

#if defined(APP_LOG_ENABLED) && (APP_LOG_ENABLED == 1)
#define APP_LOG(TS, VL, ...)                                                   \
  do {                                                                         \
    { UTIL_ADV_TRACE_COND_FSend(VL, T_REG_OFF, TS, __VA_ARGS__); }             \
  } while (0);
#elif defined(APP_LOG_ENABLED) && (APP_LOG_ENABLED == 0) /* APP_LOG disabled   \
                                                          */
#define APP_LOG(TS, VL, ...)
#else
#error "APP_LOG_ENABLED not defined or out of range <0,1>"
#endif /* APP_LOG_ENABLED */

/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/**
 * @brief initialize the system (dbg pins, trace, mbmux, sys timer, LPM, ...)
 */
void SystemApp_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __SYS_APP_H__ */
