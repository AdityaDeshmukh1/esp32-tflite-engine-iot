#ifndef TFLITE_HANDLER_H
#define TFLITE_HANDLER_H

#include <Arduino.h>
#include "model_quantized.h"  
#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

void setupModel();
float runInference(float* rawInput);

#endif
