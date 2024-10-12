/* System includes */
#include <DHT.h>
#include "config.h"

/* Own includes */
#include "dht22_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

/* The device itself */
#if DHT22_PRESENT
  DHT dht22(DHT22_PIN, DHT22);
#endif

/* Storing values we read */
#if DHT22_PRESENT
  float dht22_temperature = 0;
  float dht22_humidity = 0;
#endif

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void dht22_setup()
{
  #if DHT22_PRESENT
    /* initialize the DHT22 sensor */
    dht22.begin();
  #endif
}

void dht22_loop(JsonDocument* json_message)
{
  #if DHT22_PRESENT
    /* Read all values */
    dht22_temperature = dht22.readTemperature();
    dht22_humidity  = dht22.readHumidity();

    /* Check if we got garbage... */
    if ( isnan(dht22_temperature) || isnan(dht22_humidity) ) {
      /* Failed to read from DHT22 sensor! */
      /* Maybe DHT was not found, or we encountered error */
    } else {
      /* Then store them in the JSON */
      (*json_message)["dth_temp"] = dht22_temperature;
      (*json_message)["dth_humid"] = dht22_humidity;
    }
  #endif
}