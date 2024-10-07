#ifndef SYSTEM_H
#define SYSTEM_H

/* System includes */
#include <ArduinoJson.h>

/* Function to be used outside our little world */
extern void system_setup();
extern void system_loop(JsonDocument* json_message);
extern void system_print_log(JsonDocument* json_message);
extern void system_background();

#if PCB_TEMP_NTC_PRESENT
  /* How many lookup values we have */
  #define NUM_OF_NTC_LOOKUPS 124
  
  /* Custom struct to hold lookup values */
  typedef struct ntc_lookup_struct {
    float temperature;
    int raw_adc;
  };

  /* Let there be a lookup table (somewhere) */
  extern const ntc_lookup_struct ntc_lookup_values[NUM_OF_NTC_LOOKUPS];
#endif

#endif /* #ifndef SYSTEM_H */