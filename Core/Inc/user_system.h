#ifndef SYSTEM_H
#define SYSTEM_H

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32wlxx_hal.h"
#include "string.h"

#define FILTER_BUFFER_SIZE 10U
#define UART_RX_BUFFER_SIZE 60U
#define LORA_LISTENING_DURATION 3000U /* UNIT ms */
#define NUMBER_OF_SENSORS 1U
/* MQTT_DATA_SEND_INTERVAL has to be smaller than MQTT_DATA_SEND_MAIN_INTERVAL
 */
#define MQTT_DATA_SEND_INTERVAL 5U /* UNIT second */
/* MQTT_DATA_SEND_MAIN_INTERVAL cannot be less than (NUMBER_OF_SENSORS *
 * MQTT_DATA_SEND_INTERVAL) */
#define MQTT_DATA_SEND_MAIN_INTERVAL 60U /* UNIT second */

#define MAX_TIME_CANNOT_SEND_MQTT 120U      /* UNIT second */
#define MAX_TIME_LORA_INCOMING_MISSING 120U /* UNIT second */

#define IOT_GATEWAY_KEY "0123456789" /*Gateway unique ID*/
#define IOT_GATEWAY_KEY_SIZE 10U
#define LORA_MESSAGE_DELIMITER "#"
#define LORA_MESSAGE_COMMA ","
#define LORA_MESSAGE_DELIMITER_SIZE 1
#define SENSOR_ID_DIGIT_SIZE 3U
#define SENSOR_TYPE_DIGIT_SIZE 3U
#define LORA_END_NODE_VALUE_SIZE 28U

/*Type of sensor Supported */
#define SOIL_SENSOR 101U
#define EC_SENSOR 102U

/* Must sync with END node. Other wise LoRa communication will hang*/
#define LORA_TX_BUFFER_SIZE                                                    \
  (IOT_GATEWAY_KEY_SIZE + LORA_MESSAGE_DELIMITER_SIZE +                        \
   SENSOR_ID_DIGIT_SIZE + LORA_MESSAGE_DELIMITER_SIZE + SENSOR_TYPE_DIGIT_SIZE)
#define LORA_RX_BUFFER_SIZE                                                    \
  (LORA_TX_BUFFER_SIZE + LORA_MESSAGE_DELIMITER_SIZE + LORA_END_NODE_VALUE_SIZE)

#define usb_uart &huart1
#define gsm_uart &huart2
#define wifi_uart &hlpuart1

#define AT_OK "OK"
#define AT_SAPBR "SAPBR"
#define AT_SMCONF "SMCONF"
#define AT_SMCONN "SMCONN"
#define ADD_AT true
#define NO_AT false

#define MQTT_TOPIC_SIZE 50U
#define MQTT_MESSAGE_SIZE 300U
#define MQTT_SEND_MESSAGE_SIZE (MQTT_TOPIC_SIZE + MQTT_MESSAGE_SIZE)
#define WIFI_SEND_MESSAGE_SIZE 100U

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

/* This number of IDs should be same as the NUMBER_OF_SENSORS

   This IDs should be same with END node. END node reply based on comparing this
   IDs
*/
typedef enum {
  sensorID_0 =
      100, /* Should start with 0 to keep the NUMBER_OF_SENSORS accurate */
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

typedef struct {
  int16_t temperature;     // Soil Temperature in °C
  int16_t moisture;        // Soil Moisture in %
  int16_t conductivity;    // Soil Conductivity (EC) in uS/cm
  int16_t pH;              // pH value
  int16_t nitrogen;        // Nitrogen content (mg/kg or similar)
  int16_t phosphorus;      // Phosphorus content
  int16_t potassium;       // Potassium content
} __attribute__((__packed__)) SoilSensorData_t;

void setRawDataReceived(bool status);
bool isRawDataReceived(void);

void setLastCommandOK(bool status);
bool isLastCommandOK(void);

uint8_t getSensorType(void);

void setSystemError(SystemError error);
SystemError getSystemError(void);

Uart2Status_e getUart2Status(void);
void setUart2Status(Uart2Status_e status);

SystemError initSensorFilter(void);
void runAllFilter(void);
void setNewValueBuffer(uint16_t newValue, uint8_t sID);
uint16_t getFilteredValueByIndex(uint8_t index);

/* Uart Flag */
void setSendingFlag(bool flag);
bool getSendingFlag(void);
void setReceivingFlag(bool flag);
bool getReceivingFlag(void);

/* WiFi Uart Flag */
void setDataArrived(bool status);
bool isWaitingForRx(void);
void sendDirectAT(char *command, char *param);

/* GSM */
SystemError gsmInit(void);
void setGMSReadinessFlag(bool);
bool isGSMReady(void);

uint16_t GetTemperatureLevel(void);

/* AT command */
SystemError sendATCommand(char *command, char *param, char *reply, bool addAT);
SystemError sendATCommandRetry(char *command, char *param, char *reply,
                               bool addAT);

/* Custom Delay */
void Delay_CustomTimer(uint32_t delayMs);
uint32_t getTick_CustomTimer(void);
uint32_t getTick_CustomTimer_Sec(void);
void initDelayCustomTimer(void);
char *prepareLoraMessage(uint8_t sID, uint8_t sType);
void sendRawAT(char *command, char *param);

SoilSensorData_t* getSoilSensorData(void);

/* Soil Sensor Data Getters */
uint16_t getSoilSensorTemperature(void);
uint16_t getSoilSensorMoisture(void);
uint16_t getSoilSensorConductivity(void);
uint16_t getSoilSensorPH(void);
uint16_t getSoilSensorNitrogen(void);
uint16_t getSoilSensorPhosphorus(void);
uint16_t getSoilSensorPotassium(void);

#endif /* SYSTEM_H */