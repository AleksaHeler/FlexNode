#ifndef CONFIG_H
#define CONFIG_H

/* Multiply 'seconds' with this to get value in 'milliseconds' */
#define S_TO_MS (1000)

/* How much to wait between two MQTT frames (us) */
const unsigned long send_delay = 300 * S_TO_MS; /* Every 5min */

/* Cycle time of the main program loop (ms) */
const unsigned long main_cycle_time = 10;

#endif /* #ifndef CONFIG_H */