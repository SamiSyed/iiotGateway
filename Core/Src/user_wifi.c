#include "user_wifi.h"
#include "index_html.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

/* Wifi variables */
static bool gotHttpRequest = false;
static char requestedPage[RX_BUFFER_SIZE_WIFI];
static char datatosend[TX_BUFFER_SIZE_WIFI];
volatile uint8_t rxDataSavedWiFi[RX_BUFFER_SIZE_WIFI];

/* Move back to local */
SendingStatus_e sendingStatus = SEND_CMD;
HttpStatus_e httpStatus = WAITING_HTTP;

int8_t linkId = 0;

char *Basic_inclusion = "<!DOCTYPE html> <html>\n<head><meta name=\"viewport\"\
		content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
		<title>IoTelliGen</title>\n<style>html { font-family: Helvetica; \
		display: inline-block; margin: 0px auto; text-align: center;}\n\
		body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\
		h3 {color: #444444;margin-bottom: 50px;}\n.button {display: block;\
		width: 80px;background-color: #1abc9c;border: none;color: white;\
		padding: 13px 30px;text-decoration: none;font-size: 25px;\
		margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n\
		.button-on {background-color: #1abc9c;}\n.button-on:active \
		{background-color: #16a085;}\n.button-off {background-color: #34495e;}\n\
		.button-off:active {background-color: #2c3e50;}\np {font-size: 14px;color: #888;margin-bottom: 10px;}\n\
		</style>\n</head>\n<body>\n<h1>IoTelliGen Gateway Configuration</h1>\n";

char *LED_ON = "<p>LED Status: ON</p><a class=\"button button-off\" "
               "href=\"/ledoff\">OFF</a>";
char *LED_OFF = "<p>LED1 Status: OFF</p><a class=\"button button-on\" "
                "href=\"/ledon\">ON</a>";
char *Terminate = "</body></html>";

static bool isAngleReceived(void);
static bool isSendOkReceived(void);
static bool isCloseOkReceived(void);
static bool replyHttp(char *str, int strLen, int Link_ID);
static void prepareHttpReply(void);
int get_param_value(const char *http_str, const char *param_name, char *out_val,
                    int max_len);

SystemError initWifi(void) {
  SystemError status = ERROR_NO_AT_REPLY;

  sendDirectAT("CWMODE", "=1");
  Delay_CustomTimer(1000);
  sendDirectAT("CWJAP", "=\"NTGR_031F\",\"6XbMM3F2\"");
  Delay_CustomTimer(10000);
  sendDirectAT("CIFSR", "");
  Delay_CustomTimer(2000);
  sendDirectAT("CIPMUX", "=1"); /* This has to be set as 1 to start as server */
  Delay_CustomTimer(1000);
  return status;
}

void setGotHttpRequest(bool status) { gotHttpRequest = status; }

SystemError startServer(void) {
  SystemError status = ERROR_NO_AT_REPLY;

  sendDirectAT("CIPSERVER", "=1,80");
  return status;
}

static void prepareHttpReply(void) {
  char value[20];

  if (strstr(requestedPage, "saveTopic")) {
    printf("SAVETOPIC\n\r");
    get_param_value(rxDataSavedWiFi, "tmp", value, sizeof(value));
    printf("tmp = %s\n", value);
    sprintf(datatosend, index_html);
    strcat(datatosend, Terminate);
  } else if (strstr(requestedPage, "ledoff")) {
    printf("LEDOFF\n\r");
    sprintf(datatosend, index_html);
    strcat(datatosend, LED_OFF);
    strcat(datatosend, Terminate);
  } else {
    sprintf(datatosend, index_html);
    // strcat(datatosend, LED_OFF);
    strcat(datatosend, Terminate);
  }
}

void serveHttpRequest(void) {
  // static HttpStatus_e httpStatus = WAITING_HTTP;

  switch (httpStatus) {
  case HTTP_GOT_REQ:
    extractLinkId();
    extractRequestedPage(requestedPage, RX_BUFFER_SIZE_WIFI);
    httpStatus = HTTP_PREPARE_REPLY;
    break;
  case HTTP_PREPARE_REPLY:
    prepareHttpReply();
    httpStatus = HTTP_SEND_REPLY;
    break;
  case HTTP_SEND_REPLY:
    // if (gotHttpRequest) {
      if (replyHttp(datatosend, index_html_len, linkId)) {
        httpStatus = WAITING_HTTP;
      }
      gotHttpRequest = false;
    // }
    break;
  case WAITING_HTTP:
    if (gotHttpRequest) {
      httpStatus = HTTP_GOT_REQ;
    }
  default:
    break;
  }
}

int get_param_value(const char *http_str, const char *param_name, char *out_val,
                    int max_len) {
  char search_pattern[32];
  snprintf(search_pattern, sizeof(search_pattern), "%s=", param_name);

  const char *start = strstr(http_str, search_pattern);
  if (!start) {
    return -1; // Parameter not found
  }

  start += strlen(search_pattern); // move past 'param='

  const char *end = strchr(start, '&');
  if (!end) {
    end = strchr(start, ' '); // In case it's the last param before HTTP/1.1
  }
  if (!end) {
    end = start + strlen(start); // fallback to end of string
  }

  int len = end - start;
  if (len >= max_len) {
    len = max_len - 1;
  }

  strncpy(out_val, start, len);
  out_val[len] = '\0';

  return 0; // Success
}

bool replyHttp(char *str, int strLen, int Link_ID) {
  uint16_t len = strlen(str);

  char data[80];
  // static SendingStatus_e sendingStatus = SEND_CMD;

  switch (sendingStatus) {
  case SEND_CMD:
    sprintf(data, "=%d,%d\r\n", Link_ID, len);
    sendRawAT("CIPSEND", data);
    sendingStatus = WAITING_ANGLE;
    break;
  case WAITING_ANGLE:
    if (isAngleReceived()) {
      HAL_UART_Transmit_IT(wifi_uart, (uint8_t *)str, len);
      sendingStatus = WAITING_OK;
    }
    break;
  case WAITING_OK:
    if (isSendOkReceived()) {
      sendDirectAT("CIPCLOSE", "=5");
      sendingStatus = WAITING_CLOSE;
    }
    break;
  case WAITING_CLOSE:
    if (isCloseOkReceived()) {
      sendingStatus = SEND_CMD;
      return true;
    }
    break;
  default:
    break;
  }

  return false; // Uart_sendstring(data, wifi_uart);
}

bool isAngleReceived(void) {
  getWiFiBufferData(rxDataSavedWiFi);
  const char *pos = strstr(rxDataSavedWiFi, ">");
  if (!pos) {
    return false; // search string not found
  }

  return true;
}

bool isSendOkReceived(void) {
  getWiFiBufferData(rxDataSavedWiFi);
  const char *pos = strstr(rxDataSavedWiFi, "SEND OK");
  if (!pos) {
    return false; // search string not found
  }

  return true;
}

bool isCloseOkReceived(void) {
  getWiFiBufferData(rxDataSavedWiFi);
  const char *pos = strstr(rxDataSavedWiFi, "OK\r\n");
  if (!pos) {
    return false; // search string not found
  }

  return true;
}

void extractLinkId(void) {
  const char *marker = "+IPD";
  memset((void *)rxDataSavedWiFi, 0, RX_BUFFER_SIZE_WIFI);
  getWiFiBufferData(rxDataSavedWiFi);
  char *found = strstr(rxDataSavedWiFi, marker);

  if (!found) {
    return NULL; // "+IPD" not found
  }

  found += strlen(marker); // move pointer to after "+IPD"
  linkId = found[1] - 48;  /* Convert from ASCII to integer */
                           // printf("linkId : %i\r\n", linkId);
}

bool extractRequestedPage(char *dest, int8_t maxLen) {
  const char *pos = strstr(rxDataSavedWiFi, "HTTP/1.1");
  if (!pos) {
    return false; // search string not found
  }

  size_t len = pos - (char *)rxDataSavedWiFi;
  if (len >= maxLen) {
    len = maxLen - 1; // reserve space for null terminator
  }

  strncpy(dest, rxDataSavedWiFi, len);
  dest[len] = '\0'; // null terminate

  return true;
}

void runServer(void) {
  char buftocopyinto[64] = {0};
  char Link_ID;
  // while (!(Get_after("+IPD,", 1, &Link_ID, wifi_uart)));
  // Link_ID -= 48;
  // while (!(Copy_upto(" HTTP/1.1", buftocopyinto, wifi_uart)));
  // if (Look_for("/ledon", buftocopyinto) == 1)
  // {
  // 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
  // 	Server_Handle("/ledon",Link_ID);
  // }

  // else if (Look_for("/ledoff", buftocopyinto) == 1)
  // {
  // 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  // 	Server_Handle("/ledoff",Link_ID);
  // }

  // else if (Look_for("/favicon.ico", buftocopyinto) == 1);

  // else
  // {
  // 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  // 	Server_Handle("/ ", Link_ID);
  // }
}