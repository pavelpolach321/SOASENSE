#include "ui.h"
#include "hardware/hardware.h"
#include <Wire.h>
#include "U8x8lib.h"
#include "EasyLed.h"
#include "../node_setup/setup.h"

HardwareSerial& serial = Serial;
EasyLed led(LED_BUILTIN, EasyLed::ActiveLevel::Low);
U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ OLED_RST, /*scl*/ SCL, /*sda*/ SDA);

void initDisplay()
{
    display.begin();
    display.setFont(u8x8_font_victoriamedium8_r); 
}

bool initSerial(unsigned long speed = 115200, int16_t timeoutSeconds = 0)
{
    serial.begin(speed);
        return serial;
}

void initOutputs(){
    initDisplay();
    initSerial(MONITOR_SPEED, 0);  
}

void uiWifiBegin(){
    display.setCursor(COL_0, ROW_1);
    display.print(F("Connecting.."));
}

void uiWifiConnected(){
    display.setCursor(COL_0, ROW_2);
    display.print(F("Wifi connected!"));
}

void uiDisplayHwInit(){
    display.setCursor(COL_0, ROW_4);
    display.print(F("Booting sensors"));
}

void uiPrintHeader() {
    serial.print(F("############################################################################\n"));
    serial.print(F("#                                                                          #\n"));
    serial.print(F("#                         WELCOME in SOASENSEs, fw  "));
    serial.print(FW_VERSION);serial.print(".");serial.print(NODE_SUBVERSION);
    serial.print(F("                    #\n"));
    serial.print(F("#                                                                          #\n"));
    serial.print(F("############################################################################\n"));
    delay(100);
}

void uiDisplayHeader(node_object_t *handle) {
    //#ifdef USE_DISPLAY
    display.clear();
    display.setCursor(COL_0, ROW_0);
    display.print(F("id:"));
    display.setCursor(COL_4, ROW_0);
    display.print(F(handle->provision.device_name));
}

void uiEraseCredentialsAndWait(){
    display.clear();
    display.setCursor(COL_0, ROW_0);
    display.print(F("Clearing WIFI..."));
    delay(200);
    display.setCursor(COL_0, ROW_1);
    display.print(F("done"));
}

void uiDisplayProvision() {
    display.clear();
    display.setCursor(COL_0, ROW_0);
    display.print(F("ssid:"));
    display.setCursor(COL_5, ROW_0);
    display.print(F(AP_NAME));

    display.setCursor(COL_0, ROW_1);
    display.print(F("pass:"));
    display.setCursor(COL_6, ROW_1);
    display.print(F(AP_PASSWORD));

    display.setCursor(COL_0, ROW_2);
    display.print(F("MAC addr:"));

    display.setCursor(COL_0, ROW_4);
    display.print(node_object.mac_addr[0], 16);
    display.setCursor(2, ROW_4);
    display.print(node_object.mac_addr[1], 16);
    display.setCursor(4, ROW_4);
    display.print(node_object.mac_addr[2], 16);
    display.setCursor(6, ROW_4);
    display.print(node_object.mac_addr[3], 16);
    display.setCursor(8, ROW_4);
    display.print(node_object.mac_addr[4], 16);
    display.setCursor(10, ROW_4);
    display.print(node_object.mac_addr[5], 16);
}

void uiDisplayLoop(node_object_t *handle, uint16_t metadata, uint16_t reconfig, uint16_t update) {
    display.clear();
    display.setCursor(COL_0, ROW_0);
    display.print(F(handle->provision.device_name));
    display.setCursor(COL_0, ROW_1);
    display.print(F("[Running]"));

    display.setCursor(COL_0, ROW_3);
    display.print(F("Counter: "));
    display.setCursor(COL_10, ROW_3);
    display.print(handle->data.counter);

    display.setCursor(COL_0, ROW_4);
    display.print(F("Update: "));
    display.setCursor(COL_10, ROW_4);
    display.print(update);

    display.setCursor(COL_0, ROW_5);
    display.print(F("Reconfig: "));
    display.setCursor(COL_10, ROW_5);
    display.print(reconfig);

    display.setCursor(COL_0, ROW_6);
    display.print(F("Meta: "));
    display.setCursor(COL_10, ROW_6);
    display.print(metadata);
}


void uiPrintProvision(node_object_t *handle) {
    Serial.println("[PROVISION]");
    Serial.print("[name] "); Serial.println(handle->provision.device_name);    
    Serial.print("[deveui] ");
    for(int i = 0; i<8; i++){
        Serial.print(handle->provision.otaa_deveui[i], 16);
        Serial.print(" ");
    }
    Serial.println();
    Serial.print("[appeui] ");
    for(int i = 0; i<8; i++){
        Serial.print(handle->provision.otaa_appeui[i], 16);
        Serial.print(" ");
    }
    Serial.println();
    Serial.print("[appkey] ");
    for(int i = 0; i<16; i++){
        Serial.print(handle->provision.otaa_appkey[i], 16);
        Serial.print(" ");
    }
    Serial.println();
}

