#include <ESP32AnalogRead.h>
#include "internal_adc.h"

ESP32AnalogRead adc3;
ESP32AnalogRead adc4;
ESP32AnalogRead adc5;

int hardwareBoardInit() {
    adc3.attach(ADC_3V5_RAIL);
    adc4.attach(ADC_BATT);
    //adc5.attach(ADC_5V_RAIL);
    return 0;
}

void hardwareBoardUpdate(node_object_t *handle) {
    adc3.attach(ADC_3V5_RAIL);
    adc4.attach(ADC_BATT);
    //adc5.attach(ADC_5V_RAIL);
    delay(100);
    handle->data.board_3_5V= (uint16_t)(100 * adc3.readVoltage() * ADC_DIVIDER_3V5);
    handle->data.board_batt = (uint16_t)(100 * adc4.readVoltage() * ADC_DIVIDER_BATT);
    //handle->data.board_5V = (uint16_t)(100 * adc5.readVoltage() * ADC_DIVIDER_5V);
}