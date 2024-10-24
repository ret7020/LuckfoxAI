#include <ESP8266WiFi.h>
#include <espnow.h>

const int BUFFER_SIZE = 128;
char inputBuffer[BUFFER_SIZE];
int bufferIndex = 0;
uint8_t receiverMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};



void setup() {
  Serial.begin(115200);
  
  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  clearBuffer();
}

void loop() {
  if (Serial.available() > 0) {
    char currByte = Serial.read();

    if (currByte == '\n' || currByte == '\r') {
      inputBuffer[bufferIndex] = '\0';
      processCommand(inputBuffer);
      clearBuffer();
    } else if (bufferIndex < BUFFER_SIZE - 1) inputBuffer[bufferIndex++] = currByte;
  }
}

void clearBuffer() {
  memset(inputBuffer, 0, BUFFER_SIZE);
  bufferIndex = 0;
}

void processCommand(const char* cmd) {
  if (strncmp(cmd, "TX", 2) == 0)
  {
    const char* sendPayload = cmd + 3;
    esp_now_send(receiverMAC, (uint8_t *) &sendPayload , strlen(sendPayload ) * 4);
  }
}
