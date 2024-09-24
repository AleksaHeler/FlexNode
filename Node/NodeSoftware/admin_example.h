#ifndef ADMIN_H
#define ADMIN_H

/***********************************************
 *  WiFi specific things
 ***********************************************/

const char *wifi_ssid = "ssid";
const char *wifi_password = "pass";

/***********************************************
 *  MQTT specific stuff
 ***********************************************/

/* Used as signature in JSON data being sent */
const char *mqtt_devicename = "ESP32_Device_Name";
/* Used as user/password for MQTT server connection */
const char *mqtt_username = "node_username";
const char *mqtt_password = "node_password";
/* IP address or URL of the MQTT broker */
const char *mqtt_server = "192.168.0.10";

#endif /* #ifndef ADMIN_H */