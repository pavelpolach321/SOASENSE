function decodeUplink(input) {
    var warnings = [];
    var data = {};
    var index = 0;
    
    present_1 = input.bytes[index];
    present_2 = input.bytes[index+1];
    index += 2;

    if((input.bytes[input.bytes.length-2] == 0xCA) && (input.bytes[input.bytes.length-1] == 0xFE)) {
      if(present_1 & 0x01) {
        data.counter = (input.bytes[index] << 8) | input.bytes[index+1];
        index += 2;
      }
      if(present_1 & 0x02) {
        data.solar_voltage = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.batt_voltage = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.rail_voltage = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
      }
    
      if(present_2 & 0x01) {
        data.bme280_temp = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.bme280_pres = ((input.bytes[index] << 8) | input.bytes[index+1]);
        index += 2;
        data.bme280_hum = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
      }
      if(present_2 & 0x02) {
        data.opcn3_PM1 = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.opcn3_PM25 = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.opcn3_PM10 = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
      }
      if(present_2 & 0x04) {
        data.AD_socket_1_1 = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.AD_socket_1_2 = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.AD_socket_2_1 = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.AD_socket_2_1 = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
      }
      if(present_2 & 0x80) {
        data.metan = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
        data.metan_temp = ((input.bytes[index] << 8) | input.bytes[index+1]) / 100;
        index += 2;
      }
 

    return {
        data: data,
        warnings: warnings
    };
  }
}



