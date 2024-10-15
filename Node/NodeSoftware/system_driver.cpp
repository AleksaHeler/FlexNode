/* System includes */
#include "esp32s3/rom/rtc.h"
#include "config.h"

/* Own includes */
#include "system_driver.h"

/** @todo Aleksa Heler: add a heartbeat signal here to be sent via MQTT perhaps? */
/** @todo Aleksa Heler: make this work like a state machine, so we have initialization, connection to WiFi, then MQTT, normal operation, error, failsafe... */

/***********************************************
 *  VARIABLES
 ***********************************************/

/* Storing values we read */
#if PCB_TEMP_NTC_PRESENT
  float system_pcb_temp = 0;
#endif

unsigned long last_cycle_timestamp = 0;

/* Define the lookup values used to estimate PCB temperature */
#if PCB_TEMP_NTC_PRESENT
  const ntc_lookup_struct ntc_lookup_values[NUM_OF_NTC_LOOKUPS] = {
    { -23, 298 },
    { -22, 316 },
    { -21, 335 },
    { -20, 354 },
    { -19, 375 },
    { -18, 396 },
    { -17, 418 },
    { -16, 441 },
    { -15, 465 },
    { -14, 490 },
    { -13, 516 },
    { -12, 543 },
    { -11, 571 },
    { -10, 600 },
    {  -9, 629 },
    {  -8, 660 },
    {  -7, 692 },
    {  -6, 724 },
    {  -5, 758 },
    {  -4, 792 },
    {  -3, 827 },
    {  -2, 864 },
    {  -1, 901 },
    {   0, 938 },
    {   1, 977 },
    {   2, 1017 },
    {   3, 1057 },
    {   4, 1098 },
    {   5, 1140 },
    {   6, 1182 },
    {   7, 1225 },
    {   8, 1268 },
    {   9, 1312 },
    {  10, 1357 },
    {  11, 1402 },
    {  12, 1447 },
    {  13, 1493 },
    {  14, 1539 },
    {  15, 1585 },
    {  16, 1631 },
    {  17, 1678 },
    {  18, 1724 },
    {  19, 1771 },
    {  20, 1817 },
    {  21, 1864 },
    {  22, 1910 },
    {  23, 1956 },
    {  24, 2002 },
    {  25, 2048 },
    {  26, 2093 },
    {  27, 2138 },
    {  28, 2182 },
    {  29, 2227 },
    {  30, 2270 },
    {  31, 2314 },
    {  32, 2356 },
    {  33, 2398 },
    {  34, 2440 },
    {  35, 2481 },
    {  36, 2522 },
    {  37, 2561 },
    {  38, 2600 },
    {  39, 2639 },
    {  40, 2676 },
    {  41, 2713 },
    {  42, 2750 },
    {  43, 2785 },
    {  44, 2820 },
    {  45, 2854 },
    {  46, 2920 },
    {  47, 2952 },
    {  48, 2952 },
    {  49, 2983 },
    {  50, 3014 },
    {  51, 3044 },
    {  52, 3073 },
    {  53, 3101 },
    {  54, 3129 },
    {  55, 3156 },
    {  56, 3182 },
    {  57, 3207 },
    {  58, 3232 },
    {  59, 3256 },
    {  60, 3280 },
    {  61, 3303 },
    {  62, 3325 },
    {  63, 3347 },
    {  64, 3368 },
    {  65, 3389 },
    {  66, 3408 },
    {  67, 3428 },
    {  68, 3446 },
    {  69, 3465 },
    {  70, 3482 },
    {  71, 3500 },
    {  72, 3517 },
    {  73, 3533 },
    {  74, 3549 },
    {  75, 3564 },
    {  76, 3579 },
    {  77, 3593 },
    {  78, 3607 },
    {  79, 3621 },
    {  80, 3634 },
    {  81, 3647 },
    {  82, 3659 },
    {  83, 3671 },
    {  84, 3683 },
    {  85, 3694 },
    {  86, 3705 },
    {  87, 3716 },
    {  88, 3726 },
    {  89, 3736 },
    {  90, 3746 },
    {  91, 3755 },
    {  92, 3764 },
    {  93, 3773 },
    {  94, 3782 },
    {  95, 3790 },
    {  96, 3798 },
    {  97, 3806 },
    {  98, 3814 },
    {  99, 3821 },
    { 100, 3828 }
  };
