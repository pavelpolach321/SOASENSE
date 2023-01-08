#include "stdio.h"
#include "hardware.h"
#include <Adafruit_SPIDevice.h>
#include "opcn3.h"

Adafruit_SPIDevice spi_dev = Adafruit_SPIDevice(SPIDEVICE_CS, \
                                                SPIDEVICE_SCK, \
                                                SPIDEVICE_MISO, \
                                                SPIDEVICE_MOSI, \
                                                SPIDEVICE_SPEED, \
                                                SPI_BITORDER_MSBFIRST, \
                                                SPI_MODE1);


void opcn3ReadFwVer(node_object_t *handle){
  uint8_t buffer[OPCN3_BUFFER_SIZE];
  buffer[0] = 0x12;
  spi_dev.write_and_read(buffer, 1); 
  //Serial.print("  Byte 0: 0x"); Serial.print(buffer[0], HEX); 
  delay(10);
  buffer[0] = 0x12;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  for (uint8_t i=0; i<2; i++) {
    delayMicroseconds(10);
    spi_dev.read((uint8_t*)&buffer[i], 1, 0x12);
  }

  char version[8];
  char subversion[4];
  sprintf(version, "%x", buffer[0]);
  sprintf(subversion, "%x", buffer[1]);
  strcat(version, ".");
  strcat(version, subversion);
  memcpy(handle->data.opcn3_fw_ver, version, 8);
}


void opcn3ReadSerNum(node_object_t *handle){

  uint8_t buffer[OPCN3_BUFFER_SIZE];
  buffer[0] = 0x10;
  spi_dev.write_and_read(buffer, 1); 
  //Serial.print("  Byte 0: 0x"); Serial.print(buffer[0], HEX); 
  delay(10);
  buffer[0] = 0x10;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  for (uint8_t i=0; i<60; i++) {
    delayMicroseconds(10);
    spi_dev.read((uint8_t*)&buffer[i], 1, 0x10);
  }

  if(1){//!memcpy(buffer, "OPC-N3 ", 6)){
    for(int i=0; i<9; i++){
      handle->data.opcn3_ser_num[i] = buffer[i+7];
    }
  }
  handle->data.opcn3_ser_num[9] = '\0';
}


int opcn3Init(node_object_t *handle){
  delay(10000);
  uint8_t buffer[OPCN3_BUFFER_SIZE];
  buffer[0] = 0x3F;
  spi_dev.write_and_read(buffer, 1);  
  delay(10);
  buffer[0] = 0x3F;
  spi_dev.write_and_read(buffer, 1); 

  for (uint8_t i=0; i<60; i++) {
    delayMicroseconds(10);
    spi_dev.read((uint8_t*)&buffer[i], 1, 0x3F);
  }
  if(!memcmp(buffer, "OPC-N3", 6)){
    opcn3_reset();
    opcn3ReadFwVer(handle);
    opcn3ReadSerNum(handle);
    opcn3PowerOn();
    return 1;
  } else {
    return 0;
  }
}



void probeOPCN3(node_object_t *handle){
  if (!spi_dev.begin()) {
    handle->probes.OPCN3_present = false;
    return;
  } else {
    if(opcn3Init(handle)){
      handle->probes.OPCN3_present = true;
      handle->probes.OPCN3_send_hist = true;
    }
  }
}



void opcn3_reset(){
  uint8_t buffer[1];
  //Serial.println("OPCN3 reset");
  buffer[0] = 0x06;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(15);
  buffer[0] = 0x06;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.println(buffer[0], HEX);
  delay(10000);
}


void opcn3PowerOn(void) {
  uint8_t buffer[1];
  //Serial.println("Laser switching ON");
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(10);
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.print(buffer[0], HEX);
  delayMicroseconds(10);
  buffer[0] = 0x07;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 3: 0x"); Serial.print(buffer[0], HEX);
  //Serial.println("\r\nLaser switched ON\r\n");
  //Serial.println("Fan switching ON");
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(10);
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.print(buffer[0], HEX);
  delayMicroseconds(10);
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 3: 0x"); Serial.print(buffer[0], HEX);
  delay(1500);
  //Serial.println("\r\nFan switched ON\r\n");
}

void opcn3PowerOff(void) {
  uint8_t buffer[1];
  //Serial.println("Laser switching OFF");
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(10);
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.print(buffer[0], HEX);
  delayMicroseconds(10);
  buffer[0] = 0x06;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 3: 0x"); Serial.print(buffer[0], HEX);
  //Serial.println("\r\nLaser switched OFF\r\n");
  //Serial.println("Fan switching OFF");
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(10);
  buffer[0] = 0x03;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.print(buffer[0], HEX);
  buffer[0] = 0x02;
  delayMicroseconds(10);
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 3: 0x"); Serial.print(buffer[0], HEX);
  delay(1000);
  //Serial.println("\r\nFan switched OFF\r\n");
}

