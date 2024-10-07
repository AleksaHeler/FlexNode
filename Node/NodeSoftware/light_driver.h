#ifndef LIGHT_H
#define LIGHT_H

/* System includes */
#include <ArduinoJson.h>

/* Function to be used outside our little world */
extern void light_setup();
extern void light_loop(JsonDocument* json_message);

#endif /* #ifndef LIGHT_H */