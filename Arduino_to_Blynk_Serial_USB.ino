/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  =>
  =>          USB HOWTO: http://tiny.cc/BlynkUSB
  =>

  This sketch shows how to write values to Virtual Pins

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT SwSerial

#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX
    
#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c2sm-B7M3qfYZ2LfHU6n2pUa49RHMY-6";

BlynkTimer timer;

// Set your LED pins here
const int ledPin = 13;
int ledState = LOW;

// Set variables to calculate averages values
float sum_avg1 = 0, sum_avg2 = 0;
unsigned int count_avg = 0;

// Every time we connect to the cloud...
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V1);
}

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V1
BLYNK_WRITE(V1)
{
  ledState = param.asInt(); // assigning incoming value from pin V1 to a variable
  digitalWrite(ledPin, ledState);
}

// This function read from the ADC each second to calculate an average value
void myData()
{
    // Acumulate the ADC measurements each second, 
    // to calculate latter and average value each 5 seconds
    sum_avg1 += analogRead(A0);
    sum_avg2 += analogRead(A1);
    // Count to calculate the number of acumulated measurements
    count_avg++;
}

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
  Blynk.virtualWrite(V6, sum_avg1/count_avg);
  Blynk.virtualWrite(V7, sum_avg2/count_avg);
  sum_avg1 = 0;
  sum_avg2 = 0;
  count_avg = 0;
}

void setup()
{
  // Debug console
  SwSerial.begin(9600);

  // Setup LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  // Setup a function to be called every 5 seconds
  timer.setInterval(5000L, myTimerEvent);

  // Setup a function to adquire every 1 second
  timer.setInterval(1000L, myData);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  // Here you can put your own code, remember to never use delays functions
}
