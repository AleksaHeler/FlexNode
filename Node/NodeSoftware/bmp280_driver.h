#ifndef BMP280_H
#define BMP280_H

/* System includes */
#include <ArduinoJson.h>

/* Function to be used outside our little BMP280 world */
extern void bmp280_setup();
extern void bmp280_loop(JsonDocument* json_message);

#endif /* #ifndef BMP280_H */