#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NUMBER_OF_SENSORS 1U
#define MQTT_TOPIC_SIZE 50U
#define MQTT_MESSAGE_SIZE 300U
#define MQTT_SEND_MESSAGE_SIZE (MQTT_TOPIC_SIZE + MQTT_MESSAGE_SIZE)
#define ADD_AT true
#define NO_AT false

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

void setSystemError(SystemError error);
SystemError getSystemError(void);

/* Inline stubs for unused functions */
static inline uint16_t getSoilSensorTemperature(void){return 0;}
static inline uint16_t getSoilSensorMoisture(void){return 0;}
static inline uint16_t getSoilSensorConductivity(void){return 0;}
static inline uint16_t getSoilSensorPH(void){return 0;}
static inline uint16_t getSoilSensorNitrogen(void){return 0;}
static inline uint16_t getSoilSensorPhosphorus(void){return 0;}
static inline uint16_t getSoilSensorPotassium(void){return 0;}
static inline SystemError sendATCommand(char *command, char *param, char *reply, bool addAT){return NO_ERROR;}

#endif /* SYSTEM_H */
