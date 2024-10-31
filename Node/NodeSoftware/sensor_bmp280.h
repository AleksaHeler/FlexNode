#ifndef SENSOR_BMP280_H
#define SENSOR_BMP280_H

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"

/* Sensor specific includes */
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

/* Create a class for this sensor */
class BMP280Sensor : public Driver
{
protected:
  Adafruit_BMP280 bmp;
  bool bmp_found = false;
  float temperature = 0;
  float pressure = 0;
  float altitude = 0;

public:
  /*****************************************************/
  /* Constructor                                       */
  /*****************************************************/
  BMP280Sensor()
  {
  }

  /*****************************************************/
  /* Init function                                     */
  /*****************************************************/
  void Init()
  {
    if (bmp.begin(0x76))
    {
      bmp_found = true;
      /* Default settings from datasheet.  */
      bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode */
                      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                      Adafruit_BMP280::FILTER_X16,      /* Filtering */
                      Adafruit_BMP280::STANDBY_MS_500); /* Standby time */
    }
    else
    {
      Serial.println(" > Could not find a valid BMP280 sensor, check wiring!");
    }
  }

  /*****************************************************/
  /* Handle function                                   */
  /*****************************************************/
  void Handle(JsonDocument *json_message)
  {
    if(bmp_found)
    {
      /* Read all values */
      temperature = bmp.readTemperature();
      pressure = bmp.readPressure();
      altitude = bmp.readAltitude();

      /* Then store them in the JSON */
      (*json_message)["bmp_temp"] = temperature;
      (*json_message)["bmp_press"] = pressure;
      // (*json_message)["bmp_alt"] = altitude;
    }
    else
    {
      /* If we didn't find the BMP sensor, try again */
      if (bmp.begin(0x76))
      {
        bmp_found = true;
      }
    }
  }

  /*****************************************************/
  /* Helper functions                                  */
  /*****************************************************/
  /* None for now */
};

#endif /* #ifndef SENSOR_BMP280_H */