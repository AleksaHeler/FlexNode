#ifndef PRESENCE_H
#define PRESENCE_H

/* System includes */
#include <ArduinoJson.h>

/* Function to be used outside our little world */
extern void presence_setup();
extern void presence_loop(JsonDocument* json_message);

#endif /* #ifndef PRESENCE_H */