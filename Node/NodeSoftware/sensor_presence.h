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
  float presence_filter = 0; /* For simple filtering: (w * old) + ((1-w) * new) */
  const float filter_weight = 0.8; /* Value from 0 to 1. Higher value means more previous values kept, lower value means new value has more weight */
  const float filter_treshold = 0.5; /* Above what value will we consider motion sensor being activated? Higher value means more movement/consistent signal needed */

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
    pinMode(sensor_pin, INPUT_PULLDOWN);
  }

  /*****************************************************/
  /* Handle function                                   */
  /*****************************************************/
  void Handle(JsonDocument *json_message)
  {
    /* Read digital input */
    uint16_t curr_reading = digitalRead(sensor_pin);
    /* Now filter it by taking weighted value of old vs new */
    presence_filter = (filter_weight * presence_filter) + ((1 - filter_weight) * curr_reading);
    /* Treshold it and write to our buffer */
    presence_detected = (uint16_t)(presence_filter > filter_treshold);
    (*json_message)["presence"] = presence_detected;
  }

  /*****************************************************/
  /* Helper functions                                  */
  /*****************************************************/
  /* None for now */
};

#endif /* #ifndef SENSOR_PRESENCE_H */