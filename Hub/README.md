# Hub

Raspberry Pi 3 B+, running NodeRed, InfluxDB and Grafana.

## System requirement

![System reference idea](system_idea.png)

## Setup procedure

Taken from [this video](https://www.youtube.com/watch?v=I4alYbgDi4I&list=PL3XBzmAj53RnezxZ_uq8YMymURnnLTqZP&index=9)

- install raspbian lite 64bit
- enable ssh, enter wifi data and username an password
- sudo apt-get update
- sudo apt-get dist-upgrade

- now follow peter scargill's instructions for his script
- apps:
  - quiet
  - prereq
  - mosquitto
  - apache
  - nodenew
  - java
  - phpsysinfo
  - modpass
  - addindex
  - rpiclone
  - log2ram
  - grafana
  - wolfram
  - office

- sudo nano /etc/influxdb/influxdb.conf (HTTP enable, and bind to standard port)
- create admin user
- check if grafana and nodered work
- todo: change from default ports, to use url/grafana and url/nodered (not port :3000, that should be blocked)

- Hearbeat signal / watchdog
  https://www.youtube.com/watch?v=IGB2eRvhvB0&list=PL3XBzmAj53RnezxZ_uq8YMymURnnLTqZP&index=10

- Add users to MQTT (for esp32 that will be connected) with command:
  > cd /etc/mosquitto/
  > mosquitto_passwd -c passwords user
  Now you can use this command to add additional users to the file:
  > mosquitto_passwd -b passwords user password
  And check all the passwords:
  > cat passwords


