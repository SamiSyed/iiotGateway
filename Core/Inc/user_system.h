#ifndef __USER_SYSTEM_H
#define __USER_SYSTEM_H

#include "main.h"
#include "usart.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Defines */
#define MQTT_TOPIC_SIZE 50U
#define MQTT_MESSAGE_SIZE 300U
#define MQTT_SEND_MESSAGE_SIZE (MQTT_TOPIC_SIZE + MQTT_MESSAGE_SIZE)
#define WIFI_SEND_MESSAGE_SIZE 100U

#define LORA_MESSAGE_DELIMITER ","
#define LORA_RX_BUFFER_SIZE 255
#define LORA_TX_BUFFER_SIZE 255

#define AT_OK "OK"
#define AT_SAPBR "SAPBR"
#define AT_SMCONF "SMCONF"
#define AT_SMCONN "SMCONN"
#define ADD_AT true
#define NO_AT false

#define NUMBER_OF_SENSORS 1U
#define MQTT_DATA_SEND_INTERVAL 5U /* UNIT second */
#define MQTT_DATA_SEND_MAIN_INTERVAL 60U /* UNIT second */
#define MAX_TIME_CANNOT_SEND_MQTT 120U /* UNIT second */
#define MAX_TIME_LORA_INCOMING_MISSING 120U /* UNIT second */

/* Type definitions */
typedef enum {
  NO_ERROR,
  ERROR_MESSAGE_QUEUE,
  ERROR_FILTER_BUFFER_SIZE,
  ERROR_INDEX_OUT_OF_RANGE,
  ERROR_UART_TRANSMIT,
  ERROR_REPLY,
  ERROR_NO_AT_REPLY,
  ERROR_LAST_COMMAND_FAILED,
  ERROR_SEND_MQTT_FAILED,
} SystemError;

typedef enum {
  sensorID_0 = 100,
  sensorID_1,
  sensorID_2,
  sensorID_3,
  sensorID_4,
} SensorId;

/* UART handles */
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef hlpuart1;

/* Function declarations */
void gsmInit(void);
bool sendATCommand(const char *command, const char *param, bool addAT);
bool sendATCommandRetry(const char *command, const char *param, bool addAT);
bool sendDirectAT(const char *command);
bool sendRawAT(const char *command);

/* External declarations */
extern void initUart(void);

/* Additional function declarations */
void setRawDataReceived(bool status);
bool isRawDataReceived(void);
void setLastCommandOK(bool status);
bool isLastCommandOK(void);
void setSendingFlag(bool flag);
bool getSendingFlag(void);
void setReceivingFlag(bool flag);
bool getReceivingFlag(void);
void setGMSReadinessFlag(bool status);
bool isGSMReady(void);
void initDelayCustomTimer(void);
uint32_t getTick_CustomTimer(void);
uint32_t getTick_CustomTimer_Sec(void);
SystemError initSensorFilter(void);
void runAllFilter(void);
uint8_t getSensorType(void);

#endif /* __USER_SYSTEM_H */