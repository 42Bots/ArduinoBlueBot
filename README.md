ArduinoBlueBot
==============

An Arduino compatible differential steering robot controlled from an Android phone via Bluetooth.


Description
===========
This is a differential steering robot that can be controlled from an Android phone via Bluetooth. The robot’s “brain” is Arduino Uno compatible board (an “Arduino on a breadboard”). The robot uses a JY-MCU Bluetooth module for communication with the Android phone. A custom Android app, created with the MIT App Inventor 2 essentially acts as a remote control, sending commands to the Arduino that tell the robot to move forward, reverse, stop or rotate.

The robot also has two ultrasonic sensors (one mounted on the front and one on the back) that are used for obstacle detection. If you try to drive the robot into a wall using your phone remote, the robot will overwrite that command and will stop when it detects an obstacle closer than 10 cm.

The communication between the phone and the Arduino board is very basic at the moment. When you press a button on the phone, it sends a byte over the Bluetooth connection (currently the numbers 0 to 5 are used). The Arduino sketch reads the data on the serial port and acts based on the command (0 is stop, 1 is move forward etc.). This can be easily extended and can go both ways. For example, we can easily display the data from the two ultrasonic distance sensor on the phone.


Brief video demo
================
http://www.youtube.com/watch?v=l7f4o7RvLOU



Parts list
==========
- Arduino Uno (or compatible board)
- JY-MCU Bluetooth Module
- 2 Servos modified for continuous rotation
- 2 HC-SR04 ultrasonic sensors for obstacle detection
- 2 Wheels (salvaged from a printer)
- 2 acrylic places for base
- 2 casters
- Android phone
- 9V battery with case for the Arduino
- 5 AA batteries and case for the servos and other components
- Screws, zip ties and some hot glue to keep it all together
- Protoboard & wires to connect the electronic components
- A 20k and 10k resistors used to make a voltage divider (shift down the Arduino 5V signal on the TX pin to 3.3V needed by the JY-MCU)

Main connections:
=================
- left servo signal to Arduino pin D2
- right servo signal to Arduino pin D4
- front HC-SR04 sensor echo and trigger pin connected to Arduino pin D6
- back HC-SR04 sensor echo and trigger pin connected to Arduino pin D7
- JY-MCU TX pin to the Arduino pin 12 (used as RX pin in SoftWareSerial)
- JY-MCU RX pin through voltage divider and to the Arduino pin  9 (used as TX pin in SoftWareSerial)
- all grounds connected together
- HC-SR04 sensors and the JY-MCU are powered from regulted 5V line
- the continuous rotation servos are powered from a 5 rechargeable AA battery pack

Tutorials on putting the key components together
================================================
- Simple Android apps with App Inventor: 
http://42bots.com/resources/simple-android-apps-with-app-inventor/

- Dirrential steering robot with continuous rotation servos: 
http://42bots.com/tutorials/differential-steering-with-continuous-rotation-servos-and-arduino/

- Connecting an Arduino Uno to an Android phone via Bluetooth: 
http://42bots.com/tutorials/how-to-connect-arduino-uno-to-android-phone-via-bluetooth/

- Using software serial and the JY-MCU: 
http://42bots.com/tutorials/arduino-uno-and-the-jy-mcu-bluetooth-module-with-softwareserial/