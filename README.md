# About this repository:

The code is in the src/ folder

Mode of operation can be controlled with microswitches on the top board.

position  function           LEFT                     RIGHT     Comment
[8]       wifi credentials   setup or use old setup   erase     Implemented
[7]       connectivity       wifi                     lorawan   Not implemented yet
[6]       display            on                       off       Not implemented yet
[5]       reserved           [x]                      [x]       Not implemented yet
[4]       serial debug       on                       off       Not implemented yet
[3]       serial output      text                     json      Not implemented yet
[2]       reserved           [x]                      [x]       Not implemented yet
[1]       reserved           [x]                      [x]       Not implemented yet

# Connect the node

* Toggle the on/off switch down.
* Connect the power cable
* To erase/reset wifi credentials, put the swqitch n.8 to the right
* Toggle the on/off switch up, the board will start and display will show MAC address and AP details. Previously saved wifi cponnection is erased. Node is ready to connect.
* Put the switch n.8 to the left and reset/re-power the node
* Check accessible AP points with your mobile, search for SOASENSE_AP
* Connect with password "tacrnode"
* Page should appear, if not, gGo to 192.168.0.4, setup wifi credentials where you want node to be conected
* Node will restart itself and start sending data. You can see counter and other details on display.


# Firmware files

For provisioning use files in node_setup/ and keyfiles/ folders

During normal fw compilation, EEPROM mode should be "#define EEPROM_MODE         EEPROM_SHOW"


All sensor's related files are in a folder src/hardware'

To disable/enable/control sensor's behaviour', check setup.h file.

Most of the functional code of LMIC-node stays the same, we just introduced a few function calls to collect data, create payload and write results into debug. Function related to our fork are in hardware.cpp/.h

