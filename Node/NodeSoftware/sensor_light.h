#ifndef SENSOR_LIGHT_H
#define SENSOR_LIGHT_H

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"

/* Create a class for this sensor */
class LightSensor : public Driver
{
protected:
  uint8_t sensor_pin;
  uint16_t raw_light_value = 0;
  float ambient_light_value = 0;

public:
  /*****************************************************/
  /* Constructor                                       */
  /*****************************************************/
  LightSensor(uint8_t l_pin)
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
    raw_light_value = analogRead(sensor_pin);
    /* And convert it to go from 0 to 100% */
    ambient_light_value = (raw_light_value / 4096.0) * 100; // %
    
    (*json_message)["ambient_light"] = ambient_light_value;
  }

  /*****************************************************/
  /* Helper functions                                  */
  /*****************************************************/
  /* None for now */
};

#endif /* #ifndef SENSOR_LIGHT_H */