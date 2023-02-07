
echo "{  \"version\":    0, \"subversion\": $1 }" > version.json
scp Firmware/.pio/build/ttgo_lora32_v21/firmware.bin soasense:/var/www/html/ppz-update/
#scp version.json soasense:/var/www/html/ppz-update
