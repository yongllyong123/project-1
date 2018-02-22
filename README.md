# project-1

## Team Members:
Yong Zhao & Dan Lynch

## Introduction and Project Scope
This project is a prototype for a Internet-of-Things system for detecting if a curbside parking space is occupied.
* The system uses a pair of distance sensors and a microcontroller with WiFi capability to stream measured distances to a web page, potentially served up using the LAMP stack or Node.js.
* The web page periodically compares the measured distances to thresholds that approximate the size of a car.
* If both thresholds are satisfied, the web page starts a timer.
* Once the timer reaches a certain value, the web page notifies the end user, via a smartphone app, that a car is parked in the corresponding parking space.

This prototype is scalable to a large number of parking spaces. Each microcontroller can have a unique identifier (from 1 to N, for N parking spaces), and each sensor connected to that microcontroller can have its own unique identifier (A or B), so every sensor in the entire network has a unique identifier and can therefore provide information about a unique parking space.

## High-level Block Diagram
![block_diagram_0](/images/block_diagram_0.png)

Each Arduino UNO communicates with two [HC-SR04](https://github.com/sparkfun/HC-SR04_UltrasonicSensor) ultrasonic distance sensors. Each Arduino publishes distance information from each of its sensors (along with status info, TBD) to a web page, using a WiFi adapter (part TBD). An Android app ([CarSensor](/CarSensor)) subscribes to the web page for distance information.

Critical to-do:
* Decide where to fuse sensor data to determine a car's presence/absence. Should this happen on the web page or in the Android app? Factors to consider are computation cost and timeliness.

## Vehicle Detection Algorithm
The actual process for detecting if a vehicle is present in a parking space is simple. Here's a pseudocode representation of that process:

```
initialize timer;
if  ((Arduino1.sensorA < threshold) && (Arduino2.sensorB < threshold)) {
  increment timer;
  if (timer > timeThreshold) {
    vehicle_detected = TRUE;
  }
  else {
    vehicle_detected = FALSE;
  }
}
```
