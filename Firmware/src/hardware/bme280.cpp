#include "stdio.h"
#include "hardware.h"
#include <Adafruit_BME280.h>
#include "bme280.h"

Adafruit_BME280 bme;
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();
sensors_event_t temp_event, pressure_event, humidity_event;

int bme280Init(void) {
    unsigned status;
    // default settings
    status = bme.begin(BMEADDR);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        //Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        /*Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");*/
        return 1;
    }

    return 0;
}
int bme280Update(node_object_t *handle) {
    bme_temp->getEvent(&temp_event);
    handle->data.bme280_t = (uint16_t)(temp_event.temperature *100);
    bme_pressure->getEvent(&pressure_event);
    handle->data.bme280_p = (uint16_t)(pressure_event.pressure);
    bme_humidity->getEvent(&humidity_event);
    handle->data.bme280_h = (uint16_t)(humidity_event.relative_humidity *100);

    return 0;
}

void probeBME280(node_object_t *handle) {
    unsigned status = bme.begin(BMEADDR);  
    if (!status) {
        handle->probes.BME280_present = false;
        return;
    }
    handle->probes.BME280_present = true;
    
    return;
}
