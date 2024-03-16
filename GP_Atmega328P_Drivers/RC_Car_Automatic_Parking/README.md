# Automatic Parking Test Algorithm on Small RC Car

<img align="center" alt="Simulation" width="800" src="https://github.com/Ahmedalaalotfy/GP_Drivers/blob/main/GP_Atmega328P_Drivers/RC_Car_Automatic_Parking/Pics/1.1.png">

<img align="center" alt="Simulation" width="800" src="https://github.com/Ahmedalaalotfy/GP_Drivers/blob/main/GP_Atmega328P_Drivers/RC_Car_Automatic_Parking/Pics/2.2.png">

This code is used to control the motion of an RC car using a blutooth module (HC-05) to interface with a mobile application.
The code contains the function of Automatic free parking space detection and Automatic parking.
all variable in the code are based on the used project components , test environment , car dimensions , and some specific assumptions.

## Key assumptions guiding the project include:

1- The minimum distance between the car and parking walls/sides is set at 40cm.

2- The width of the parked car falls within the range of 15 to 25cm.

3- The length of a free parking spot must be at least twice the length of the car (which is 25cm in our case).

4- The batteries powering the car motors have a capacity of 12V.

5- The placement and orientation of the 4 ultrasonic sensors are as depicted in the provided images.

6- The parking detection mechanism is designed to function exclusively for the right side of the car.

### #It's crucial to note that any alterations to these assumptions or conditions will necessitate corresponding adjustments within the code.
