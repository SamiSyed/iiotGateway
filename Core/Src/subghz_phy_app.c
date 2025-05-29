void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  memset((void *)BufferRx, 0, LORA_RX_BUFFER_SIZE);
  memcpy((void *)BufferRx, payload, LORA_RX_BUFFER_SIZE);
  processIncomingLoraMessage();
}

void processIncomingLoraMessage(void) {
  char *message;
  message = strtok((char *)BufferRx, LORA_MESSAGE_DELIMITER);
// ... existing code ...
} 