#ifndef MQTT_H
#define MQTT_H

/* System includes */
#include <ArduinoJson.h>

/* Function to be used outside our little MQTT world */
extern void mqtt_setup();
extern void mqtt_loop(JsonDocument* json_message);

#endif /* #ifndef MQTT_H */