/* System includes */
#include <ArduinoJson.h>
#include "config.h"

/* Drivers include */
#include "bmp280_driver.h"
#include "mqtt_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

JsonDocument main_json_message;

long last_main_cycle = 0;

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void setup()
{
  Serial.begin(115200);

  /* Now initialize all the separate drivers */
  bmp280_setup();
  mqtt_setup();
}

void loop()
{
  /* Main loop... */
  long now = millis();
  if (now - last_main_cycle > main_cycle_time)
  {
    last_main_cycle = now;

    /* Call each drivers handle function */
    bmp280_loop(&main_json_message);
    mqtt_loop(&main_json_message);
  }
  else
  {
    /* Something to do in the background while waiting... */
  }
}
