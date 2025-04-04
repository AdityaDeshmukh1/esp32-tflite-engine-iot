#include "TFLiteHandler.h"

namespace {
    const tflite::Model* tflModel;
    tflite::ErrorReporter* tflErrorReporter;
    constexpr int tensorArenaSize = 20 * 1024;
    uint8_t tensorArena[tensorArenaSize];

    TfLiteTensor* tflInputTensor;
    TfLiteTensor* tflOutputTensor;
    tflite::MicroInterpreter* tflInterpreter;
}

const float mean[] = {791.2393, 3.3038, 6.6556, 2.3354, 77.6434, 78.4274, 1.818e-05, 0.1104};  
const float scale[] = {267.6043, 1.0216, 2.7609, 1.0364, 3.1109, 6.2066, 6.434e-06, 0.0394};  
const int num_features = 8;  // Our model expects 8 features

// Function to standardize input data
void scaleInput(float* inputData, float* outputData) {
    for (int i = 0; i < num_features; i++) {
        outputData[i] = (inputData[i] - mean[i]) / scale[i];  // Standardization formula
    }
}

// Setup TensorFlow Lite Model
void setupModel() {
    static tflite::MicroErrorReporter micro_error_reporter;
    tflErrorReporter = &micro_error_reporter;

    tflModel = tflite::GetModel(model_quantized);
    if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
        tflErrorReporter->Report("Model version mismatch!");
        return;
    }

    static tflite::ops::micro::AllOpsResolver micro_op_resolver;
    static tflite::MicroInterpreter static_interpreter(
        tflModel, micro_op_resolver, tensorArena, tensorArenaSize, tflErrorReporter
    );

    tflInterpreter = &static_interpreter;
    tflInterpreter->AllocateTensors();

    tflInputTensor = tflInterpreter->input(0);
    tflOutputTensor = tflInterpreter->output(0);

    Serial.println("Model setup complete");
}

// Run Inference
float runInference(float* rawInput) {
    Serial.println("Running inference...");

    // Check input tensor size
    if (tflInputTensor->dims->data[1] != num_features) {
        Serial.println("ERROR: Model input size does not match expected features.");
        return -1;
    }

    float scaledInput[num_features];  
    scaleInput(rawInput, scaledInput);  // Scale input data

    // Assign scaled input to the TensorFlow Lite model
    for (int i = 0; i < num_features; i++) {
        tflInputTensor->data.f[i] = scaledInput[i];
    }

    // Run inference
    if (tflInterpreter->Invoke() != kTfLiteOk) {
        Serial.println("Inference failed!");
        return -1;
    }

    // Print and return model output
    Serial.print("Model Output: ");
    float result = tflOutputTensor->data.f[0];  // Our model outputs a single float
    Serial.println(result);
    return result;
}
