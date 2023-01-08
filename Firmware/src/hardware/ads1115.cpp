#include "stdio.h"
#include "hardware.h"
#include <Adafruit_ADS1X15.h>
#include "ads1115.h"

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

/*  #
    #           
###################
#  ___   #   ___  #
# | 2 |  #  | 4 | #
# |___|  #  |___| #
#  ___   #   ___  #
# | 1 |  #  | 3 | #
# |___|  #  |___| #
#        #        #
###################
*/

int socketsInit(uint8_t index) {//Adafruit_ADS1115 *ads){
  // ADS1115 16bit over i2c
  //Serial.println("Getting single-ended readings from AIN0..3");
  //Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads1.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads1.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads1.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads1.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads1.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads1.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  if(index==0){
    if (!ads1.begin(0x48)) {
      return 1;
    }
    ads1.setGain(GAIN_ONE);
  }

  if(index==1){
    if (!ads2.begin(0x49)) {
      return 1;
    }
    ads2.setGain(GAIN_ONE);
  }
  return 0;
}

void probeADS1115(node_object_t *handle) {
  if (!ads1.begin(ADS1115_1_ADDR)) {
    handle->probes.ADS1115_1_present = false;
  } else {
    handle->probes.ADS1115_1_present = true;
    ads1.setGain(GAIN_ONE);
  }

  if (!ads2.begin(ADS1115_2_ADDR)) {
    handle->probes.ADS1115_2_present = false;
  } else {
    handle->probes.ADS1115_2_present = true;
    ads2.setGain(GAIN_ONE);
  }
}

void socketsUpdate(node_object_t *handle, uint8_t index) {//}), Adafruit_ADS1115 *aads)
  if (index == 0) {
    handle->data.AD_socket_1_1 = (uint16_t)(100 * ads1.computeVolts(ads1.readADC_SingleEnded(0)));
    handle->data.AD_socket_1_2 = (uint16_t)(100 * ads1.computeVolts(ads1.readADC_SingleEnded(1)));
    handle->data.AD_socket_2_1 = (uint16_t)(100 * ads1.computeVolts(ads1.readADC_SingleEnded(2)));
    handle->data.AD_socket_2_2 = (uint16_t)(100 * ads1.computeVolts(ads1.readADC_SingleEnded(3)));
  }
  if (index == 1) {
    handle->data.AD_socket_3_1 = (uint16_t)(100 * ads2.computeVolts(ads2.readADC_SingleEnded(0)));
    handle->data.AD_socket_3_2 = (uint16_t)(100 * ads2.computeVolts(ads2.readADC_SingleEnded(1)));
    handle->data.AD_socket_4_1 = (uint16_t)(100 * ads2.computeVolts(ads2.readADC_SingleEnded(2)));
    handle->data.AD_socket_4_2 = (uint16_t)(100 * ads2.computeVolts(ads2.readADC_SingleEnded(3)));
  }
}
