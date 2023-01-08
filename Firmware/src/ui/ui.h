#ifndef UI_H
#define UI_H

#include "hardware/hardware.h"

#define ROW_0             0
#define ROW_1             1
#define ROW_2             2
#define ROW_3             3
#define ROW_4             4
#define ROW_5             5
#define ROW_6             6
#define ROW_7             7    
#define HEADER_ROW        ROW_0
#define DEVICEID_ROW      ROW_1
#define INTERVAL_ROW      ROW_2
#define TIME_ROW          ROW_4
#define EVENT_ROW         ROW_5
#define STATUS_ROW        ROW_6
#define FRMCNTRS_ROW      ROW_7
#define COL_0             0
#define COL_1             1
#define COL_2             2
#define COL_3             3
#define COL_4             4
#define COL_5             5
#define COL_6             6
#define COL_7             7
#define COL_8             8
#define COL_9             9
#define COL_10            10
#define COL_11            11
#define COL_12            12
#define COL_13            13
#define COL_14            14
#define COL_15            15
#define COL_16            16
#define COL_17            17
#define COL_18            18
#define ABPMODE_COL       10
#define CLMICSYMBOL_COL   14
#define TXSYMBOL_COL      15


enum class InitType { Hardware, PostInitSerial };

void uiEraseCredentialsAndWait();
void initOutputs();
void uiWifiBegin();
void uiWifiConnected();
void uiPrintHeader();
void uiDisplayHeader(node_object_t *handle);
void uiDisplayProvision();
void uiSerialProvision();
void uiDisplayHwInit();
void uiPrintProvision(node_object_t *handle);

void uiDisplayLoop(node_object_t *handle, uint16_t metadata, uint16_t reconfig, uint16_t update);
void uiSerialLoop(node_object_t *handle);
void dbg(char mesage[]);

#endif