/*
This code is used to control the motion of an RC car using a blutooth module (HC-05) to interface with a mobile application.
The code contains the function of Automatic free parking space detection and Automatic parking.
all variable in the code are based on the used project components , test environment , car dimensions , and some specific assumptions

 #Key assumptions guiding the project include:

1- The minimum distance between the car and parking walls/sides is set at 40cm.
2- The width of the parked car falls within the range of 15 to 25cm.
3- The length of a free parking spot must be at least twice the length of the car (which is 25cm in our case).
4- The batteries powering the car motors have a capacity of 12V.
5- The placement and orientation of the 4 ultrasonic sensors are as depicted in the provided images.
6- The parking detection mechanism is designed to function exclusively for the right side of the car.

### It's crucial to note that any alterations to these assumptions or conditions will necessitate corresponding adjustments within the code. ###

*/

  #include <SoftwareSerial.h>
  SoftwareSerial bluetooth(10, 11); //RX, TX
  //################### Global variables ###################

    int command;

  //-----------Counters & Flags----------
  int lc = 0 , rc = 0 , ec= 0 , sc = 0, count_Done =0 ,free_Space_Detected = 0 ,fc =0;
  bool Auto_Mode_Is_On = 0;
  unsigned long Tstart = 0 , Tstop = 0;

  //-----------Pins----------

  int LedG = 12;
  int LedR = 13;

  // Motor A connections
  int enA = 9;
  int in1 = A2;
  int in2 = A3;
  // Motor B connections
  int enB = 6;
  int in3 = A4;
  int in4 = A5;

  //Note that 1 = Front , 2 = Back , 3 =  Right, 4 = Left

  // Define the pins for the ultrasonic sensors
  const int trigPin1 = 2;
  const int echoPin1 = 3;

  const int trigPin2 = 4;
  const int echoPin2 = 5;

  const int trigPin3 = 7;
  const int echoPin3 = 8;

  const int trigPin4 = A1;
  const int echoPin4 = A0;

  // Variables to store the distance readings
  long distance1, distance2, distance3, distance4;


  //################### SETUP ###################

  void setup() {

  //Reset all counters & Flags

    lc=0;
    rc=0;
    ec=0;
    count_Done=0;
    Auto_Mode_Is_On = 0;
    Tstart = 0 , Tstop = 0;

    // Initialize serial communication

    Serial.begin(9600);
   bluetooth.begin(9600);

  //-------- MOTOTRS INIT --------------------

    // Set all the motor control pins to outputs
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    
    
    // Turn off motors - Initial state
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

    //Enable motors
    digitalWrite(enA,HIGH);
    digitalWrite(enB,HIGH);

    //-------- SENSORS INIT --------

    
    // Set the trigPin as output and echoPin as input
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);
    pinMode(trigPin4, OUTPUT);
    pinMode(echoPin4, INPUT);


    //------- LED init -----------
    pinMode(LedG, OUTPUT);
    pinMode(LedR, OUTPUT);

    digitalWrite(LedG, LOW);
    digitalWrite(LedR, LOW);

      //starting delay
      delay(4500);

  }


  //################### SUPER LOOP ###################


  void loop() {

        while(empty_Parking_Detection() == false);
        
        automatic_Parking_Algorithm();

  }


  //################### APPLICATION FUNCTIONS ###################



