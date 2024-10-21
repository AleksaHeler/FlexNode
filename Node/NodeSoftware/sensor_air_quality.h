#ifndef SENSOR_AIR_QUALITY_H
#define SENSOR_AIR_QUALITY_H

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"

/* Create a class for this sensor */
class AirQualitySensor : public Driver
{
protected:
  uint8_t sensor_pin;
  uint16_t raw_sensor_value = 0;
  float air_quality_value = 0;

public:
  /*****************************************************/
  /* Constructor                                       */
  /*****************************************************/
  AirQualitySensor(uint8_t l_pin)
  {
    sensor_pin = l_pin;
  }

  /*****************************************************/
  /* Init function                                     */
  /*****************************************************/
  void Init()
  {
    pinMode(sensor_pin, INPUT);
  }

  /*****************************************************/
  /* Handle function                                   */
  /*****************************************************/
  void Handle(JsonDocument *json_message)
  {
    /* Get raw ADC */
    raw_sensor_value = analogRead(sensor_pin);
    /* And convert it to go from 0 to 100% */
    air_quality_value = (raw_sensor_value / 4096.0) * 100; // %
    
    (*json_message)["air_quality"] = air_quality_value;
  }

  /*****************************************************/
  /* Helper functions                                  */
  /*****************************************************/
  /* None for now */
};

#endif /* #ifndef SENSOR_AIR_QUALITY_H */