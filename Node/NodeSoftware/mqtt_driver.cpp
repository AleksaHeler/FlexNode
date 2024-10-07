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

#if MQTT_PRESENT
  WiFiClient espClient;
  PubSubClient client(espClient);
#endif

/* Keep track of the time when we last send an MQTT message */
#if MQTT_PRESENT
  unsigned long last_send_time = 0;
#endif

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

#if MQTT_PRESENT
  void mqtt_setup_wifi()
  {
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(200);
      Serial.print(".");
      digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN)); /* This will toggle the LED every 200ms while trying to connect to WiFi */
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
#endif

#if MQTT_PRESENT
  void callback(char *topic, byte *message, unsigned int length)
  {
    Serial.println();
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }
    Serial.println();
  }
#endif

#if MQTT_PRESENT
  void wifi_reconnect()
  {
    // Loop until we're reconnected
    while (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect(mqtt_devicename, mqtt_username, mqtt_password))
      {
        Serial.println("connected");
        // Subscribe
        client.subscribe("esp32/output");
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again soon...");
        // Wait 1 second before retrying
        delay(200); /** @todo Make this an active wait */
        digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
        /** @todo Or make this restart the device? */
      }
    }
  }
#endif

void mqtt_setup()
{
  #if MQTT_PRESENT
    mqtt_setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
  #endif
}

void mqtt_loop(JsonDocument* json_message)
{
  /* If WiFi connection is lost... */
  /** @todo Aleksa Heler: make this an active wait function, so we have some state machine here, and first it tries to connect, and only then sends */
  #if MQTT_PRESENT
    if (!client.connected())
    {
      wifi_reconnect();
    }
    client.loop(); /* MQTT client handle */
  #endif
  
  /* Store our device name in the JSON */
  (*json_message)["name"] = mqtt_devicename;

  /* If we have connection, then handle the sending of data (every now and then) */
  #if MQTT_PRESENT
    long now = millis();
    if (now - last_send_time > send_delay)
    {
      /* Keep track of time */
      last_send_time = now;

      char tempString[512]; /* TODO: Make this parametric, and make it even bigger just in case */
      serializeJson(*json_message, tempString);
      Serial.print("Sending MQTT message: ");
      Serial.println(tempString);
      client.publish("home/nodes", tempString);
    }
  #endif
}
