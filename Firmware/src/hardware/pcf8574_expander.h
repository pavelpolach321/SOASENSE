#ifndef PCF8574_EXPANDER_H
#define PCF8574_EXPANDER_H

#include "stdint.h"

/*
position  function           LEFT                     RIGHT
[8]       wifi credentials   setup or use old setup   erase
[7]       connectivity       wifi                     lorawan
[6]       display            on                       off
[5]       reserved           [x]                      [x]
[4]       serial debug       on                       off
[3]       serial output      text                     json
[2]       reserved           [x]                      [x]
[1]       reserved           [x]                      [x]
*/

#define LEFT_POSITION                   (false)
#define RIGHT_POSITION                  (true)
#define CONNECTIVITY_ERASE_CREDENTIALS  (node_object.setup.sw_pos_8 == RIGHT_POSITION)
#define MODE_USE_WIFI                   (node_object.setup.sw_pos_7 == LEFT_POSITION)
#define MODE_USE_LORAWAN                (node_object.setup.sw_pos_7 == RIGHT_POSITION)
#define DISPLAY_ON                      (node_object.setup.sw_pos_6 == LEFT_POSITION)
#define SERIAL_OUTPUT_ON                 (node_object.setup.sw_pos_4 == LEFT_POSITION)
#define SERIAL_OUTPUT_OFF                (node_object.setup.sw_pos_4 == RIGHT_POSITION)
#define SERIAL_OUTPUT_DBG                (node_object.setup.sw_pos_3 == LEFT_POSITION)
#define SERIAL_OUTPUT_VALUES             (node_object.setup.sw_pos_3 == RIGHT_POSITION)

#define SW_POS_8                         (uint8_t)(0x80) 
#define SW_POS_7                         (uint8_t)(0x40)
#define SW_POS_6                         (uint8_t)(0x20)
#define SW_POS_5                         (uint8_t)(0x10)
#define SW_POS_4                         (uint8_t)(0x08)
#define SW_POS_3                         (uint8_t)(0x04)
#define SW_POS_2                         (uint8_t)(0x02)
#define SW_POS_1                         (uint8_t)(0x01)

uint8_t get_setup_switches();

#endif