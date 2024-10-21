#ifndef SENSOR_GENERIC_H
#define SENSOR_GENERIC_H

/* System includes */
#include <ArduinoJson.h>

/* Generic base class for defining a sensor or a driver */
class Driver
{
protected:
public:
  /* Mandatory functions: init and handle */
  virtual void Init() = 0;
  virtual void Handle(JsonDocument *json_message) = 0;
};

#endif /* #ifndef SENSOR_GENERIC_H */