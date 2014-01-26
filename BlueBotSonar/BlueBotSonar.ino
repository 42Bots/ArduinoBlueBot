/*******************************************************************************
Version: 1.3 Using software serial to free up the RX / TX ports on the Uno
Date: December 11th, 2013
Author: Stan
http://42bots.com

The code below was adapted from the following work of smarter people:
http://randomnerdtutorials.wordpress.com/2013/02/24/arduino-control-dc-motor-via-bluetooth/
http://www.instructables.com/id/Success-Using-the-JY-MCU-linvor-Bluetooth-Module/

Useful tips for troubleshooting the Bluetooth module can be found here:
http://forum.arduino.cc/index.php?topic=101452.0
********************************************************************************/

#include <Servo.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

// Ultrasonic sensor configuration
#define SONAR_NUM     2 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 50 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
unsigned int currentSensor = 0;     // Keeps track of which sensor is active.
unsigned long stopDistance = 10;    // stops if an obstacle is detected ahead within that distance in cm

NewPing sonar[SONAR_NUM] = {         // Sensor object array.
  NewPing(6, 6, MAX_DISTANCE),     // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(7, 7, MAX_DISTANCE)
};
 
Servo LeftServo;
Servo RightServo;

int input = 0;            // input from the control device
int flag = 0;             // makes sure that the serial only prints once the input
const int ledPin = 8;     // an LED used to indicate visually when a known 
						   // command is received

const int leftServoPin = 2;   // the control signal for the left servo
const int rightServoPin = 4;  // the control signal for the right servo

SoftwareSerial mySerial(12, 9); // Initialize SoftwareSerial (RX, TX)
 
void setup() 
{ 
  LeftServo.attach(leftServoPin); 
  RightServo.attach(rightServoPin); 
  
  // sets the LED pin as outputs:
  pinMode(ledPin, OUTPUT);
  mySerial.begin(9600);
  
  pingTimer[0] = millis() + 100;           // First ping starts at 100ms, gives time for the Arduino to "chill" before starting.
  for (unsigned int i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
} 
 
 
void loop() 
{ 
    //if some date is sent, reads it and saves it in the input variable
    if(mySerial.available() > 0){     
      input = mySerial.read();   
      flag=0;
    }
    
    for (unsigned int i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
      if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
        pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
        //if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
        sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
        currentSensor = i;                          // Sensor being accessed.
        cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
        sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
      }
    }
    
    // If it receives '5' on the serial port, stop both servos
    if (input == 5) {

        LeftServo.write(90);
        RightServo.write(90);        
        
        if(flag == 0){
          flag = 1;
        }
    }
    // If you receive '1' on the serial port, check the distance to the closest obstacle ahead
    else if (input == 1) {
      
      if (cm[0] > 0 && cm[0] <=stopDistance)
      {
        // stop in order to prevent collision
        LeftServo.write(90);
        RightServo.write(90); 
        digitalWrite(ledPin, HIGH); 
      }
      
      else 
      {
        //move forward if path ahead is clear
        LeftServo.write(0);
        RightServo.write(180);
        if(flag == 0){
          flag = 1;
        }
      }

    }
    // If you receive '3' on the serial port, rotate right
    else if (input == 3) {
        LeftServo.write(180);
        RightServo.write(180);
        if(flag == 0){
          flag = 1;
        }
        digitalWrite(ledPin, LOW); 
    }
    // If you receive '4' on the serial port, rotate left
    else if (input == 4) {
        LeftServo.write(0);
        RightServo.write(0);
        if(flag == 0){
          flag = 1;
        }
        digitalWrite(ledPin, LOW); 
    }
    // If you receive '2' on the serial port, check the distance to the closest obstacle behind
    else if (input == 2) {
        
      if (cm[1] > 0 && cm[1] <=stopDistance)
      {
        // stop in order to prevent collision
        LeftServo.write(90);
        RightServo.write(90); 
        digitalWrite(ledPin, HIGH); 
      }
        
        
      else {  // path behind is clear, go in reverse
        LeftServo.write(180);
        RightServo.write(0);
        if(flag == 0){
          flag = 1;
        }
        digitalWrite(ledPin, LOW); 
      }
    }
    else {
      digitalWrite(ledPin, LOW);
    }
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

//void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
//  for (unsigned int i = 0; i < SONAR_NUM; i++) {
//    Serial.print(i);
//    Serial.print("=");
//    Serial.print(cm[i]);
//    Serial.print("cm ");
//  }
//}

