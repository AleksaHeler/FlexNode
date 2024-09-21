/* System includes */
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

/* Own includes */
#include "bmp280_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

/* The device itself */
Adafruit_BMP280 bmp; // via I2C

/* Storing values we read */
float temperature = 0;
float pressure = 0;
float altitude = 0;

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void bmp280_setup()
{
  if (!bmp.begin(0x76))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1)
    {
      /** @todo Aleksa Heler: maybe reset the device here? Or first wait then reset? */
    }
  }

  /* Default settings from datasheet.  */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time */
}

void bmp280_loop(JsonDocument* json_message)
{
  /* Read all values */
  temperature = bmp.readTemperature();
  pressure = bmp.readPressure();
  altitude = bmp.readAltitude();

  /* Then store them in the JSON */
  (*json_message)["temperature"] = temperature;
  (*json_message)["pressure"] = pressure;
  (*json_message)["altitude"] = altitude;
}