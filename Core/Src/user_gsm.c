#include "../Inc/user_gsm.h"
#include "../Inc/user_system.h"
#include "../Inc/main.h"
#include "../Inc/usart.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef *gsm_uart;
extern UART_HandleTypeDef *wifi_uart;
char p_command[MQTT_SEND_MESSAGE_SIZE];
char p_wifCommand[WIFI_SEND_MESSAGE_SIZE];

/* External variables */
bool lastCommandOK = true;

void gsmInit(void) {
  // Initialize GSM module
  sendATCommand("AT", "", true);
  HAL_Delay(1000);
  sendATCommand("ATE0", "", true);  // Disable echo
  HAL_Delay(1000);
  
  // Configure GPRS connection
  sendATCommandRetry("cfun", "1,1", true);
  HAL_Delay(5000);
  
  // Configure bearer settings
  sendATCommandRetry("SAPBR", "3,1,\"Contype\",\"GPRS\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "3,1,\"APN\",\"telia\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "3,1,\"USER\",\"\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "3,1,\"PWD\",\"\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "1,1", true);
  HAL_Delay(2000);
  
  // Check bearer status
  sendATCommandRetry("SAPBR", "2,1", true);
  HAL_Delay(1000);
  
  // Configure MQTT settings
  sendATCommandRetry("SMCONF", "\"URL\",broker.emqx.io:1883", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SMCONF", "\"KEEPALIVE\",60", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SMCONF", "\"CLEANSS\",1", true);
  HAL_Delay(1000);
  
  // Connect to MQTT broker
  sendATCommandRetry("SMCONN", "", true);
  HAL_Delay(5000);
}

bool sendATCommand(const char *command, const char *param, bool addAT) {
  char p_command[64] = {0};
  
  if (addAT) {
    if (param[0] == '\0') {
      snprintf(p_command, sizeof(p_command), "AT + %s\r\n", command);
    } else {
      snprintf(p_command, sizeof(p_command), "AT + %s=%s\r\n", command, param);
    }
  } else {
    snprintf(p_command, sizeof(p_command), "%s\r\n", command);
  }
  
  HAL_UART_Transmit(&huart2, (uint8_t *)p_command, strlen(p_command), 1000);
  HAL_Delay(1000);
  
  return true;
}

bool sendATCommandRetry(const char *command, const char *param, bool addAT) {
  int retries = 3;
  bool result = false;
  
  while (retries > 0 && !result) {
    result = sendATCommand(command, param, addAT);
    if (!result) {
      HAL_Delay(1000);
      retries--;
    }
  }
  
  return result;
}

bool sendDirectAT(const char *command) {
  return sendATCommand(command, "", false);
}

bool sendRawAT(const char *command) {
  HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), 1000);
  HAL_Delay(1000);
  return true;
}

SystemError gsmInit(void) {
  SystemError status = ERROR_NO_AT_REPLY;
  status = sendATCommand("ATZ", "", AT_OK, NO_AT);

  if (!isLastCommandOK()) {
    return status;
  }
  status = sendATCommandRetry("cfun", "1,1", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(15000);

  status = sendATCommandRetry(AT_SAPBR, "3,1,\"Contype\",\"GPRS\"", AT_SAPBR,
                              ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(50);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"APN\",\"telia\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"USER\",\"\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"PWD\",\"\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  status = sendATCommandRetry(AT_SAPBR, "1,1", AT_OK, ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  status = sendATCommandRetry(AT_SAPBR, "2,1", AT_OK, ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(2000);
  status = sendATCommandRetry(AT_SMCONF, "\"URL\",broker.emqx.io:1883", AT_OK,
                              ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(500);
  status = sendATCommandRetry(AT_SMCONF, "\"KEEPALIVE\",60", "OK", ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  status = sendATCommandRetry(AT_SMCONF, "\"CLEANSS\",1", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  status = sendATCommandRetry(AT_SMCONN, "", AT_OK, ADD_AT);
  Delay_CustomTimer(2000);

  return status;
}

SystemError sendATCommandRetry(char *command, char *param, char *reply,
                               bool addAT) {
  SystemError status = ERROR_LAST_COMMAND_FAILED;
  for (uint8_t i = 0; i < 5; i++) {
    status = sendATCommand(command, param, addAT);
    if (status != NO_ERROR) {
      HAL_UART_Transmit(&huart2, (uint8_t *)"AT\r\n", 5, 100);
      setReceivingFlag(false);
      setLastCommandOK(true);
      cleanAllBuffers();

      Delay_CustomTimer(2000);
    } else {
      return NO_ERROR;
    }
  }
  setLastCommandOK(false);

  return ERROR_NO_AT_REPLY;
}

void sendDirectAT(char *command, char *param) {
  snprintf(p_wifCommand, sizeof(p_wifCommand), "AT+%s%s\r\n", command, param);
  HAL_UART_Transmit(wifi_uart, (uint8_t *)p_wifCommand, strlen(p_wifCommand),
                    100);
}

void sendRawAT(char *command, char *param) {
  snprintf(p_wifCommand, sizeof(p_wifCommand), "AT+%s%s", command, param);
  HAL_UART_Transmit(wifi_uart, (uint8_t *)p_wifCommand, strlen(p_wifCommand),
                    100);
}

SystemError sendATCommand(char *command, char *param, char *reply, bool addAT) {
  SystemError status = ERROR_LAST_COMMAND_FAILED;
  HAL_StatusTypeDef halStatus = HAL_ERROR;
  if (isLastCommandOK()) {
    status = ERROR_NO_AT_REPLY;
    lastCommandOK = false;

    if (addAT) {
      if (param[0] == '\0') {
        snprintf(p_command, sizeof(p_command), "AT + %s\r\n", command);
      } else {
        snprintf(p_command, sizeof(p_command), "AT + %s=%s\r\n", command,
                 param);
      }
    } else {
      sprintf(p_command, "%s\r\n", command);
    }

    if (getReceivingFlag() == false) {
      setReceivingFlag(true);
      halStatus = HAL_UART_Transmit_IT(gsm_uart, (uint8_t *)p_command,
                                       strlen(p_command));
      if (HAL_OK == halStatus) {
        for (uint8_t i = 0; i < 10; i++) {
          Delay_CustomTimer(50);
          if (!getReceivingFlag()) {
            if (replyContains(reply)) {
              if (replyContains("SMPUB") && replyContains("ERROR")) {
                status = ERROR_REPLY;
              } else {
                status = NO_ERROR;
              }
            } else {
              status = ERROR_REPLY;
            }
            break;
          }
        }

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