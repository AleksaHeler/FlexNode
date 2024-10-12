#ifndef ADMIN_H
#define ADMIN_H

/***********************************************
 *  WiFi specific things
 ***********************************************/

/* TODO Make an array of WiFis to use, so if it cant connect to one, it tries another (maybe also same with MQTT server) */
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

/* To what topic to subscribe to? */
const char *mqtt_subscribe_topic = "admin";
/* To what topic should we send our data? */
const char *mqtt_publish_topic = "nodes";

#endif /* #ifndef ADMIN_H */
