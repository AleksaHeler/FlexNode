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

WiFiClient espClient;
PubSubClient client(espClient);

/* Keep track of the time when we last send an MQTT message */
long last_send_time = 0;

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

void mqtt_setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
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
      delay(1000); /** @todo Make this an active wait */
    }
  }
}

void mqtt_setup()
{
  mqtt_setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void mqtt_loop(JsonDocument* json_message)
{
  /* If WiFi connection is lost... */
  /** @todo Aleksa Heler: make this an active wait function, so we have some state machine here, and first it tries to connect, and only then sends */
  if (!client.connected())
  {
    wifi_reconnect();
  }
  client.loop();

  /* If we have connection, then handle the sending of data (every now and then) */
  long now = millis();
  if (now - last_send_time > send_delay)
  {
    /* Keep track of time */
    last_send_time = now;

    /* Store our device name in the JSON */
    (*json_message)["name"] = mqtt_devicename;

    char tempString[256];
    serializeJson(*json_message, tempString);
    Serial.print("Message: ");
    Serial.println(tempString);
    client.publish("home/nodes", tempString); }
  else
  {
    /* Something to do in the background while waiting... */
  }
}
