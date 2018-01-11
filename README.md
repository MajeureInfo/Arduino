# Arduino object

### Description
This is a prototype to simulate a room in the context of smart buildings management.

### Features
This repository provides 3 separate features as well as some basic tests.

* __ButtonLedPost__ contains a script to send a POST request to the [REST API](https://github.com/MajeureInfo/webProject) in order to update the database when switching the LED from the Arduino object.
* __ButtonMqtt__ contains a script to send a MQTT message to a [CloudMQTT](https://www.cloudmqtt.com/) broker in order to update in real-time other connected user interfaces such as the [website](https://github.com/MajeureInfo/webProjectFront) or the [Android application](https://github.com/MajeureInfo/AndroidApp).
* __ServerSwitch__ contains a script that launches a HTTP server which receives POST messages from the others interfaces and switch the LED when receiving such message. For the moment this has to be made in a local network for routing reasons.
* __test__ contains several basic scripts for testing functionnalities.
