#include "main.h"
#include "hardware/hardware.h"
#include "hardware/wifi-support.h"


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


void setup() 
{
    initOutputs();

    hardwareHandleEEPROM(&node_object);

    hardwareGetMAC(&node_object);

    hardwareSetup(&node_object);

    if(CONNECTIVITY_ERASE_CREDENTIALS) {
        delete_wifi_credentials();
        uiEraseCredentialsAndWait();
        uiDisplayProvision();
        uiSerialProvision();
        while(1)
          ;
    } else if(DISPLAY_ON){
        uiDisplayHeader(&node_object);
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

    hardwareInit(&node_object);
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
