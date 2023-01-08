
#include <EEPROM.h>
#include "../../node_setup/setup.h"
#include "eeprom.h"
#include "hardware.h"

void provisionDevice(){
  uint8_t otaa_deveui_to_save[8] = {OTAA_DEVEUI};
  uint8_t otaa_appeui_to_save[8] = {OTAA_APPEUI};
  uint8_t otaa_appkey_to_save[16] = {OTAA_APPKEY};
  node_provision_data_t provision_this = {0};

  memcpy(provision_this.device_name, DEVICEID, sizeof(DEVICEID));
  memcpy(provision_this.otaa_appeui, otaa_appeui_to_save, sizeof(otaa_appeui_to_save));
  memcpy(provision_this.otaa_appkey, otaa_appkey_to_save, sizeof(otaa_appkey_to_save));
  memcpy(provision_this.otaa_deveui, otaa_deveui_to_save, sizeof(otaa_deveui_to_save));
  memcpy(provision_this.AP_name, AP_NAME, sizeof(AP_NAME));

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(0, provision_this);
  EEPROM.commit();
}


void readProvisionedData(node_object_t *handle){
  node_provision_data_t persistent_data = {0};  

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, persistent_data);

  memcpy(handle->provision.device_name, persistent_data.device_name, sizeof(persistent_data.device_name));
  memcpy(handle->provision.otaa_deveui, persistent_data.otaa_deveui, sizeof(persistent_data.otaa_deveui));
  memcpy(handle->provision.otaa_appeui, persistent_data.otaa_appeui, sizeof(persistent_data.otaa_appeui));
  memcpy(handle->provision.otaa_appkey, persistent_data.otaa_appkey, sizeof(persistent_data.otaa_appkey));
  memcpy(handle->provision.AP_name, persistent_data.AP_name, sizeof(persistent_data.AP_name));
}

void eraseProvisionedData(){
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0xFF);
  }
  EEPROM.commit();
}