#endif

/***********************************************
 *  FUNCTION DEFINITIONS
 ***********************************************/

/** Debug to serial why the ECU was reset! */
void verbose_print_reset_reason(int reason) {
  switch (reason) {
    case 1:  Serial.println("Vbat power on reset"); break;
    case 3:  Serial.println("Software reset digital core"); break;
    case 4:  Serial.println("Legacy watch dog reset digital core"); break;
    case 5:  Serial.println("Deep Sleep reset digital core"); break;
    case 6:  Serial.println("Reset by SLC module, reset digital core"); break;
    case 7:  Serial.println("Timer Group0 Watch dog reset digital core"); break;
    case 8:  Serial.println("Timer Group1 Watch dog reset digital core"); break;
    case 9:  Serial.println("RTC Watch dog Reset digital core"); break;
    case 10: Serial.println("Instrusion tested to reset CPU"); break;
    case 11: Serial.println("Time Group reset CPU"); break;
    case 12: Serial.println("Software reset CPU"); break;
    case 13: Serial.println("RTC Watch dog Reset CPU"); break;
    case 14: Serial.println("for APP CPU, reset by PRO CPU"); break;
    case 15: Serial.println("Reset when the vdd voltage is not stable"); break;
    case 16: Serial.println("RTC Watch dog reset digital core and rtc module"); break;
    default: Serial.println("NO_MEAN");
  }
}

#if PCB_TEMP_NTC_PRESENT
  /* Convert raw ADC value from a 10k NTC thermistor (connected to 3.3V on one side,
    and on another via 10k pulldown to GND) */
  float calculate_pcb_temp(float raw_adc)
  {
    int i = 0;
    /* Assume the first one is the best */
    int closest_adc = ntc_lookup_values[0].raw_adc;
    int closest_temp = ntc_lookup_values[0].temperature;

    /* Go over all... */
    for(i = 1; i < NUM_OF_NTC_LOOKUPS; i++)
    {
      /* If there's a new closest value found */
      if(abs(ntc_lookup_values[i].raw_adc - raw_adc) < abs(closest_adc - raw_adc))
      {
        /* Memorize it */
        closest_adc = ntc_lookup_values[i].raw_adc;
        closest_temp = ntc_lookup_values[i].temperature;
      }
    }
    
    /* For now, just return the closest temperature found (later we shall interpolate for precision) */
    return closest_temp;
  }
#endif

void system_setup()
{
  /** @todo Add some ASCII art welcome message here! */
  Serial.println("\n\nCPU0 reset reason:"); 
  verbose_print_reset_reason(rtc_get_reset_reason(0));

  #if PCB_TEMP_NTC_PRESENT
    pinMode(PCB_TEMP_PIN, INPUT);
  #endif
}

void system_loop(JsonDocument* json_message)
{
  /* Take a note of the main cycle execution time: */
  unsigned long now = millis();

  /* Log free memory */
  /* This function returns the minimum size of free heap memory that was available during program execution */
  /* the size of RAM on ESP32 is fixed at 512KB, roughly 200KB of which is used by IRAM cache/code sections, 
     leaving around 320KB (327680 bytes) for program memory */
  (*json_message)["free_heap"] = esp_get_free_heap_size() / 1000.0; // In kilobytes

  /* Log the total time taken by the main cycle */
  (*json_message)["cycle_time"] = float(now - last_cycle_timestamp);
  last_cycle_timestamp = now;

  /* Also log how much time was the device running */
  (*json_message)["sys_millis"] = now;

  /* Note down the PCB temperature (on-board NTC thermistor) */
  #if PCB_TEMP_NTC_PRESENT
    system_pcb_temp = calculate_pcb_temp(analogRead(PCB_TEMP_PIN));
    (*json_message)["pcb_temp"] = system_pcb_temp;
  #endif

  /* Since we crash after a bit over 1h of runtime: */
  /* If enoguh time has passed, just give up and try again */
  // if (millis() >= (unsigned long)(45 * 60 * 1000))
  // {
  //   ESP.restart();
  // }
}

void system_print_log(JsonDocument* json_message)
{
  /* Just print the JSON to serial monitor */
  serializeJson(*json_message, Serial);
  Serial.println();
}

void system_background()
{
  /* Doing nothing... */
}