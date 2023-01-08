function decodeUplink(input) {
    var warnings = [];
    var data = {};
    
    for (var i = 0; i < input.bytes.length; i++) {
      switch (i) {
        case 0:
          data.counter = (input.bytes[i] << 8) + input.bytes[i+1];
          break;
        case 2:
          data.pm1 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 4:
          data.pm25 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 6:
          data.pm10 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 8:
          data.no2_1 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 10:
          data.no2_2 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 12:
          data.ox_1 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 14:
          data.ox_2 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 16:
          data.batt = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 18:
          data.socket1_1 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 20:
          data.socket1_2 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 22:
          data.socket2_1 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 24:
          data.socket2_2 = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 26:
          data.temp = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
        case 28:
          data.pres = ((input.bytes[i] << 8) + input.bytes[i+1]);
          break;
        case 30:
          data.hum = ((input.bytes[i] << 8) + input.bytes[i+1])/100;
          break;
      }
    }

    return {
        data: data,
        warnings: warnings
    };
}
