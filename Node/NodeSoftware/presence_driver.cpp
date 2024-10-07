/* System includes */
#include "config.h"

/* Own includes */
#include "presence_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

/* The device itself */

/* Storing values we read */
#if PRESENCE_SENSOR_PRESENT
  int presence_detected = 0;
#endif

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void presence_setup()
{
  #if PRESENCE_SENSOR_PRESENT
    pinMode(PRESENCE_SENSOR_PIN, INPUT);
  #endif
}

void presence_loop(JsonDocument* json_message)
{
  #if PRESENCE_SENSOR_PRESENT
    presence_detected = digitalRead(PRESENCE_SENSOR_PIN);
    (*json_message)["presence_detected"] = presence_detected;
  #endif
}