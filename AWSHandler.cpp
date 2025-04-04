#include "AWSHandler.h"

WiFiClientSecure net;
PubSubClient client(net);

// Connect to AWS IoT
void connectAWS() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    client.setServer(AWS_IOT_ENDPOINT, 8883);
    client.setCallback(messageHandler);

    Serial.println("\nConnecting to AWS IoT...");
    while (!client.connect(THINGNAME)) {
        Serial.print(".");
        delay(100);
    }

    if (!client.connected()) {
        Serial.println("AWS IoT Timeout!");
        return;
    }

    client.subscribe("esp32/command");
    Serial.println("AWS IoT Connected!");
}

// MQTT Message Handler
void messageHandler(char* topic, byte* payload, unsigned int length) {
    Serial.print("Incoming message from: ");
    Serial.println(topic);
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    const char* command = doc["command"];
    Serial.println(command);
}

// Publish Float Inference Result to AWS IoT
void publishInference(float result) {
    StaticJsonDocument<200> doc;
    doc["type"] = "inference";  // Type identifier
    doc["inference_result"] = result;
    doc["timestamp"] = millis();

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    if (client.publish("esp32/inference", jsonBuffer)) {
        Serial.println("Inference result published successfully!");
    } else {
        Serial.println("Failed to publish inference result!");
    }
}

// Publish Alert Message to AWS IoT
void publishAlert(const char* message) {
    StaticJsonDocument<200> doc;
    doc["type"] = "alert";  // Type identifier
    doc["alert"] = message;
    doc["timestamp"] = millis();

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    if (client.publish("esp32/alerts", jsonBuffer)) {
        Serial.println("Oil leak alert published successfully!");
    } else {
        Serial.println("Failed to publish oil leak alert!");
    }
}

void testAwsConnection() {
  if(client.connected())
    Serial.println("Connected to AWS!");
  else
    Serial.println("Failed to Connect to AWS XXXX");
}
