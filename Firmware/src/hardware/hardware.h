#ifndef CITYNODE_H
#define CITYNODE_H

#include "stdint.h"
#include "../../node_setup/setup.h"

// PRESENT byte 1
// Internal board's counter
#define PRESENT_1_COUNTER_mask             0x01
// Board's internal ADC's
#define PRESENT_1_BOARD_mask               0x02
// Free positions here
#define PRESENT_1_GPS_mask                 0x04
// Free positions here
// PRESENT byte 2
// BME280 temp humidity pressure
#define PRESENT_2_BME_mask                 0x01
// Alphasense particlemeter
#define PRESENT_2_OPCN3_mask               0x02
// ADS1115 4x 16bit ADC
#define PRESENT_2_AD_SOCKETS_mask          0x04
// Sensirion particlemeter SPS30
#define PRESENT_2_SPS30_mask               0x08
// Sensirion humidity and temperature SHT40
#define PRESENT_2_SHT40_mask               0x10
// Sensirion calibrated CO2 SCD40
#define PRESENT_2_SCD40_mask               0x20
// Sensirion volatile compound VOC + NOX SGP41 
#define PRESENT_2_SGP41_mask               0x40
// FIS3031
//#define PRESENT_2_METAN_mask               0x80



#define PRESENT_1_DEFAULT     (uint8_t)((PRESENT_1_COUNTER_mask * PRESENT_1_COUNTER_BIT) | \
                                        (PRESENT_1_BOARD_mask * PRESENT_1_BOARD_BIT)) | \
                                        (PRESENT_1_GPS_mask * PRESENT_1_GPS_BIT)

#define PRESENT_2_DEFAULT     (uint8_t)((PRESENT_2_BME_mask * PRESENT_2_BME_BIT) | \
                                        (PRESENT_2_OPCN3_mask * PRESENT_2_OPCN3_BIT) | \
                                        (PRESENT_2_AD_SOCKETS_mask * PRESENT_2_AD_SOCKETS_BIT) | \
                                        (PRESENT_2_SPS30_mask * PRESENT_2_SPS30_BIT) | \
                                        (PRESENT_2_SHT40_mask * PRESENT_2_SHT40_BIT) | \
                                        (PRESENT_2_SCD40_mask * PRESENT_2_SCD40_BIT) | \
                                        (PRESENT_2_SGP41_mask * PRESENT_2_SGP41_BIT))

#define WIFI_LOOPS_TO_RECONFIGURE         (uint32_t)((WIFI_MINUTES_TO_RECONFIGURE * 60) / (WIFI_SECONDS_TO_LOOP_DEFAULT))
#define WIFI_LOOPS_TO_SEND_METADATA       (uint32_t)((WIFI_MINUTES_TO_SENSORS_METADATA_DEFAULT * 60) / (WIFI_SECONDS_TO_LOOP_DEFAULT))
#define WIFI_LOOPS_TO_RUN_FWUPDATE_CHECK  (uint32_t)((WIFI_MINUTES_TO_FWUPDATE_CHECK_DEFAULT * 60) / (WIFI_SECONDS_TO_LOOP_DEFAULT))

