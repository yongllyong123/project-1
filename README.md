# CarSensor
Internet-of-Things application for smart curbside parking

## Team Members:
Yong Zhao & Dan Lynch

## Relevant Software

1. [SensorClient.ino](/SensorClient/SensorClient.ino):

  Arduino code that interfaces with HC-SR04 sensors and ESP8266 WiFi adapter to send info to server

2. [TCPserver.js](/NodeJS/TCPserver.js):

  Node.js script that implements a server on my laptop
  
3. [CarSensor](/CarSensor):

  Android app that communicates with server

## Introduction and Project Scope
This project is a prototype for a Internet-of-Things system for detecting if a curbside parking space is occupied.
* The system uses modules consisting of a pair of distance sensors and a microcontroller with WiFi capability to stream distance-related information to a web server.
  * The Arduino compares each measured distance to a threshold and sends a '1' to the server if the distance is below the threshold or a '0' otherwise.
* When new sensor information arrives, the web server uses a simple algorithm to estimate if a car is in the parking space (see the [pseudocode below](#vehicle-detection-algorithm))
* The end user uses an app to ask the server if it has detected a car in the parking space.

This prototype is scalable to a large number of parking spaces. Each microcontroller can have a unique identifier (from 1 to N, for N parking spaces), and each sensor connected to that microcontroller can have its own unique identifier (A or B), so every sensor in the entire network has a unique identifier and can therefore provide information about a unique parking space.

## High-level Block Diagram
![block_diagram_0](/images/block_diagram_0.png)

Each Arduino UNO communicates with two [HC-SR04](https://github.com/sparkfun/HC-SR04_UltrasonicSensor) ultrasonic distance sensors. Each Arduino publishes distance information from each of its sensors  to a web server, using an [ESP8266 WiFi adapter](https://www.sparkfun.com/products/13287). An Android app ([CarSensor](/CarSensor)) subscribes to the web page for distance information.

## Sensing distance with the HC-SR04
The HC-SR04 is similar to old-school sonar: when its TRIG pin is toggled HIGH, it emits a ping, and sets its ECHO pin HIGH once the ping bounces off an object and returns back. By timing the delay between sending and receiving a ping, we can estimate how far away the object is.

## Vehicle Detection Algorithm
The actual process for detecting if a vehicle is present in a parking space is simple. Here's a pseudocode representation of that process:

```
initialize counter;
if  ((Arduino1sensorA < threshold) && (Arduino2sensorB < threshold)) {
  increment counter;
  if (counter > countThreshold) {
    vehicle_detected = TRUE;
  }
  else {
    vehicle_detected = FALSE;
  }
}
```
The actual implementation of this algorithm is in [`TCPserver.js`](/NodeJS/TCPserver.js), in a function cleverly named `vehicle_detect()`.
