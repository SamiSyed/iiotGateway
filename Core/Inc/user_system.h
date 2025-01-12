#ifndef SYSTEM_H
#define SYSTEM_H

#include "stdbool.h"
#include "stdint.h"
#include "stm32wlxx_hal.h"

#define NUMBER_OF_SENSORS 5U
#define FILTER_BUFFER_SIZE 10U
#define UART_RX_BUFFER_SIZE 60U
#define LORA_LISTENING_DURATION 3000U /* UNIT ms */
#define MAX_APP_BUFFER_SIZE 2U

/* MQTT_DATA_SEND_INTERVAL has to be smaller than MQTT_DATA_SEND_MAIN_INTERVAL */
#define MQTT_DATA_SEND_INTERVAL 1000U /* UNIT ms */
#define MQTT_DATA_SEND_MAIN_INTERVAL 20000U /* UNIT ms */

#define IS_GATEWAY 1

#define usb_uart &huart1
#define gsm_uart &huart2

#define AT_OK "OK"
#define AT_SAPBR "SAPBR"
#define AT_SMCONF "SMCONF"
#define AT_SMCONN "SMCONN"
#define ADD_AT true
#define NO_AT false

#define MQTT_TOPIC_SIZE 50U
#define MQTT_MESSAGE_SIZE 50U
#define MQTT_SEND_MESSAGE_SIZE (MQTT_TOPIC_SIZE + MQTT_MESSAGE_SIZE)

// #define log_uart &huart3

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

/* This number of IDs should be same as the NUMBER_OF_SENSORS */
typedef enum {
  sensorID_0,
  sensorID_1,
  sensorID_2,
  sensorID_3,
  sensorID_4,
} SensorId;

typedef enum {
  UART2_WAITING_TX,
  UART2_TX_COMPLETE,
  UART2_WAITING_RX,
  UART2_RX_COMPLETE,
} Uart2Status_e;

void setRawDataReceived(bool status);
bool isRawDataReceived(void);

void setLastCommandOK(bool status);
bool isLastCommandOK(void);

void setSystemError(SystemError error);
SystemError getSystemError(void);

Uart2Status_e getUart2Status(void);
void setUart2Status(Uart2Status_e status);

SystemError initSensorFilter(void);
void runAllFilter(void);
void setNewValueBuffer(uint16_t newValue);
uint16_t getFilteredValueByIndex(uint8_t index);

/* Uart Flag */
void setSendingFlag(bool flag);
bool getSendingFlag(void);
void setReceivingFlag(bool flag);
bool getReceivingFlag(void);

/* GSM */
SystemError gsmInit(void);
void setGMSReadinessFlag(bool);
bool isGSMReady(void);

uint16_t GetTemperatureLevel(void);

/* AT command */
SystemError sendATCommand(char *command, char *param, char *reply, bool addAT);
SystemError sendATCommandRetry(char *command, char *param, char *reply,
                               bool addAT);
void sendCommand(char *command);

/* Custom Delay */
void Delay_CustomTimer(uint32_t delayMs);
uint32_t getTick_CustomTimer(void);
void initDelayCustomTimer(void);
#endif /* SYSTEM_H */