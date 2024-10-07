#ifndef CONFIG_H
#define CONFIG_H

/*******************************************************/
/*** COMPONENTS PRESENT DEFINES ************************/
/*******************************************************/

#define MQTT_PRESENT              true   /* ... */
#define PCB_TEMP_NTC_PRESENT      true   /* ... */
#define STATUS_LED_PRESENT        true   /* ... */
#define DHT22_PRESENT             true   /* ... */
#define BMP280_PRESENT            true   /* ... */
#define PRESENCE_SENSOR_PRESENT   true   /* ... */
#define LIGHT_SENSOR_PRESENT      true   /* ... */

/*******************************************************/
/*** TIMING DEFINES ************************************/
/*******************************************************/

/* Multiply 'seconds' with this to get value in 'milliseconds' */
#define S_TO_MS (1000)

/* Cycle time of the main program loop (ms) */
#define main_cycle_time (200)             /* 200ms cycle time (no need for being faster than this) */
/* This 200ms is enough for ESP32 to do everything, so we always execute every 200ms, and have a stable SW */

/* Cycle time of the debug loop (printing log to serial) in ms */
#define debug_cycle_time (2 * S_TO_MS)    /* Print to Serial every 2s */

/* How many ms to wait before toggling the status LED */
#define LED_BLINK_COUNTDOWN (1 * S_TO_MS) /* 1000ms = 1s */

/* How much to wait between two MQTT frames (us) */
#define send_delay (500 * S_TO_MS)        /* Every 5min == 300s */

/*******************************************************/
/*** PIN DEFINES ***************************************/
/*******************************************************/

/* Pin to which the system status LED is connected */
#define STATUS_LED_PIN 35

/* DHT22 connected to pin GPIO20 */
#define DHT22_PIN  20

/* NTC thermistor to measure PCB temp is connected to... */
#define PCB_TEMP_PIN 18

/* Presence sensor is connected to... */
#define PRESENCE_SENSOR_PIN 2

/* Light dependent resistor is connected to... */
#define LIGHT_SENSOR_PIN 1

/* BMP-280 is connected via I2C: */
/* SDA - GPIO8 */
/* SCL - GPIO9 */

#endif /* #ifndef CONFIG_H */