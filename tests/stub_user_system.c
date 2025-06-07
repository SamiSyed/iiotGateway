#include "user_system.h"

static SystemError currentError = NO_ERROR;

void setSystemError(SystemError error){
    currentError = error;
}

SystemError getSystemError(void){
    return currentError;
}
