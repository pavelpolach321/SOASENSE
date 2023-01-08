#ifndef WIFI_CERTIFICATE_H
#define WIFI_CERTIFICATE_H

#include "stdio.h"
// Dronuv server
//const char* mqtt_server = "193.84.207.107";
const char* mqtt_server = "lesni.space";
uint16_t mqtt_port = 1883;
// Check new firmware image metadate here
static const char *version_file_url = "http://lesni.space/version.json";
// Check new firmware image here
static const char *url = "http://lesni.space/firmware.bin"; 

// Web certificate for lesni.space
static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n"
"MIIGHTCCBQWgAwIBAgISA1gLXP0UnbAHx3q8Gv/COF33MA0GCSqGSIb3DQEBCwUA\n"
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n"
"EwJSMzAeFw0yMjExMTMyMDM3MTJaFw0yMzAyMTEyMDM3MTFaMBYxFDASBgNVBAMT\n"
"C2xlc25pLnNwYWNlMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAzKwM\n"
"QXyFeFaIkBkDS++cXke1hVB/Q3hta+oS3j5NbApgeC3bII8SIzOFmteIThPCEA7E\n"
"4fYnWWtT3ExCBjs1chiX95MBq5t4ilIef7oCqr0eS6D5sUzbyOmvtFtV6+XBqRDh\n"
"2vQHh944UGD2luYJ+KCuKu/kGAyK948w0HrljLY8xXKBfCOXzEr0tKN53FB8Jp7G\n"
"1VPnGb//HDSmnzjRDiXpk1oxSi7/W4azvuxGAktQkYdNL/QVhK78CGIjm4VphrhP\n"
"LDOhagnuhh8DCKhaa/HJgVxRhtpPGEWYwBtWSzCsj8n8NFFtpK4dy5w5VLaMeqSB\n"
"PrEmOBPPJi/LQTZWzfjNDSYrK4vGU50z9ImrD525eMLgdgxrcC5dKh183N3LP81t\n"
"8+toLlpXOglP7BMAp/ZDikicjEWHqtl2r46xdFKw+/14Df8qVw/IbNfM5Zl9PlH2\n"
"kwdP+y8oYR2mu4Mpt7G2tS0AiyNo9yMt+yqFR5WlbUHEY8y5QCz7NWQX8XwJo5Va\n"
"us51TRU8u+L3cdoFSzfvU9Uh+lmJLZPlFdjdxrFqX3+ui4Pl+JzuwetImNDc34HD\n"
"E6vAyicYRb3lkH6APk8lc+BwOpOiGOBus4p70LU6QLhvPfVuxXjSLSlVJ9e75VIr\n"
"J+0S5ro9K8QWbO7ISPgzTmBOqxrWt0iBmWXXnDkCAwEAAaOCAkcwggJDMA4GA1Ud\n"
"DwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0T\n"
"AQH/BAIwADAdBgNVHQ4EFgQUIxmju2aXFronabvU1ZKq2cCskGQwHwYDVR0jBBgw\n"
"FoAUFC6zF7dYVsuuUAlA5h+vnYsUwsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUF\n"
"BzABhhVodHRwOi8vcjMuby5sZW5jci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9y\n"
"My5pLmxlbmNyLm9yZy8wFgYDVR0RBA8wDYILbGVzbmkuc3BhY2UwTAYDVR0gBEUw\n"
"QzAIBgZngQwBAgEwNwYLKwYBBAGC3xMBAQEwKDAmBggrBgEFBQcCARYaaHR0cDov\n"
"L2Nwcy5sZXRzZW5jcnlwdC5vcmcwggEFBgorBgEEAdZ5AgQCBIH2BIHzAPEAdwC3\n"
"Pvsk35xNunXyOcW6WPRsXfxCz3qfNcSeHQmBJe20mQAAAYRy7M2EAAAEAwBIMEYC\n"
"IQCdeKd+eqGlrsEbVE8clbW/2L1xLDGeQkFG5v6JfvwCEgIhAP1BSpqrfQQvv0Ub\n"
"Rre5aU2kO9qUeD/Xo+s2aU9gFZNxAHYAejKMVNi3LbYg6jjgUh7phBZwMhOFTTvS\n"
"K8E6V6NS61IAAAGEcuzPngAABAMARzBFAiB4q2ffOIqO+7DclmxAVERkduuNIkXl\n"
"/7D0lhYDsrLTOgIhAOVNJZVapkN1DvxtkBKQYqUzcfM9sWo0rJX6/dlu+n/gMA0G\n"
"CSqGSIb3DQEBCwUAA4IBAQBjsEMXnMPtm+UAMfwhQJSnAGapKCcje7oIUI92TIFZ\n"
"7MH7R2OTlXW1ySccWJ6YM7WbfgUB4njD3uzRmuvi2yuveetbTNBFh8UGFg4dpWhP\n"
"+Xfum2A6PnByuYSVVnDFgL7L2YrWQLxvVzNh8weh7gn/ac+ur6pT+aKzRygcvHgV\n"
"yHHJ8TmLYS5vLcTBPVvdhR/ltQUr0FvoNcGB0a6I7Rnj1Z69eEVlhyn2sLwHKPOf\n"
"t2kAWUF+Pl01YXizyZHG9jGGULGizT0GnKMMsZO/zoT0JhCOPRkzvXNib+fGgl8u\n"
"UISoRWz1wE+yB6EuA7xUjiSG3My/AD3k716NVhZ5K746\n"
"-----END CERTIFICATE-----";

#endif