#ifndef SENSOR_PCB_TEMP_H
#define SENSOR_PCB_TEMP_H

/* System includes */
#include <ArduinoJson.h>
#include "generic_driver.h"

/* Create a class for this sensor */
class PcbTempSensor : public Driver
{
protected:
  #define NUM_OF_NTC_LOOKUPS 124 /* How many lookup values we have */
  uint8_t sensor_pin;
  float system_pcb_temp = 0;

  /* Custom struct to hold lookup values */
  typedef struct ntc_lookup_struct {
    float temperature;
    uint16_t raw_adc;
  };

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

public:
  /*****************************************************/
  /* Constructor                                       */
  /*****************************************************/
  PcbTempSensor(uint8_t l_pin)
  {
    sensor_pin = l_pin;
  }

  /*****************************************************/
  /* Init function                                     */
  /*****************************************************/
  void Init()
  {
    pinMode(sensor_pin, INPUT);
  }

  /*****************************************************/
  /* Handle function                                   */
  /*****************************************************/
  void Handle(JsonDocument *json_message)
  {
    system_pcb_temp = calculate_pcb_temp(analogRead(sensor_pin));
    (*json_message)["pcb_temp"] = system_pcb_temp;
  }

  /*****************************************************/
  /* Helper functions                                  */
  /*****************************************************/
  
  /* Convert raw ADC value from a 10k NTC thermistor (connected to 3.3V on one side, and on another via 10k pulldown to GND) */
  float calculate_pcb_temp(float raw_adc)
  {
    uint16_t i = 0;

    /* Assume the first one is the best */
    uint16_t closest_adc = ntc_lookup_values[0].raw_adc;
    uint16_t closest_temp = ntc_lookup_values[0].temperature;

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
};

#endif /* #ifndef SENSOR_PCB_TEMP_H */