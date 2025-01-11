#include "user_system.h"

typedef struct {
  char topic[MQTT_TOPIC_SIZE];
  char value[MQTT_MESSAGE_SIZE];
  // timestamp
} MqttMessage_t;

void prepareMqttMessageStruct(MqttMessage_t *mqttMessage);
SystemError sendAllDataToMqttBroker(MqttMessage_t *mqttMessage);
void setMqttTopic(void);
MqttMessage_t *getMqttMessageByIndex(uint8_t index);
bool sendMqttServer(MqttMessage_t mqttMessage);
MqttMessage_t *getMqttMessage(void);
