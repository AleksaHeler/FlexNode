#ifndef SENSOR_SYSTEM_HEALTH_H
#define SENSOR_SYSTEM_HEALTH_H

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"

/* Sensor specific includes */
#include "esp32s3/rom/rtc.h"

/* Create a class for this sensor */
class SystemHealthSensor : public Driver {
  protected:
    uint8_t system_led_pin;
    uint32_t debug_cycle_time;

    /* Timestamps */
    uint32_t last_cycle_timestamp;
    uint32_t last_debug_cycle_timestamp;

  public:
    /*****************************************************/
    /* Constructor                                       */
    /*****************************************************/
    SystemHealthSensor(uint8_t l_led_pin, uint32_t l_debug_cycle_time)
    {
      this->system_led_pin = l_led_pin;
      this->debug_cycle_time = l_debug_cycle_time;
    }

    /*****************************************************/
    /* Init function                                     */
    /*****************************************************/
    void Init()
    {
      print_welcome_message();
      verbose_print_reset_reason();

      /* Take a note of the current execution time */
      last_debug_cycle_timestamp = millis();
      last_cycle_timestamp = millis();

      /* Configure sys LED pin */
      pinMode(system_led_pin, OUTPUT);
      digitalWrite(system_led_pin, LOW);
    }

    /*****************************************************/
    /* Handle function                                   */
    /*****************************************************/
    void Handle(JsonDocument* json_message)
    {
      /* Take a note of the main cycle execution time: */
      uint32_t now = millis();
      
      /* Toggle the LED every cycle (every 1s) */
      digitalWrite(system_led_pin, !digitalRead(system_led_pin));
      
      /* Log free memory */
      /* This function returns the minimum size of free heap memory that was available during program execution */
      /* the size of RAM on ESP32 is fixed at 512KB, roughly 200KB of which is used by IRAM cache/code sections, leaving around 320KB (327680 bytes) for program memory */
      (*json_message)["free_heap_kb"] = esp_get_free_heap_size() / 1000.0; // In kilobytes

      /* Log the total time taken by the main cycle */
      (*json_message)["cycle_time_ms"] = now - last_cycle_timestamp;
      last_cycle_timestamp = now;

      /* Also log how much time was the device running */
      (*json_message)["sys_millis"] = now;

      /* And debug the JSON message in the Serial monitor... */
      if (now - last_debug_cycle_timestamp >= debug_cycle_time)
      {
        last_debug_cycle_timestamp = now;
        Debug(json_message);
      }
    }

    /*****************************************************/
    /* Helper functions                                  */
    /*****************************************************/

    /** Just print the JSON to serial monitor */
    void Debug(JsonDocument* json_message)
    {
      serializeJson(*json_message, Serial);
      Serial.println();
    }

    /** Debug to serial why the MCU was reset! */
    void verbose_print_reset_reason()
    {
      uint16_t reason = rtc_get_reset_reason(0);
      Serial.print(" > CPU0 reset reason: "); 
      switch (reason) {
        case 1:  Serial.println(F("Vbat power on reset")); break;
        case 3:  Serial.println(F("Software reset digital core")); break;
        case 4:  Serial.println(F("Legacy watch dog reset digital core")); break;
        case 5:  Serial.println(F("Deep Sleep reset digital core")); break;
        case 6:  Serial.println(F("Reset by SLC module, reset digital core")); break;
        case 7:  Serial.println(F("Timer Group0 Watch dog reset digital core")); break;
        case 8:  Serial.println(F("Timer Group1 Watch dog reset digital core")); break;
        case 9:  Serial.println(F("RTC Watch dog Reset digital core")); break;
        case 10: Serial.println(F("Instrusion tested to reset CPU")); break;
        case 11: Serial.println(F("Time Group reset CPU")); break;
        case 12: Serial.println(F("Software reset CPU")); break;
        case 13: Serial.println(F("RTC Watch dog Reset CPU")); break;
        case 14: Serial.println(F("for APP CPU, reset by PRO CPU")); break;
        case 15: Serial.println(F("Reset when the vdd voltage is not stable")); break;
        case 16: Serial.println(F("RTC Watch dog reset digital core and rtc module")); break;
        default: Serial.println(F("Not sure why the ESP32 was reset"));
      }
    }

    /** Print some ASCII art welcome message */
    void print_welcome_message(void)
    {
      /* A nice ascii logo: https://patorjk.com/software/taag/#p=display&f=Doom&t=FlexNode */
      Serial.println(F("\n/********************************************\\"));
      Serial.println(F("| ______ _           _   _           _       |"));
      Serial.println(F("| |  ___| |         | \\ | |         | |      |"));
      Serial.println(F("| | |_  | | _____  _|  \\| | ___   __| | ___  |"));
      Serial.println(F("| |  _| | |/ _ \\ \\/ / . ` |/ _ \\ / _` |/ _ \\ |"));
      Serial.println(F("| | |   | |  __/>  <| |\\  | (_) | (_| |  __/ |"));
      Serial.println(F("| \\_|   |_|\\___/_/\\_\\_| \\_/\\___/ \\__,_|\\___| |"));
      Serial.println(F("\\********************************************/\n"));
    }
};
                                          
#endif /* #ifndef SENSOR_SYSTEM_HEALTH_H */