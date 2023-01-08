
#include <PCF8574.h>
#include "../node_setup/setup.h"

#define EXPANDER_ADDRESS   0x24

PCF8574 pcf20(EXPANDER_ADDRESS);

uint8_t get_setup_switches() {

  if(pcf20.begin()) {
    return pcf20.read8();
  } else {
    return DEFAULT_MICROSWITCHES;
  }
}
