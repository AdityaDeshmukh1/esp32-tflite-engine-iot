#ifndef AWS_HANDLER_H
#define AWS_HANDLER_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "WiFi.h"


// AWS MQTT Client
extern WiFiClientSecure net;
extern PubSubClient client;

void connectAWS();
void publishInference(float result);
void publishAlert(const char* message);
void messageHandler(char* topic, byte* payload, unsigned int length);
void testAwsConnection();

#endif
