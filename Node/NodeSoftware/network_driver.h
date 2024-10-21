/**
 * This file is... (@todo add description)
 * 
 * Note: this file should not be named just 'network.h', becuase that confuses the compiler.
 * Because the board libraries for WiFi use '#pragma once' which works on filename basis, it
 * confuses the compiler to think a network driver header file was already included (but it's 
 * not), so here we have to name the file something else, and use proper header guards (#ifndef)
 * to not cause more issues later.
 */
#ifndef NETWORK_DRIVER_H
#define NETWORK_DRIVER_H

/* Network includes */
#include <WiFi.h> /* Included in ESP32 board libraries by Espressif */
#include <PubSubClient.h> /* PubSubClient by Nick O'Leary */

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"
#include "admin.h"

class NetworkHandler : public Driver
{
protected:
  /** Enum define for the state machine */
  typedef enum
  {
    network_idle = 0,
    network_wifi_start = 1,
    network_wifi_connecting = 2,
    network_wifi_connected = 3,
    network_mqtt_start = 4,
    network_mqtt_connecting = 5,
    network_mqtt_connected = 6,
    network_error = 7
  } network_states_e;
  network_states_e network_state = network_idle;

  /* Basic magic numbers */
  #define MQTT_MESSAGE_MAX_LENGTH 1024        /** Buffer size for sending the MQTT message as string */
  /* Pins */
  uint8_t network_led_pin;                    /** Given as parameter */
  /* Delays */
  uint32_t mqtt_send_delay;                   /** Given as parameter */
  /* Timestamp defines */
  unsigned long last_mqtt_send_timestamp = 0;
  /* Network object defines */
  WiFiClient espClient;
  PubSubClient *client;

public:
  /*****************************************************/
  /* Constructor                                       */
  /*****************************************************/
  NetworkHandler(uint8_t l_network_led_pin, uint32_t l_mqtt_send_delay)
  {
    this->network_led_pin = l_network_led_pin;
    this->mqtt_send_delay = l_mqtt_send_delay;
    client = new PubSubClient(espClient);
  }

  /*****************************************************/
  /* Init function                                     */
  /*****************************************************/
  void Init()
  {
    /* Initialize the network status LED */
    pinMode(network_led_pin, OUTPUT);
    digitalWrite(network_led_pin, LOW);

    /* We only have to kick off the state machine, and it will go its way: */
    network_state = network_wifi_start;

    /* And print the device name, just in case... */
    Serial.print(" > Hello! My name is ");
    Serial.println(network_devicename);
  }

  /*****************************************************/
  /* Handle function                                   */
  /*****************************************************/
  void Handle(JsonDocument *json_message)
  {
    uint32_t i = 0;
    uint32_t now = millis();

    /* Store our device name in the JSON (no matter what) */
    (*json_message)["name"] = network_devicename;

    /* Now for some WiFi/MQTT state machine stuff... */
    switch (network_state)
    {
    /***********************************************
     *  Idle: main functonality code
     ***********************************************/
    case (network_idle):
      /* If we lost WiFi connection, try again! */
      if (WiFi.status() != WL_CONNECTED)
      {
        network_state = network_wifi_start;
        break;
      }
      /* If we lost MQTT connection, try again! */
      if (!((*client).connected()))
      {
        network_state = network_mqtt_start;
        break;
      }

      /* MQTT client handle */
      (*client).loop();

      /* When all is working ok: network LED is off */
      digitalWrite(network_led_pin, LOW);

      /* Did the time come to send the data? */
      /** @todo Maybe make this sync to some time server, so we send every 5min on the clock! */
      /** @todo Or change this to count main cycles instead of how much time passed */
      if (now - last_mqtt_send_timestamp >= mqtt_send_delay)
      {
        /* Keep track of time */
        last_mqtt_send_timestamp = now;

        char tempString[MQTT_MESSAGE_MAX_LENGTH];
        serializeJson((*json_message), tempString);
        Serial.print(" > Sending MQTT message: ");
        Serial.println(tempString);
        /* Do we have a huge message to be sent? Should we send it in chunks? */
        #if MQTT_LARGE_MESSAGE /** @note To be tested */
          (*client).beginPublish(mqtt_publish_topic, strlen(tempString), false);
          for (i = 0; i < strlen(tempString); i++)
          {
            /** @todo Make this go over blocks, instead of every single character */
            (*client).print(tempString[i]);
          }
          (*client).endPublish();
        #else
          (*client).publish(mqtt_publish_topic, tempString);
        #endif
      }
      break;

    /***********************************************
     *  WiFi related states:
     ***********************************************/
    case (network_wifi_start):
      /* We start by connecting to a WiFi network */
      Serial.print(" > Connecting to '");
      Serial.print(wifi_ssid);
      Serial.println("'...");
      WiFi.begin(wifi_ssid, wifi_password);
      network_state = network_wifi_connecting;
      break;

    case (network_wifi_connecting):
      /* If still not connected, do active waiting... */
      if (WiFi.status() != WL_CONNECTED)
      {
        /* While connecting to WiFi: toggle the LED every cycle (every 1s) */
        digitalWrite(network_led_pin, !digitalRead(network_led_pin));
      }
      else
      {
        /* Wow, we connected to WiFi! Onwards! */
        network_state = network_wifi_connected;
        digitalWrite(network_led_pin, LOW);
      }
      break;

    case (network_wifi_connected):
      Serial.println(" > WiFi connected");
      Serial.print(" > IP address: ");
      Serial.println(WiFi.localIP());
      network_state = network_mqtt_start;
      break;

    /***********************************************
     *  MQTT related states:
     ***********************************************/
    case (network_mqtt_start):
      /* Start the MQTT connection procedure, and set the LED on */
      /* LED remains on untill we successfully connect to MQTT */
      digitalWrite(network_led_pin, HIGH);
      (*client).setServer(mqtt_server, 1883);
      (*client).setCallback(callback);
      network_state = network_mqtt_connecting;
      break;

    case (network_mqtt_connecting):
      if (!(*client).connected())
      {
        /* Notify the world what we are about to do */
        Serial.print(" > Attempting MQTT connection as '");
        Serial.print(network_devicename);
        Serial.print("' with username '");
        Serial.print(mqtt_username);
        Serial.print("' and password...\n");

        /* Attempt to connect */
        if ((*client).connect(network_devicename, mqtt_username, mqtt_password))
        {
          /* Connection successful! */
          Serial.println(" > MQTT connected");
          (*client).subscribe(mqtt_subscribe_topic);
          digitalWrite(network_led_pin, LOW);
        }
        else
        {
          /* Failed again. Ouch! */
          Serial.print(" > MQTT connection failed, return code = ");
          Serial.print((*client).state());
          Serial.println(", Trying again soon...");
          /* So this function will be called again in one second, which can be enough wait time */
        }
      }
      else
      {
        network_state = network_mqtt_connected;
        digitalWrite(network_led_pin, LOW);
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
  }

  /*****************************************************/
  /* Callback function                                 */
  /*****************************************************/
  /** This will be called when we receive a message on a topic we are subscribed to */
  static void callback(char *topic, byte *message, unsigned int length)
  {
    Serial.println();
    Serial.print(" > Message arrived on topic: '");
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
};


#endif /* #ifndef NETWORK_DRIVER_H */