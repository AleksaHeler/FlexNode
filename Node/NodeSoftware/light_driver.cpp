/* System includes */
#include "config.h"

/* Own includes */
#include "light_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/


/* Storing values we read */
#if LIGHT_SENSOR_PRESENT
  int raw_light_value = 0;
  float ambient_light_value = 0;
#endif

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void light_setup()
{
  #if LIGHT_SENSOR_PRESENT
    pinMode(LIGHT_SENSOR_PIN, INPUT);
  #endif
}

void light_loop(JsonDocument* json_message)
{
  #if LIGHT_SENSOR_PRESENT
    /* Get raw ADC */
    raw_light_value = analogRead(LIGHT_SENSOR_PIN);
    /* And convert it to go from 0 to 100% */
    ambient_light_value = (raw_light_value / 4096.0) * 100; // %
    
    (*json_message)["light"] = ambient_light_value;
  #endif
}