#include "system.h"
#include "UartRingbuffer_multi.h"
#include "filter.h"
#include "usart.h"

#define AT_OK "OK"
#define ADD_AT true
#define NO_AT false

static bool rawDataReceived = false;
static SystemError systemError = NO_ERROR;
static bool sendingData = false;
static bool initialized = false;

static uint16_t newValueBuffer[NUMBER_OF_SENSORS];
static uint16_t
    filteredSensorValues[NUMBER_OF_SENSORS]; /* Global Sensor Values*/
static MovingAverageFilter sensorFilters[NUMBER_OF_SENSORS];
static uint16_t filterBuffer[NUMBER_OF_SENSORS][FILTER_BUFFER_SIZE];
static MqttMessage_t mqttMessages[NUMBER_OF_SENSORS];
static Uart2Status_e uart2Status = UART2_RX_COMPLETE;

/* GSM variables */
char dataUart[80];
// static bool gSMReadiness = false;
/* MQTT variables */
char mqttTopic[NUMBER_OF_SENSORS][MQTT_TOPIC_SIZE];

/* Flag */
void setRawDataReceived(bool status) { rawDataReceived = status; }
MqttMessage_t *getMqttMessage(void) { return mqttMessages; }
bool isRawDataReceived(void) { return rawDataReceived; }

/* Error */
void setSystemError(SystemError error) {
  /* Keep first error so it is not over written */
  if (systemError == NO_ERROR) {
    systemError = error;
  }
}

Uart2Status_e getUart2Status(void) { return uart2Status; }

void setUart2Status(Uart2Status_e status) { uart2Status = status; }

SystemError getSystemError(void) { return systemError; }

/* Filter */
SystemError initSensorFilter(void) {
  SystemError error = NO_ERROR;
  /* Create NUMBER_OF_SENSORS amount of filter */
  for (uint8_t sId = 0; sId < (uint8_t)NUMBER_OF_SENSORS; sId++) {
    error = MovingAverage_Init(&sensorFilters[sId], &filterBuffer[sId][0],
                               FILTER_BUFFER_SIZE);
    if (error != NO_ERROR) {
      break;
    }
  }

  return error;
}

void runAllFilter(void) {
  for (uint8_t sId = 0; sId < (uint8_t)NUMBER_OF_SENSORS; sId++) {
    filteredSensorValues[sId] =
        runFilter(&sensorFilters[sId], newValueBuffer[sId]);
  }
}

void setNewValueBuffer(uint16_t newValue) {
  for (uint8_t sId = 0; sId < (uint8_t)NUMBER_OF_SENSORS; sId++) {
    newValueBuffer[sId] = newValue;
  }
}

/* Get value from global sensor array*/
uint16_t getFilteredValueByIndex(uint8_t index) {
  return filteredSensorValues[index];
}

/* Update MQTT struct with latest value to be send */
void prepareMqttMessageStruct(MqttMessage_t *mqttMessages) {
  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
    mqttMessages[i].topic = mqttTopic[i];
    char data[20];
    sprintf(data, "{\\\"value\\\":\\\"%i\\\"}", getFilteredValueByIndex(i));
    mqttMessages[i].value = data;
    printf("mqttMessages[i].value : %s \r\n", mqttMessages[i].value);
  }
}

void setMqttTopic(void) {
  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
    strcpy(mqttTopic[i], "iiot/sensorID_2");
    // printf("%s\r\n", mqttTopic[i]);
  }
}

MqttMessage_t *getMqttMessageByIndex(uint8_t index) {
  if (index > NUMBER_OF_SENSORS) {
    setSystemError(ERROR_INDEX_OUT_OF_RANGE);
  }
  return &mqttMessages[index];
}

bool sendMqttServer(MqttMessage_t mqttMessage) {
  char data[80];
  if (sendAT() == NO_ERROR) {

    sprintf(data, "SMPUB=\"%s\",1,0,\"%s\"", mqttMessage.topic,
            mqttMessage.value);
    sendCommand(data);
    osDelay(1000);
    return true;
  } else {
    return false;
  }
}

SystemError sendAllDataToMqttBroker(MqttMessage_t *mqttMessage) {
  // for (uint8_t i = 0; i < 1; i++) {
  sendMqttServer(mqttMessage[0]);
  // }

  return NO_ERROR;
}
void sendCommand(char *command) {
  printf("at+");
  printf(command);
  printf("\r\n");
  osDelay(100);
}