float calculateFloat(uint32_t val) {
  // Return an IEEE754 float from an array of 4 bytes
  union u_tag {
    uint32_t b;
    float val;
  } u;
  u.b = val;
  return u.val;
}

void opcn3ReadPm(node_object_t *handle) {
  uint8_t buffer[OPCN3_BUFFER_SIZE];
  //Serial.println("\r\nReading raw pm data:");
  buffer[0] = 0x32;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(10);
  buffer[0] = 0x32;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.print(buffer[0], HEX);
  for (uint8_t i=0; i<14; i++) {
    delayMicroseconds(10);
    spi_dev.read((uint8_t*)&buffer[i], 1, 0x32);
  }
  
  //for (int i=0;i<16; i++){
  //  Serial.print(" ");
  //  Serial.print(buffer[i], 16);
  //}
  //Serial.println();
  //Serial.print("\r\n  Pm1: "); Serial.print(calculateFloat((buffer[0] << 0) | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24)));  Serial.print("\r\n");
  //Serial.print("  Pm2_5: ");   Serial.print(calculateFloat((buffer[4] << 0) | (buffer[5] << 8) | (buffer[6] << 16) | (buffer[7] << 24)));  Serial.print("\r\n");
  //Serial.print("  Pm10: ");    Serial.print(calculateFloat((buffer[8] << 0) | (buffer[9] << 8) | (buffer[10] << 16) | (buffer[11] << 24)));Serial.print("\r\n");
  //Serial.print("\r\n\r\n");
  handle->data.opcn3_PM1 =  (uint16_t)(calculateFloat((buffer[0] << 0) | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24)) * 100);
  handle->data.opcn3_PM2_5 = (uint16_t)(calculateFloat((buffer[4] << 0) | (buffer[5] << 8) | (buffer[6] << 16) | (buffer[7] << 24)) * 100);
  handle->data.opcn3_PM10 = (uint16_t)(calculateFloat((buffer[8] << 0) | (buffer[9] << 8) | (buffer[10] << 16) | (buffer[11] << 24)) * 100);
}

void opcn3ReadFlush(void) {
  uint8_t buffer[OPCN3_BUFFER_SIZE];
  //Serial.println("Flush reading raw pm data:");
  buffer[0] = 0x32;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(10);
  buffer[0] = 0x32;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.print(buffer[0], HEX);
  for (uint8_t i=0; i<14; i++) {
    delayMicroseconds(10);
    spi_dev.read(buffer, 1, 0x00);
  }
}


#define POLYNOMIAL 0xA001 //Generator polynomial for CRC
#define InitCRCval 0xFFFF //Initial CRC value
uint16_t opcn3Checksum(uint8_t buffer[]) {
  unsigned char _bit; // bit mask
  unsigned int crc = InitCRCval; // initialise calculated checksum
  unsigned char byteCtr; // byte counter
  // calculates 16-Bit checksum with given polynomial
  for(byteCtr = 0; byteCtr < 84; byteCtr++)
  {
    crc ^= (unsigned int)buffer[byteCtr];
    for(_bit = 0; _bit < 8; _bit++)
    {
      if (crc & 1) //if bit0 of crc is 1
      {
        crc >>= 1;
        crc ^= POLYNOMIAL;
      }
      else
        crc >>= 1;
    }
  }
  return crc;
}

