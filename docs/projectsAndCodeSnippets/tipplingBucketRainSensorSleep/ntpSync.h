
#define GMT_OFFSET 3600*1
#define DL_SAVING_OFFSET 3600
#define NTP_UPDATE_PERIOD 60*60
#define RTC_TM_TOO_OLD 3600*72    /// TODO: read/ write last NTP time update to eeprom, and only use RTC when time not too old.
#define NTP_UPDATE_TIMEOUT 20000

time_t now;
struct tm timeinfo;
const char* ntpServer = "pool.ntp.org";
RTC_DATA_ATTR volatile uint32_t updatedAt = (NTP_UPDATE_PERIOD + 1);

uint32_t getEpoch() {
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return ((time_t)(0x00));
  }
  return (time(NULL));
}

struct tm* getLocalTime(time_t unixEpoch){

  struct tm* retTm = {0};

  if(!localtime(&unixEpoch)){
    return retTm;
  }
  retTm = localtime(&unixEpoch);
  return retTm;
}

void printLocalTime() { /// Basically it's the same implementation as Arduino's getLocalTime, hawe no idea why both time_t and tm. @Todo: read the man
  struct tm eSPSysTime;
  time_t now;
  time(&now);
  localtime_r(&now, &eSPSysTime);
  if (!(eSPSysTime.tm_year > (2020 - 1900))) {
    Serial.println(F("ESP TIME|[ ER ] Sys. time is invalid"));
  }
  Serial.println(&eSPSysTime, "ESP TIME|System time is: %A, %B %d %Y %H:%M:%S");
  Serial.println(now);
}

IRAM_ATTR void NTP_timeSynced(struct timeval *tv) /// Callback function runs when time is updated successfully - @TODO:not a very nice solution, could interrupt something important
{
  updatedAt = getEpoch();
}

int8_t NTP_updateTimeFromNTP(bool reconnect = true) { /// @TODO: not really related to other functions in this header, might get it's own.
  /// From: https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
  /// From: https://techtutorialsx.com/2021/09/03/esp32-sntp-additional-features/#Setting_the_SNTP_sync_interval

  if ((getEpoch() - updatedAt) < NTP_UPDATE_PERIOD) { /// Config time should contain it's own functions for periodically updating time from ntp, but this way I can easily set the period (as long as it's slower than ESP core default)
    return 0;
  }

  Serial.println(F("NTP| Updating..."));
  if ((WiFi.status() != WL_CONNECTED)) {
    return -1;
  }

  
  sntp_set_sync_interval(2*3600*1000); // 2 hours
  printf("sntp_get_sync_interval %u\n",sntp_get_sync_interval());// 7200000
  sntp_set_time_sync_notification_cb(NTP_timeSynced); /// Set NTP update callback function*/
  uint32_t prevUpdatedEpoch = updatedAt;
  configTime(GMT_OFFSET, DL_SAVING_OFFSET, ntpServer);

  /*if (sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
    printf("Failed to update system time within 10s timeout");
    return -1;
  }*/
  
  Serial.println(F("NTP| Update req sent, waiting for server."));
  for(int i=0; (i<(NTP_UPDATE_TIMEOUT / 300) && (prevUpdatedEpoch == updatedAt)); i++){
    vTaskDelay(300 / portTICK_PERIOD_MS);
    if(!(i%10)){
      Serial.print(F("."));
    }
  }

  Serial.println(F("NTP|[ OK ] Time was updated."));
  printLocalTime();
  
  return 0;
}
