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

/* Multiply 'milliseconds' with this to get value in 'microseconds' */
#define MS_TO_US (1000)

/* How much to wait between two MQTT frames (us) */
#define SEND_DELAY (300 * S_TO_MS * MS_TO_US)        /* Every 5min == 300s */

/* Cycle time of the main program loop (us) */
#define MAIN_CYCLE_TIME (200 * MS_TO_US)     /* 200ms cycle time (no need for being faster than this) */
/* For now it seems ~120ms is enough for ESP32 to do everything, so set this to be more than that for a stable SW */

/* Cycle time of the debug loop (printing log to serial) in us */
#define DEBUG_CYCLE_TIME (2 * S_TO_MS * MS_TO_US)    /* Print to Serial every 2s */

/* How many us to wait before toggling the status LED */
#define LED_BLINK_DELAY (1 * S_TO_MS * MS_TO_US) /* 1000ms = 1s */

/** How often to blink the LED while waiting for WiFi connection? */
#define WIFI_CONNECT_LED_BLINK_DELAY (200 * MS_TO_US)


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

/*******************************************************/
/*** OTHER USEFUL DEFINES ******************************/
/*******************************************************/

/* Maximum number of characters in an MQTT message being sent (as a JSON object). Make sure the whole message fits here, or else we lose data! */
#define MQTT_MESSAGE_MAX_LENGTH 1024


#endif /* #ifndef CONFIG_H */