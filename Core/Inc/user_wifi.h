#include "user_system.h"

typedef enum {
  SEND_CMD,
  WAITING_ANGLE,
  WAITING_OK,
  WAITING_CLOSE,
} SendingStatus_e;

typedef enum {
  WAITING_HTTP,
  HTTP_GOT_REQ,
  HTTP_PREPARE_REPLY,
  HTTP_SEND_REPLY,
} HttpStatus_e;

SystemError initWifi(void);
SystemError startServer(void);
void setGotHttpRequest(bool status);
void extractLinkId(void);
bool extractRequestedPage(char *dest, int8_t maxLen);
