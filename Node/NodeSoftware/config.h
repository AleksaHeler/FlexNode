#ifndef FLEX_CONFIG_H
#define FLEX_CONFIG_H

/*******************************************************/
/*** TIMING DEFINES ************************************/
/*******************************************************/

/* Baudrate used for serial communication */
#define SERIAL_BAUDRATE (115200)

/* Cycle time of the main program loop (ms). Set to 1s, as we don't need faster handling */
/* Other sensors might rely on thei 'Handle' functions being called exactly every 1s (led blinking, active wait, etc.) */
/* Usually more than 100ms is needed to do everything in a loop, but that has to be measured for specifc scenarios */
#define MAIN_CYCLE_TIME (1000)

/* How much to wait between two MQTT frames (ms) */
#define SEND_DELAY (300 * 1000)        /* Every 5min == 300s */

/* Cycle time of the debug loop (printing log to serial) in ms */
#define DEBUG_CYCLE_TIME (5 * 1000)    /* Print to Serial every 5s */


/*******************************************************/
/*** PIN DEFINES ***************************************/
/*******************************************************/

/* Pin to which the system status LED is connected */
#define SYSTEM_STATUS_LED_PIN 35

/* Pin to which the system status LED is connected */
#define WIFI_STATUS_LED_PIN 4

/* DHT22 connected to pin GPIO20 */
#define DHT22_PIN  20

/* NTC thermistor to measure PCB temp is connected to... */
#define PCB_TEMP_PIN 18

/* Presence sensor is connected to... */
#define PRESENCE_SENSOR_PIN 2

/* Light dependent resistor is connected to... */
#define LIGHT_SENSOR_PIN 1

/* Pin to which the MQ-2 gas sensor is connected */
#define AIR_QUALITY_SENSOR_PIN 5

/* BMP-280 is connected via I2C: */
/* SDA - GPIO8 */
/* SCL - GPIO9 */


#endif /* #ifndef FLEX_CONFIG_H */