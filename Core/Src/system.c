#include "system.h"
#include "UartRingbuffer_multi.h"
#include "filter.h"

static bool rawDataReceived = false;
static SystemError systemError = NO_ERROR;
static bool sendingData = false;
static bool initialized = false;

static uint16_t newValueBuffer[NUMBER_OF_SENSORS];
static uint16_t filteredSensorValues[NUMBER_OF_SENSORS];
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

uint16_t getFilteredValueByIndex(uint8_t index) {
  return filteredSensorValues[index];
}

void prepareMqttMessageStruct(MqttMessage_t *mqttMessages) {
  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
    mqttMessages[i].topic = mqttTopic[i];
    mqttMessages[i].value = getFilteredValueByIndex(i);
  }
}

void setMqttTopic(void) {
  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
    strcpy(mqttTopic[i], "iiot/sensorID_1");
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

  sprintf(data, "SMPUB=\"%s\",1,0,\"%i\"", mqttMessage.topic, mqttMessage.value);
  sendCommand(data);
  // sendCommand("SMPUB=\"SensorReadings2024sami\",1,0,\"hello world\"");
  osDelay(1000);
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
  printf("ATZ\r\n");
  osDelay(50);
  sendCommand("cfun=1,1");
  osDelay(15000);

  sendCommand("cpin?");
  sendCommand("csq");
  sendCommand("creg?");
  sendCommand("cgatt?");
  sendCommand("cgdcont?");

  /* MQTT */
  sendCommand("sapbr=3,1,\"Contype\",\"GPRS\"");
  sendCommand("sapbr=3,1,\"APN\",\"telia\"");
  sendCommand("SAPBR=3,1,\"USER\",\"\"");
  sendCommand("SAPBR=3,1,\"PWD\",\"\"");
  sendCommand("sapbr=1,1");
  osDelay(2000);
  sendCommand("sapbr=2,1");
  osDelay(2000);
  sendCommand("smconf=\"URL\",broker.emqx.io:1883");
  osDelay(500);
  sendCommand("smconf=\"KEEPTIME\",60");
  sendCommand("smconf=\"CLEANSS\",1");
  sendCommand("SMCONN");
  osDelay(2000);

  // sendCommand("sapbr=0,1");
  // osDelay(2000);
  // setGMSReadinessFlag(true);
}

// void setGMSReadinessFlag(bool state){
//   gSMReadiness = state;
// }

// bool isGSMReady(void){
//   return gSMReadiness;
// }

SystemError atCommandCheck(void) {
  // Take care of BAUDRATE
  // AT
  // Uart_sendstring("AT\r\n", gsm_uart);
  // while (!(Wait_for("OK\r\n", gsm_uart)))
  //   ;
  // Uart_sendstring("AT---->OK\n\n", usb_uart);
  for (uint8_t i = 0; i < 3; i++) {
    sprintf(dataUart, "AT\r\n");
    printf(dataUart);
    osDelay(10);
  }

  return NO_ERROR;
}

void gsmSend(void) {
  // osDelay(1000);
  // sendCommand("sapbr=1,1");
  // osDelay(2000);

  // sendCommand("SMCONN");
  // osDelay(2000);
  // sendCommand("SMPUB=\"SensorReadings2024sami\",1,0,\"hello world\"");
  // osDelay(1000);
  // sendCommand("SMDISC");
  // sendCommand("sapbr=0,1");
  // osDelay(1000);
}

HAL_StatusTypeDef atCommandToGSM(const uint8_t *command) {
  printf("Log Print : atCommandToGSM\n\a");
  printArray(command, 40);

  HAL_StatusTypeDef status =
      HAL_UART_Transmit_IT(gsm_uart, command, countCommandLength(command));

  // Add code to check for OK response from ESP8266
  return status;
}

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
