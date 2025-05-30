# ESP32 TFLite Engine IoT Demo

A complete IoT project demonstrating alerting and monitoring of an automotive engine in real-time using an ESP32 microcontroller.
This system performs on-device inference with a quantized TensorFlow Lite (TFLite) model and streams the results to AWS IoT Core
for cloud monitoring.

---

## Project Overview

This project combines embedded systems, machine learning, and cloud computing to detect engine health anomalies. It integrates:

- **On-Device inference** using a quantized TFLite Model.
- **AWS IoT Core MQTT messaging** for remote cloud communication.
- A **complete ML Pipeline** from data generation and model training to deployment on an embedded microcontroller.


## Getting Started

### Prerequisites

- ESP32 development board 
- Arduino IDE
- AWS IoT Core Account
- Python 3.x environment for machine learning tasks

### Arduino IDE Libraries

You will need to install the following libraries in Arduino IDE:

- Adafruit Unified Sensor 1.1.15
- ArduinoJson 7.3.0
- DHT sensor library 1.4.6
- EloquentTensorFlow32 1.0.4
- EloquentTinyML 3.0.1
- ESPPubSubClientWrapper 1.0.2
- PubSubClient 2.8
- TensorFlowLite_ESP32 0.8.0
- tflm_esp32 2.0.0

