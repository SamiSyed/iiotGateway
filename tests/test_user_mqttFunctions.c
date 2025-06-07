#include "user_mqttFunctions.h"
#include <assert.h>

int main(void) {
    setSystemError(NO_ERROR);
    getMqttMessageByIndex(NUMBER_OF_SENSORS);
    assert(getSystemError() == ERROR_INDEX_OUT_OF_RANGE);
    return 0;
}
