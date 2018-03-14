/************************************************************
ESP8266_Shield_Demo.h
SparkFun ESP8266 AT library - Demo
Jim Lindblom @ SparkFun Electronics
Original Creation Date: July 16, 2015
https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

This example demonstrates the basics of the SparkFun ESP8266
AT library. It'll show you how to connect to a WiFi network,
get an IP address, connect over TCP to a server (as a client),
and set up a TCP server of our own.

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  ESP8266 WiFi Shield Version: 1.0

This code is released under the MIT license.

Distributed as-is; no warranty is given.
************************************************************/

//////////////////////
// Library Includes //
//////////////////////
// SoftwareSerial is required (even you don't intend on
// using it).
#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>

//////////////////////////////
// HC-SR04 Settings //////////
//////////////////////////////
const int TRIG_PIN1 = 4;
const int ECHO_PIN1 = 5;
const int TRIG_PIN2 = 6;
const int ECHO_PIN2 = 7;
const unsigned int MAX_DIST = 23200; // Anything over 400 cm (23200 us pulse) is "out of range"
int count=0; // 计数
int empty=1; //判断车位为空
unsigned long t1;
unsigned long t2;
unsigned long pulse_width_1;
unsigned long t3;
unsigned long t4;
unsigned long pulse_width_2;
float cm_1 = 0;
float cm_2 = 0;
int a_1, a_2;

// sensor requirement function:
boolean requirement( float centimeters ) {
  if( centimeters < 50 ) {
    return 1;
  } else {
    return 0;
  }
}

// HC-SR04 #1 read:
float HC_1_read(void) {
  float centimeters = 0;
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
  pulse_width_1 = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  centimeters = pulse_width_1 / 58.0;

  // Print out results
  if ( pulse_width_1 > MAX_DIST ) {
  } else {
  }

  return centimeters;
}

// HC-SR04 #2 read:
float HC_2_read(void) {
  float centimeters = 0;
  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN2, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN2) == 0 );
  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t3 = micros();
  while ( digitalRead(ECHO_PIN2) == 1);
  t4 = micros();
  pulse_width_2 = t4 - t3;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  centimeters = pulse_width_2 / 58.0;

  // Print out results
  if ( pulse_width_2 > MAX_DIST ) {
  } else {
  }

  return centimeters;
}

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
const char mySSID[] = "Device-Northwestern";
const char myPSK[] = "";

//////////////////////////////
// ESP8266Server definition //
//////////////////////////////
// server object used towards the end of the demo.
// (This is only global because it's called in both setup()
// and loop()).
ESP8266Server server = ESP8266Server(80);

// Global variable for ESP8266 MAC address:
char myMacAddr[20];

//////////////////
// HTTP Strings //
//////////////////
const char destServer[] = "10.105.128.210";
const String htmlHeader = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Connection: close\r\n\r\n"
                          "<!DOCTYPE HTML>\r\n"
                          "<html>\r\n";

String httpRequest = "( ͡° ͜ʖ ͡°)\n";

// All functions called from setup() are defined below the
// loop() function. They modularized to make it easier to
// copy/paste into sketches of your own.
void setup()
{
  // Serial Monitor is used to control the demo and view
  // debug information.
  Serial.begin(9600);
  
  // HC-SR04 setup:
  pinMode(TRIG_PIN1, OUTPUT);
  digitalWrite(TRIG_PIN1, LOW);
  pinMode(TRIG_PIN2, OUTPUT);
  digitalWrite(TRIG_PIN2, LOW);

  cm_1 = 0;
  cm_2 = 0;
  a_1 = 0;
  a_2 = 0;
  
//  serialTrigger(F("Press any key to begin."));
  delay(1000);

  // initializeESP8266() verifies communication with the WiFi
  // shield, and sets it up.
  initializeESP8266();

  // Display MAC address:
  int rc = esp8266.localMAC(myMacAddr);
  Serial.print("My MAC address is: [");
  char i = 0;
  for(i = 0; i<17; i++) {
    Serial.print(myMacAddr[i]);
    }
  Serial.println("]");

  // connectESP8266() connects to the defined WiFi network.
  connectESP8266();

  // displayConnectInfo prints the Shield's local IP
  // and the network it's connected to.
  displayConnectInfo();

//  serialTrigger(F("Press any key to connect client."));
  delay(1000);
  clientDemo();

//  serialTrigger(F("Press any key to test server."));
  delay(1000);
  serverSetup();
}

