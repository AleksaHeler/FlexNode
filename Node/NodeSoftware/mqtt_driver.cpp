/* System includes */
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "admin.h"

/* Own includes */
#include "mqtt_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

#if STATUS_LED_PRESENT
  /* Pretty self explanitory */
  long long last_led_blink_time = 0;
#endif

#if MQTT_PRESENT
  mqtt_states_e mqtt_state = mqtt_idle;
  WiFiClient espClient;
  PubSubClient client(espClient);

  /* Keep track of the time when we last send an MQTT message */
  long long last_send_time = 0;
  long long active_delay_start_time = 0;
#endif

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

#if MQTT_PRESENT
  /** Callback function, which will be called when we receive a message on a topic we are subscribed to */
  void callback(char *topic, byte *message, unsigned int length)
  {
    Serial.println();
    Serial.print("Message arrived on topic: '");
    Serial.print(topic);
    Serial.print("'. Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }
    Serial.println();
  }
#endif

void mqtt_setup()
{
  #if STATUS_LED_PRESENT
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);
  #endif

  #if MQTT_PRESENT
    /* Initially, we only have to kick off the state machine, and it will go its way: */
    mqtt_state = mqtt_wifi_start;
  #endif
}

void mqtt_loop(JsonDocument* json_message)
{
  /* Get current time. Needed later... */
  long long now = micros();

  /* Store our device name in the JSON (no matter what) */
  (*json_message)["name"] = mqtt_devicename;

  /* Now for some WiFi/MQTT state machine stuff... */
  #if MQTT_PRESENT
    switch(mqtt_state)
    {
      /***********************************************
      *  Idle: main functonality code
      ***********************************************/
      case (mqtt_idle):
        /* If we lost MQTT connection, try to connect again! */
        if (!client.connected())
        {
          mqtt_state = mqtt_mqtt_start;
          break;
        }

        /* MQTT client handle */
        client.loop();
        
        /* Blink the blinky every now and then... */
        #if STATUS_LED_PRESENT
          if (now - last_led_blink_time > LED_BLINK_DELAY)
          {
            last_led_blink_time = now;

            /* Blink the LED */
            digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
          }
        #endif

        /* Did the time come to send the data? */
        if (now - last_send_time > SEND_DELAY)
        {
          /* Keep track of time */
          last_send_time = now;

          char tempString[MQTT_MESSAGE_MAX_LENGTH];
          serializeJson((*json_message), tempString);
          Serial.print("Sending MQTT message: ");
          Serial.println(tempString);
          client.publish(mqtt_publish_topic, tempString);
        }
        break;

      /***********************************************
      *  WiFi related states:
      ***********************************************/
      case (mqtt_wifi_start):
        /* We start by connecting to a WiFi network */
        Serial.print("\nConnecting to ");
        Serial.println(wifi_ssid);
        WiFi.begin(wifi_ssid, wifi_password);
        mqtt_state = mqtt_wifi_connecting;
        break;

      case (mqtt_wifi_connecting):
        /* If still not connected, blink the LED every once in a while */
        if(WiFi.status() != WL_CONNECTED)
        {
          /* Using 'smart' active delay! */
          if (now - active_delay_start_time > WIFI_CONNECT_LED_BLINK_DELAY)
          {
            active_delay_start_time = now;
            Serial.print(".");
            digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN)); /* This will toggle the LED every 200ms while trying to connect to WiFi */
          }
        }
        else
        {
          /* Wow, we connected to wifi! Onwards! */
          mqtt_state = mqtt_wifi_connected;
        }
        break;

      case (mqtt_wifi_connected):
        Serial.println("\nWiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        mqtt_state = mqtt_mqtt_start;
        break;

      /***********************************************
      *  MQTT related states:
      ***********************************************/
      case (mqtt_mqtt_start):
        client.setServer(mqtt_server, 1883);
        client.setCallback(callback);
        mqtt_state = mqtt_mqtt_connecting;
        break;

      case (mqtt_mqtt_connecting):
        if (!client.connected())
        {
          /* Actively wait before trying re-connection */
          if (now - active_delay_start_time > WIFI_CONNECT_LED_BLINK_DELAY)
          {
            active_delay_start_time = now;
            Serial.print("Attempting MQTT connection... ");
            if (client.connect(mqtt_devicename, mqtt_username, mqtt_password))
            {
              Serial.println("connected");
              client.subscribe(mqtt_subscribe_topic);
            }
            else
            {
              Serial.print("failed, return code = ");
              Serial.print(client.state());
              Serial.println(". Trying again soon...");
              digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
            }
          }
        }
        else
        {
          mqtt_state = mqtt_mqtt_connected;
        }
        break;

      case (mqtt_mqtt_connected):
        mqtt_state = mqtt_idle;
        break;

      /***********************************************
      *  Errors:
      ***********************************************/
      case (mqtt_error):
        /* Do the unthinkable :o */
        ESP.restart();
        break;

      default:
        /* Impossible! */
        mqtt_state = mqtt_error;
        break;
    }
  #endif
}
