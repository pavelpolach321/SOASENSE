#ifndef WIFI_CERTIFICATE_H
#define WIFI_CERTIFICATE_H

#include "stdio.h"
// Dronuv server
const char* mqtt_server = "193.84.207.107";
uint16_t mqtt_port = 1883;
const char* mqqt_user = "ppz_node";
const char* mqqt_password = "KsDv5aDprrd";
// Check new firmware image metadate here
static const char *version_file_url = "http://ppz-update.stud.slu.cz/version.json";
// Check new firmware image here
static const char *url = "https://ppz-update.stud.slu.cz/firmware.bin";

// Web certificate for https://ppz-update.stud.slu.cz/
static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n"
"MIIDeTCCAmGgAwIBAgIULQy+Nwr61qglnaRkIPfIUVDuaa0wDQYJKoZIhvcNAQEL\n"
"BQAwTDELMAkGA1UEBhMCQ1oxDjAMBgNVBAcMBU9wYXZhMQwwCgYDVQQKDANwcHox\n"
"HzAdBgNVBAMMFnBwei11cGRhdGUuc3R1ZC5zbHUuY3owHhcNMjMwMTA4MjIxNzMy\n"
"WhcNMzMwMTA1MjIxNzMyWjBMMQswCQYDVQQGEwJDWjEOMAwGA1UEBwwFT3BhdmEx\n"
"DDAKBgNVBAoMA3BwejEfMB0GA1UEAwwWcHB6LXVwZGF0ZS5zdHVkLnNsdS5jejCC\n"
"ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAK7rsqjJc2EL5zsWNngoXdXF\n"
"98AWd4M4WcygZQGFkyk0QT6/A/l4qpzWU568czrT+mz8PSByAJ6VNAtJPnURJnM5\n"
"u00VRIPHBuVX0dCM4vcya9KBfMiq7BBc5BU7WQvtsY475lEt402lusw+0rmeX/ha\n"
"54u+y8NTBC1ekFMR8280ADayEUI70t9y4NAjJubOpDl1mcf9ACqlWi3jYILWadGo\n"
"0z/UzkSxtrg5RsiaKuGBQ8r20NfftVefxUsnNLQuhFi6g1Hk92NDkCZW99QS+raJ\n"
"D25EKC9GOTvfP/zVIAs49gc70OjsuZSbxBVhTX3Oy2Jmwkx1QCJGuhSVuENmnp8C\n"
"AwEAAaNTMFEwHQYDVR0OBBYEFCLf8mhkH+e61G1MltYC8Jw+KiZxMB8GA1UdIwQY\n"
"MBaAFCLf8mhkH+e61G1MltYC8Jw+KiZxMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZI\n"
"hvcNAQELBQADggEBAFrORkV1hfaww2Vvs1BJpcwQLGJXklzRhVPP4oO0h9GCN504\n"
"M+25B+ZQpb4SBKcrM0gwqR6nK15lakU3f7svGCOPrU7PkghwDGxRObazchoguaHo\n"
"xi3E6RNRwsfkVMCfnQkprT6LSaL9ScZ85IFkv/kCskWOMltBxV/yqs0EB8ujhf4Q\n"
"xD5QqbDWmwK8dOcmPzAfeihhYhvV9V0xilhYphygpEBpbf8Ws3JT9VyvFmXmMM95\n"
"yTCE1lwsJeMxINnQDwPqOkgYa0BXYNXFPgRye9ahsJc1jU9wVNHxuux7kfiFp/V1\n"
"2AgFd5jNSdaE5Up+byFKLw7DN4fmO71PdYMFdpQ=\n"
"-----END CERTIFICATE-----";

#endif