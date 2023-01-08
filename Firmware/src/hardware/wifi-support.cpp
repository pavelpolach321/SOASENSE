#include "WiFi.h"
#include "wifi-certificate.h"
#include <AsyncMqttClient.h>
//#include "../../hardware_setup/certificate.h"
#include "../../node_setup/setup.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "HttpsOTAUpdate.h"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "ui/ui.h"
#include "hardware.h"

#define MQTT_PUB_TOPIC_BEGINNING "ppz/devices/"
#define MQTT_PUB_TOPIC_END       "/up"

unsigned long previousMillis = 0;                                                // Stores last time temperature was published
const long values_interval = 1000 * WIFI_SECONDS_TO_LOOP_DEFAULT;                // Interval at which to publish sensor readings
const long fota_loops = (1000 * 60 * WIFI_MINUTES_TO_FWUPDATE_CHECK_DEFAULT) / values_interval ;   // Interval at which to check fw update
const long reconfigure_loops = 1000 * WIFI_MINUTES_TO_RECONFIGURE;            // Interval at which to check sensors presence on the board
const long metadata_loops = 1000 * WIFI_MINUTES_TO_SENSORS_METADATA_DEFAULT;  // Interval at which to send metadata
uint32_t update_index = 0;
uint32_t reconfigure_index = 0;
uint32_t metadata_index = 0;

////////////////////////////////
///   MQTT starts here       ///
////////////////////////////////
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void delete_wifi_credentials(){
  WiFiManager wm;
  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  wm.resetSettings();
  return;
}

void connectToWifi() {
  WiFiManager wm;
  bool res;
  uiWifiBegin();
  res = wm.setHostname("SOASENSE_LAB");
  if(!res) {
    Serial.println("Failed to set hostname");
    // ESP.restart();
  } 
  res = wm.autoConnect(AP_NAME, AP_PASSWORD); // password protected ap
  if(!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
    uiWifiConnected();
  }
}

void connectToMqtt() {
  Serial.println("[MQQT] Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  //Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("\n[WIFI] WiFi connected, IP: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
    default:
      ;//Serial.printf("[WiFi-event] event: %d\n", event);
  }
}

