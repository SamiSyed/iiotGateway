/**
 ******************************************************************************
 * @file    STM32WLxx_LoRa_E5_mini.h
 * @author  MCD Application Team
 * @brief   Header for STM32WLxx_LoRa_E5_mini.c
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2020(-2021) STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WLXX_LORA_E5_MINI_H
#define STM32WLXX_LORA_E5_MINI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_LoRa_E5_conf.h"
#include "stm32wlxx_LoRa_E5_errno.h"

#if (USE_BSP_COM_FEATURE > 0)
#if (USE_COM_LOG > 0)
#ifndef __GNUC__
#include "stdio.h"
#endif
#endif
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup STM32WLXX_LoRa_E5_mini STM32WLXX-LoRa_E5_mini
 * @{
 */

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL LOW LEVEL
 * @{
 */

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_Exported_Types LOW LEVEL Exported
 * Types
 * @{
 */
typedef enum { LED1 = 0, LED_RED = LED1 } Led_TypeDef;

typedef enum {
  BUTTON_SW1 = 0,
} Button_TypeDef;

typedef enum { BUTTON_MODE_GPIO = 0, BUTTON_MODE_EXTI = 1 } ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE > 0)
typedef enum { COM1 = 0U, COMn } COM_TypeDef;

typedef enum {
  COM_STOPBITS_1 = UART_STOPBITS_1,
  COM_STOPBITS_2 = UART_STOPBITS_2,
} COM_StopBitsTypeDef;

typedef enum {
  COM_PARITY_NONE = UART_PARITY_NONE,
  COM_PARITY_EVEN = UART_PARITY_EVEN,
  COM_PARITY_ODD = UART_PARITY_ODD,
} COM_ParityTypeDef;

typedef enum {
  COM_HWCONTROL_NONE = UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS = UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS = UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS = UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

typedef enum {
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
} COM_WordLengthTypeDef;

typedef struct {
  uint32_t BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef StopBits;
  COM_ParityTypeDef Parity;
  COM_HwFlowCtlTypeDef HwFlowCtl;
} COM_InitTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct {
  pUART_CallbackTypeDef pMspInitCb;
  pUART_CallbackTypeDef pMspDeInitCb;
} BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */
#endif /* (USE_BSP_COM_FEATURE > 0) */

typedef enum {
  ABSENT = 0,
  PRESENT = 1,
} Presence_TypeDef;
/**
 * @}
 */

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_Exported_Constants LOW LEVEL
 * Exported Constants
 * @{
 */

/**
 * @brief STM32WLXX LoRa_E5_mini BSP Driver version number
 */
#define __STM32WLXX_LoRa_E5_mini_BSP_VERSION_MAIN                              \
  (0x01U) /*!< [31:24] main version */
#define __STM32WLXX_LoRa_E5_mini_BSP_VERSION_SUB1                              \
  (0x01U) /*!< [23:16] sub1 version */
#define __STM32WLXX_LoRa_E5_mini_BSP_VERSION_SUB2                              \
  (0x00U) /*!< [15:8]  sub2 version */
#define __STM32WLXX_LoRa_E5_mini_BSP_VERSION_RC                                \
  (0x00U) /*!< [7:0]  release candidate */
#define __STM32WLXX_LoRa_E5_mini_BSP_VERSION                                   \
  ((__STM32WLXX_LoRa_E5_mini_BSP_VERSION_MAIN << 24) |                         \
   (__STM32WLXX_LoRa_E5_mini_BSP_VERSION_SUB1 << 16) |                         \
   (__STM32WLXX_LoRa_E5_mini_BSP_VERSION_SUB2 << 8) |                          \
   (__STM32WLXX_LoRa_E5_mini_BSP_VERSION_RC))

/**
 * @brief Define for STM32WLXX_LoRa_E5_mini board
 */
#if !defined(USE_STM32WLXX_LoRa_E5_mini)
#define USE_STM32WLXX_LoRa_E5_mini
#endif

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_LED LOW LEVEL LED Constants
 * @{
 */
#define LEDn 1

#define LED1_PIN GPIO_PIN_5
#define LED1_GPIO_PORT GPIOB
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)                                        \
  __HAL_RCC_GPIOB_CLK_ENABLE() /* All Led on same port */
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)                                       \
  __HAL_RCC_GPIOB_CLK_DISABLE() /* All Led on same port */
/**
 * @}
 */

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_BUTTON LOW LEVEL BUTTON Constants
 * @{
 */
#define BUTTONn 1

/**
 * @brief Key push-buttons
 */
#define BUTTON_SW1_PIN GPIO_PIN_13
#define BUTTON_SW1_GPIO_PORT GPIOB
#define BUTTON_SW1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_SW1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_SW1_EXTI_LINE EXTI_LINE_13
#define BUTTON_SW1_EXTI_IRQn EXTI15_10_IRQn
#define H_EXTI_GPIO_PIN_13 hpb_exti[BUTTON_SW1]

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)                                     \
  do {                                                                         \
    BUTTON_SW1_GPIO_CLK_ENABLE();                                              \
  } while (0)

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)                                    \
  do {                                                                         \
    BUTTON_SW1_GPIO_CLK_DISABLE();                                             \
  } while (0)

/**
 * @}
 */

#if (USE_BSP_COM_FEATURE > 0)
/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_COM LOW LEVEL COM Port Constants
 * @{
 */
#define COM1_UART LPUART1
#define COM1_CLK_ENABLE() __HAL_RCC_LPUART1_CLK_ENABLE()
#define COM1_CLK_DISABLE() __HAL_RCC_LPUART1_CLK_DISABLE()

#define COM1_TX_PIN GPIO_PIN_2
#define COM1_TX_GPIO_PORT GPIOA
#define COM1_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_TX_AF GPIO_AF8_LPUART1

#define COM1_RX_PIN GPIO_PIN_3
#define COM1_RX_GPIO_PORT GPIOA
#define COM1_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_RX_AF GPIO_AF8_LPUART1
#define COM_POLL_TIMEOUT 1000

#define MX_UART_InitTypeDef COM_InitTypeDef
/**
 * @}
 */
#endif /* (USE_BSP_COM_FEATURE > 0)*/

/**
 * @}
 */

/** @addtogroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_Exported_Variables
 * @{
 */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
#endif /* (USE_BSP_COM_FEATURE > 0) */
/**
 * @}
 */

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_Exported_Functions LOW LEVEL
 * Exported Functions
 * @{
 */
uint32_t BSP_GetVersion(void);

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_LED_Functions LOW LEVEL LED
 * Functions
 * @{
 */
int32_t BSP_LED_Init(Led_TypeDef Led);
int32_t BSP_LED_DeInit(Led_TypeDef Led);
int32_t BSP_LED_On(Led_TypeDef Led);
int32_t BSP_LED_Off(Led_TypeDef Led);
int32_t BSP_LED_Toggle(Led_TypeDef Led);
int32_t BSP_LED_GetState(Led_TypeDef Led);
/**
 * @}
 */

/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_BUTTON_Functions  LOW LEVEL
 * BUTTON Functions
 * @{
 */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t BSP_PB_DeInit(Button_TypeDef Button);
int32_t BSP_PB_GetState(Button_TypeDef Button);
void BSP_PB_Callback(Button_TypeDef Button);
void BSP_PB_IRQHandler(Button_TypeDef Button);
/**
 * @}
 */

#if (USE_BSP_COM_FEATURE > 0)
/** @defgroup STM32WLXX_LoRa_E5_mini_LOW_LEVEL_COM_Functions LOW LEVEL COM Port
 * Functions
 * @{
 */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG > 0)
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef *huart,
                                  MX_UART_InitTypeDef *COM_Init);
/**
 * @}
 */
#endif /* (USE_BSP_COM_FEATURE > 0) */

/**
 * @}
 */

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

#endif /* STM32WLXX_LoRa_E5_mini_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
