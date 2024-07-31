#define GPIO_TIPPLING_RAIN_BUCKET GPIO_NUM_1

#define TIMER_WAKEUP_TIME_US 30000 * 1000
#define MINIMUM_GOTO_SLEEP_SECONDS 5
#define MAXIMUM_SLEEP_SECONDS (2*60*60)

#define SEND_REPORT_EVERY_SECS (60 * 2)
#define SEND_REPORT_EARLY_WAKE_SECS (23)
#define SEND_REPORT_EARLY_SEND_SECS (2)

typedef struct tMCUInfo{
  
  uint8_t wakeUpCause;
  uint8_t resetCause;
  uint8_t softResetReason;

  uint32_t upTime;
  uint32_t nextScheduledWakeUp;
  uint32_t unixSendNextReportAt;
  
}tMCUInfo;

RTC_NOINIT_ATTR tMCUInfo mcuStateInfoESP32 = {0xFF, 0xFF, 0xFF,0x00, 0x00};

bool mcuReadyForSleep = false;

typedef struct tRainfallData_rtc {

  uint32_t unixStartOfNextHour;
  uint32_t unixStartOfNextDay;
  
  uint16_t tipplingBucketCount;
  uint16_t tipplingBucketCountHour;
  uint16_t tipplingBucketCountDay;
  uint8_t  tipplingBucketDigInPrevious;

} tRainfallData_rtc;

RTC_DATA_ATTR tRainfallData_rtc rainData = {0};