void uiSerialLoop(node_object_t *handle) {
    Serial.println();
    if (handle->probes.counter) {
        Serial.println("[------------------------------------------------------------------------------------------------------------------]");
        Serial.print("[DBG]        Counter:            "); Serial.println(handle->data.counter);//Serial.println(" \t[rounds]");
    }
    if (handle->probes.board_voltages) {
        Serial.println("[------------------------------------------------------------------------------------------------------------------]");
        Serial.print("[Voltages]  3v5, Vin:            "); Serial.print(((float)handle->data.board_3_5V / 100)); Serial.print(", ");
                                                        Serial.print(((float)handle->data.board_batt / 100)); Serial.println("\t[V]");
    }
    if (handle->probes.BME280_present) {
        Serial.println("[------------------------------------------------------------------------------------------------------------------]");
        Serial.print("[BME280]    temp, Hum, Pres:     "); Serial.print(((float)handle->data.bme280_t / 100)); Serial.print(" [*C],\t");
                                                          Serial.print(((float)handle->data.bme280_h / 100)); Serial.print(" [%],\t");
                                                          Serial.print((float)handle->data.bme280_p); Serial.println(" [hPa]");
    }
    if (handle->probes.SPS30_present) {
        Serial.println("[------------------------------------------------------------------------------------------------------------------]");
        Serial.print("[SPS-30]    sernum, fwver:       "); Serial.print(handle->data.sps30_ser_num);Serial.print(", "); 
                                                           Serial.println(handle->data.sps30_fw_ver);
        Serial.print("[SPS-30]    pm1, pm2, pm4, pm10: "); Serial.print(((float)handle->data.sps30_PM1 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.sps30_PM2 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.sps30_PM4 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.sps30_PM10 / 100)); Serial.println("\t [ug/m3]");
        Serial.print("[SPS-30]    Num PM0, PM1, PM2:   "); Serial.print(((float)handle->data.sps30_NumPM0 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.sps30_NumPM1 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.sps30_NumPM2 / 100)); Serial.println("\t[#]");
        Serial.print("[SPS-30]    Num PM4, PM10:       "); Serial.print(((float)handle->data.sps30_NumPM4 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.sps30_NumPM10 / 100)); Serial.println("\t[#]");
        Serial.print("[SPS-30]    PartSize:            "); Serial.print(((float)handle->data.sps30_PartSize / 100)); Serial.println("\t[?? ug/m3]");
    }
    if (handle->probes.OPCN3_present) {
        Serial.println("[------------------------------------------------------------------------------------------------------------------]");
        Serial.print("[OPC-N3]    ser num, fwver:      "); Serial.print(handle->data.opcn3_ser_num);Serial.print(", "); 
                                                           Serial.println(handle->data.opcn3_fw_ver);
        Serial.print("[OPC-N3]    PM1, PM2.5, PM10:    "); Serial.print(((float)handle->data.opcn3_PM1 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.opcn3_PM2_5 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.opcn3_PM10 / 100)); Serial.println(" [ug/m3]");
    }
    if (handle->probes.OPCN3_send_hist) {
        Serial.print("[OPC-N3]    bin0-7:              "); 
        Serial.print(handle->data.opcn3_bin0);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin1);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin2);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin3);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin4);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin5);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin6);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin7);
        Serial.print("\t");
        Serial.println();
        Serial.print("[OPC-N3]    bin8-15:             "); 
        Serial.print(handle->data.opcn3_bin8);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin9);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin10);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin11);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin12);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin13);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin14);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin15);
        Serial.print("\t");
        Serial.println();
        Serial.print("[OPC-N3]    bin16-23:            "); 
        Serial.print(handle->data.opcn3_bin16);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin17);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin18);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin19);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin20);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin21);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin22);
        Serial.print("\t");
        Serial.print(handle->data.opcn3_bin23);
        Serial.print("\t");
        Serial.println();
    }
    if (handle->probes.ADS1115_1_present) {
        Serial.println("[------------------------------------------------------------------------------------------------------------------]");
        Serial.print("[ADS1115]   Socket_1_1, 1_2:     "); Serial.print(((float)handle->data.AD_socket_1_1 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.AD_socket_1_2 / 100)); Serial.println("\t[V]");
        Serial.print("[ADS1115]   Socket_2_1, 2_2:     "); Serial.print(((float)handle->data.AD_socket_2_1 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.AD_socket_2_2 / 100)); Serial.println("\t[V]");
    }
    if (handle->probes.ADS1115_2_present) {
        Serial.println("[------------------------------------------------------------------------------------------------------------------]");
        Serial.print("[ADS1115-2] Socket_3_1, 3_2:     "); Serial.print(((float)handle->data.AD_socket_3_1 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.AD_socket_3_2 / 100)); Serial.println("\t[V]");
        Serial.print("[ADS1115-2] Socket_4_1, 4_2:     "); Serial.print(((float)handle->data.AD_socket_4_1 / 100)); Serial.print(", ");
                                                           Serial.print(((float)handle->data.AD_socket_4_2 / 100)); Serial.println("\t[V]");
    }
}

void uiSerialProvision() {
    serial.print("MAC ADDRESS: ");
    for(int i=0; i<6; i++){
        serial.print(node_object.mac_addr[i], 16);
        if(i<5){
          serial.print("-");
        }
    }
    serial.println();
}


void dbg(char message[]){
    serial.print("\r\n");
    serial.println(message);
    serial.print("\r\n");
}