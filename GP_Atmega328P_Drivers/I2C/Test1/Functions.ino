



//**********************************************************/**********************************************************
//**********************************************************/**********************************************************
//**********************************************************/**********************************************************


void Display_Sensor_Readings(){

 
    Serial.print("distance1:  ");
    Serial.print(SerialData1.distance1);
    Serial.print(" ;;");

    Serial.print("distance2:  ");
    Serial.print(SerialData1.distance2);
    Serial.println(" ;;");

     Serial.print("distance3:  ");
    Serial.print(SerialData1.distance3);
    Serial.print(" ;;");

       Serial.print("distance4:  ");
    Serial.print(SerialData1.distance4);
    Serial.println(" ;;");

  
    Serial.print("DriverState:  ");
    Serial.println(SerialData1.DriverState);
    Serial.println(" ;;");

  }




  //**********************************************************/**********************************************************
//**********************************************************/**********************************************************
//**********************************************************/**********************************************************


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


  //**********************************************************/**********************************************************
//**********************************************************/**********************************************************
//**********************************************************/**********************************************************

void Set_Motor_Direction(int* speed)
{

if(digitalRead(Direction_Switch_Pin) == HIGH){

//forward state
*speed = 0-(*speed);
  
}else{

//Reverse state
//leave it positive
  
}

}



//////////

void Pedal_Motor_Drive()
{
  
  // select the input pin for the potentiometer
  int Acceleration_Pedal;
 
// Acceleration_Pedal = analogRead(Acceleration_Pedal_Pin);            // reads the value of the potentiometer (value between 0 and 1023)
// Acceleration_Pedal = map(Acceleration_Pedal, 655,70, 0, 1000); 




  Acceleration_Pedal = analogRead(Acceleration_Pedal_Pin);            // reads the value of the potentiometer (value between 0 and 1023)
  Acceleration_Pedal = map( Acceleration_Pedal , 120,680, 0, 1000); 

  Set_Motor_Direction(&Acceleration_Pedal);

  //                 Serial.print("Acceleration_Pedal:");
  // Serial.println(Acceleration_Pedal);



  if((Acceleration_Pedal < -280) || (Acceleration_Pedal > 280) )
  {

    if(Acceleration_Pedal <500)
    {
    Send(0,Acceleration_Pedal*0.75);
    }
    else
    {
    Send(0,Acceleration_Pedal);
    }
   
    }else{
      Send(0,0);
      }


}


//////////////////////////////////////////

void Automatic_Steering_Test(unsigned int angle)
{

      Steering_POT_Value = analogRead(Steering_POT_Pin);

      angle = map(angle, 0, 100 , POT_MAX_LEFT_VALUE, POT_MAX_RIGHT_VALUE);

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

      

  /*    

      //turn left
      if(Steering_POT_Value > POT_MAX_LEFT_VALUE)
          {
             digitalWrite(dirPin,HIGH);
            first=millis();
             while(analogRead(Steering_POT_Pin) > (POT_MAX_LEFT_VALUE+1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
             }

          }


        //turn Right
  
         if(Steering_POT_Value < (POT_MAX_RIGHT_VALUE-1))
          {
             digitalWrite(dirPin,LOW);
             first=millis();

             while(analogRead(Steering_POT_Pin) < (POT_MAX_RIGHT_VALUE-1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
             }

          }



       //move centr
       //Serial.write('C');

        if(Steering_POT_Value > POT_CENTRE_VALUE)
          {

             digitalWrite(dirPin,HIGH);

             first=millis();
             while(analogRead(Steering_POT_Pin) > (POT_CENTRE_VALUE+1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
             }

          }
          else if(Steering_POT_Value < POT_CENTRE_VALUE)
          {
            digitalWrite(dirPin,LOW);
            first=millis();
             while(analogRead(Steering_POT_Pin) < (POT_CENTRE_VALUE-1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
                // if((milis()-first)== 4999){
                //   Serial.print("Time out ");
                }
             }

*/
}

void Steering_Left()
{
        //turn left
      if(Steering_POT_Value > POT_MAX_LEFT_VALUE)
          {
             digitalWrite(dirPin,HIGH);
            first=millis();
             while(analogRead(Steering_POT_Pin) > (POT_MAX_LEFT_VALUE+1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
             }

          }
}

void Steering_Right()
{
          //turn Right
  
         if(Steering_POT_Value < (POT_MAX_RIGHT_VALUE-1))
          {
             digitalWrite(dirPin,LOW);
             first=millis();

             while(analogRead(Steering_POT_Pin) < (POT_MAX_RIGHT_VALUE-1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
             }

          }
}

void Steering_Centr()
{
         //move centr
    

        if(Steering_POT_Value > POT_CENTRE_VALUE)
          {

             digitalWrite(dirPin,HIGH);

             first=millis();
             while(analogRead(Steering_POT_Pin) > (POT_CENTRE_VALUE+1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
             }

          }
          else if(Steering_POT_Value < POT_CENTRE_VALUE)
          {
            digitalWrite(dirPin,LOW);
            first=millis();
             while(analogRead(Steering_POT_Pin) < (POT_CENTRE_VALUE-1) && ((millis()-first)<5000))
             {
                digitalWrite(stepPin,HIGH);
                delayMicroseconds(num);
                digitalWrite(stepPin,LOW);
                delayMicroseconds(num);
                // if((milis()-first)== 4999){
                //   Serial.print("Time out ");
                }
             }
}
