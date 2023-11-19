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
  // no sensors:
  //node_object->setup.sw_pos_8 = true;
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

#include "sensirion-scd4x.h" 
void hardwareProbeSensors(node_object_t *handle) {
  probeBME280(handle);
  //probeOPCN3(handle);
  probeADS1115(handle);
  probeSPS30(handle);
  //probeGPS();
  //hardwareProbeSHT40(handle);
  //hardwareProbeSCD40(handle);
  //hardwareProbeSGP41(handle);
  Serial.println("SCD4X setup:");
  //setup_stuff();
  probeSDC4X(handle);
}

void hardwareInit(node_object_t *handle) {
    hardwareInitInbuilt(handle);
    hardwareEnable5V();
    //delay(100);
    hardwareProbeSensors(handle);
}

void hardwareUpdateData(node_object_t *handle) {
    counterUpdate(&node_object);
    hardwareBoardUpdate(&node_object);

    if (handle->probes.SPS30_present) {
        sps30Update(&node_object);
        //delay(4000);
        //sps30Update(&node_object);
    }
    if (handle->probes.BME280_present) {
        bme280Update(&node_object);
    }
    //if (handle->probes.OPCN3_present) {
    //    opcn3ReadFlush();
    //    delay(OPC_N3_SAMPLING_TIME); 
    //    opcn3ReadAll(&node_object);
    //    delay(100);
    //}
    if (handle->probes.ADS1115_1_present) {
        socketsUpdate(&node_object, ADS1115_1);
    }
    if (handle->probes.ADS1115_2_present) {
        socketsUpdate(&node_object, ADS1115_2);
    }
    if (handle->probes.SCD4X_present) {
      sdc4xUpdate(handle);
    }
}

// no sensors
void hardwareUpdateData_DUMMY(node_object_t *handle) {
  counterUpdate(&node_object);
  handle->probes.counter=true;
  //hardwareBoardUpdate(&node_object);

  //sps30
  handle->probes.SPS30_present=true;
  handle->data.sps30_PM1=random(0,2000);
  handle->data.sps30_PM2=random(0,2000);
  handle->data.sps30_PM4=random(0,2000);
  handle->data.sps30_PM10=random(0,2000);
  handle->data.sps30_NumPM0=random(0,2000);
  handle->data.sps30_NumPM1=random(0,2000);
  handle->data.sps30_NumPM2=random(0,2000);
  handle->data.sps30_NumPM4=random(0,2000);
  handle->data.sps30_NumPM10=random(0,2000);
  handle->data.sps30_PartSize=random(0,2000);
  //bme280
  handle->probes.BME280_present = true;
  handle->data.bme280_t=random(0,20000);
  handle->data.bme280_p=random(900000,1000000);
  handle->data.bme280_h=random(0,100000);
  //opcn3
  handle->probes.OPCN3_present = true;
  handle->data.opcn3_PM1=random(0,2000);
  handle->data.opcn3_PM2_5=random(0,2000);
  handle->data.opcn3_PM10=random(0,2000);
  // opcn3hist
  handle->probes.OPCN3_send_hist=true;
  handle->data.opcn3_bin0=random(0,50);
  handle->data.opcn3_bin1=random(0,50);
  handle->data.opcn3_bin2=random(0,50);
  handle->data.opcn3_bin3=random(0,50);
  handle->data.opcn3_bin4=random(0,50);
  handle->data.opcn3_bin5=random(0,50);
  handle->data.opcn3_bin6=random(0,50);
  handle->data.opcn3_bin7=random(0,50);
  handle->data.opcn3_bin8=random(0,50);
  handle->data.opcn3_bin9=random(0,50);
  handle->data.opcn3_bin10=random(0,50);
  handle->data.opcn3_bin11=random(0,50);
  handle->data.opcn3_bin12=random(0,50);
  handle->data.opcn3_bin13=random(0,50);
  handle->data.opcn3_bin14=random(0,50);
  handle->data.opcn3_bin15=random(0,50);
  handle->data.opcn3_bin16=random(0,50);
  handle->data.opcn3_bin17=random(0,50);
  handle->data.opcn3_bin18=random(0,50);
  handle->data.opcn3_bin19=random(0,50);
  handle->data.opcn3_bin20=random(0,50);
  handle->data.opcn3_bin21=random(0,50);
  handle->data.opcn3_bin22=random(0,50);
  handle->data.opcn3_bin23=random(0,50);

  handle->data.opcn3_bin1_MToF=random(0,10);
  handle->data.opcn3_bin3_MToF=random(0,10);
  handle->data.opcn3_bin5_MToF=random(0,10);
  handle->data.opcn3_bin7_MToF=random(0,10);

  handle->data.opcn3_sampling_p=random(0,10);
  handle->data.opcn3_flow_rate=random(0,10);
  handle->data.opcn3_t=random(0,10);
  handle->data.opcn3_h=random(0,10);

  handle->data.opcn3_RC_glitch=random(0,30);
  handle->data.opcn3_RC_long=random(0,30);
  handle->data.opcn3_RC_ratio=random(0,30);
  handle->data.opcn3_out_of_range=random(0,30);
  handle->data.opcn3_fan_rev=random(0,30);
  handle->data.opcn3_laser_stat=random(0,30);
  
  //sockets1
  handle->probes.ADS1115_1_present=true;
  handle->data.AD_socket_1_1=random(2000,30000);
  handle->data.AD_socket_1_2=random(2000,30000);
  handle->data.AD_socket_2_1=random(2000,30000);
  handle->data.AD_socket_2_2=random(2000,30000);
  //sockets2
  handle->probes.ADS1115_2_present=true;
  handle->data.AD_socket_3_1=random(2000,30000);
  handle->data.AD_socket_3_2=random(2000,30000);
  handle->data.AD_socket_4_1=random(2000,30000);
  handle->data.AD_socket_4_2=random(2000,30000);
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

void hardwareSleep(node_object_t *handle) {
    sps30Deinit(handle);
    hardwareDisable5V();
}
