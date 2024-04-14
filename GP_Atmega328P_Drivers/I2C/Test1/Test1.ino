#include <Wire.h>
#include <SoftwareSerial.h>

// ***********************************************
// **   Arduino controlled robot project code   **
// **                                           **
// **                                          **
// **                                           **
// **                                           **
// ***********************************************
// INFO:
// • This sketch uses the the Serial Software interface to communicate and send commands to the hoverboard
// • The built-in (HW) Serial interface is used for debugging and visualization. In case the debugging is not needed,
//   it is recommended to use the built-in Serial interface for full speed perfomace.
// • The data packaging includes a Start Frame, checksum, and re-syncronization capability for reliable communication
//
// This code controls the robot wheels,Suction fan(Relay),
// and Neon LEDS(Relay)using HC-05 Blutooth module with smartphone interface.
//
// CONFIGURATION on the hoverboard side in config.h:
// • Option 1: Serial on Right Sensor cable (short wired cable) - recommended, since the USART3 pins are 5V tolerant.
//   #define CONTROL_SERIAL_USART3
//   #define FEEDBACK_SERIAL_USART3
//   // #define DEBUG_SERIAL_USART3
// • Option 2: Serial on Left Sensor cable (long wired cable) - use only with 3.3V devices! The USART2 pins are not 5V tolerant!
//   #define CONTROL_SERIAL_USART2
//   #define FEEDBACK_SERIAL_USART2
//   // #define DEBUG_SERIAL_USART2
// ***********************

// ########################## DEFINES ##########################
#define HOVER_SERIAL_BAUD 115200  // [-] Baud rate for HoverSerial (used to communicate with the hoverboard)
#define SERIAL_BAUD 115200        // [-] Baud rate for built-in Serial (used for the Serial Monitor)
#define START_FRAME 0xABCD        // [-] Start frme definition for reliable serial communication
#define SPEED_MAX 300             // [-] Maximum speed for testing         (-300  to 300)


#define num 600
#define POT_MAX_RIGHT_VALUE 900
#define POT_MAX_LEFT_VALUE 620
#define POT_CENTRE_VALUE 746

// #define DEBUG_RX                     // [-] Debug received data. Prints all bytes to serial (comment-out to disable)


SoftwareSerial HoverSerial(8, 9);  // RX, TX

/////////////////////////////////////
/////////////////////////////////////

//pins definations
const int stepPin = 2;
const int dirPin = 5;
const int enPin = 6;
const int Auto_Park_EN = 7;
const int Buzzer_Pin = 11;
const int Direction_Switch_Pin = 12;

int Steering_POT_Pin = A0;
int Acceleration_Pedal_Pin = A1;


///////////////  Global variables       //////////////////////

int Steering_POT_Value = 0;
unsigned long first = 0;


uint8_t idx = 0;  // Index for new data pointer
byte *p;          // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;
uint16_t bufStartFrame;  // Buffer Start Frame
uint16_t Serial_cmd;
uint16_t fspeed = -300;  //Set initial forward speed
uint16_t bspeed = 300;   //Set initial Backward speed




///////////////   Global structs      //////////////////////

struct SerialData {
  short int distance1;
  short int distance2;
  short int distance3;
  short int distance4;
  short int DriverState;
} SerialData1;


typedef struct {
  uint16_t start;
  int16_t steer;
  int16_t speed;
  uint16_t checksum;
} SerialCommand;
SerialCommand Command;

typedef struct {
  uint16_t start;
  int16_t cmd1;
  int16_t cmd2;
  int16_t speedR_meas;
  int16_t speedL_meas;
  int16_t batVoltage;
  int16_t boardTemp;
  uint16_t cmdLed;
  uint16_t checksum;
} SerialFeedback;
SerialFeedback Feedback;
SerialFeedback NewFeedback;

///////////////////////////////////////////////////



// ########################## SETUP ##########################

void setup() {
  Serial.begin(9600);                    // serial baud rate (pins 0,1)
  HoverSerial.begin(HOVER_SERIAL_BAUD);  //Hoverboard serial baud rate


  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(Buzzer_Pin, OUTPUT);

  pinMode(Auto_Park_EN, INPUT_PULLUP);
  pinMode(Direction_Switch_Pin, INPUT_PULLUP);


  // Initialize I2C communication as slave with address 8
  Wire.begin(8);


  // Register receive event handler
  Wire.onReceive(receiveEvent);


  fspeed = -250;
  bspeed = 250;

  Serial.println("Welcome Agina");
}


// ########################## LOOP ##########################

void loop(void) {

  Receive();


  if (SerialData1.DriverState == 2) {

    digitalWrite(Buzzer_Pin, HIGH);
    //Serial.print("7777");

  } else if (SerialData1.DriverState == 1) {

    digitalWrite(Buzzer_Pin, HIGH);
  } else {
    digitalWrite(Buzzer_Pin, LOW);
  }

  //Serial.println("2222");




  if ((digitalRead(Auto_Park_EN) == LOW)  || (SerialData1.DriverState == 2)) {

    Serial.println("111111");

    //Activate steering control (Stepper motor Enable)
    digitalWrite(enPin, LOW);


    // Check for new received data
    Steering_POT_Value = analogRead(Steering_POT_Pin);

    //**********************************************************/**********************************************************
    //**********************************************************/**********************************************************
    //**********************************************************/**********************************************************

    //move forward
    Send(0, fspeed);
    delay(200);
    Send(0, 0);

    //Serial.println("3333");

    //**********************************************************/**********************************************************
    //**********************************************************/**********************************************************
    //**********************************************************/**********************************************************


    //move Backward
    Send(0, bspeed);
    delay(200);
    Send(0, 0);



    //**********************************************************/**********************************************************
    //**********************************************************/**********************************************************
    //**********************************************************/**********************************************************


    //Automatic_Steering_Test();

    delay(50);  //Debouncing Delay

    // Serial.println("4444");
  }




  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************
  //**********************************************************/**********************************************************


  else if ((digitalRead(Auto_Park_EN) == HIGH)  || (SerialData1.DriverState != 2)) {

    //disable steering
    digitalWrite(enPin, HIGH);

    //Control the motors depending on acceleration pedal reading
    Pedal_Motor_Drive();



    //Serial.println("6666");
  }

  delay(150);
}

// ########################## END ##########################
