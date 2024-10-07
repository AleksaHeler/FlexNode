#ifndef DHT22_H
#define DHT22_H

/* System includes */
#include <ArduinoJson.h>

/* Function to be used outside our little world */
extern void dht22_setup();
extern void dht22_loop(JsonDocument* json_message);

#endif /* #ifndef DHT22_H */