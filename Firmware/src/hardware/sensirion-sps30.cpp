#include "sps30.h"
#include "hardware.h"

#include "sensirion-sps30.h"
#include "HardwareSerial.h"

/////////////////////////////////////////////////////////////
/*define communication channel to use for SPS30
 valid options:
 *   I2C_COMMS              use I2C communication
 *   SOFTWARE_SERIAL        Arduino variants (NOTE)
 *   SERIALPORT             ONLY IF there is NO monitor attached
 *   SERIALPORT1            Arduino MEGA2560, Due. Sparkfun ESP32 Thing : MUST define new pins as defaults are used for flash memory)
 *   SERIALPORT2            Arduino MEGA2560, Due and ESP32
 *   SERIALPORT3            Arduino MEGA2560, Due only for now

 * NOTE: Softserial has been left in as an option, but as the SPS30 is only
 * working on 115K the connection will probably NOT work on any device. */
/////////////////////////////////////////////////////////////
#define SP30_COMMS I2C_COMMS

/////////////////////////////////////////////////////////////
/* define RX and TX pin for softserial and Serial1 on ESP32
 * can be set to zero if not applicable / needed           */
/////////////////////////////////////////////////////////////
#define TX_PIN 0
#define RX_PIN 0

/////////////////////////////////////////////////////////////
/* define driver debug
 * 0 : no messages
 * 1 : request sending and receiving
 * 2 : request sending and receiving + show protocol errors */
 //////////////////////////////////////////////////////////////
#define DEBUG 0

//////////////////////////////////////////////////////////////
/* define what you want to see plotted
 *  MASS 1: include MASS information     0: exclude
 *  NUM  1: include NUM information      0: exclude
 *  AVG  1: include average size         0: exclude
 *
 * In case communication is I2C see remark in top of example */
//////////////////////////////////////////////////////////////
#define MASS 1
#define NUM 1
#define AVG 0

//////////////////////////////////////////////////////////////
/* skip first x readings as they might be wrong
 * due to start-up */
//////////////////////////////////////////////////////////////
#define SKIPFIRST 5

///////////////////////////////////////////////////////////////
/////////// NO CHANGES BEYOND THIS POINT NEEDED ///////////////
///////////////////////////////////////////////////////////////

// function prototypes (sometimes the pre-processor does not create prototypes themself on ESPxx)
void ErrtoMess(char *mess, uint8_t r);
void Errorloop(char *mess, uint8_t r);

// create constructors
SPS30 sps30;

/**
 * @brief : read and display device info
 */
void GetDeviceInfo(node_object_t *handle) 
{
  char buf[32];
  uint8_t ret;
  SPS30_version v;

  //try to read serial number
  ret = sps30.GetSerialNumber(buf, 32);
  if (ret == SPS30_ERR_OK) {
    Serial.print(F("Serial number : "));
    if(strlen(buf) > 0) {
      Serial.println(buf);
      handle->data.sps30_ser_num[0] = 0x00;
      strcat(handle->data.sps30_ser_num, buf);
    } else {
      Serial.println(F("not available"));
    }
  }
  else
    ErrtoMess((char *) "could not get serial number", ret);

  // try to get product name
  ret = sps30.GetProductName(buf, 32);
  if (ret == SPS30_ERR_OK)  {
    Serial.print(F("Product name  : "));

    if(strlen(buf) > 0){
      Serial.println(buf);
      //handle->data.sps30_prod_name[0] = 0x00;
      strcat(handle->data.sps30_prod_name, buf);
      handle->data.sps30_prod_name[8] = 0x00;
    }
    else{
       Serial.println(F("not available"));
    }
  }
  else
    ErrtoMess((char *) "could not get product name.", ret);

  // try to get version info
  ret = sps30.GetVersion(&v);
  if (ret != SPS30_ERR_OK) {
    Serial.println(F("Can not read version info"));
    return;
  }

  Serial.print(F("Firmware level: "));  Serial.print(v.major);
  Serial.print("."); Serial.println(v.minor);

  char subversion[4];
  sprintf(handle->data.sps30_fw_ver, "%x", v.major);
  sprintf(subversion, "%x", v.minor);
  strcat(handle->data.sps30_fw_ver, ".");
  strcat(handle->data.sps30_fw_ver, subversion);

  if (SP30_COMMS != I2C_COMMS) {
    Serial.print(F("Hardware level: ")); Serial.println(v.HW_version);

    Serial.print(F("SHDLC protocol: ")); Serial.print(v.SHDLC_major);
    Serial.print("."); Serial.println(v.SHDLC_minor);
  }

  Serial.print(F("Library level : "));  Serial.print(v.DRV_major);
  Serial.print(".");  Serial.println(v.DRV_minor);
  //sps30.reset()
}

