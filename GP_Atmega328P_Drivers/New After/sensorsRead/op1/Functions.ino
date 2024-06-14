  long readDistance(int trigPin, int echoPin) {
    // Clear the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Set the trigPin on HIGH state for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Read the echoPin, calculate the duration in microseconds
    long duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance in centimeters
    long distance = duration * 0.034 / 2;
    
    return distance;
  }




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

  void Send_Motor_Speed(int speed)
  {
    
    speed = map( speed , 0,100, 0, 255); 

    //Serial.println(speed);
    
    analogWrite(Motor_Speed_Pin,speed);
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
 
  Acceleration_Pedal = analogRead(Acceleration_Pedal_Pin);            // reads the value of the potentiometer (value between 0 and 1023)
   
  Acceleration_Pedal = map( Acceleration_Pedal , 290,860, 0, 100); 


 // Set_Motor_Direction(&Acceleration_Pedal);



  
    //Serial.println(Acceleration_Pedal);


  if((Acceleration_Pedal < -8) || (Acceleration_Pedal >8) )
  {

    Send_Motor_Speed(Acceleration_Pedal);
   
    }else{
      
      Send_Motor_Speed(0);
      
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

void Steering_Centre()
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
