#ifndef MQTT_H
#define MQTT_H

/* System includes */
#include <ArduinoJson.h>

/** MQTT state machine enumerator typedef */
#if MQTT_PRESENT
  typedef enum
  {
      mqtt_idle                 = 0,
      mqtt_wifi_start           = 1,
      mqtt_wifi_connecting      = 2,
      mqtt_wifi_connected       = 3,
      mqtt_mqtt_start           = 4,
      mqtt_mqtt_connecting      = 5,
      mqtt_mqtt_connected       = 6,
      mqtt_error                = 7
  } mqtt_states_e;
#endif

/* Function to be used outside our little MQTT world */
extern void mqtt_setup();
extern void mqtt_loop(JsonDocument* json_message);

#endif /* #ifndef MQTT_H */