#include "user_system.h"

typedef struct {
  char topic[MQTT_TOPIC_SIZE];
  char value[MQTT_MESSAGE_SIZE];
  // timestamp
} MqttMessage_t;

void prepareMqttMessageStruct(uint8_t sensorIndex);
SystemError sendDataToMqttBroker(uint8_t sensorIndex);
void setMqttTopic(void);
MqttMessage_t *getMqttMessageByIndex(uint8_t index);
SystemError sendMqttServer(MqttMessage_t mqttMessage);
MqttMessage_t *getMqttMessage(void);
