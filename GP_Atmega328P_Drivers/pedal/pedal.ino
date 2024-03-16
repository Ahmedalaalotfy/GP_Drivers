// ***********************************************************
// **   electrcic car motors control using a pedal code     **
// **   Code Made by: Mohamed Nabil Mohamed,                **
// **                 Ahmed Alaa Lotfy,                     **     
// **                 Ahmed Hussien Agina,                  **      
// **                  Omar Adel Mohamed                    **                      
// ***********************************************************
//
// This code is used to control the speed and direction of rotation of the motors used in our electric car project 
// The pedals are 3D printed frame mounted on a potentiometer
// This code uses the readings of the pedals and direction switch to send speed and direction of rotation data to motors control board
//
// INFO:
//
// • This sketch uses the the Serial Software interface to communicate and send commands to the hoverboard
// • The built-in (HW) Serial interface is used for debugging and visualization. In case the debugging is not needed,
//   it is recommended to use the built-in Serial interface for full speed performance.
// • The data packaging includes a Start Frame, checksum, and re-syncronization capability for reliable communication
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
#define HOVER_SERIAL_BAUD   115200      // [-] Baud rate for HoverSerial (used to communicate with the hoverboard)
#define SERIAL_BAUD         115200      // [-] Baud rate for built-in Serial (used for the Serial Monitor)
#define START_FRAME         0xABCD      // [-] Start frme definition for reliable serial communication
#define DEBUG_RX                        // [-] Debug received data. Prints all bytes to serial (comment-out to disable)
#define Direction_Switch    12           //Direction_Switch pin number


#include <SoftwareSerial.h>
SoftwareSerial HoverSerial(8,9);        // RX, TX

// ########################## Global Variables ##########################

uint8_t idx = 0;                        // Index for new data pointer
byte *p;                                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;
uint16_t bufStartFrame;                 // Buffer Start Frame
uint16_t Bluetooth_cmd;

int analog1 = A0    ;
//int analog2 = A1    ; 
int speed_pot = 0;
//int steer_pot = 0;

typedef struct{
   uint16_t start;
   int16_t  steer;
   int16_t  speed;
   uint16_t checksum;
} SerialCommand;
SerialCommand Command;

typedef struct{
   uint16_t start;
   int16_t  cmd1;
   int16_t  cmd2;
   int16_t  speedR_meas;
   int16_t  speedL_meas;
   int16_t  batVoltage;
   int16_t  boardTemp;
   uint16_t cmdLed;
   uint16_t checksum;
} SerialFeedback;
SerialFeedback Feedback;
SerialFeedback NewFeedback;

// ########################## SETUP ##########################
void setup() 
{
Serial.begin(9600);                     
HoverSerial.begin(HOVER_SERIAL_BAUD);   //Hoverboard serial baud rate
Serial.println("Hoverboard Serial v1.0");


pinMode(Direction_Switch, INPUT_PULLUP);
pinMode(LED_BUILTIN, OUTPUT);
}

// ########################## SEND ##########################
void Send(int16_t uSteer, int16_t uSpeed)
{
  // Create command
  Command.start    = (uint16_t)START_FRAME;
  Command.steer    = (int16_t)uSteer;
  Command.speed    = (int16_t)uSpeed;
  Command.checksum = (uint16_t)(Command.start ^ Command.steer ^ Command.speed);

  // Write to Serial
  HoverSerial.write((uint8_t *) &Command, sizeof(Command)); 

}

// ########################## RECEIVE ##########################
void Receive()
{
    // Check for new data availability in the Serial buffer
    if (HoverSerial.available()) {
        incomingByte    = HoverSerial.read();                                   // Read the incoming byte
        bufStartFrame = ((uint16_t)(incomingByte) << 8) | incomingBytePrev;       // Construct the start frame
    }
    else {
        return;
    }

  // If DEBUG_RX is defined print all incoming bytes
  #ifdef DEBUG_RX
        Serial.print(incomingByte);
        return;
    #endif

    // Copy received data
    if (bufStartFrame == START_FRAME) {                     // Initialize if new data is detected
        p       = (byte *)&NewFeedback;
        *p++    = incomingBytePrev;
        *p++    = incomingByte;
        idx     = 2;  
    } else if (idx >= 2 && idx < sizeof(SerialFeedback)) {  // Save the new received data
        *p++    = incomingByte; 
        idx++;
    } 
    
    // Check if we reached the end of the package
    if (idx == sizeof(SerialFeedback)) {
        uint16_t checksum;
        checksum = (uint16_t)(NewFeedback.start ^ NewFeedback.cmd1 ^ NewFeedback.cmd2 ^ NewFeedback.speedR_meas ^ NewFeedback.speedL_meas
                            ^ NewFeedback.batVoltage ^ NewFeedback.boardTemp ^ NewFeedback.cmdLed);

        // Check validity of the new data
        if (NewFeedback.start == START_FRAME && checksum == NewFeedback.checksum) {
            // Copy the new data
            memcpy(&Feedback, &NewFeedback, sizeof(SerialFeedback));

            // Print data to built-in Serial                (if debug is open)
            Serial.print("1: ");   Serial.print(Feedback.cmd1);
            Serial.print(" 2: ");  Serial.print(Feedback.cmd2);
            Serial.print(" 3: ");  Serial.print(Feedback.speedR_meas);
            Serial.print(" 4: ");  Serial.print(Feedback.speedL_meas);
            Serial.print(" 5: ");  Serial.print(Feedback.batVoltage);
            Serial.print(" 6: ");  Serial.print(Feedback.boardTemp);
            Serial.print(" 7: ");  Serial.println(Feedback.cmdLed);
        } else {
          Serial.println("Non-valid data skipped");
        }
        idx = 0;    // Reset the index (it prevents to enter in this if condition in the next cycle)
    }

    // Update previous states
    incomingBytePrev = incomingByte;
}

// ########################## LOOP ##########################


int final_steer = 0;

void loop(void)
{ 
 // Check for new received data
 Receive();


  speed_pot = analogRead(A0);            // reads the value of the potentiometer (value between 0 and 1023)
  speed_pot = map(speed_pot, 300,820, 0, 1000);  

  Set_Motor_Direction(&speed_pot);
  
  Serial.println(speed_pot);

  if(speed_pot < -50 )
  {
   Send(0,speed_pot);
    }else{
      Send(0,0);
      }
  delay(50);
  



  // Blink the LED
  //digitalWrite(LED_BUILTIN, (timeNow%2000)<1000);
}

void Set_Motor_Direction(int* speed)
{

if(digitalRead(Direction_Switch) == HIGH){

//forward state
*speed = 0-(*speed);
  
}else{

//Reverse state

//leave it positive
  
}

  
}
// ########################## END ##########################
