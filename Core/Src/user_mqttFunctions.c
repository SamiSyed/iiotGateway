#include "user_mqttFunctions.h"
#include "user_system.h"

char mqttTopic[NUMBER_OF_SENSORS][MQTT_TOPIC_SIZE];
static MqttMessage_t mqttMessages[NUMBER_OF_SENSORS];

MqttMessage_t *getMqttMessage(void) { return mqttMessages; }

SystemError sendDataToMqttBroker(uint8_t sensorIndex) {
  return sendMqttServer(mqttMessages[sensorIndex]);
}

SystemError sendMqttServer(MqttMessage_t mqttMessage) {
  char data[MQTT_SEND_MESSAGE_SIZE];

  sprintf(data, "\"%s\",1,0,\"%s\"", mqttMessage.topic, mqttMessage.value);
  return sendATCommand("SMPUB", data, "SMPUB", ADD_AT);
}

void setMqttTopic(void) {
  char topic[MQTT_TOPIC_SIZE] = "SensorReadings2024sami";
  // char topic[MQTT_TOPIC_SIZE] = "iiot/sensorID_";

  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
    // sprintf(topic, "iiot/sensorID_%i", i);
    strcpy(mqttTopic[i], topic);

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
void prepareMqttMessageStruct(uint8_t sensorIndex) {
  static bool firstTime = true;
  char data[MQTT_MESSAGE_SIZE];
  int len =
      snprintf(data, sizeof(data), "{\\\"ID\\\":\\\"0129784251391295\\\",");
  // if (len < sizeof(data)) {
  //   len += snprintf(data + len, sizeof(data) - len,
  //                   "\\\"Sensor\\\":\\\"Temperature\\\",");
  // }

  // if (len < sizeof(data) && !firstTime) {
  //   len += snprintf(data + len, sizeof(data) - len,
  //   "\\\"Value\\\":\\\"%i\\\",",
  //                   getFilteredValueByIndex(sensorIndex));
  // } else if (len < sizeof(data) && firstTime) {
  //   len +=
  //       snprintf(data + len, sizeof(data) - len,
  //       "\\\"Value\\\":\\\"50\\\",");
  //   firstTime = false;
  // }

  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len, "\\\"Temperature\\\":\\\"%i\\\",",
                    getSoilSensorTemperature());
  }
  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len, "\\\"Moisture\\\":\\\"%i\\\",",
                    getSoilSensorMoisture());
  }
  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len, "\\\"Conductivity\\\":\\\"%i\\\",", 
                    getSoilSensorConductivity());
  }
  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len, "\\\"pH\\\":\\\"%i\\\",",
                    getSoilSensorPH());
  }
  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len, "\\\"Nitrogen\\\":\\\"%i\\\",",
                    getSoilSensorNitrogen());
  }
  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len, "\\\"Phosphorus\\\":\\\"%i\\\",",
                    getSoilSensorPhosphorus());
  }
  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len, "\\\"Potassium\\\":\\\"%i\\\",",
                    getSoilSensorPotassium());
  }

  if (len < sizeof(data)) {
    len += snprintf(data + len, sizeof(data) - len,
                    "\\\"Timestamp\\\":\\\"2025-05-28 10:16:05\\\"}");
  }

  strcpy(mqttMessages[sensorIndex].value, data);
  strcpy(mqttMessages[sensorIndex].topic, mqttTopic[sensorIndex]);

  // printf("mqttMessages[i].value : %s \r\n", mqttMessages[sensorIndex].value);
}
