#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

char* ssid = "Daniel";
char* password = "senhadowifi";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int value = 0;
float latitude = -9.374167;
float longitude = -40.497799;

void onWsEvent(
  AsyncWebSocket *server, 
  AsyncWebSocketClient *client, 
  AwsEventType type, 
  void *arg, 
  uint8_t *data, 
  size_t len
) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("Cliente conectado");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Cliente desconectado");
  } else if (type == WS_EVT_DATA) {
    Serial.print("Dados recebidos: ");
    Serial.write(data, len);
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");

    if(ssid == "Daniel") {
      ssid == "PNZ_NET - DANIEL";
      password = "principedapaz";
    } else {
      ssid = "Daniel";
      password = "senhadowifi";
    }

  }
  Serial.print("Conectado ao WiFi: ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // if (!MDNS.begin("esp32")) {
  //   Serial.println("Erro ao configurar mDNS");
  //   return;
  // }
  // Serial.println("mDNS configurado como esp32.local");

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // Adicione uma rota básica para verificar a conexão HTTP
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Servidor ESP32 funcionando");
  });

  server.begin();
}

void loop() {
  if (ws.count() > 0) {
    StaticJsonDocument<200> doc;

    // Preencha as informações
    doc["maximumAltitude"] = value + 1;
    doc["altitude"] = value + 2;
    doc["maximumVelocity"] = value + 3;
    doc["velocity"] = value + 4;
    doc["maximumAcceleration"] = value + 5;
    doc["acceleration"] = value + 6;
    doc["velocityX"] = value + 7;
    doc["velocityY"] = value + 8;
    doc["velocityZ"] = value + 9;
    doc["accelerationX"] = value + 10;
    doc["accelerationY"] = value + 11;
    doc["accelerationZ"] = value + 12;
    doc["latitude"] = latitude;
    doc["longitude"] = longitude;

    JsonObject skibs = doc.createNestedObject("skibs");
    skibs["skib1"] = value % 2;
    skibs["skib2"] = value % 3;

    String json;
    serializeJson(doc, json);
    ws.textAll(json);
    Serial.print("Enviando dados: ");
    Serial.println(json);

    value++;
    latitude = latitude + 0.0002;
    longitude = longitude - 0.00004;
  }
  delay(500);
}
