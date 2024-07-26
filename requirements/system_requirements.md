# System requirements

## Hub
 - RaspberryPi + DIY shield for Pi
 - Backup battery, GSM module for SMS and backup internet
 - On power loss, use backup battery to send warning of power outage
 - Use existing WiFi networks, inside, always running
 - Running MQTT - for nodes to send data to
 - Uploads all data to main server, which is hosted somewhere else
 
## Node
 - DIY PCB
 - ESP32 S3 module
 - Modular - same PCB/HW, almost same SW (just config different) for all possible scenarios
 - Over The Air updates (OTA)
 - Many different sensor/actuator options
 - Option to run in ultra low power mode, with battery and solar panel options 
   (for installing in locations without power supply)
 - Option to monitor sensors constanly (alarm function)
 - Periodic wakeup, gather data, send to hub
 - Configurable via simple config file
 - Sending a heartbeat signal to the hub if necessary, also status (starting up, shutting down...)
 - Auto error detections and mitigations (resets, sending a warning/error, self shutdown...)
 - Possible sensors: temp, humidity, atm. pressure, light, microwave presence sensor, PIR, 
   reed switch for doors, air quality (mostly dust particles), microphone/sound sensor, 
   knock-over sensor (when something falls), fire sensor, soil moisture, weight scale...  
 - Possible actuators: buzzer, alarm/siren, LEDs, oled/LCD/display, linear actuator, servo motor, relay...
