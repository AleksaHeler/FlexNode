# Hardware requirements

## Hub
 - Shield for RaspberryPi
 - Integrated small battery, and on-board battery charger (what type of battery? how many cells? possibly small bike battery?)
   - On power loss, use backup battery to send warning of power outage
 - GSM module for SMS and backup internet
 - All in a single box, using wifi, or even ethernet connection to a router
 - Input voltage range for example 12-36V, which is stepped down to charge the battery and to run Pi. On power loss, able to detect
   main supply is out, but continue working on battery for some time to send last message

### Hub component selection
 - Battery?
 - Charger IC for the battery?
 - GSM module?
 - Power regulation, buck converter?
 - ...

## Node
 - Main processor: ESP32 S3 module
 - Option for adding a battery and a charger - find suitable ICs, and low power LDO regulator
 - How many cells (most likely one cell li-ion, 4.2V charging)
 - Input power from a small solar panel to charge the battery
 - All in a box (except solar cell, to be positioned at best angle)
 - Option to run in ultra low power mode (at least 1 month without any additional charge)
 - WiFi communication to the Hub + Over The Air updates (OTA)
 - Couple of on-board sensors and feedbacks
 - As many GPIO outputs as possible - for external sensors/actuators
 - All comms: SPI, I2C, UART... break out to connectors
 - What actuators will we have? What are the power requirements?
   - H bridge? servo output? DC motor output? Relay/SSR output? What will be on the PCB, and what connected externally within the box
 - Possible sensors: temp, humidity, atm. pressure, light, microwave presence sensor, PIR,
   reed switch for doors, air quality (mostly dust particles), microphone/sound sensor,
   knock-over sensor (when something falls), fire sensor, soil moisture, weight scale...
 - Possible actuators: buzzer, alarm/siren, LEDs, oled/LCD/display, linear actuator, servo motor, relay...

### Node component selection
 - ESP32 S3, which one exactly?
 - Battery?
 - Charger IC for the battery?
 - Low power, low dropoff regulator?
 - Solar panel handling IC?
 - Which breakout connectors? Screw terminals?
 - H bridge? servo output? open drain outputs (so we can have external supply for larger loads, with common ground only)
 - Power pins also available on the breakout
 - All comms: SPI, I2C, UART... break out to connectors
 - Neatly organized and labled all analog or digital outputs/inputs 
 - ...


