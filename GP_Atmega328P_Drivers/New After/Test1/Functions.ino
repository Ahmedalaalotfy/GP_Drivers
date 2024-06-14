
//********************************************************************************************************************
//****************************************** Parking Functions  ******************************************
//********************************************************************************************************************

void Driver_Not_Responding_Algorithm(){

  // if(sleep_state == 1)
  // {


//Disable electric break
Disable_SoftBreak();
//start alarm

//check for  forward direction 

//activate forward if not activated

//disable driver control
 Enable_Auto_Drive();
// centre the car
 Send_Steering_Angle(0);
 delay(500);

//move with slow speed
 Send_Motor_Speed(30);
delay(1000);
// Activate turning right LED

  Send_Steering_Angle(23);

//check for car distance from the road pavement
while( SerialData1.distance2 > 170)
{

  digitalWrite(Right_LED_Relay_Pin, LOW);
//delay 
delay(400);
digitalWrite(Right_LED_Relay_Pin, HIGH);
delay(400);

}

   delay(1000);

   //check for car distance from the road pavement
while( SerialData1.distance2 > 160 ||  SerialData1.distance3 > 130 )
{

  digitalWrite(Right_LED_Relay_Pin, LOW);
//delay 
delay(200);
digitalWrite(Right_LED_Relay_Pin, HIGH);
delay(200);

}

 delay(250);

while( SerialData1.distance2 > 160 ||  SerialData1.distance3 > 130 )
{

  digitalWrite(Right_LED_Relay_Pin, LOW);
//delay 
delay(100);
digitalWrite(Right_LED_Relay_Pin, HIGH);
delay(100);

}

  // Send_Steering_Angle(-60);
  // delay(700);
 Send_Steering_Angle(-100);
  Send_Motor_Speed(10);

delay(500);
 Send_Steering_Angle(0);

// while( SerialData1.distance2 > 125)
// {

//   digitalWrite(Right_LED_Relay_Pin, LOW);
// //delay 
// delay(200);
// digitalWrite(Right_LED_Relay_Pin, HIGH);
// delay(200);

// }

        //Turn on natural buzzer
    digitalWrite(enPin, LOW);


  //stop
     Send_Motor_Speed(0);
  
  Enable_SoftBreak();

  //reset sleep state 
  
sleep_state = 0;

  while(digitalRead(Auto_Park_Pin) == HIGH)
  {

        digitalWrite(Red_LED_Relay_Pin, LOW);
        digitalWrite(Left_LED_Relay_Pin, LOW);
        digitalWrite(Right_LED_Relay_Pin, LOW);
        digitalWrite(Buzzer_Relay_Pin, LOW);
      delay(600);

      digitalWrite(Red_LED_Relay_Pin, HIGH);
      digitalWrite(Left_LED_Relay_Pin, HIGH);
      digitalWrite(Right_LED_Relay_Pin, HIGH);
      digitalWrite(Buzzer_Relay_Pin, HIGH);
      delay(600);

      Send_Motor_Speed(0);

  }
  
        //Turn off natural buzzer
    digitalWrite(enPin, HIGH);

//   }else
//   {
//  Disable_Auto_Drive();
//   }


}



//The starting directions in this function should be forward
void Auto_Parking_Algorithm(){

///sodu codo
while(digitalRead(Auto_Park_Pin) == LOW)
{

  Callibrate_Direction();

    delay(25);
    Control_Direction();

 Enable_Auto_Drive();

  //turn right
  Send_Steering_Angle(100);
  //reverse directions [Backward]
  change_Direction();

  
while(SerialData1.distance2< 250)
{
    //move [40] 
  Send_Motor_Speed(39);

}
  //Stop
Send_Motor_Speed(0);
  //delay 
  delay(1000);


      //turn left
  Send_Steering_Angle(-100);


while( SerialData1.distance2 > 100)
{
    //move [40] 
  Send_Motor_Speed(35);

}

while( SerialData1.distance2 > 100)
{
    //move [40] 
  Send_Motor_Speed(35);

}


  //Stop
Send_Motor_Speed(0);
  //delay 
  delay(1000);

        //turn centre
  Send_Steering_Angle(0);


    //reverse directions [Forward]
  change_Direction();
//////////////////////////////////


  while(SerialData1.distance1 > 150)
{
  //move [40] 
  Send_Motor_Speed(36);
}

  //Stop

     Send_Motor_Speed(0);
   delay(2000);

       //Turn on natural buzzer
    digitalWrite(enPin, LOW);
   Disable_Auto_Drive();

while(1);

}



}



void distance_Safety_Action()
{

//todo  || SerialData1.distance3 <= 50 || SerialData1.distance4 <= 50

  while(SerialData1.distance1 <= 50 || SerialData1.distance2 <= 50 )
  {

  Send_Motor_Speed(0);
   
  //turn on back leds
    
  }

   
}


  //**********************************************************/**********************************************************
//**********************************************************/**********************************************************
//**********************************************************/**********************************************************

