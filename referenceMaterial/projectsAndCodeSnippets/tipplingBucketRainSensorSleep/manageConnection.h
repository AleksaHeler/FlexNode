bool wifi_connectionEnabled = false;
bool wifi_connectionActive = false;

void WIFI_KeepConnectionTask(void* parargs) {

#define WIFI_SSID "adsl sandor" //"TS-uG65"
#define WIFI_PASS "floriflori" //"4XfuPgEx"  /* and defined twice?? */

#define WIFI_TRY_CONNECT_TIME   20000u
#define WIFI_WAIT_TIME          30000u
#define WIFI_NOCON_RESET_AFTER  5u

  uint8_t con = 0;
  uint8_t noNetworkResetCounter = 0;
  uint16_t connectionStableCounter = 0;

  for (;;) {

    if (true == wifi_connectionEnabled) {
      if (WiFi.status() != WL_CONNECTED) {

        connectionStableCounter = 0;
        wifi_connectionActive = false;

        WiFi.mode(WIFI_STA);

        Serial.println("Wifi is connecting...");

        for (; ((con < (WIFI_TRY_CONNECT_TIME / 1000)) && (WiFi.status() != WL_CONNECTED)); con++) {
          WiFi.mode(WIFI_STA);
          WiFi.begin(WIFI_SSID, WIFI_PASS);
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          Serial.print(".");
        }

        if (con >= (WIFI_TRY_CONNECT_TIME / 1000)) {
          Serial.println("Wifi connection failed, will reset and retry in 30sec...");
          vTaskDelay((WIFI_WAIT_TIME) / portTICK_PERIOD_MS);
          noNetworkResetCounter++;
          if (noNetworkResetCounter >=  WIFI_NOCON_RESET_AFTER) {
            mcuStateInfoESP32.softResetReason = 1u;
            esp_restart();
          }
        }
        else {
          Serial.println("Connected [OK]");
        }
      }
      else {
        if (connectionStableCounter >= 60000) {
          wifi_connectionActive = true;
        }
        else {
          connectionStableCounter += 10000;
        }
        vTaskDelay(10000 / portTICK_PERIOD_MS);
      }
    }
    else {

      if (WiFi.status() == WL_CONNECTED) {
        /* TODO: disconnect wifi */
        wifi_connectionActive = false;
        noNetworkResetCounter = 0;
        con = 0;
      }
    }
  }
}

#define NEW_CONNECT_TIMEOUT 15000
int8_t NETWORK_wlanConnect() {

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print(F("WLAN|[ OK ] Aleready connected. IP: "));
    Serial.println(WiFi.localIP());
    return 0;
  }

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print(F("WLAN| Connecting to AP:"));
  Serial.println(WIFI_SSID);

  for (uint16_t i = 0; i < (NEW_CONNECT_TIMEOUT / 300); i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print(F("\nWLAN|[ OK ] Connected. IP: "));
      Serial.println(WiFi.localIP());
      return 0;
    }
    if (i % 10 == 0) {
      Serial.print(F("."));
    }
    vTaskDelay((300) / portTICK_PERIOD_MS);
  }
  Serial.println(F("\nWLAN|[ WR ] Could not connect wlan."));
  return -1;
}