void loop()
{
  char buffer[20];
  cm_1 = HC_1_read();
  cm_2 = HC_2_read();
  a_1 = requirement(cm_1);
  a_2 = requirement(cm_2);
  sprintf(buffer,"%d %d\n",a_1, a_2);
  httpRequest = String(buffer);
  clientDemo();
  delay(2000);
}

void initializeESP8266()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int test = esp8266.begin();
  if (test != true)
  {
    Serial.println(F("Error talking to ESP8266."));
    errorLoop(test);
  }
  Serial.println(F("ESP8266 Shield Present"));
}

void connectESP8266()
{
  // The ESP8266 can be set to one of three modes:
  //  1 - ESP8266_MODE_STA - Station only
  //  2 - ESP8266_MODE_AP - Access point only
  //  3 - ESP8266_MODE_STAAP - Station/AP combo
  // Use esp8266.getMode() to check which mode it's in:
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_STA)
  { // If it's not in station mode.
    // Use esp8266.setMode([mode]) to set it to a specified
    // mode.
    retVal = esp8266.setMode(ESP8266_MODE_STA);
    if (retVal < 0)
    {
      Serial.println(F("Error setting mode."));
      errorLoop(retVal);
    }
  }
  Serial.println(F("Mode set to station"));

  // esp8266.status() indicates the ESP8266's WiFi connect
  // status.
  // A return value of 1 indicates the device is already
  // connected. 0 indicates disconnected. (Negative values
  // equate to communication errors.)
  retVal = esp8266.status();
  if (retVal <= 0)
  {
    Serial.print(F("Connecting to "));
    Serial.println(mySSID);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.
    retVal = esp8266.connect(mySSID, myPSK);
    if (retVal < 0)
    {
      Serial.println(F("Error connecting"));
      errorLoop(retVal);
    }
  }
}

void displayConnectInfo()
{
  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.
  int retVal = esp8266.getAP(connectedSSID);
  if (retVal > 0)
  {
    Serial.print(F("Connected to: "));
    Serial.println(connectedSSID);
  }

  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
  Serial.print(F("My IP: ")); Serial.println(myIP);
}

void clientDemo()
{
  // To use the ESP8266 as a TCP client, use the
  // ESP8266Client class. First, create an object:
  ESP8266Client client;

  // ESP8266Client connect([server], [port]) is used to
  // connect to a server (const char * or IPAddress) on
  // a specified port.
  // Returns: 1 on success, 2 on already connected,
  // negative on fail (-1=TIMEOUT, -3=FAIL).
  int retVal = client.connect(destServer, 8080);
  if (retVal <= 0)
  {
    Serial.println(F("Failed to connect to server."));
    return;
  }

//  client.println("Send this data to server"

  // print and write can be used to send data to a connected
  // client connection.
  client.print(httpRequest);

  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char

  // connected() is a boolean return value - 1 if the
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.
}

void serverSetup()
{
  // begin initializes a ESP8266Server object. It will
  // start a server on the port specified in the object's
  // constructor (in global area)
  server.begin();
  Serial.print(F("Server started! Go to "));
  Serial.println(esp8266.localIP());
  Serial.println();
}

void serverDemo()
{  
  // available() is an ESP8266Server function which will
  // return an ESP8266Client object for printing and reading.
  // available() has one parameter -- a timeout value. This
  // is the number of milliseconds the function waits,
  // checking for a connection.
  int server_timeout = 500;
  ESP8266Client client = server.available(server_timeout);

  if (client)
  {
    Serial.println(F("Client Connected!"));
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank)
        {
          Serial.println(F("Sending HTML page"));
          // send a standard http response header:
          client.print(htmlHeader);
          String htmlBody;
          
//          cm = HC_read();
          
          htmlBody += "cm: ";
          htmlBody += String(cm_1);
          htmlBody += "<br>\n";
          //htmlBody += "empty: ";
          //htmlBody += vehicle_detect();
          htmlBody += "<br>\n";
          htmlBody += "</html>\n";
          client.print(htmlBody);
//          cm++;
          break;
        }
        if (c == '\n')
        {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println(F("Client disconnected"));
  }
}

// errorLoop prints an error code, then loops forever.
void errorLoop(int error)
{
  Serial.print(F("Error: ")); Serial.println(error);
  Serial.println(F("Looping forever."));
  for (;;)
    ;
}

// serialTrigger prints a message, then waits for something
// to come in from the serial port.
void serialTrigger(String message)
{
  Serial.println();
  Serial.println(message);
  Serial.println();
  while (!Serial.available())
    ;
  while (Serial.available())
    Serial.read();
}
