#include "main.h"
#include "hardware/hardware.h"
#include "hardware/wifi-support.h"
#include "Arduino.h"


//        ██████  ▒█████   ▄▄▄        ██████ ▓█████  ███▄    █   ██████ ▓█████             
//      ▒██    ▒ ▒██▒  ██▒▒████▄    ▒██    ▒ ▓█   ▀  ██ ▀█   █ ▒██    ▒ ▓█   ▀             
//      ░ ▓██▄   ▒██░  ██▒▒██  ▀█▄  ░ ▓██▄   ▒███   ▓██  ▀█ ██▒░ ▓██▄   ▒███               
//        ▒   ██▒▒██   ██░░██▄▄▄▄██   ▒   ██▒▒▓█  ▄ ▓██▒  ▐▌██▒  ▒   ██▒▒▓█  ▄             
//      ▒██████▒▒░ ████▓▒░ ▓█   ▓██▒▒██████▒▒░▒████▒▒██░   ▓██░▒██████▒▒░▒████▒            
//      ▒ ▒▓▒ ▒ ░░ ▒░▒░▒░  ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒░   ▒ ▒ ▒ ▒▓▒ ▒ ░░░ ▒░ ░            
//      ░ ░▒  ░ ░  ░ ▒ ▒░   ▒   ▒▒ ░░ ░▒  ░ ░ ░ ░  ░░ ░░   ░ ▒░░ ░▒  ░ ░ ░ ░  ░            
//      ░  ░  ░  ░ ░ ░ ▒    ░   ▒   ░  ░  ░     ░      ░   ░ ░ ░  ░  ░     ░               
//            ░      ░ ░        ░  ░      ░     ░  ░         ░       ░     ░  ░            
//                                                                                         
//                      ███▄    █  ▒█████  ▓█████▄ ▓█████                                  
//                      ██ ▀█   █ ▒██▒  ██▒▒██▀ ██▌▓█   ▀                                  
//                     ▓██  ▀█ ██▒▒██░  ██▒░██   █▌▒███                                    
//                     ▓██▒  ▐▌██▒▒██   ██░░▓█▄   ▌▒▓█  ▄                                  
//                     ▒██░   ▓██░░ ████▓▒░░▒████▓ ░▒████▒                                 
//                     ░ ▒░   ▒ ▒ ░ ▒░▒░▒░  ▒▒▓  ▒ ░░ ▒░ ░                                 
//                     ░ ░░   ░ ▒░  ░ ▒ ▒░  ░ ▒  ▒  ░ ░  ░                                 
//                        ░   ░ ░ ░ ░ ░ ▒   ░ ░  ░    ░                                    
//                              ░     ░ ░     ░       ░  ░                                 
//                                          ░                                                 


node_object_t node_object;

#ifdef DO_DEEP_SLEEP
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60*3        /* Time ESP32 will go to sleep (in seconds) */
#endif


#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


void setup() 
{
    // FIXME stupid lilygo board 
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

    esp_task_wdt_init(WDT_TIMEOUT, true);  // enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);

    initOutputs();

    hardwareHandleEEPROM(&node_object);

    hardwareGetMAC(&node_object);

    hardwareSetup(&node_object);

    if((strcmp("LIGHT_01", node_object.provision.device_name) != 0) || \
       (strcmp("LIGHT_02", node_object.provision.device_name) != 0)){
      if(CONNECTIVITY_ERASE_CREDENTIALS) {
          delete_wifi_credentials();
          uiEraseCredentialsAndWait();
#ifdef USE_DISPLAY
          uiDisplayProvision();
#endif
          uiSerialProvision();
          while(1)
            ;
      } else if(DISPLAY_ON){
          uiDisplayHeader(&node_object);
      }
    }

    if(SERIAL_OUTPUT_ON && SERIAL_OUTPUT_DBG){
        uiPrintHeader();
    }

    if(MODE_USE_LORAWAN)
    {
        // TODO Lorawan stuff
        //initLmic();
    } else if(MODE_USE_WIFI)
    {
        node_object.setup.behaviour.seconds_to_loop = WIFI_SECONDS_TO_LOOP_DEFAULT;
        node_object.setup.behaviour.minutes_to_fwupdate_check = WIFI_MINUTES_TO_FWUPDATE_CHECK_DEFAULT;
        node_object.setup.behaviour.minutes_to_reconfigure = WIFI_MINUTES_TO_RECONFIGURE;
        node_object.setup.behaviour.minutes_to_sensors_metadata = WIFI_MINUTES_TO_SENSORS_METADATA_DEFAULT;
        wifimode_setup();
    }

    uiDisplayHwInit();

  if(strcmp("LIGHT_02", node_object.provision.device_name) != 0){
    hardwareInit(&node_object);
  }
#ifdef DO_DEEP_SLEEP
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
#endif
}

void loop() 
{
    if(MODE_USE_LORAWAN)
    {
        // LMIC: os_runloop_once();
    } else if(MODE_USE_WIFI)
    {
        wifimode_loop(&node_object);
    }
}