void Callibrate_Direction(){
   // Read switch state into directionState variable
    int newState = digitalRead(Direction_Switch_Pin);

  // Read value in EEPROM and display to serial monitor
  EEPROMValue = EEPROM.read(0);
  Serial.print("EEPROM Value: ");
  Serial.println(EEPROMValue);

    Serial.print("newState Value: ");
  Serial.println(newState);

     Serial.print("Callibrating_Directions............... ");
 
  
  if (newState != directionState) {
    directionState = newState;

    //Update EEPROM Direction Value
    EEPROM.write(0, newState);

     Direction_Flag = 1;

  }

Serial.println(" : : : : : : :");

EEPROMValue = EEPROM.read(0);
  Serial.print("EEPROM Value: ");
  Serial.println(EEPROMValue);

    Serial.print("newState Value: ");
  Serial.println(newState);

}


//********************************************************************************************************************
//****************************************** Car Motors Control Functions  ******************************************
//********************************************************************************************************************


// ########################## SEND ##########################

  void Send_Motor_Speed(int speed)
  {
     

    int Last_Angle = SpeedServo.read();
    int angle;
    int Desired_Angle = map( speed , 0,100, 180,10); 

if(Last_Angle > Desired_Angle)
{

  for ( angle= Last_Angle; angle >= Desired_Angle; angle -= 5) {  // Sweep from 180 to 0 degrees
    SpeedServo.write(angle);  // Set servo position
    delay(40);  // Wait for servo to reach the position
  }

}else if(Last_Angle < Desired_Angle)
{

  for ( angle= Last_Angle; angle <= Desired_Angle; angle += 5) {  // Sweep from 180 to 0 degrees
    SpeedServo.write(angle);  // Set servo position
    delay(40);  // Wait for servo to reach the position
  }

}else{
   SpeedServo.write(Desired_Angle);  // Set servo position
}

    //Serial.println(speed);
 

  }

  

void Control_Direction()
{

  if(Direction_Flag == 1)
  {
          // Power the Relay for 500ms
    digitalWrite(Direction_Relay_Pin, HIGH);
    delay(500);
    digitalWrite(Direction_Relay_Pin, LOW);

    Direction_Flag = 0;
  }

}




//********************************************************************************************************************
//****************************************** Automatic Steering Functions  *******************************************
//********************************************************************************************************************

//Send Steering_Angle for stepper motor || Range { -100 L : 100 R }
void Send_Steering_Angle(int angle)
{

  // //Disable all interrupts  
   noInterrupts();

    //Disable8 i2c Data recieve
    // I2c_IR_Disable();
     delay(10);

    //Enable steering control (Stepper motor Enable pin)
    digitalWrite(enPin, LOW);

      Steering_POT_Value = analogRead(Steering_POT_Pin);

      angle = map(angle, -100, 100 , POT_MAX_LEFT_VALUE, POT_MAX_RIGHT_VALUE);

      if(Steering_POT_Value > angle)
      {
        digitalWrite(dirPin,HIGH);
        first=millis();
        while(analogRead(Steering_POT_Pin) > (angle) && ((millis()-first)<5000))
        {
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(num);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(num);
        }
      }else
      {

        digitalWrite(dirPin,LOW);
        first=millis();
        while(analogRead(Steering_POT_Pin) < (angle) && ((millis()-first)<5000))
        {
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(num);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(num);
        }
        
      }

        //disable steering control (Stepper motor Enable pin)  
    digitalWrite(enPin, HIGH);

        //Enable i2c Data recieve
   //  I2c_IR_Enable();

     
   //Enable all interrupts  
  interrupts();

}


//********************************************************************************************************************
//****************************************** Temporarly Ignored Functions  *******************************************
//********************************************************************************************************************


// void Set_Motor_Direction(int* speed)
// {

// if(digitalRead(Direction_Switch_Pin) == HIGH){

// //forward state
// *speed = 0-(*speed);
  
// }else{

// //Reverse state
// //leave it positive

//   }

// }

///////////////////////////////////////////////////

// void Pedal_Motor_Drive()
// {
  
//   // select the input pin for the potentiometer
//   int Acceleration_Pedal;
 
//   Acceleration_Pedal = analogRead(Acceleration_Pedal_Pin);            // reads the value of the potentiometer (value between 0 and 1023)
   
//   Acceleration_Pedal = map( Acceleration_Pedal , 290,860, 0, 100); 


//  // Set_Motor_Direction(&Acceleration_Pedal);

//     //Serial.println(Acceleration_Pedal);


//   if((Acceleration_Pedal < -8) || (Acceleration_Pedal >8) )
//   {

//     Send_Motor_Speed(Acceleration_Pedal);
   
//     }else{
      
//       Send_Motor_Speed(0);
      
//       }


// }

///////////////////////////////////////////////////

  // void DAC_Write(int value)
  // {
  //   int i = 0;

  //   for( i = 0; i < 3; i++)
  //   {
  //     digitalWrite(DAC_OUT_Pins[i],(1 &(value >> i)));
  //   }

  // }


///////////////////////////////////////////////////

  
// void Send_Forward_Speed(int speed)
// {

//   int newState = FORWARD;

//   if (newState != directionState) {

//      Direction_Flag = 1;
//   }

//   Control_Direction();

//   Send_Motor_Speed(speed);

// }
