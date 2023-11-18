#ifndef CITYNODE_SETUP_H
#define CITYNODE_SETUP_H

#include "stdint.h"

#define FW_VERSION ((uint8_t)       0)
#define NODE_SUBVERSION ((uint8_t)  47)

#define WIFI_SECONDS_TO_LOOP_DEFAULT               10
#define WIFI_MINUTES_TO_FWUPDATE_CHECK_DEFAULT     1
#define WIFI_MINUTES_TO_SENSORS_METADATA_DEFAULT   2
#define WIFI_MINUTES_TO_RECONFIGURE                2

#define EEPROM_PROVISION     (1)
#define EEPROM_ERASE         (2)
#define EEPROM_SHOW          (3)

#define EEPROM_MODE         EEPROM_SHOW
//#define EEPROM_MODE         EEPROM_PROVISION

#define AP_NAME     "SOASENSE_AP"
#define AP_PASSWORD  "tacrnode"


#define WDT_TIMEOUT (WIFI_SECONDS_TO_LOOP_DEFAULT*10)     // define a 3 seconds WDT (Watch Dog Timer)

/* Define node which you are about to flash - choose keys to flash*/
#define NODE_ID             NODE_VER2_01

#define NODE_VER2_01       (201)
#define NODE_VER2_02       (202)
#define NODE_VER2_03       (203)
#define NODE_VER2_04       (204)
#define NODE_VER2_05       (205)
#define NODE_VER2_06       (206)

#define NODE_ID_DEFAULT    (0)
#define NODE_ID_DEVNODE    (1)
#define NODE_ID_LAB_1      (10)
#define NODE_ID_LAB_2      (11)
#define NODE_ID_LAB_3      (12)
#define NODE_ID_EXTERN_1   (21)
#define NODE_ID_EXTERN_2   (22)
#define NODE_ID_EXTERN_3   (23)
#define NODE_ID_EXTERN_4   (24)
#define NODE_ID_EXTERN_5   (25)
#define NODE_ID_EXTERN_6   (26)
#define NODE_ID_EXTERN_7   (27)
#define NODE_ID_EXTERN_8   (28)
#define NODE_ID_EXTERN_9   (29)
#define NODE_ID_EXTERN_10  (30)
#define NODE_ID_EXTERN_11  (31)
#define NODE_ID_LIGHT_1    (100)
#define NODE_ID_LIGHT_2    (101)

#define NODE_ID_MINIMAL_2  (101)
#define NODE_ID_MINIMAL_3  (102)
#define NODE_ID_MINIMAL_4  (103)
#define NODE_ID_MINIMAL_5  (104)
#define NODE_ID_MINIMAL_6  (105)




#if (NODE_ID == NODE_ID_DEVNODE)
  #define DEVICEID           "DEVNODE"
#elif (NODE_ID == NODE_ID_LAB_1)
  #define DEVICEID           "LAB_NODE_01"
#elif (NODE_ID == NODE_ID_LIGHT_1)
  #define DEVICEID           "LIGHT_01"
#elif (NODE_ID == NODE_ID_LIGHT_2)
  #define DEVICEID           "LIGHT_02"
#elif (NODE_ID == NODE_VER2_01)
  #define DEVICEID           "VER02_DEV01"
#elif (NODE_ID == NODE_VER2_02)
  #define DEVICEID           "VER02_DEV02"
#elif (NODE_ID == NODE_VER2_03)
  #define DEVICEID           "VER02_DEV03"
#elif (NODE_ID == NODE_VER2_04)
  #define DEVICEID           "VER02_DEV04"
#elif (NODE_ID == NODE_VER2_05)
  #define DEVICEID           "VER02_DEV05"
#elif (NODE_ID == NODE_VER2_06)
  #define DEVICEID           "VER02_DEV06"
#endif

#include "../keyfiles/lorawan-keys.h"

// n case microswitches read failure, 
//init this config:
#define DEFAULT_MICROSWITCHES 0b00000000
//--------------------------------------
// Allocated bytes for payload
#define PAYLOAD_BUFFER_MAX_SIZE     128

//--------------------------------------
// Internal board AD channels and constants
// ADC pins on board
#define ADC_DIVIDER_3V5             2.0
#define ADC_DIVIDER_BATT            4.5944
#define ADC_DIVIDER_5V              2.0
#define ADC_3V5_RAIL                GPIO_NUM_35
#define ADC_BATT                    GPIO_NUM_34
#define ADC_5V_RAIL                 GPIO_NUM_2   

//----------------------------------------
// OPC-N3 Particle meter
#define OPCN3_BUFFER_SIZE          64
#define OPC_N3_SAMPLING_TIME       7000
#define SPIDEVICE_CS               GPIO_NUM_15
#define SPIDEVICE_SCK              GPIO_NUM_14
#define SPIDEVICE_MISO             GPIO_NUM_12
#define SPIDEVICE_MOSI             GPIO_NUM_13
#define SPIDEVICE_SPEED            500000

//----------------------------------------
// GPS GY-NEO6MV2
#define GPS_RX_PIN            36
#define GPS_TX_PIN            39

//----------------------------------------
// 5V enable pin

#define ENABLE_5V_PIN   GPIO_NUM_4
#define ENABLE_GPS_PIN  GPIO_NUM_0
#endif