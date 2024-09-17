#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

// Replace these with your WiFi network credentials
const char* ssid = "OnePlus Nord 2 5G";
const char* password = "81Code/@2";

// Create a WebSocket server on port 80
WebSocketsServer webSocket = WebSocketsServer(80);

unsigned long lastSendTime = 0;  // Last time the random text was sent
const unsigned long interval = 1000;  // Interval to send random text (in milliseconds)

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  // Handle WebSocket events
  webSocket.loop();

  // Check if it's time to send the random text
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= interval) {
    lastSendTime = currentMillis;
    sendRandomText();
  }
}

// Function to handle WebSocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("Client %u disconnected\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("Client %u connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
        // Send a welcome message to the new client
        webSocket.sendTXT(num, "Hello World!");
      }
      break;
    case WStype_TEXT:
      Serial.printf("Message from client %u: %s\n", num, payload);
      break;
    case WStype_ERROR:
    case WStype_PING:
    case WStype_PONG:
      // Handle other WebSocket events if needed
      break;
  }
}

// Function to send random text to all connected clients
void sendRandomText() {
  String randomText = "Random text: " + String(random(1000, 9999));
  webSocket.broadcastTXT(randomText);
}