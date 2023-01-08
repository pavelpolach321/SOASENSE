#include "stdio.h"
#include <Adafruit_BME280.h>
#include "internal_adc.h"
#include "hardware.h"
#include "opcn3.h"
#include "bme280.h"
#include "ads1115.h"
#include "fis3031.h"
#include "gps.h"
#include "sensirion-sps30.h"
#include "sps30.h"
#include "pcf8574_expander.h"
#include "eeprom.h"
#include "ui/ui.h"

#include "wifi-support.h"


void hardwareHandleEEPROM(node_object_t *handle){
    #if (EEPROM_MODE == EEPROM_PROVISION)
      eraseProvisionedData();
      provisionDevice();
      //serial.println("EEPROM provisioned: ");
      readProvisionedData(handle);
      uiPrintProvision(handle);
      while(1)
        ;
    #elif (EEPROM_MODE == EEPROM_ERASE)
      eraseProvisionedData();
      //serial.println("EEPROM erased: ");
      while(1)
        ;
    #elif (EEPROM_MODE == EEPROM_SHOW)
      readProvisionedData(handle);
      uiPrintProvision(handle);
    #endif
}

void hardwareEnable5V(){
    gpio_pulldown_en(ENABLE_5V_PIN);
    gpio_set_direction(ENABLE_5V_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(ENABLE_5V_PIN, 1);
    delay(50);
}

void hardwareDisable5V(){
    gpio_pulldown_en(ENABLE_5V_PIN);
    gpio_set_direction(ENABLE_5V_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(ENABLE_5V_PIN, 0);
    delay(50);
}

void hardwareGetMAC(node_object_t *node_object){
    esp_efuse_mac_get_default(node_object->mac_addr);
}

void hardwareSetup(node_object_t *node_object){
  uint8_t microswitches = get_setup_switches();
  if (microswitches & SW_POS_8) {
    node_object->setup.sw_pos_8 = true;
  } else {
    node_object->setup.sw_pos_8 = false;
  }
  if (microswitches & SW_POS_7) {
    node_object->setup.sw_pos_7 = true;
  } else {
    node_object->setup.sw_pos_7 = false;
  }
  if (microswitches & SW_POS_6) {
    node_object->setup.sw_pos_6 = true;
  } else {
    node_object->setup.sw_pos_6 = false;
  }
  if (microswitches & SW_POS_5) {
    node_object->setup.sw_pos_5 = true;
  } else {
    node_object->setup.sw_pos_5 = false;
  }
  if (microswitches & SW_POS_4) {
    node_object->setup.sw_pos_4 = true;
  } else {
    node_object->setup.sw_pos_4 = false;
  }
  if (microswitches & SW_POS_3) {
    node_object->setup.sw_pos_3 = true;
  } else {
    node_object->setup.sw_pos_3 = false;
  }
  if (microswitches & SW_POS_2) {
    node_object->setup.sw_pos_2 = true;
  } else {
    node_object->setup.sw_pos_2 = false;
  }
  if (microswitches & SW_POS_1) {
    node_object->setup.sw_pos_1 = true;
  } else {
    node_object->setup.sw_pos_1 = false;
  }
}

int counterInit(){
    return 0;
}

void counterUpdate(node_object_t *handle)
{
    ++handle->data.counter;
}

/*
void hardwareProbeOPCN3(node_object_t *handle);
void hardwareProbeADS1115(node_object_t *handle);
void hardwareProbeSPS30(node_object_t *handle);
void hardwareProbeSHT40(node_object_t *handle);
void hardwareProbeSCD40(node_object_t *handle);
void hardwareProbeSGP41(node_object_t *handle);
*/
  
void hardwareInitInbuilt(node_object_t *handle) {
    handle->probes.counter = true;
    handle->probes.board_voltages = true;
    handle->data.counter = 0;
    hardwareBoardInit();
};

void hardwareProbeSensors(node_object_t *handle) {
  probeBME280(handle);
  probeOPCN3(handle);
  probeADS1115(handle);
  probeSPS30(handle);
  //probeGPS();
  //hardwareProbeSHT40(handle);
  //hardwareProbeSCD40(handle);
  //hardwareProbeSGP41(handle);
}

void hardwareInit(node_object_t *handle) {
    hardwareInitInbuilt(handle);
    hardwareEnable5V();
    delay(1000);
    hardwareProbeSensors(handle);
}

void hardwareUpdateData(node_object_t *handle) {
    counterUpdate(&node_object);
    hardwareBoardUpdate(&node_object);
    if (handle->probes.SPS30_present) {
        sps30Update(&node_object);
    }
    if (handle->probes.BME280_present) {
        bme280Update(&node_object);
    }
    if (handle->probes.OPCN3_present) {
        opcn3ReadFlush();
        delay(OPC_N3_SAMPLING_TIME); 
        opcn3ReadAll(&node_object);
        delay(100);
    }
    if (handle->probes.ADS1115_1_present) {
        socketsUpdate(&node_object, ADS1115_1);
    }
    if (handle->probes.ADS1115_2_present) {
        socketsUpdate(&node_object, ADS1115_2);
    }
}

void hardwareCreatePayload(node_object_t *handle) {
    // clear handle
    memset(handle->payload_buf, 0, PAYLOAD_BUFFER_MAX_SIZE);
    handle->payload_len = 0;
    // Add info about sensors_present in the payload to payload
    // store what is present, so payload decoder knows what to do
    // FIXME
    // FIXME

    handle->payload_buf[handle->payload_len] = handle->setup.sensors_present_1;
    handle->payload_len++;
    handle->payload_buf[handle->payload_len] = handle->setup.sensors_present_2;
    // Add each present sensor and increment the payload length, 
    // use payload length as index 
    if (handle->probes.counter) {
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.counter >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.counter & 0xFF;
    }
    if (handle->probes.board_voltages) {
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.board_3_5V >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.board_3_5V & 0xFF;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.board_batt >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.board_batt & 0xFF;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.board_5V >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.board_5V & 0xFF;
    }
    if (handle->probes.BME280_present) {
        // temp
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.bme280_t >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.bme280_t & 0xFF;
        // pres
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.bme280_p >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.bme280_p & 0xFF;
        // hum
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.bme280_h >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.bme280_h & 0xFF;
    }
    if (handle->probes.SPS30_present) {
        // pm1
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM1 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM1 & 0xFF;
        // pm2
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM2 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM2 & 0xFF;
        // pm4
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM4 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM4 & 0xFF;
        // pm10
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM10 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.sps30_PM10 & 0xFF;
    }
    if (handle->probes.OPCN3_present) {
        // pm1
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.opcn3_PM1 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.opcn3_PM1 & 0xFF;
        // pm25
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.opcn3_PM2_5 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.opcn3_PM2_5 & 0xFF;
        // pm10
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.opcn3_PM10 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.opcn3_PM10 & 0xFF;
    }
    if (handle->probes.ADS1115_1_present) {
        // voltage on socket 1 position 1
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_1_1 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_1_2 & 0xFF;
        // voltage on socket 1 position 2
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_2_1 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_2_2 & 0xFF;
    }
    if (handle->probes.ADS1115_2_present) {
        // voltage on socket 2 position 1
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_3_1 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_3_2 & 0xFF;
        // voltage on socket 2 position 2
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_4_1 >> 8;
        handle->payload_len++;
        handle->payload_buf[handle->payload_len] = handle->data.AD_socket_4_2 & 0xFF;
    }

    // Simplistic sanity check, so payload decoder knows that this is the end of message
    handle->payload_len++;
    handle->payload_buf[handle->payload_len] = 0xCA;
    handle->payload_len++;
    handle->payload_buf[handle->payload_len] = 0xFE;

    // Must increase index so it became length
    handle->payload_len++;
}