void opcn3ReadAll(node_object_t *handle) {
  uint8_t buffer[88];
  //Serial.println("\r\nReading raw pm data:");
  buffer[0] = 0x30;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 1: 0x"); Serial.print(buffer[0], HEX);
  delay(50);
  buffer[0] = 0x30;
  spi_dev.write_and_read(buffer, 1);  
  //Serial.print("  Byte 2: 0x"); Serial.print(buffer[0], HEX);
  for (uint8_t i=0; i<86; i++) {
    delayMicroseconds(10);
    spi_dev.read((uint8_t*)&buffer[i], 1, 0x30);
  }
  
  //for (int i=0;i<86; i++){
  //  if (i%16 == 0)
  //    Serial.println();
  //  Serial.print("\t");
  //  Serial.print(buffer[i], 16);
  //}
  //Serial.println();
  //Serial.print("\r\n  Pm1: "); Serial.print(calculateFloat((buffer[0] << 0) | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24)));  Serial.print("\r\n");
  //Serial.print("  Pm2_5: ");   Serial.print(calculateFloat((buffer[4] << 0) | (buffer[5] << 8) | (buffer[6] << 16) | (buffer[7] << 24)));  Serial.print("\r\n");
  //Serial.print("  Pm10: ");    Serial.print(calculateFloat((buffer[8] << 0) | (buffer[9] << 8) | (buffer[10] << 16) | (buffer[11] << 24)));Serial.print("\r\n");
  //Serial.print("\r\n\r\n");
  
  uint16_t checksum_payload = (buffer[84]) | (buffer[85] << 8);
  uint16_t checksum_calculated = opcn3Checksum(buffer);

  //Serial.print("Checksum incomming: ");Serial.println(checksum_payload, 16);
  //Serial.print("Checksum calculated: ");Serial.println(opcn3Checksum(buffer), 16);

  if(checksum_payload == checksum_calculated){
    handle->data.opcn3_bin0 =  (buffer[0]  << 0) | (buffer[1]  << 8);
    handle->data.opcn3_bin1 =  (buffer[2]  << 0) | (buffer[3]  << 8);
    handle->data.opcn3_bin2 =  (buffer[4]  << 0) | (buffer[5]  << 8);
    handle->data.opcn3_bin3 =  (buffer[6]  << 0) | (buffer[7]  << 8);
    handle->data.opcn3_bin4 =  (buffer[8]  << 0) | (buffer[9]  << 8);
    handle->data.opcn3_bin5 =  (buffer[10] << 0) | (buffer[11] << 8);
    handle->data.opcn3_bin6 =  (buffer[12] << 0) | (buffer[13] << 8);
    handle->data.opcn3_bin7 =  (buffer[14] << 0) | (buffer[15] << 8);
    handle->data.opcn3_bin8 =  (buffer[16] << 0) | (buffer[17] << 8);
    handle->data.opcn3_bin9 =  (buffer[18] << 0) | (buffer[19] << 8);
    handle->data.opcn3_bin10 = (buffer[20] << 0) | (buffer[21] << 8);
    handle->data.opcn3_bin11 = (buffer[22] << 0) | (buffer[23] << 8);
    handle->data.opcn3_bin12 = (buffer[24] << 0) | (buffer[25] << 8);
    handle->data.opcn3_bin13 = (buffer[26] << 0) | (buffer[27] << 8);
    handle->data.opcn3_bin14 = (buffer[28] << 0) | (buffer[29] << 8);
    handle->data.opcn3_bin15 = (buffer[30] << 0) | (buffer[31] << 8);
    handle->data.opcn3_bin16 = (buffer[32] << 0) | (buffer[33] << 8);
    handle->data.opcn3_bin17 = (buffer[34] << 0) | (buffer[35] << 8);
    handle->data.opcn3_bin18 = (buffer[36] << 0) | (buffer[37] << 8);
    handle->data.opcn3_bin19 = (buffer[38] << 0) | (buffer[39] << 8);
    handle->data.opcn3_bin20 = (buffer[40] << 0) | (buffer[41] << 8);
    handle->data.opcn3_bin21 = (buffer[42] << 0) | (buffer[43] << 8);
    handle->data.opcn3_bin22 = (buffer[44] << 0) | (buffer[45] << 8);
    handle->data.opcn3_bin23 = (buffer[46] << 0) | (buffer[47] << 8);
    
    handle->data.opcn3_bin1_MToF = buffer[48];
    handle->data.opcn3_bin3_MToF = buffer[49];
    handle->data.opcn3_bin5_MToF = buffer[50];
    handle->data.opcn3_bin7_MToF = buffer[51];
  
    handle->data.opcn3_sampling_p = (buffer[52] << 0) | (buffer[53] << 8);
    handle->data.opcn3_flow_rate  = (buffer[54] << 0) | (buffer[55] << 8);
    
  
    handle->data.opcn3_t = (buffer[56] << 0) | (buffer[57] << 8);
    handle->data.opcn3_h = (buffer[58] << 0) | (buffer[59] << 8);
    
    handle->data.opcn3_PM1   = (uint16_t)(calculateFloat((buffer[60] << 0) | (buffer[61] << 8) | (buffer[62] << 16) | (buffer[63] << 24)) * 100);
    handle->data.opcn3_PM2_5 = (uint16_t)(calculateFloat((buffer[64] << 0) | (buffer[65] << 8) | (buffer[66] << 16) | (buffer[67] << 24)) * 100);
    handle->data.opcn3_PM10  = (uint16_t)(calculateFloat((buffer[68] << 0) | (buffer[69] << 8) | (buffer[70] << 16) | (buffer[71] << 24)) * 100);
  
    handle->data.opcn3_RC_glitch    = (buffer[72] << 0) | (buffer[73] << 8);
    handle->data.opcn3_RC_long      = (buffer[74] << 0) | (buffer[75] << 8);
    handle->data.opcn3_RC_ratio     = (buffer[76] << 0) | (buffer[77] << 8);
    handle->data.opcn3_out_of_range = (buffer[78] << 0) | (buffer[79] << 8);
  
    handle->data.opcn3_fan_rev    = (buffer[80] << 0) | (buffer[81] << 8);
    handle->data.opcn3_laser_stat = (buffer[82] << 0) | (buffer[83] << 8);
  }
}


