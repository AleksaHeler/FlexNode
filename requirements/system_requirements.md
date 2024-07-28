# System requirements

## Hub
 - RaspberryPi + DIY shield for Pi
 - Backup battery, GSM module for SMS and backup internet
 - On power loss, use backup battery to send warning of power outage
 - Use existing WiFi networks (but possibility to use GSM module for standalone operation), inside, always running
 - Uploads all data to main server, which is hosted somewhere else
 
## Node
 - DIY PCB, with ESP32 S3 module
 - Many different sensor/actuator options
 - Option to run in ultra low power mode, with battery and solar panel options 
   (for installing in locations without power supply)
 - WiFi communication to the Hub
 - Modular - same PCB/HW, almost same SW (just config different) for all possible scenarios
 - Over The Air updates (OTA)
 - Option to monitor sensors constanly (alarm function)
 - Periodic wakeup, gather data, send to Hub
 - Configurable via simple config file
 - Sending a heartbeat signal to the hub if necessary, also status (starting up, shutting down...)
 - Auto error detections and mitigations (resets, sending a warning/error, self shutdown...)
 - Possible sensors: temp, humidity, atm. pressure, light, microwave presence sensor, PIR,
   reed switch for doors, air quality (mostly dust particles), microphone/sound sensor,
   knock-over sensor (when something falls), fire sensor, soil moisture, weight scale...
 - Possible actuators: buzzer, alarm/siren, LEDs, oled/LCD/display, linear actuator, servo motor, relay...
