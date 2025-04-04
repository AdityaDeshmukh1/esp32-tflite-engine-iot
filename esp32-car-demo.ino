#include "AWSHandler.h"
#include "TFLiteHandler.h"
#define TOUCH_SENSOR_PIN 4  
#define BUZZER_PIN 5        

float featureArray[5][8] = {
    {700.0, 2.49359182, 11.7909274, 3.17898079, 84.1441629, 81.6321865, 1.69776652e-05, 1.16617409e-01},
    {876.0, 2.94160593, 16.1938656, 2.46450370, 77.6409342, 82.4457245, 1.47029724e-05, 9.41161239e-02},
    {520.0, 2.96174558, 6.55314691, 1.06434676, 77.7522657, 79.6457767, 2.47333876e-05, 1.53164955e-01},
    {473.0, 3.70783474, 19.5101717, 3.72745536, 74.1299071, 71.7746287, 2.85197296e-05, 1.51743401e-01},
    {619.0, 5.67291858, 15.7388714, 2.05225145, 78.3969888, 87.0002254, 2.06067721e-05, 1.40549637e-01}
};


void setup() {
  pinMode(TOUCH_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.begin(115200);
    connectAWS();       // AWS Connection
    setupModel();       // TensorFlow Lite Model Initialization
    runAndPublishInferences();

}

void loop() {
  client.loop();

  if (!client.connected()) {
    Serial.println("MQTT disconnected. Attempting reconnect...");
    connectAWS();  // Re-run Wi-Fi + MQTT connect
  }

  int touchState = digitalRead(TOUCH_SENSOR_PIN); 

  if (touchState == HIGH) {  
          Serial.println("Touched! Buzzer ON");
          digitalWrite(BUZZER_PIN, HIGH);
          
          // Publish alert to AWS under the topic esp32/alerts  
          publishAlert("Leak_Detected!");  
      } else {
          digitalWrite(BUZZER_PIN, LOW);  
      }
}

void runAndPublishInferences() {
  for (int i = 0; i < 5; i++) {  
         Serial.print("Processing Sample ");
         Serial.println(i + 1);

         float inference_result = runInference(featureArray[i]);  // Run inference
         Serial.print("Inference Result: ");
         Serial.println(inference_result, 6);  // Print with 6 decimal places

         // Publish result to AWS under the topic esp32/inference
         publishInference(inference_result);   

         client.loop();
         delay(1000);  // Wait 1 second before the next sample
     }

  Serial.println("---- All inferences executed and published successfully!!! ----"); 

}
