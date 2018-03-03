/**
 * HC-SR04 Demo
 * Demonstration of the HC-SR04 Ultrasonic Sensor
 * Date: August 3, 2016
 * 
 * Description:
 *  Connect the ultrasonic sensor to the Arduino as per the
 *  hardware connections below. Run the sketch and open a serial
 *  monitor. The distance read from the sensor will be displayed
 *  in centimeters and inches.
 * 
 * Hardware Connections:
 *  Arduino | HC-SR04 
 *  -------------------
 *    5V    |   VCC     
 *    7     |   Trig     
 *    8     |   Echo     
 *    GND   |   GND
 *  
 * License:
 *  Public Domain
 */

// Pins
const int TRIG_PIN1 = 6;
const int ECHO_PIN1 = 7;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void setup() {

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN1, OUTPUT);
  digitalWrite(TRIG_PIN1, LOW);
  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

int count=0; // 计数
int empty=1; //判断车位为空
unsigned long t1;
unsigned long t2;
unsigned long pulse_width;
float cm;

//requirement function
  boolean requirement( float cm ) {
    if( cm < 50 ) {
      return 1;
    } else {
      return 0;
    }
    
  }

void loop() {
  
  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN1, LOW);
  
  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN1) == 0 );
  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN1) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed 
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.print("1 Out of range          ");
  } else {
    Serial.print("sensor1: ");
    Serial.print(cm);
    Serial.println(" cm \t");
  }

  
  /*determine when to send a signal 
   * 1. when the sensor sense the distance meet the requirement 
   *     for ten times consecutively(10 seconds), it means there 
   *     is a car in the parking spot.
   * 2. when the seonor sense distance out of requirement
   *     for five times consecutively(5 seconds), it means the car
   *     has left.
  */
  if( empty == 1 ) { //parking spot is not used
    if( requirement( cm ) == 1 ) {// meeting the requirement
      count++;//计数
      if( count == 11 ) {
        Serial.println(" the car is arrived");//send a signal for parking
        count = 0;
        empty = 0; //parking spot is used
      } else {
        
      }
    } else {
      count = 0;

    } 
  } else { //parking spot is used
    if ( requirement( cm ) == 0 ) {
      count++;
      if( count == 6){
        Serial.println(" the car is leaving");//send a signal for removing
        count = 0;
        empty = 1;
      } else {
        
      } 
    } else {
      count = 0;
    }
  }
  // Wait at least 60ms before next measurement
  delay(1000);
} 
