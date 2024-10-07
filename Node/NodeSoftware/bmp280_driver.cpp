/* System includes */
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include "config.h"

/* Own includes */
#include "bmp280_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

/* The device itself */
#if BMP280_PRESENT
  Adafruit_BMP280 bmp;
#endif

/* Storing values we read */
#if BMP280_PRESENT
  bool bmp_found = false;
  float temperature = 0;
  float pressure = 0;
  float altitude = 0;
#endif

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void bmp280_setup()
{
  #if BMP280_PRESENT
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
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }
  #endif
}

void bmp280_loop(JsonDocument* json_message)
{
  #if BMP280_PRESENT
    if(bmp_found)
    {
      /* Read all values */
      temperature = bmp.readTemperature();
      pressure = bmp.readPressure();
      altitude = bmp.readAltitude();

      /* Then store them in the JSON */
      (*json_message)["bmp_temperature"] = temperature;
      (*json_message)["bmp_pressure"] = pressure;
      (*json_message)["bmp_altitude"] = altitude;
    }
    else
    {
      /* If we didn't find the BMP sensor, try again */
      if (bmp.begin(0x76))
      {
        bmp_found = true;
      }
    }
  #endif
}