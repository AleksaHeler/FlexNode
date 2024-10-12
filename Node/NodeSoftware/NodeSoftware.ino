/* System includes */
#include <ArduinoJson.h>
#include "config.h"

/* Drivers include */
#include "system_driver.h"
#include "bmp280_driver.h"
#include "dht22_driver.h"
#include "presence_driver.h"
#include "light_driver.h"
#include "mqtt_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

JsonDocument main_json_message;

long long last_main_cycle = 0;
long long last_debug_cycle = 0;

/** @todo Check for buffer overflows or anything similar. We get restarting of the device from time to time (sometimes often) */
/** @todo Transition to multicore approach: one core for WiFi/MQTT/OTA, other core for real-time sensor gathering! */

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void setup()
{
  Serial.begin(115200);

  /* Now initialize all the separate drivers */
  system_setup(); // We make sure to initialize the system first!
  bmp280_setup();
  dht22_setup();
  presence_setup();
  light_setup();
  mqtt_setup();
}

void loop()
{
  /* Main loop... */
  long long now = micros();
  if (now - last_main_cycle > MAIN_CYCLE_TIME)
  {
    last_main_cycle = now;

    /* Call each drivers handle function */
    system_loop(&main_json_message);
    bmp280_loop(&main_json_message);
    dht22_loop(&main_json_message);
    presence_loop(&main_json_message);
    light_loop(&main_json_message);
    mqtt_loop(&main_json_message);

  }
  /* And debug the JSON message in the Serial monitor... */
  else if(now - last_debug_cycle > DEBUG_CYCLE_TIME)
  {
    last_debug_cycle = now;
    system_print_log(&main_json_message);
  }
  /* Time for some background tasks...  */
  else
  {
    /* Something to do in the background while waiting... */
    system_background();
  }
}
