/*
-------------------------------------------------------------------------------------
Transmitter Code

microsecondsToCentimeters converts the travel time (μs) of an ultrasonic wave to half
of the distance it has covered (cm). This effectively obtains the distance between an
ultrasonic sensor and an object.

getDist1 & getDist2 get the distance (cm) of an object from each ultrasonic sensor
-------------------------------------------------------------------------------------
*/

// Import necessary libraries for RadioHead
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
  #include <SPI.h> 
#endif

// Trig & Echo Pins for US Sensor
const int trigPin1 = 5;
const int echoPin1 = 4;
const int trigPin2 = 7;
const int echoPin2 = 6;

// RF Transmitter (Set to Pin 2)
RH_ASK driver(1000, 4, 2, 0); 

void setup()
{
    // Set-up RF Transmitter
    #ifdef RH_HAVE_SERIAL
      Serial.begin(115200);   
    #endif
    if (!driver.init())
      #ifdef RH_HAVE_SERIAL
        Serial.println("init failed");
      #else;
      #endif

    // Set up US Sensors
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
}

void loop()
{
    // Define messages to be sent
    const char * positive = "pos";
    const char * negative = "neg";

    // Obtain distances and send the appropriate message
    int distance1 = getDist1();
    int distance2 = getDist2();
    if (distance1 <= 10 && distance2 <= 10)
    {
      driver.send((uint8_t *)positive, strlen(positive));
      driver.waitPacketSent();
      delay(700);
    }
    else
    {
      driver.send((uint8_t *)negative, strlen(negative));
      driver.waitPacketSent();
      delay(700);
    }
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

int getDist1()
{
   long duration, cm;

   // Send pulse from transmitter
   digitalWrite(trigPin1, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin1, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin1, LOW);

   // Get travel time between sensor and object, then convert to distance
   duration = pulseIn(echoPin1, HIGH);
   cm = microsecondsToCentimeters(duration);
   return (int) cm;
}

int getDist2()
{
   long duration, cm;

   // Send pulse from transmitter
   digitalWrite(trigPin2, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin2, LOW);

   // Get travel time between sensor and object, then convert to distance
   duration = pulseIn(echoPin2, HIGH);
   cm = microsecondsToCentimeters(duration);
   return (int) cm;
}
