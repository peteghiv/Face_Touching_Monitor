/*
-------------------------------------------------------------------------------------
Receiver Code
-------------------------------------------------------------------------------------
*/


// Include necessary libraries for RadioHead
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
  #include <SPI.h>
#endif

// Include necessary libraries for Servo
#include <ServoTimer2.h>

// Servo Motor on pin 10
const int servopin = 10;
ServoTimer2 myservo;

// RF Receiver (Set to Pin 2)
RH_ASK driver(1000, 2, 4, 0);

void setup()
{
    // Set-up RF Receiver
    #ifdef RH_HAVE_SERIAL
      Serial.begin(115200);    
    #endif
    if (!driver.init())
      #ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
      #else;
      #endif

    // Set-up Servo Motor
    myservo.attach(servopin);
    myservo.write(750);
}

void loop()
{
    // Generate array to receive data
    uint8_t buf[3];
    uint8_t buflen = sizeof(buf);
    // Receive message
    if (driver.recv(buf, &buflen)) driver.printBuffer("Got:", buf, buflen);
    
    // Convert message to ASCII characters
    String ans = "";
    for (int i = 0; i < 3; ++i) ans += (char)buf[i];
    
    // Turn Servo Motor if the message received is "pos"
    if (ans == "pos")
    {
      for (int i = 1750; i > 750; i -= 50)
      {
        myservo.write(i);
        delay(15); 
      }
      delay(150);
      for (int i = 750; i < 1750; i += 50)
      {
        myservo.write(i);
        delay(15);
      }
      delay(150);
    }
}