void send_metadata_1(node_object_t *handle){

    StaticJsonDocument<1000> doc;
    char json_to_send[1000];
    char version[10];
    char subversion[4];
    sprintf(version, "%d", FW_VERSION);
    sprintf(subversion, "%d", NODE_SUBVERSION);
    strcat(version, ".");
    strcat(version, subversion);

    doc["fw_version"] = version;
    doc["opcn3_fw_ver"] = handle->data.opcn3_fw_ver;
    doc["opcn3_ser_num"] = handle->data.opcn3_ser_num;//handle->data.sps30_ser_num;
    doc["sps30_prod_name"] = handle->data.sps30_prod_name;//handle->data.sps30_prod_name;
    doc["sps30_fw_ver"] = handle->data.sps30_fw_ver;
    doc["sps30_ser_num"] = handle->data.sps30_ser_num;
    
    serializeJsonPretty(doc, json_to_send);
    //Serial.println("[LOOP] Metadata to send:");Serial.println(json_to_send);
    char publish_string[128];
    publish_string[0] = '\0';
    strcat(publish_string, MQTT_PUB_TOPIC_BEGINNING);
    strcat(publish_string, handle->provision.device_name);
    strcat(publish_string, MQTT_PUB_TOPIC_END);
    uint16_t packetIdPub1 = mqttClient.publish(publish_string , 1, true, String(json_to_send).c_str());                            
    Serial.printf("[LOOP] Publishing on topic %s at QoS 1, packetId: ", publish_string);
    Serial.println(packetIdPub1);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("[MQQT] Connected to MQTT, FIX password and id access.");
  Serial.print("[MQQT] Session present: ");
  Serial.println(sessionPresent);
    send_metadata_1(&node_object);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("[MQQT] Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

/*void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}
void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}*/

void onMqttPublish(uint16_t packetId) {
  Serial.println("[MQQT] Publish acknowledged.");
  Serial.print("[MQQT]   packetId: ");
  Serial.println(packetId);
}

////////////////////////////////
///   MQTT ends here         ///
////////////////////////////////

////////////////////////////////
///   FOTA starts here       ///
////////////////////////////////
static HttpsOTAStatus_t otastatus;

void HttpEvent(HttpEvent_t *event)
{
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            Serial.println("[FOTA] Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            Serial.println("[FOTA] Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            Serial.println("[FOTA] Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            Serial.printf("[FOTA] Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            break;
        case HTTP_EVENT_ON_FINISH:
            Serial.println("[FOTA] Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
            Serial.println("[FOTA] Http Event Disconnected");
            break;
    }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  String payload = "{}"; 
  if (httpResponseCode>0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("[FOTA] HTTP request Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  return payload;
}

//#include <Arduino_JSON.h>
bool update_pending()
{
  Serial.println("[FOTA] Getting firmware metadata");
  String version_file = httpGETRequest(version_file_url);
  StaticJsonDocument<40> doc;
  deserializeJson(doc, version_file);
  uint8_t version = doc["version"];
  uint8_t subversion = doc["subversion"];

  Serial.print("[FOTA] Local fw version: ");
  Serial.print(FW_VERSION);
  Serial.print(".");
  Serial.println(NODE_SUBVERSION);
  Serial.print("[FOTA]  Available firmware version: ");
  Serial.print(version);
  Serial.print(".");
  Serial.println(subversion);

  if((version > FW_VERSION) || (subversion > NODE_SUBVERSION))
  {
    return true;
  } else {
    Serial.println("[FOTA] Device is up to date");
    return false;
  }
}

int start_update()
{
  HttpsOTA.onHttpEvent(HttpEvent);
  Serial.println("[FOTA] Starting download");
  HttpsOTA.begin(url, server_certificate); 
  while(1) {
    //Serial.print(".");
    otastatus = HttpsOTA.status();
    if(otastatus == HTTPS_OTA_SUCCESS) {  
      Serial.println("[FOTA] Firmware written successfully. Rebooting now\r\n\r\n");
      ESP.restart();
    } else if(otastatus == HTTPS_OTA_FAIL) { 
      Serial.println("[FOTA] ERROR, rebooting now\r\n\r\n");
      ESP.restart();
    }
  }
}

////////////////////////////////
///   FOTA ends here         ///
////////////////////////////////

void sendValues(node_object_t *handle){
    StaticJsonDocument<2500> doc;
    char json_to_send[2500];
    if (handle->probes.counter) {
          doc["counter"] = handle->data.counter;
    }
    if (handle->probes.board_voltages) {
        doc["board_3_5V"] = ((float)handle->data.board_3_5V)/100;
        doc["board_5V"] = ((float)handle->data.board_5V)/100;
        doc["board_batt"] = ((float)handle->data.board_batt)/100;
    }
    if (handle->probes.BME280_present) {
        doc["bme280_t"] = ((float)handle->data.bme280_t)/100;
        doc["bme280_p"] = handle->data.bme280_p;
        doc["bme280_h"] = ((float)handle->data.bme280_h)/100;
    }
    if (handle->probes.SPS30_present) {
        doc["sps30_PM1"] = ((float)handle->data.sps30_PM1)/100;
        doc["sps30_PM2"] = ((float)handle->data.sps30_PM2)/100;
        doc["sps30_PM4"] = ((float)handle->data.sps30_PM4)/100;
        doc["sps30_PM10"] = ((float)handle->data.sps30_PM10)/100;
        doc["sps30_NumPM0"] = ((float)handle->data.sps30_NumPM0)/100;
        doc["sps30_NumPM1"] = ((float)handle->data.sps30_NumPM1)/100;
        doc["sps30_NumPM2"] = ((float)handle->data.sps30_NumPM2)/100;
        doc["sps30_NumPM4"] = ((float)handle->data.sps30_NumPM4)/100;
        doc["sps30_NumPM10"] = ((float)handle->data.sps30_NumPM10)/100;
        doc["sps30_PartSize"] = ((float)handle->data.sps30_PartSize)/100;
    }
    if (handle->probes.OPCN3_present) {
        doc["opcn3_PM1"] = ((float)handle->data.opcn3_PM1)/100;
        doc["opcn3_PM2_5"] = ((float)handle->data.opcn3_PM2_5)/100;
        doc["opcn3_PM10"] = ((float)handle->data.opcn3_PM10)/100;
    }
    if (handle->probes.OPCN3_send_hist) {
        doc["opcn3_bin0"] = handle->data.opcn3_bin0;
        doc["opcn3_bin1"] = handle->data.opcn3_bin1;
        doc["opcn3_bin2"] = handle->data.opcn3_bin2;
        doc["opcn3_bin3"] = handle->data.opcn3_bin3;
        doc["opcn3_bin4"] = handle->data.opcn3_bin4;
        doc["opcn3_bin5"] = handle->data.opcn3_bin5;
        doc["opcn3_bin6"] = handle->data.opcn3_bin6;
        doc["opcn3_bin7"] = handle->data.opcn3_bin7;
        doc["opcn3_bin8"] = handle->data.opcn3_bin8;
        doc["opcn3_bin9"] = handle->data.opcn3_bin9;
        doc["opcn3_bin10"] = handle->data.opcn3_bin10;
        doc["opcn3_bin11"] = handle->data.opcn3_bin11;
        doc["opcn3_bin12"] = handle->data.opcn3_bin12;
        doc["opcn3_bin13"] = handle->data.opcn3_bin13;
        doc["opcn3_bin14"] = handle->data.opcn3_bin14;
        doc["opcn3_bin15"] = handle->data.opcn3_bin15;
        doc["opcn3_bin16"] = handle->data.opcn3_bin16;
        doc["opcn3_bin17"] = handle->data.opcn3_bin17;
        doc["opcn3_bin18"] = handle->data.opcn3_bin18;
        doc["opcn3_bin19"] = handle->data.opcn3_bin19;
        doc["opcn3_bin20"] = handle->data.opcn3_bin20;
        doc["opcn3_bin21"] = handle->data.opcn3_bin21;
        doc["opcn3_bin22"] = handle->data.opcn3_bin22;
        doc["opcn3_bin23"] = handle->data.opcn3_bin23;

        doc["opcn3_bin1_MToF"] = handle->data.opcn3_bin1_MToF;
        doc["opcn3_bin3_MToF"] = handle->data.opcn3_bin3_MToF;
        doc["opcn3_bin5_MToF"] = handle->data.opcn3_bin5_MToF;
        doc["opcn3_bin7_MToF"] = handle->data.opcn3_bin7_MToF;

        doc["opcn3_sampling_p"] = handle->data.opcn3_sampling_p;
        doc["opcn3_flow_rate"] = handle->data.opcn3_flow_rate;
        doc["opcn3_t"] = handle->data.opcn3_t;
        doc["opcn3_h"] = handle->data.opcn3_h;

        doc["opcn3_RC_glitch"] = handle->data.opcn3_RC_glitch;
        doc["opcn3_RC_long"] = handle->data.opcn3_RC_long;
        doc["opcn3_RC_ratio"] = handle->data.opcn3_RC_ratio;
        doc["opcn3_RC_out_of_range"] = handle->data.opcn3_out_of_range;
        doc["opcn3_fan_rev"] = handle->data.opcn3_fan_rev;
        doc["opcn3_laser_stat"] = handle->data.opcn3_laser_stat;
    }
    if (handle->probes.ADS1115_1_present) {
        doc["AD_socket_1_1"] = ((float)handle->data.AD_socket_1_1)/100;
        doc["AD_socket_1_2"] = ((float)handle->data.AD_socket_1_2)/100;
        doc["AD_socket_2_1"] = ((float)handle->data.AD_socket_2_1)/100;
        doc["AD_socket_2_2"] = ((float)handle->data.AD_socket_2_2)/100;
    }
    if (handle->probes.ADS1115_2_present) {
        doc["AD_socket_3_1"] = ((float)handle->data.AD_socket_3_1)/100;
        doc["AD_socket_3_2"] = ((float)handle->data.AD_socket_3_2)/100;
        doc["AD_socket_4_1"] = ((float)handle->data.AD_socket_4_1)/100;
        doc["AD_socket_4_2"] = ((float)handle->data.AD_socket_4_2)/100;
    }
    serializeJsonPretty(doc, json_to_send);
    //Serial.print("[LOOP] Sizeof(json_to_send): ");
    //Serial.println(sizeof(json_to_send));
    char publish_string[128];
    publish_string[0] = '\0';
    strcat(publish_string, MQTT_PUB_TOPIC_BEGINNING);
    strcat(publish_string, handle->provision.device_name);
    strcat(publish_string, MQTT_PUB_TOPIC_END);

    //Serial.println("[LOOP] Pretty json to send:");Serial.println(json_to_send);
    uint16_t packetIdPub1 = mqttClient.publish(publish_string, 1, true, String(json_to_send).c_str());                            
    Serial.printf("[LOOP] Publishing on topic %s at QoS 1, packetId: ", publish_string);
    Serial.println(packetIdPub1);
}

void wifimode_setup()
{
  Serial.begin(115200);
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  WiFi.onEvent(WiFiEvent);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(mqtt_server, mqtt_port);
  // If your broker requires authentication (username and password), set them below
  uiPrintHeader();
  //mqttClient.setCredentials("init_username", "init_password");
  connectToWifi();
}



void wifimode_loop(node_object_t *handle){

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= values_interval) {
    hardwareUpdateData(&node_object);

    previousMillis = currentMillis;
    update_index++;
    reconfigure_index++;
    metadata_index++;

    uiDisplayLoop(&node_object, \
                 WIFI_LOOPS_TO_SEND_METADATA - metadata_index, \
                 WIFI_LOOPS_TO_RECONFIGURE - reconfigure_index, \
                 WIFI_LOOPS_TO_RUN_FWUPDATE_CHECK - update_index);

    if(update_index == WIFI_LOOPS_TO_RUN_FWUPDATE_CHECK)
    {
      update_index = 0;
      if(update_pending()) {
        start_update();
      }
    }

    if (metadata_index == WIFI_LOOPS_TO_SEND_METADATA)
    {
      metadata_index = 0;
      send_metadata_1(&node_object);
    }

    if (reconfigure_index == WIFI_LOOPS_TO_RECONFIGURE)
    {
      reconfigure_index = 0;
      //hardwareSetup(&node_object);
      //hardwareInit(&node_object);
    }

    Serial.println();

    uiSerialLoop(&node_object);
    sendValues(&node_object);
  }
}