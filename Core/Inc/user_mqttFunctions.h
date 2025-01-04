#include "system.h"

#define MQTT_TOPIC_SIZE 50U
#define MQTT_MESSAGE_SIZE 50U
#define MQTT_SEND_MESSAGE_SIZE (MQTT_TOPIC_SIZE + MQTT_MESSAGE_SIZE)

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
