#ifndef NETWORK_H
#define NETWORK_H

/* System includes */
#include <ArduinoJson.h>

/** MQTT state machine enumerator typedef */
#if MQTT_PRESENT
  typedef enum
  {
      network_idle                 = 0,
      network_wifi_start           = 1,
      network_wifi_connecting      = 2,
      network_wifi_connected       = 3,
      network_mqtt_start           = 4,
      network_mqtt_connecting      = 5,
      network_mqtt_connected       = 6,
      network_error                = 7
  } network_states_e;
#endif

/* Function to be used outside our little MQTT world */
extern void network_setup();
extern void network_loop(JsonDocument* json_message);

#endif /* #ifndef NETWORK_H */