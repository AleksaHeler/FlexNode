namespace RainSens {

uint8_t getDigInDebounced(const uint16_t debounceMs, const uint16_t blockingTimeout = 0) {

#define BUCKET_DEBOUNCE_CYCLE 10u
  static uint8_t debouncedVal = 2u;
  static uint8_t previousVal = 0u;
  static uint16_t debounceCounter = 0;

  if (0 != blockingTimeout) {
    debouncedVal = 2u;
  }
  
  for (uint16_t i = 0; ((i <= blockingTimeout) && (2u == debouncedVal)); i+= BUCKET_DEBOUNCE_CYCLE) {

    if (0 != blockingTimeout) {

      vTaskDelay(BUCKET_DEBOUNCE_CYCLE / portTICK_PERIOD_MS);
    }

    uint8_t currentVal = (uint8_t)digitalRead(GPIO_TIPPLING_RAIN_BUCKET);

    if (previousVal == currentVal) {
      if ((debounceCounter < debounceMs) && ((debounceCounter + BUCKET_DEBOUNCE_CYCLE) < 0xFFFF)) {
        debounceCounter += BUCKET_DEBOUNCE_CYCLE;
      }
      else {
        debouncedVal = currentVal;
      }
    }
    else {
      debounceCounter = 0u;
    }

    previousVal = currentVal;
  }


  return debouncedVal;
}

}
