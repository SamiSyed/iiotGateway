#include "user_mqttFunctions.h"
#include "user_system.h"


char mqttTopic[NUMBER_OF_SENSORS][MQTT_TOPIC_SIZE];
static MqttMessage_t mqttMessages[NUMBER_OF_SENSORS];

MqttMessage_t *getMqttMessage(void) { return mqttMessages; }

SystemError sendAllDataToMqttBroker(MqttMessage_t *mqttMessage) {
  // for (uint8_t i = 0; i < 1; i++) {
  SystemError status = sendMqttServer(mqttMessage[0]);
  // }

  return status;
}

SystemError sendMqttServer(MqttMessage_t mqttMessage) {
  char data[MQTT_SEND_MESSAGE_SIZE];

  sprintf(data, "\"%s\",1,0,\"%s\"", mqttMessage.topic, mqttMessage.value);
  SystemError status = sendATCommand("SMPUB", data, "SMPUB", ADD_AT);
  return status;
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

/* Update MQTT struct with latest value to be send */
void prepareMqttMessageStruct(MqttMessage_t *mqttMessages) {
  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
    char data[20];

    sprintf(data, "{\\\"value\\\":\\\"%i\\\"}", getFilteredValueByIndex(i));
    strcpy(mqttMessages[i].value, data);
    strcpy(mqttMessages[i].topic, mqttTopic[i]);

    // printf("mqttMessages[i].value : %s \r\n", mqttMessages[i].value);
  }
}
