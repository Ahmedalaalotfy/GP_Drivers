#include <Wire.h>
#include <SoftwareSerial.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <EEPROM.h>

// ***********************************************
// **   Arduino controlled CAR project code     **
// **                                           **
// **                                           **
// **                                           **
// **                                           **
// ***********************************************

// ########################## DEFINES ##########################

//Steering potentiometer defines
#define num 600
#define POT_MAX_RIGHT_VALUE 900
#define POT_MAX_LEFT_VALUE 610
#define POT_CENTRE_VALUE 746


#define FORWARD  0
#define REVERSE  1

/////////////////////////////////////
/////////////////////////////////////


SoftwareSerial dd(5,6);
int byteCount;
//pins definations
const int stepPin = 2;
const int Direction_Switch_Pin = 3;
const int dirPin =   A3;
const int enPin =    A4;
//const int Auto_Park_EN = 7;
const int Motor_Speed_Pin = 9;
const int Buzzer_Pin = 11;
const int Direction_Relay_Pin = 8;


int Steering_POT_Pin = A0;
int Acceleration_Pedal_Pin = A1;


///////////////  Global variables       //////////////////////

uint16_t Serial_cmd;
int Steering_POT_Value = 0;
unsigned long first = 0;

uint16_t fspeed = -100;  //Set initial forward speed
uint16_t bspeed = 100;   //Set initial Backward speed

int directionState = 0;
int EEPROMValue = 0;
int Direction_Flag = 0;

///////////////   Global structures      //////////////////////

struct SerialData {
  short int distance1;
  short int distance2;
  short int distance3;
  short int distance4;
  short int DriverState;
} SerialData1;


///////////////////////////////////////////////////



// ########################## SETUP ##########################

void setup() {
  Serial.begin(9600);                    // serial baud rate (pins 0,1)
  dd.begin(9600);
  Serial.println("Welcome Not Agina");


  // // Initialize I2C communication as slave with address 8
  // Wire.begin(8);
  // // Register receive event handler
  // Wire.onReceive(receiveEvent);


  //Steering Stepper motor driver pins configuration
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);

    //disable steering
    digitalWrite(enPin, HIGH);


  pinMode(Direction_Relay_Pin, OUTPUT);
  digitalWrite(Direction_Relay_Pin, HIGH);


  //Other pins configuration
  pinMode(Buzzer_Pin, OUTPUT);


  // pinMode(Auto_Park_EN, INPUT_PULLUP);
  pinMode(Direction_Switch_Pin, INPUT_PULLUP);


///////////////////////////// EXTI and EEPROM Setup  ///////////////////////////////////////

 // Read switch state into directionState variable
  directionState = digitalRead(Direction_Switch_Pin);
  
  // Read value in EEPROM and display to serial monitor
  EEPROMValue = EEPROM.read(0);
  Serial.print("EEPROM Value: ");
  Serial.println(EEPROMValue);
  
  if (directionState == 0 && EEPROMValue == 1) {
    // Make it identical to the value in EEPROM
    
      EEPROMValue = directionState ;
      EEPROM.write(0, EEPROMValue);
      
      Direction_Flag = 1;

  }

  // Compare it to last value in EEPROM
  if (directionState != EEPROMValue) {
    // Make it identical to the value in EEPROM
    
      EEPROMValue = directionState ;
      EEPROM.write(0, EEPROMValue);

      Direction_Flag = 1;

  }
  

  // Display both values to Serial Monitor
  Serial.print("Direction State: ");
  Serial.println(directionState);
  Serial.print("Last EEPROM Value: ");
  Serial.println(EEPROMValue);


digitalWrite(Direction_Relay_Pin, HIGH);

  //attach inturrupt ISR to EXTI Pin 
  attachInterrupt(digitalPinToInterrupt(Direction_Switch_Pin), switchInterrupt, CHANGE);
}


// ########################## LOOP ##########################

void loop(void) {


  Control_Direction();

    //Control the motors depending on acceleration pedal reading
    Pedal_Motor_Drive();


if(dd.available())
{
    // Function to send data over I2C

 if (byteCount == sizeof(SerialData)) {
    SerialData receivedData;
    receiveData(receivedData);
    processReceivedData(receivedData); 
  } else {
    // Handle incorrect data length
    while (Wire.available()) {
      Wire.read();
    }
}


  delay(150);
}

}
// ########################## Functions ##########################


void switchInterrupt() {
  // Interrupt handler for switch pin
  int newState = digitalRead(Direction_Switch_Pin);
  if (newState != directionState) {
    directionState = newState;

    //Update EEPROM Direction Value
    EEPROM.write(0, newState);

     Direction_Flag = 1;

  }
}


void Control_Direction()
{

  if(Direction_Flag == 1)
  {
          // Power the Relay for 500ms
    digitalWrite(Direction_Relay_Pin, LOW);
    delay(500);
    digitalWrite(Direction_Relay_Pin, HIGH);

    Direction_Flag = 0;
  }

}

void Forward()
{

}



// ########################## END ##########################




// ########################## END ##########################
void BUZZ(){

    // if (SerialData1.DriverState == 2) {

  //   digitalWrite(Buzzer_Pin, HIGH);

  // } else if (SerialData1.DriverState == 1) {

  //   digitalWrite(Buzzer_Pin, HIGH);
  // } else {
  //   digitalWrite(Buzzer_Pin, LOW);
  // }
}