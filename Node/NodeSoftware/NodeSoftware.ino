/**
 * We create an array of pointers to 'generic drivers' which include all sensors, networking and system stuff,
 * then we call 'Handle' function for each of them every 1s. Each driver is given a JSON object as parameter,
 * where they note down any values that need to be shared via MQTT. Networking is called last so it sends the 
 * complete json string via MQTT.
 *
 * @todo Check for buffer overflows or anything similar. We get restarting of the device from time to time (sometimes often)
 * @todo Maybe tansition to multicore approach: one core for WiFi/MQTT/OTA, other core for real-time sensor gathering!
 * @todo For sensors, at begining try pulling the sensor to high, and to low, and see if the input really changes. 
 *       Basically do open load detect, and ignore the sensor in case nothing connected.
 */

/* System includes */
#include <ArduinoJson.h>
#include "config.h"

/* All the sensors and drivers */
#include "generic_driver.h" /* Used to define array of 'Driver' classes */
#include "network_driver.h" /* Handles all network related functions (WiFi, MQTT) */
#include "sensor_air_quality.h"
#include "sensor_bmp280.h"
#include "sensor_dht22.h"
#include "sensor_light.h"
#include "sensor_pcb_temp.h"
#include "sensor_presence.h"
#include "sensor_system_health.h" /* Monitors SW stuff: cycle time, free heap... */

/***********************************************
 *  VARIABLES
 ***********************************************/

uint32_t last_main_cycle = 0;

/***********************************************
 *  Objects (drivers, sensors, JSON...)
 ***********************************************/

JsonDocument main_json_message;

Driver *drivers[] = 
{
  new SystemHealthSensor(SYSTEM_STATUS_LED_PIN, DEBUG_CYCLE_TIME), /* Should always be first (so reading millis() will be accurate) */
  new AirQualitySensor(AIR_QUALITY_SENSOR_PIN),
  new BMP280Sensor(/* Uses I2C pins 8 and 9 */),
  new DHT22Sensor(DHT22_PIN),
  new LightSensor(LIGHT_SENSOR_PIN),
  new PcbTempSensor(PCB_TEMP_PIN),
  new PresenceSensor(PRESENCE_SENSOR_PIN),
  new NetworkHandler(WIFI_STATUS_LED_PIN, SEND_DELAY) /* Should always be last (to send all gathered data) */
};

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);

  /* Let's go! Initialize all the given sensors and drivers... */
  for(Driver *driver : drivers)
  {
    (*driver).Init();
  }
}

void loop()
{
  /* Main loop: handle every x milliseconds */
  uint32_t now = millis();
  if (now - last_main_cycle >= MAIN_CYCLE_TIME)
  {
    last_main_cycle = now;

    /* Call each drivers handle function */
    for(Driver *driver : drivers)
    {
      (*driver).Handle(&main_json_message);
    }
  }
}
