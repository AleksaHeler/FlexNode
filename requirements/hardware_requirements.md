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
 - Example sensor inputs (3x 1wire, 3x I2C, 4x ADC, 5x dig in):
   - temp - 1 wire
   - humidity - 1 wire
   - atm. pressure - I2C, 1 wire
   - light - analog input
   - microwave presence sensor - dig in
   - PIR - dig in
   - reed switch for doors - dig in
   - air quality (mostly dust particles) - I2C
   - microphone/sound sensor - analog input
   - knock-over sensor (when something falls) - dig in
   - fire sensor - dig in/ analog input
   - soil moisture - analog input
   - weight scale - I2C
 - Example actuator outputs (5x open drain, 1x PWM, I2C):
   - buzzer - dig out, open drain
   - alarm/siren - open drain
   - LEDs - dig out, open drain
   - oled/LCD/display - I2C
   - linear actuator - open drain
   - servo motor - PWM output
   - relay - open drain
 - Based on examples:
   - 6x digital inputs/outpus (with 1wire support)
   - 4x analog inputs
   - 4x open drain, high power outputs
   - 2x PWM, servo outputs

### Node component selection
 - [ESP32 S3 WROOM 1 N16R8](https://www.lcsc.com/product-detail/WiFi-Modules_Espressif-Systems-ESP32-S3-WROOM-1-N16R8_C2913202.html)
 - Battery management IC - [MCP73831T](https://www.lcsc.com/product-detail/span-style-background-color-ff0-Battery-span-Management_Microchip-Tech-MCP73831T-2ACI-OT_C424093.html)
 - LDO, low quiescent current 3v3 regulator - [TLV70433DBVR](https://www.lcsc.com/product-detail/Voltage-Regulators-Linear-Low-Drop-Out-span-style-background-color-ff0-LDO-span-Regulators_Texas-Instruments-TLV70433DBVR_C94917.html)
 - Low-Side Gate Driver for open drain outputs - [UCC27517](https://www.lcsc.com/product-detail/Gate-Drivers_Texas-Instruments-UCC27517DBVR_C99395.html)
 - Battery: 1 cell LiIon
 - Screw terminals
 - Battery