typedef struct {
    uint16_t counter;
    uint16_t board_3_5V;
    uint16_t board_5V;
    uint16_t board_batt;
    int16_t bme280_t;
    uint16_t bme280_p;
    uint16_t bme280_h;
    // OPCN3
    char opcn3_fw_ver[8];
    char opcn3_ser_num[10];

    uint16_t opcn3_PM1;
    uint16_t opcn3_PM2_5;
    uint16_t opcn3_PM10;

    uint16_t opcn3_bin0;
    uint16_t opcn3_bin1;
    uint16_t opcn3_bin2;
    uint16_t opcn3_bin3;
    uint16_t opcn3_bin4;
    uint16_t opcn3_bin5;
    uint16_t opcn3_bin6;
    uint16_t opcn3_bin7;
    uint16_t opcn3_bin8;
    uint16_t opcn3_bin9;
    uint16_t opcn3_bin10;
    uint16_t opcn3_bin11;
    uint16_t opcn3_bin12;
    uint16_t opcn3_bin13;
    uint16_t opcn3_bin14;
    uint16_t opcn3_bin15;
    uint16_t opcn3_bin16;
    uint16_t opcn3_bin17;
    uint16_t opcn3_bin18;
    uint16_t opcn3_bin19;
    uint16_t opcn3_bin20;
    uint16_t opcn3_bin21;
    uint16_t opcn3_bin22;
    uint16_t opcn3_bin23;

    uint16_t opcn3_bin1_MToF;
    uint16_t opcn3_bin3_MToF;
    uint16_t opcn3_bin5_MToF;
    uint16_t opcn3_bin7_MToF;
    uint16_t opcn3_sampling_p;
    uint16_t opcn3_flow_rate;

    uint16_t opcn3_t; // temp
    uint16_t opcn3_h; // hum
    uint16_t opcn3_RC_glitch;
    uint16_t opcn3_RC_long;
    uint16_t opcn3_RC_ratio;
    uint16_t opcn3_out_of_range;
    uint16_t opcn3_fan_rev;
    uint16_t opcn3_laser_stat;
    uint16_t reset_event; // we can keep a track of called resets?

    // SPS30
    char sps30_fw_ver[8];
    char sps30_lib_ver[8];
    char sps30_ser_num[17];
    char sps30_prod_name[9]; 
    uint16_t sps30_PM1;
    uint16_t sps30_PM2;
    uint16_t sps30_PM4;
    uint16_t sps30_PM10;
    uint16_t sps30_NumPM0;
    uint16_t sps30_NumPM1;
    uint16_t sps30_NumPM2;
    uint16_t sps30_NumPM4;
    uint16_t sps30_NumPM10;
    uint16_t sps30_PartSize;
    // AD sockets
    uint16_t AD_socket_1_1;
    uint16_t AD_socket_1_2;
    uint16_t AD_socket_2_1;
    uint16_t AD_socket_2_2;
    uint16_t AD_socket_3_1;
    uint16_t AD_socket_3_2;
    uint16_t AD_socket_4_1;
    uint16_t AD_socket_4_2;
    // scd4x 
    uint16_t scd4x_co2;
    uint16_t scd4x_temp;
    uint16_t scd4x_hum;
    
} node_data_t;

typedef struct { 
    uint16_t seconds_to_loop;
    uint16_t minutes_to_sensors_metadata;
    uint16_t minutes_to_fwupdate_check;
    uint16_t minutes_to_reconfigure;
} node_behaviour_t;

typedef struct {
    node_behaviour_t behaviour;
    uint8_t sensors_present_1;
    uint8_t sensors_present_2;
    bool sw_pos_1;
    bool sw_pos_2;
    bool sw_pos_3;
    bool sw_pos_4;
    bool sw_pos_5;
    bool sw_pos_6;
    bool sw_pos_7;
    bool sw_pos_8;
    uint8_t other_setup;
} node_setup_t;

typedef struct { 
    char device_name[17];
    uint8_t otaa_deveui[8];
    uint8_t otaa_appeui[8];
    uint8_t otaa_appkey[16];
    char AP_name[17];
} node_provision_data_t;

typedef struct {
    // default inbuilt
    bool counter;
    bool board_voltages;
    // others
    bool BME280_present;
    bool SPS30_present;
    bool OPCN3_present;
    bool OPCN3_send_hist;
    bool ADS1115_1_present;
    bool ADS1115_2_present;
    bool SHT40_present;
    bool SGP41_present;
    bool SCD4X_present;
} probes_t;

typedef struct node_object_t{
    // Values
    node_data_t data;
    // microswitches setup
    node_setup_t setup;
    // probed sensors
    probes_t probes;
    // node provisioned data
    node_provision_data_t provision;

    uint8_t payload_buf[PAYLOAD_BUFFER_MAX_SIZE];
    uint8_t payload_len;

    uint8_t mac_addr[6];
} node_object_t;


extern node_object_t node_object;

// init node
void hardwareInit(node_object_t *handle);
// read data
void hardwareUpdateData(node_object_t *handle);
void hardwareUpdateData_DUMMY(node_object_t *handle);
// create payload
void hardwareCreatePayload(node_object_t *handle);
// process downlink
void hardware_process_downlink();
// write debug to serial
void hardwareDbgToSerial(node_object_t *handle);
void hardwareDbgPayloadToSerial(node_object_t *handle);
// show data on display
void hardware_display_show();



///
/*
Read eeprom data ansd store it into object
*/

void hardwareHandleEEPROM(node_object_t *node_object);
/// 
/*
Updates MAC address of objet
*/
void hardwareGetMAC(node_object_t *node_object);

/// @brief 
/*
Sets internal object's variables
*/
void hardwareSetup(node_object_t *node_object);

void hardwareProbeBME280(node_object_t *handle);
void hardwareProbeOPCN3(node_object_t *handle);
void hardwareProbeADS1115(node_object_t *handle);
void hardwareProbeSPS30(node_object_t *handle);
void hardwareProbeSHT40(node_object_t *handle);
void hardwareProbeSCD40(node_object_t *handle);
void hardwareProbeSGP41(node_object_t *handle);

void hardwareSleep(node_object_t *handle);

#endif