/*
#include "stdio.h"
#include "SoftwareSerial.h"
#include "hardware.h"
#include "fis3031.h"

SoftwareSerial swSer1;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;
static byte ndx = 0;
char endMarker = '\n';
char rc;

int metanInit() {
    swSer1.begin(9600, SWSERIAL_8N1, FIS3031_RX_PIN, FIS3031_TX_PIN, false, 256);
    gpio_set_direction(FIS3031_REQUEST_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(FIS3031_RESET_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(FIS3031_RESET_PIN, 1);
    gpio_set_level(FIS3031_RESET_PIN, 0);
    delay(200);
    gpio_set_level(FIS3031_RESET_PIN, 1);
    gpio_set_level(FIS3031_REQUEST_PIN, 0);
    return 0;
}

void metanUpdate(node_object_t *handle) {
    gpio_set_level(FIS3031_REQUEST_PIN, 1);
    delay(200);
    while (swSer1.available() > 0 && newData == false) {
        //Serial.print(".\r\n");
        rc = swSer1.read();
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            Serial.print(rc);
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
    
    if (newData == true) {
        Serial.print("RECEIVED: ");
        Serial.println(receivedChars);
        strcpy(tempChars, receivedChars);
        float metan = 1.23;
        int temp = 7;
        char * strtokIndx;
        strtokIndx = strtok(tempChars,"\t");
        metan = atof(strtokIndx);
        strtokIndx = strtok(NULL, "\t");
        temp = atoi(strtokIndx);
        handle->data.metan = (uint16_t)(metan * 100);
        handle->data.metan_temp = (int16_t)(temp * 100);
        newData = false;
    }
    gpio_set_level(FIS3031_REQUEST_PIN, 0);
}

*/