#include <Wire.h>

int slaveAddress = 8;
int Sending_Flag = 1;

//Note that 1 = Front , 2 = Back , 3 =  Right, 4 = Left

  // Define the pins for the ultrasonic sensors
  const int trigPin1 = 8;
  const int echoPin1 = 9;

  const int trigPin2 = 10;
  const int echoPin2 = 11;

  const int trigPin3 = 6;
  const int echoPin3 = 7;

  const int trigPin4 = 4;
  const int echoPin4 = 5; 


  struct SerialData {
  short int distance1;
  short int distance2;
  short int distance3;
  short int distance4;
  short int DriverState ; 
}SerialData_Send;


short int Tab_CMD  = 0; 


  //////////////////////////////////////////



void setup() {

  //---Serial Communication Setup---
    Serial.begin(9600);

    // Initialize I2C communication as master
    Wire.begin();


 //---Pin Configurations---
    
    // Set the trigPin as output and echoPin as input
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);
    pinMode(trigPin4, OUTPUT);
    pinMode(echoPin4, INPUT);

    pinMode(13, OUTPUT);


}

void loop() {


  readSensors();

  if(SerialData_Send.DriverState == 8)
  {
    digitalWrite(13, 1);
  }else{
    digitalWrite(13, 0);
  }



  Wire.beginTransmission(slaveAddress); // transmit to device #8

  // Send data
  sendData(SerialData_Send);

  // Send data to I2C slave with address 8
  Wire.endTransmission();

  delay(40);

}




