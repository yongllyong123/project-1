# project-1

## Team Members:
Yong Zhao & Dan Lynch

## High-level Block Diagram
![block_diagram_0](/images/block_diagram_0.png)

Each Arduino UNO communicates with two [HC-SR04](https://github.com/sparkfun/HC-SR04_UltrasonicSensor) ultrasonic distance sensors. Each Arduino publishes distance information from each of its sensors (along with status info, TBD) to a web page, using a WiFi adapter (part TBD). An Android app ([CarSensor](/CarSensor)) subscribes to the web page for distance information.

Critical to-do:
* Decide where to fuse sensor data to determine a car's presence/absence. Should this happen on the web page or in the Android app? Factors to consider are computation cost and timeliness.