SystemError gsmInit(void) {
  osDelay(1000);
  sendATCommand("ATE1", AT_OK, NO_AT);
  sendATCommand("ATZ", AT_OK, NO_AT);

  sendATCommand("cfun=1,1", AT_OK, ADD_AT);
  osDelay(15000);
  sendATCommand("cpin?", AT_OK, ADD_AT);
  sendATCommand("csq", AT_OK, ADD_AT);
  sendATCommand("creg?", AT_OK, ADD_AT);
  sendATCommand("cgatt?", AT_OK, ADD_AT);
  /* Kept mute due to reply size more than rx_buffer size */
  // sendATCommand("cgdcont?", AT_OK, ADD_AT);

  /* MQTT */
  sendATCommand("sapbr=3,1,\"Contype\",\"GPRS\"", AT_OK, ADD_AT);
  sendATCommand("sapbr=3,1,\"APN\",\"telia\"", AT_OK, ADD_AT);
  sendATCommand("SAPBR=3,1,\"USER\",\"\"", AT_OK, ADD_AT);
  sendATCommand("SAPBR=3,1,\"PWD\",\"\"", AT_OK, ADD_AT);
  sendATCommand("sapbr=1,1", AT_OK, ADD_AT);

  osDelay(2000);
  sendATCommand("sapbr=2,1", AT_OK, ADD_AT);

  osDelay(2000);
  sendATCommand("smconf=\"URL\",broker.emqx.io:1883", AT_OK, ADD_AT);

  osDelay(500);
  sendATCommand("smconf=\"KEEPTIME\",60", AT_OK, ADD_AT);
  sendATCommand("smconf=\"CLEANSS\",1", AT_OK, ADD_AT);
  sendATCommand("SMCONN", AT_OK, ADD_AT);
  osDelay(2000);

  return NO_ERROR;
}

// void setGMSReadinessFlag(bool state){
//   gSMReadiness = state;
// }

// bool isGSMReady(void){
//   return gSMReadiness;
// }

SystemError sendAT(void) {
  sprintf(dataUart, "\r\nAT\r\n");
  if (HAL_OK == HAL_UART_Transmit(gsm_uart, (uint8_t *)dataUart,
                                  strlen(dataUart), 1000)) {
    setSendingFlag(true);

    for (size_t i = 0; i < 5; i++) {
      osDelay(200);
      if (replyContains("OK")) {
        return NO_ERROR;
      }
      printf("AT Command Check Error\r\n");
    }
    return ERROR_NO_AT_REPLY;
  }
  return ERROR_UART_TRANSMIT;
}

SystemError sendATCommand(char *command, char *reply, bool addAT) {
  char p_command[50];

  if (addAT) {
    sprintf(p_command, "AT + %s\r\n", command);
  } else {
    sprintf(p_command, "%s\r\n", command);
  }
  // printf(p_command);
  // printf("\r\n");
  osDelay(500);
  if (HAL_OK == HAL_UART_Transmit(gsm_uart, (uint8_t *)p_command,
                                  strlen(p_command), 100)) {
    setSendingFlag(true);
  }

  for (uint8_t i = 0; i < 20; i++) {
    osDelay(50);

    if (!getSendingFlag()) {
      if (isOKReceived() && replyContains(command)) {
        // if (isOKReceived()) {
        printf("OK : %s\r\n", p_command);
        return NO_ERROR;
      } else {
        printf("ERROR : %s\r\n", p_command);
        return ERROR_NO_AT_REPLY;
      }
    }
  }

  return ERROR_UART_TRANSMIT;
}

bool checkReply(void) {
  if (replyContains("OK")) {
    return true;
  }
  return false;
}

// HAL_StatusTypeDef atCommandToGSM(const uint8_t *command) {
//   printf("Log Print : atCommandToGSM\n\a");
//   printArray(command, 40);

//   HAL_StatusTypeDef status =
//       HAL_UART_Transmit_IT(gsm_uart, command, countCommandLength(command));

//   // Add code to check for OK response from ESP8266
//   return status;
// }

void setSendingFlag(bool flag) { sendingData = flag; }
bool getSendingFlag(void) { return sendingData; }

void printArray(uint8_t *array, uint8_t dataLength) {
  for (size_t i = 0; i < dataLength; ++i) {
    printf("%c", array[i]);
  }
  printf("~|\r\n");
}

uint16_t GetTemperatureLevel(void) {
  uint16_t temperatureLevel = 0;

  temperatureLevel = (uint16_t)(SYS_GetTemperatureLevel() / 256);
  /* USER CODE BEGIN GetTemperatureLevel */

  /* USER CODE END GetTemperatureLevel */
  return temperatureLevel;
}
