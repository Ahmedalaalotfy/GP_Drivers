#include <Wire.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <EEPROM.h>
#include <Servo.h>

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
#define POT_MAX_RIGHT_VALUE 850
#define POT_MAX_LEFT_VALUE 620
#define POT_CENTRE_VALUE 746


#define FORWARD  0
#define REVERSE  1

int i;
Servo SpeedServo;  // Create a servo object to control a servo

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

///////////////  Pin Definations  //////////////////////

const int stepPin = 2;
const int Direction_Switch_Pin = 3;
const int Red_LED_Relay_Pin = 4;
const int dirPin =   5;
const int enPin =    6;
const int Auto_Park_Pin = 7;
const int Direction_Relay_Pin = 8;
const int Motor_Speed_Pin = 9;
const int Pedal_Relay_Pin = 10;
const int Break_Relay_Pin = 11;
const int Buzzer_Relay_Pin = 12;

    //ANALOG PINS
    const int Steering_POT_Pin = A0;
    const int Left_LED_Relay_Pin = A1;
    const int Right_LED_Relay_Pin = A2;
    const int Front_LED_Relay_Pin = A3;
    // A4 - A5 I2C wires


    // const int Acceleration_Pedal_Pin = 70;


    //Relay pins

///////////////  Global variables       //////////////////////

uint16_t Serial_cmd;
int Steering_POT_Value = 0;
unsigned long first = 0;   //for steering functions

int directionState = 0;  
int EEPROMValue = 0;

volatile bool Direction_Flag = FORWARD; //FLAG to indicate Car Direction [0-Forward : 1-Backward]
volatile bool I2cInterruptEnabled = true;  //FLAG
volatile bool newDataAvailable = false; // Flag to indicate whether to process received data

//int I2c_Sending_Flag = 1;

///////////////   Global structures      //////////////////////

struct SerialData {
  short int distance1;
  short int distance2;
  short int distance3;
  short int distance4;
  short int DriverState;
} SerialData1;


SerialData receivedData;

SerialData temp[3];


int sleep_state = 0 ; 
int Parking_state = 0 ; 

///////////////////////////////////////////////////

// ########################## SETUP ##########################

void setup() {

  //---Pin Configurations---

  pinMode(Pedal_Relay_Pin, OUTPUT);
  digitalWrite(Pedal_Relay_Pin, HIGH); //must be HIGH at the beginning of setup to avoid active low relay switching problem at rebooting process

    pinMode(Break_Relay_Pin, OUTPUT);
  digitalWrite(Break_Relay_Pin, HIGH); //must be HIGH at the beginning of setup to avoid active low relay switching problem at rebooting process

    pinMode(Buzzer_Relay_Pin, OUTPUT);
  digitalWrite(Buzzer_Relay_Pin, HIGH); //must be HIGH at the beginning of setup to avoid active low relay switching problem at rebooting process

      pinMode(Red_LED_Relay_Pin, OUTPUT);
  digitalWrite(Red_LED_Relay_Pin, HIGH); //must be HIGH at the beginning of setup to avoid active low relay switching problem at rebooting process

        pinMode(Left_LED_Relay_Pin, OUTPUT);
  digitalWrite(Left_LED_Relay_Pin, HIGH); //must be HIGH at the beginning of setup to avoid active low relay switching problem at rebooting process

        pinMode(Right_LED_Relay_Pin, OUTPUT);
  digitalWrite(Right_LED_Relay_Pin, HIGH); //must be HIGH at the beginning of setup to avoid active low relay switching problem at rebooting process

        pinMode(Front_LED_Relay_Pin, OUTPUT);
  digitalWrite(Front_LED_Relay_Pin, HIGH); //must be HIGH at the beginning of setup to avoid active low relay switching problem at rebooting process

     //Active High relay
  pinMode(Direction_Relay_Pin, OUTPUT);
  digitalWrite(Direction_Relay_Pin, LOW); 

  //Steering Stepper motor driver pins configuration
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
    //disable steering
    digitalWrite(enPin, HIGH);
    

  //Other pins configuration

  pinMode(Direction_Switch_Pin, INPUT_PULLUP);
  pinMode(Auto_Park_Pin, INPUT_PULLUP);


  SpeedServo.attach(Motor_Speed_Pin);  // Attach the servo to the specified pin
  SpeedServo.write(178);
  Serial.println(SpeedServo.read());

  //---Serial Communication Setup---

  Serial.begin(9600);                // serial baud rate (pins 0,1)


  // Initialize I2C communication as slave with address 8
  Wire.begin(8);

  // Register receive event handler
  Wire.onReceive(receiveEvent);
  //Wire.onRequest(requestEvent); // Register the request event
    delay(5);

        Serial.println("/////////////  ////////////////");
    Serial.println("Welcome Again...");

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

  //attach inturrupt ISR to EXTI Pin 
  attachInterrupt(digitalPinToInterrupt(Direction_Switch_Pin), directionInterrupt, CHANGE);


//Enable_Auto_Drive();

Serial.println(SpeedServo.read());
delay(4000);
}


