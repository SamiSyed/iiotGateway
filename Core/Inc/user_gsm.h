#ifndef __USER_GSM_H
#define __USER_GSM_H

#include "main.h"
#include "user_system.h"
#include "usart.h"
#include <stdbool.h>
#include <stdint.h>

/* Function declarations */
void gsmInit(void);
bool sendATCommand(const char *command, const char *param, bool addAT);
bool sendATCommandRetry(const char *command, const char *param, bool addAT);
bool sendDirectAT(const char *command);
bool sendRawAT(const char *command);

/* External declarations */
extern bool lastCommandOK;
extern bool replyContains(const char* str);
extern void cleanAllBuffers(void);
extern void initUart(void);

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef hlpuart1;

#endif /* __USER_GSM_H */ 