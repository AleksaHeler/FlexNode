#include <Arduino.h>
#include <esp_sleep.h>
#include <esp_timer.h>
#include "esp_netif.h"
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include "globalVars.h"
#include "esp32s3/rom/rtc.h"
#include "manageConnection.h"
#include "ntpSync.h"
#include "tipplingBucketRainSens.h"

void setup() {

  Serial.begin(115200);
  Serial.println("woke up!");
  Serial.println(esp_sleep_get_wakeup_cause());


  mcuStateInfoESP32.wakeUpCause = esp_sleep_get_wakeup_cause();
  mcuStateInfoESP32.resetCause  = esp_reset_reason();

  // Set up timer wakeup
  /* esp_sleep_enable_timer_wakeup(TIMER_WAKEUP_TIME_US);*/

  if (2u == mcuStateInfoESP32.wakeUpCause) {

    uint8_t tipplingBucketCurrentState = 2u;
    do {
      tipplingBucketCurrentState = RainSens::getDigInDebounced(200);
      vTaskDelay(10 / portTICK_PERIOD_MS);
    } while (2u == tipplingBucketCurrentState);
    /* RTC ext wakeup */
    if (tipplingBucketCurrentState != rainData.tipplingBucketDigInPrevious) {

      rainData.tipplingBucketDigInPrevious = tipplingBucketCurrentState;
      rainData.tipplingBucketCount++;
      rainData.tipplingBucketCountHour++;
      rainData.tipplingBucketCountDay++;
      Serial.print("tipplingBucket change was counted. back to sleep. count: ");
      Serial.println(rainData.tipplingBucketCount);
    }

    WiFi.disconnect();
    uint64_t unixNextWakeUpTime = mcuStateInfoESP32.nextScheduledWakeUp;

    if (unixNextWakeUpTime >= (mcuStateInfoESP32.unixSendNextReportAt - SEND_REPORT_EARLY_WAKE_SECS)) {
      Serial.println(" sleep will be cut short because of send!");
      unixNextWakeUpTime = (mcuStateInfoESP32.unixSendNextReportAt - SEND_REPORT_EARLY_WAKE_SECS);
    }


    if (unixNextWakeUpTime > (getEpoch() + MINIMUM_GOTO_SLEEP_SECONDS)) {
      Serial.print("going to sleep for [s]: ");
      Serial.println(unixNextWakeUpTime - getEpoch());
      /* @TODO: fix transition detection on startup */
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_1, (0u == rainData.tipplingBucketDigInPrevious) ? 1u : 0u); //1 = High, 0 = Low
      esp_deep_sleep((unixNextWakeUpTime - time(NULL)) * 1000000);
    }
    else {
      Serial.println("[Er] Sleep aborted because time is to small");
    }
  }
  else if (4u == mcuStateInfoESP32.wakeUpCause) {
    Serial.println("wakeup by timer:");
  }
}

void loop() {

  int8_t networkActive = NETWORK_wlanConnect();
  if (networkActive == 0) {
    NTP_updateTimeFromNTP();
  }

  printLocalTime();



    /*struct tm* current_time = localtime(pgetEpoch());*/
    struct tm* current_time = getLocalTime(getEpoch());

    current_time->tm_min = 0;
    current_time->tm_sec = 0;
    (current_time->tm_hour == 23) ? (current_time->tm_hour = 0) : (current_time->tm_hour++);

    rainData.unixStartOfNextHour = (uint32_t)mktime(current_time);

    current_time = getLocalTime(getEpoch());
    current_time->tm_min = 0;
    current_time->tm_sec = 0;
    current_time->tm_hour = 0;
    current_time->tm_mday++;

    rainData.unixStartOfNextDay = (uint32_t)mktime(current_time);

    mcuStateInfoESP32.nextScheduledWakeUp = getEpoch() + 30;



    current_time = getLocalTime(getEpoch());;
    uint32_t secondsNextSend = (current_time->tm_hour * 3600) + (current_time->tm_min * 60) + current_time->tm_sec;
    Serial.print("current time secs: ");
    Serial.println(secondsNextSend);

    secondsNextSend = secondsNextSend + SEND_REPORT_EVERY_SECS;
    secondsNextSend -= secondsNextSend % SEND_REPORT_EVERY_SECS;
    Serial.print("next increment secs: ");
    Serial.println(secondsNextSend);

    current_time->tm_mday += secondsNextSend / (3600 * 24);
    secondsNextSend %= (3600 * 24);
    current_time->tm_hour = secondsNextSend / 3600;
    secondsNextSend %= 3600;
    current_time->tm_min = secondsNextSend / 60;
    secondsNextSend %= 60;
    current_time->tm_sec = secondsNextSend;

    mcuStateInfoESP32.unixSendNextReportAt = (uint32_t)mktime(current_time);
    Serial.print("next report at: ");
    Serial.println((mcuStateInfoESP32.unixSendNextReportAt - getEpoch()));

    Serial.print("bucket count: ");
    Serial.println(rainData.tipplingBucketCount);

    printLocalTime();
    Serial.println(getEpoch());
    Serial.println(rainData.unixStartOfNextHour - getEpoch());
    Serial.println(rainData.unixStartOfNextDay - getEpoch());
  
  
    vTaskDelay(1000 / portTICK_PERIOD_MS);

  mcuReadyForSleep = true;

  if (true == mcuReadyForSleep) {

    /*WiFi.disconnect();*/
    uint64_t unixNextWakeUpTime = mcuStateInfoESP32.nextScheduledWakeUp;

    if (getEpoch() < 1717848248) {
      Serial.println("No correct ntp time fount!");
    }
    else {
      if (unixNextWakeUpTime >= (mcuStateInfoESP32.unixSendNextReportAt - SEND_REPORT_EARLY_WAKE_SECS)) {
        Serial.println(" sleep will be cut short because of send!");
        unixNextWakeUpTime = (mcuStateInfoESP32.unixSendNextReportAt - SEND_REPORT_EARLY_WAKE_SECS);
      }
    }

    if (unixNextWakeUpTime > (getEpoch() + MINIMUM_GOTO_SLEEP_SECONDS)) {
      Serial.print("going to sleep for [s]: ");
      Serial.println(unixNextWakeUpTime - getEpoch());
      /* @TODO: fix transition detection on startup */
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_1, (0u == rainData.tipplingBucketDigInPrevious) ? 1u : 0u); //1 = High, 0 = Low
      esp_deep_sleep((unixNextWakeUpTime - time(NULL)) * 1000000);
    }
    else {
      Serial.println("[Er] Sleep aborted because time is to small");
    }
  }

}
