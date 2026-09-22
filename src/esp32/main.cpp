#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// -------------------------
// WiFi-konfiguration
// -------------------------
const char* WIFI_SSID = "DITT_WIFI";
const char* WIFI_PASS = "DITT_WIFI_LOSENORD";

// -------------------------
// MQTT-konfiguration
// -------------------------
const char* MQTT_HOST = "192.168.1.100";   // Ändra till din broker
const int   MQTT_PORT = 1883;              // Ingen TLS i denna demo
const char* MQTT_TOPIC = "iot25/indoor/temperature";

WiFiClient espClient;
PubSubClient client(espClient);

// -------------------------
// Funktion: anslut till WiFi
// -------------------------
void connectWiFi() {
  Serial.print("Ansluter till WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi anslutet!");
  Serial.print("IP-adress: ");
  Serial.println(WiFi.localIP());
}

// -------------------------
// Funktion: anslut till MQTT
// -------------------------
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Försöker ansluta till MQTT... ");

    if (client.connect("esp32-client")) {
      Serial.println("Ansluten!");
    } else {
      Serial.print("Misslyckades, rc=");
      Serial.print(client.state());
      Serial.println(" Försöker igen om 3 sek...");
      delay(3000);
    }
  }
}

// -------------------------
// Setup
// -------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  connectWiFi();
  client.setServer(MQTT_HOST, MQTT_PORT);
  connectMQTT();
}

// -------------------------
// Loop
// -------------------------
void loop() {
  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();

  // Fejkad temperatur (mellan 20.0 och 25.0)
  float fakeTemp = 20.0 + (float)random(0, 500) / 100.0;

  // Skapa JSON
  StaticJsonDocument<200> doc;
  doc["sensorId"] = "indoor-temp-01";
  doc["timestamp"] = "2026-09-17T10:30:00+02:00";
  doc["value"] = fakeTemp;
  doc["unit"] = "C";

  char buffer[256];
  serializeJson(doc, buffer);

  // Publicera
  client.publish(MQTT_TOPIC, buffer);
  Serial.print("Skickade JSON: ");
  Serial.println(buffer);

  delay(5000); // Skicka var 5:e sekund
}
