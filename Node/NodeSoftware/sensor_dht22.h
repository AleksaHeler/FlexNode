#ifndef SENSOR_DHT22_H
#define SENSOR_DHT22_H

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"

/* Sensor specific includes */
#include <DHT.h> /* DHT sensor library (by Adafruit) */

/* Create a class for this sensor */
class DHT22Sensor : public Driver
{
protected:
  DHT *dht22;
  float dht22_temperature = 0;
  float dht22_humidity = 0;

public:
  /*****************************************************/
  /* Constructor                                       */
  /*****************************************************/
  DHT22Sensor(uint8_t l_pin)
  {
    dht22 = new DHT(l_pin, DHT22);
  }

  /*****************************************************/
  /* Init function                                     */
  /*****************************************************/
  void Init()
  {
    (*dht22).begin();
  }

  /*****************************************************/
  /* Handle function                                   */
  /*****************************************************/
  void Handle(JsonDocument *json_message)
  {
    /* Read all values */
    dht22_temperature = (*dht22).readTemperature();
    dht22_humidity = (*dht22).readHumidity();

    /* Check if we got garbage... */
    if (isnan(dht22_temperature) || isnan(dht22_humidity))
    {
      /* Failed to read from DHT22 sensor! */
      /* Maybe DHT was not found, or we encountered error */
    }
    else
    {
      /* Then store them in the JSON */
      (*json_message)["dth_temp"] = dht22_temperature;
      (*json_message)["dth_humid"] = dht22_humidity;
    }
  }

  /*****************************************************/
  /* Helper functions                                  */
  /*****************************************************/
  /* None for now */
};

#endif /* #ifndef SENSOR_DHT22_H */