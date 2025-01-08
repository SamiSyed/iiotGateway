#include "system.h"
#include "UartRingbuffer_multi.h"
#include "filter.h"
#include "freeRTOS.h"
#include "stdio.h"
#include "task.h"
#include "usart.h"

static bool rawDataReceived = false;
static SystemError systemError = NO_ERROR;
static bool sendingData = false;
static bool receivingData = false;
static bool initialized = false;
/* This initially has to be true to fetch first AT Reply*/
static bool lastCommandOK = true;

static uint16_t newValueBuffer[NUMBER_OF_SENSORS];
static uint16_t
    filteredSensorValues[NUMBER_OF_SENSORS]; /* Global Sensor Values*/
static MovingAverageFilter sensorFilters[NUMBER_OF_SENSORS];
static uint16_t filterBuffer[NUMBER_OF_SENSORS][FILTER_BUFFER_SIZE];
static Uart2Status_e uart2Status = UART2_RX_COMPLETE;

/* GSM variables */

/* Flag */
void setRawDataReceived(bool status) { rawDataReceived = status; }
bool isRawDataReceived(void) { return rawDataReceived; }

void setSendingFlag(bool flag) { sendingData = flag; }
bool getSendingFlag(void) { return sendingData; }

void setReceivingFlag(bool flag) { receivingData = flag; }
bool getReceivingFlag(void) { return receivingData; }

void setLastCommandOK(bool status) { lastCommandOK = status; }
bool isLastCommandOK(void) { return lastCommandOK; }

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

void sendCommand(char *command) {
  printf("at+");
  printf(command);
  printf("\r\n");
  osDelay(100);
}

SystemError gsmInit(void) {
  SystemError status = ERROR_NO_AT_REPLY;
  // osDelay(1000);
  // sendATCommand("ATE1", AT_OK, NO_AT);
  status = sendATCommand("ATZ", "", AT_OK, NO_AT);

  if (!isLastCommandOK()) {
    return status;
  }
  status = sendATCommand("cfun", "1,1", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  osDelay(15000);
  // for (uint8_t i = 0; i < 5; i++)
  // {
  //   printf("Waiting for network registration ");
  //   osDelay(1000);
  //   printf(". ");
  //   osDelay(1000);
  //   printf(". ");
  //   osDelay(1000);
  //   printf(".\r\n");
  // }

  // sendATCommand("cpin?", AT_OK, ADD_AT);
  // osDelay(500);
  // sendATCommand("csq", AT_OK, ADD_AT);
  // sendATCommand("creg?", AT_OK, ADD_AT);
  // sendATCommand("cgatt?", AT_OK, ADD_AT);
  /* Kept mute due to reply size more than rx_buffer size */
  // sendATCommand("cgdcont?", AT_OK, ADD_AT);

  /* MQTT */
  // osDelay(500);

  status = sendATCommandRetry(AT_SAPBR, "3,1,\"Contype\",\"GPRS\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  osDelay(50);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"APN\",\"telia\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  osDelay(100);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"USER\",\"\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  osDelay(100);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"PWD\",\"\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  osDelay(100);
  status = sendATCommandRetry(AT_SAPBR, "1,1", AT_OK, ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  osDelay(100);
  // status = sendATCommand(AT_SAPBR, "2,1", AT_SAPBR, ADD_AT);
  status = sendATCommandRetry(AT_SAPBR, "2,1", AT_OK, ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  osDelay(2000);
  status = sendATCommandRetry(AT_SMCONF, "\"URL\",broker.emqx.io:1883", AT_OK,
                              ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  osDelay(500);
  status = sendATCommandRetry(AT_SMCONF, "\"KEEPALIVE\",60", "OK", ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  status = sendATCommandRetry(AT_SMCONF, "\"CLEANSS\",1", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  status = sendATCommandRetry(AT_SMCONN, "", AT_OK, ADD_AT);
  osDelay(2000);

  return status;
}

SystemError sendATCommandRetry(char *command, char *param, char *reply,
                               bool addAT) {
  SystemError status = ERROR_LAST_COMMAND_FAILED;
  for (uint8_t i = 0; i < 5; i++) {

    status = sendATCommand(command, param, reply, addAT);
    if (status != NO_ERROR) {
      HAL_UART_Transmit(&huart2, (uint8_t *)"AT\r\n", 5, 100);
      setReceivingFlag(false);
      setLastCommandOK(true);
      cleanAllBuffers();

      osDelay(1000);
    } else {
      return NO_ERROR;
      // break;
    }
  }
  setLastCommandOK(false);

  return ERROR_NO_AT_REPLY;
}

SystemError sendATCommand(char *command, char *param, char *reply, bool addAT) {
  // osDelay(2000);
  SystemError status = ERROR_LAST_COMMAND_FAILED;
  if (isLastCommandOK()) {
    status = ERROR_NO_AT_REPLY;
    char p_command[MQTT_SEND_MESSAGE_SIZE];
    lastCommandOK = false;

    if (addAT) {
      if (param == "") {
        snprintf(p_command, sizeof(p_command), "AT + %s\r\n", command);
      } else {
        snprintf(p_command, sizeof(p_command), "AT + %s=%s\r\n", command,
                 param);
      }
    } else {
      sprintf(p_command, "%s\r\n", command);
    }

    if (!getReceivingFlag()) {
      setReceivingFlag(true);
      // printf("Sending : %s\r\n", p_command);
      if (HAL_OK == HAL_UART_Transmit(gsm_uart, (uint8_t *)p_command,
                                      strlen(p_command), 100)) {

        for (uint8_t i = 0; i < 100; i++) {
          osDelay(50);
          if (!getReceivingFlag()) {
            if (replyContains(reply)) {
              status = NO_ERROR;
            } else {
              status = ERROR_REPLY;
            }
            break; /* If it gets reply it will break the loop. It does not
                      matter OK or ERROR */
          }
        }
        // cleanAllBuffers();

        /* Print message according to status */
        if (status == ERROR_NO_AT_REPLY) {
          printf("No AT Reply : %s\r\n", p_command);
          setReceivingFlag(false);
          initUart();
        } else if (status == ERROR_REPLY) {
          printf("Error : %s\r\n", p_command);
        } else if (status == NO_ERROR) {
          setLastCommandOK(true);
          printf("OK : %s\r\n", p_command);
        } else {
          setReceivingFlag(false);
        }
      } else {
        printf("Error in Transmit : %s\r\n", p_command);
        status = ERROR_UART_TRANSMIT;
      }
    }
  }
  return status;
}

uint16_t GetTemperatureLevel(void) {
  uint16_t temperatureLevel = 0;

  temperatureLevel = (uint16_t)(SYS_GetTemperatureLevel() / 256);
  /* USER CODE BEGIN GetTemperatureLevel */

  /* USER CODE END GetTemperatureLevel */
  return temperatureLevel;
}

vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  // Handle stack overflow (log, restart, etc.)
  printf("Stack Overflow in Task : %s\r\n", pcTaskName);
}