void probeSPS30(node_object_t *handle) {
  // Begin communication channel;
  sps30.begin(SP30_COMMS);
  if (sps30.probe())
  {
    // read device info
    GetDeviceInfo(handle);
    handle->probes.SPS30_present = true;
  } else {
    handle->probes.SPS30_present = false;
  }
}

int sps30Init(node_object_t *handle) {
   //Serial.begin(115200);

  //Serial.println(F("Trying to connect"));

  // set driver debug level
  sps30.EnableDebugging(DEBUG);

  // set pins to use for softserial and Serial1 on ESP32
  if (TX_PIN != 0 && RX_PIN != 0) sps30.SetSerialPin(RX_PIN,TX_PIN);

  // Begin communication channel;
  if (! sps30.begin(SP30_COMMS))
    Errorloop((char *) "could not initialize communication channel.", 0);

  // check for SPS30 connection
  if (! sps30.probe()) Errorloop((char *) "could not probe / connect with SPS30.", 0);
  //else  Serial.println(F("Detected SPS30."));

  // reset SPS30 connection
  if (! sps30.reset()) Errorloop((char *) "could not reset.", 0);

  // read device info
  GetDeviceInfo(handle);

  // start measurement
  if (sps30.start()) Serial.println(F("Measurement started"));
  //else Errorloop((char *) "Could NOT start measurement", 0);

  if (SP30_COMMS == I2C_COMMS) {
    if (sps30.I2C_expect() == 4)
      Serial.println(F(" !!! Due to I2C buffersize only the SPS30 MASS concentration is available !!! \n"));
  }

  return 0;
}


int sps30Deinit(node_object_t *handle) {
  
  // set driver debug level
  sps30.stop();

  return 0;
}


/**
 * @brief : read and display all values
 */
int sps30Update(node_object_t *handle) 
{
  //static bool header = true;
  uint8_t ret, error_cnt = 0;
  struct sps_values val;
  //static 
  uint8_t s_kip = SKIPFIRST;

  // MUST BE HERE? 
  // Begin communication channel;
  if (! sps30.begin(SP30_COMMS))
    Errorloop((char *) "could not initialize communication channel.", 0);
    // start measurement
  sps30.reset();
  delay(400);
  sps30.start();
  //delay(4000);
  ret = sps30.GetValues(&val);

  // loop to get data
  do {
    
    if (s_kip > 0) {
      Serial.println();
      Serial.print(s_kip);  
      Serial.println(" runs to skip");
      s_kip--;
    }

    delay(30000); // 2000 malo, 60 000 stabilni
    
    ret = sps30.GetValues(&val);

    // if other error
    if(ret != SPS30_ERR_OK) {
      delay(5000);
    }
    
    
  } while (ret != SPS30_ERR_OK && s_kip > 0);

  if (ret == SPS30_ERR_OK) {
    handle->data.sps30_PM1 = (uint16_t)(val.MassPM1 * 100);
    handle->data.sps30_PM2 = (uint16_t)(val.MassPM2 * 100);
    handle->data.sps30_PM4 = (uint16_t)(val.MassPM4 * 100);
    handle->data.sps30_PM10 = (uint16_t)(val.MassPM10 * 100);
    handle->data.sps30_NumPM0 = (uint16_t)(val.MassPM1 * 100);
    handle->data.sps30_NumPM1 = (uint16_t)(val.MassPM2 * 100);
    handle->data.sps30_NumPM2 = (uint16_t)(val.MassPM4 * 100);
    handle->data.sps30_NumPM4 = (uint16_t)(val.MassPM10 * 100);
    handle->data.sps30_NumPM10 = (uint16_t)(val.MassPM10 * 100);
    handle->data.sps30_PartSize = (uint16_t)(val.MassPM10 * 100);
  } else {
    //Serial.println("error reading values from sps30");
    handle->data.sps30_PM1 = 0;
    handle->data.sps30_PM2 = 0;
    handle->data.sps30_PM4 = 0;
    handle->data.sps30_PM10 = (uint16_t)10;
    handle->data.sps30_NumPM0 = 0;
    handle->data.sps30_NumPM1 = 0;
    handle->data.sps30_NumPM2 = 0;
    handle->data.sps30_NumPM4 = 0;
    handle->data.sps30_NumPM10 = 10;
    handle->data.sps30_PartSize = 0;
  }
  return(true);
}

/**
 *  @brief : continued loop after fatal error
 *  @param mess : message to display
 *  @param r : error code
 *
 *  if r is zero, it will only display the message
 */
void Errorloop(char *mess, uint8_t r)
{
  if (r) ErrtoMess(mess, r);
  else Serial.println(mess);
  Serial.println(F("Program on hold"));
  //for(;;) delay(100000);
}

/**
 *  @brief : display error message
 *  @param mess : message to display
 *  @param r : error code
 *
 */
void ErrtoMess(char *mess, uint8_t r)
{
  char buf[80];

  Serial.print(mess);

  sps30.GetErrDescription(r, buf, 80);
  Serial.println(buf);
}