// ########################## LOOP ##########################

void loop(void) {


  Control_Direction();
GUI_command_test();

//Driver_Not_Responding_Algorithm();

//  Send_Steering_Angle(0);
//  delay(1000);
//   Send_Steering_Angle(100);
//  delay(1000);
//   Send_Steering_Angle(-100);
//  delay(1000);
 
if(SpeedServo.read() < 150)
{
    SpeedServo.write(178);
}
    Send_Motor_Speed(0);
  delay(50);
}

//Auto_Parking_Algorithm();






// ########################## Functions ##########################

// Interrupt handler for direction switch pin  [Direction_Switch_Pin = 3]
void directionInterrupt() {

  int newState = digitalRead(Direction_Switch_Pin);
  if (newState != directionState) {
    directionState = newState;

    //Update EEPROM Direction Value
    EEPROM.write(0, newState);

     Direction_Flag = 1;

  }
}



//********************************************************************************************************************
//*************************************** Car Functionality Control Functions  ***************************************
//********************************************************************************************************************



//todo change every one line function to a function like macro

void Enable_Auto_Drive()
{
digitalWrite(Pedal_Relay_Pin, LOW);
}

void Disable_Auto_Drive()
{
  digitalWrite(Pedal_Relay_Pin, HIGH);
}

void Enable_SoftBreak()
{
  digitalWrite(Break_Relay_Pin, LOW);
}

void Disable_SoftBreak()
{
  digitalWrite(Break_Relay_Pin, HIGH);
}

void change_Direction()
{
  Direction_Flag = 1;
  Control_Direction();
}

void Relay_test()
{

    if(digitalRead(Auto_Park_Pin) == LOW)
  {

    
   digitalWrite(Front_LED_Relay_Pin, LOW);
   delay(4000);

      digitalWrite(Red_LED_Relay_Pin, LOW);
   delay(2000);



    for( i = 0 ;i<5 ;i++)
    {
      digitalWrite(Right_LED_Relay_Pin, LOW);
   delay(800);
         digitalWrite(Right_LED_Relay_Pin, HIGH);
   delay(800);
    }


        for( i = 0 ;i<5 ;i++)
    {
      digitalWrite(Left_LED_Relay_Pin, LOW);
      digitalWrite(Buzzer_Relay_Pin, LOW);
      
   delay(700);
         digitalWrite(Left_LED_Relay_Pin, HIGH);
         digitalWrite(Buzzer_Relay_Pin, HIGH);
   delay(700);
    }

      digitalWrite(Red_LED_Relay_Pin, HIGH);
   delay(1000);

      digitalWrite(Front_LED_Relay_Pin, HIGH);
   delay(500);


  }

}

void GUI_command_test(){
  switch(receivedData.DriverState)
  {

    case 0 :

     sleep_state = 0 ;

     break ; 

    case 1 : 

     for( i = 0 ;i<2 ;i++)
    {
      digitalWrite(Buzzer_Relay_Pin, LOW);
      
   delay(500);
         digitalWrite(Buzzer_Relay_Pin, HIGH);
   delay(700);
    }
    break; 

    case 2 : 
    sleep_state = 1 ; 
    Driver_Not_Responding_Algorithm();

    break; 

    case 3 : 
    Parking_state = 1 ;

    break ; 

    case 4 : 

      digitalWrite(Left_LED_Relay_Pin, LOW);
   delay(700);
         digitalWrite(Left_LED_Relay_Pin, HIGH);
   delay(700);

    break ;

    case 5 : 

    digitalWrite(Left_LED_Relay_Pin, HIGH);
    break ;


    case 6 : 

  digitalWrite(Right_LED_Relay_Pin, LOW);
   delay(700);
         digitalWrite(Right_LED_Relay_Pin, HIGH);
   delay(700);

    break ;

    case 7 : 

    digitalWrite(Right_LED_Relay_Pin, HIGH);
    break ;

    case 8 : 
   digitalWrite(Front_LED_Relay_Pin, LOW);

    break; 

    case 9 : 
   digitalWrite(Front_LED_Relay_Pin, HIGH ) ;

    break ; 

    default : 
    break ; 

  }
}


// ##################################### END #####################################