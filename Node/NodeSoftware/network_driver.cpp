/* System includes */
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "admin.h"

/* Own includes */
#include "network_driver.h"

/***********************************************
 *  VARIABLES
 ***********************************************/

#if STATUS_LED_PRESENT
  /* Pretty self explanitory */
  unsigned long last_led_blink_time = 0;
#endif

#if MQTT_PRESENT
  network_states_e network_state = network_idle;
  WiFiClient espClient;
  PubSubClient client(espClient);

  /* Keep track of the time when we last send an MQTT message */
  unsigned long last_send_time = 0;
  unsigned long active_delay_start_time = 0;
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

void network_setup()
{
  #if STATUS_LED_PRESENT
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);
  #endif

  #if MQTT_PRESENT
    /* Initially, we only have to kick off the state machine, and it will go its way: */
    network_state = network_wifi_start;
  #endif
}

void network_loop(JsonDocument* json_message)
{
  long i = 0;
  /* Get current time. Needed later... */
  unsigned long now = millis();

  /* Store our device name in the JSON (no matter what) */
  (*json_message)["name"] = network_devicename;

  /* Now for some WiFi/MQTT state machine stuff... */
  #if MQTT_PRESENT
    switch(network_state)
    {
      /***********************************************
      *  Idle: main functonality code
      ***********************************************/
      case (network_idle):
        /* If we lost MQTT connection, try to connect again! */
        if (!client.connected())
        {
          network_state = network_mqtt_start;
          break;
        }

        /* MQTT client handle */
        client.loop();
        
        /* Blink the blinky every now and then... */
        #if STATUS_LED_PRESENT
          if (now - last_led_blink_time >= LED_BLINK_DELAY)
          {
            last_led_blink_time = now;

            /* Blink the LED */
            digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
          }
        #endif

        /* Did the time come to send the data? */
        if (now - last_send_time >= SEND_DELAY)
        {
          /* Keep track of time */
          last_send_time = now;

          char tempString[MQTT_MESSAGE_MAX_LENGTH];
          serializeJson((*json_message), tempString);
          Serial.print("Sending MQTT message: ");
          Serial.println(tempString);
          /* Do we have a huge message to be sent? Should we send it in chunks? */
          #if MQTT_LARGE_MESSAGE
            client.beginPublish(mqtt_publish_topic, strlen(tempString), false);
            for(i = 0; i < strlen(tempString); i++)
            {
              client.print(tempString[i]);
            }
            client.endPublish();
          #else
            client.publish(mqtt_publish_topic, tempString);
          #endif
        }
        break;

      /***********************************************
      *  WiFi related states:
      ***********************************************/
      case (network_wifi_start):
        /* We start by connecting to a WiFi network */
        Serial.print("\nConnecting to ");
        Serial.println(wifi_ssid);
        WiFi.begin(wifi_ssid, wifi_password);
        network_state = network_wifi_connecting;
        break;

      case (network_wifi_connecting):
        /* If still not connected, blink the LED every once in a while */
        if(WiFi.status() != WL_CONNECTED)
        {
          /* Using 'smart' active delay! */
          if (now - active_delay_start_time >= WIFI_CONNECT_LED_BLINK_DELAY)
          {
            active_delay_start_time = now;
            Serial.print(".");
            digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN)); /* This will toggle the LED every 200ms while trying to connect to WiFi */
          }
        }
        else
        {
          /* Wow, we connected to wifi! Onwards! */
          network_state = network_wifi_connected;
        }
        break;

      case (network_wifi_connected):
        Serial.println("\nWiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        network_state = network_mqtt_start;
        break;

      /***********************************************
      *  MQTT related states:
      ***********************************************/
      case (network_mqtt_start):
        client.setServer(mqtt_server, 1883);
        client.setCallback(callback);
        network_state = network_mqtt_connecting;
        break;

      case (network_mqtt_connecting):
        if (!client.connected())
        {
          /* Notify the world what we are about to do */
          digitalWrite(STATUS_LED_PIN, HIGH);
          Serial.print("Attempting MQTT connection as '");
          Serial.print(network_devicename);
          Serial.print("' with username '");
          Serial.print(mqtt_username);
          Serial.print("' and password... ");
          
          /* Attempt to connect */
          if (client.connect(network_devicename, mqtt_username, mqtt_password)) {
            Serial.println("connected");
            client.subscribe(mqtt_subscribe_topic);
          } else {
            Serial.print("failed, return code = ");
            Serial.print(client.state());
            Serial.println(", Trying again soon...");
            delay(MQTT_CONNECTION_RETRY_DELAY);
          }
        }
        else
        {
          network_state = network_mqtt_connected;
        }
        break;

      case (network_mqtt_connected):
        network_state = network_idle;
        break;

      /***********************************************
      *  Errors:
      ***********************************************/
      case (network_error):
        /* Do the unthinkable :o */
        // ESP.restart();
        break;

      default:
        /* Impossible! */
        network_state = network_error;
        break;
    }
  #endif
}
