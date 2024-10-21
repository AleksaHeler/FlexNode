#ifndef SENSOR_PRESENCE_H
#define SENSOR_PRESENCE_H

/** @todo Add some filtering here: maybe (1 - x) * a + x * b or something similar */

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"

/* Create a class for this sensor */
class PresenceSensor : public Driver
{
protected:
  uint8_t sensor_pin;
  uint16_t presence_detected = 0;

public:
  /*****************************************************/
  /* Constructor                                       */
  /*****************************************************/
  PresenceSensor(uint8_t l_pin)
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
    presence_detected = digitalRead(sensor_pin);
    (*json_message)["presence"] = presence_detected;
  }

  /*****************************************************/
  /* Helper functions                                  */
  /*****************************************************/
  /* None for now */
};

#endif /* #ifndef SENSOR_PRESENCE_H */