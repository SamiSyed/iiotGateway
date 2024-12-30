#ifndef SYSTEM_H
#define SYSTEM_H

#include "stdbool.h"
#include "stdint.h"
#include "stm32wlxx_hal.h"

#define NUMBER_OF_SENSORS 5
#define FILTER_BUFFER_SIZE 10U
#define MQTT_TOPIC_SIZE 30U
#define UART_RX_BUFFER_SIZE 60U
#define LORA_LISTENING_DURATION 3000U /* UNIT ms */
#define MAX_APP_BUFFER_SIZE 2
#define IS_GATEWAY 1

// #define log_uart &huart3

typedef enum {
  NO_ERROR,
  ERROR_MESSAGE_QUEUE,
  ERROR_FILTER_BUFFER_SIZE,
  ERROR_INDEX_OUT_OF_RANGE,
} SystemError;

/* This number of IDs should be same as the NUMBER_OF_SENSORS */
typedef enum {
  sensorID_0,
  sensorID_1,
  sensorID_2,
  sensorID_3,
  sensorID_4,
} SensorId;

typedef struct {
  char *topic;
  char *value;
  // timestamp
} MqttMessage_t;

typedef enum {
  UART2_WAITING_TX,
  UART2_TX_COMPLETE,
  UART2_WAITING_RX,
  UART2_RX_COMPLETE,
} Uart2Status_e;

void setRawDataReceived(bool status);
bool isRawDataReceived(void);

void setSystemError(SystemError error);
SystemError getSystemError(void);

Uart2Status_e getUart2Status(void);
void setUart2Status(Uart2Status_e status);

SystemError initSensorFilter(void);
void runAllFilter(void);
void setNewValueBuffer(uint16_t newValue);
uint16_t getFilteredValueByIndex(uint8_t index);
void prepareMqttMessageStruct(MqttMessage_t *mqttMessage);
SystemError sendAllDataToMqttBroker(MqttMessage_t *mqttMessage);
void setMqttTopic(void);
MqttMessage_t *getMqttMessageByIndex(uint8_t index);
bool sendMqttServer(MqttMessage_t mqttMessage);
MqttMessage_t *getMqttMessage(void);
void setSendingFlag(bool flag);
bool getSendingFlag(void);
SystemError gsmInit(void);
void printArray(uint8_t *array, uint8_t dataLength);
uint16_t GetTemperatureLevel(void);
void sendCommand(char *command);
void setGMSReadinessFlag(bool);
bool isGSMReady(void);
SystemError atCommandCheck(void);
#endif /* SYSTEM_H */