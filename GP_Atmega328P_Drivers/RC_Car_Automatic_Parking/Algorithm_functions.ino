  // --------- Algorithm function ----------


  /*
  *   This function is used after empty_Parking_Detection() 
  * 
  * 
  */

  void automatic_Parking_Algorithm()
  {
      //Automatic Parking Algorithm depending on our Rc car (dimentions, power , and sensors orientation)
    distance_Safety_Action();
    readSensors();
    distance_Safety_Action();

    while(distance4 > 62)
    {
      //Stop the car if any obstacle appears suddenly 
      distance_Safety_Action();
      
      left();
      delay(60);
      stop();
      delay(40);

      readSensors();
    }

    readSensors();
    while(distance4 > 15)
    {
      //Stop the car if any obstacle appears suddenly 
      distance_Safety_Action();

      backward();
      delay(45);
      stop();
      delay(40);

      readSensors();
    }

        while(distance4 > 9)
    {
      //Stop the car if any obstacle appears suddenly 
      distance_Safety_Action();

      backward();
      delay(20);
      stop();
      delay(30);

      readSensors();
    }

    align_Vertically();

    finish_Parking();


      while(1)
      {          
        
        if (Serial.available()) 
        {

       command = Serial.read();

        if(command == 'M')
          { 
             //Reset all counters & Flags

            lc=0;
            rc=0;
            ec=0;
           count_Done=0;
            Auto_Mode_Is_On = 0;
            Tstart = 0 , Tstop = 0;


          //Turn off Auto mode & return to manual mode
          Auto_Mode_Is_On = 0;
       
          return;
          }else
          {
          stop();            
          }
       
        }
        stop(); 
        
      }


      //Might be needed

          /*while(distance2> 10 || distance3> 10 )
    {
      right();
      delay(40);
      stop();
      delay(40);

      readSensors();
    }*/
  }





  //empty_Parking_Detection
  /*
  this function must be used in a while(1)
  it is used to detect if there is a free parking spot for only one time

  */

  //todo add infinte a free parking spot detection (ec must be replaced with more efficient way)

 bool empty_Parking_Detection()
  {
  distance1= readDistance(trigPin1,echoPin1);
  distance2 = readDistance(trigPin2,echoPin2);

  if(distance1 < 420 && distance1 > 15)
  {

    while(distance2 > 40){
    
    if(ec == 0)
    {
  forward();
    }else
    {
          //count time  
      Tstart = millis();
    bluetooth.print(Tstart);
    bluetooth.print(";");


      set_speed(135); 
      while(distance2 > 40)
      {
        digitalWrite(LedG, HIGH);
        forward();
        distance2 = readDistance(trigPin2,echoPin2);
      }
      
     digitalWrite(LedG, LOW);


      Tstop = millis() - Tstart;
    bluetooth.print(Tstop);
    bluetooth.print(";");

    count_Done = 1;

    }


  distance2 = readDistance(trigPin2,echoPin2);
    }

    ec = 1;

    if(distance2 < 40)
    {
       // 50% PWM Duty cycle (6V)----from 12V   مش اخر كلام
        set_speed(155); 
         forward();
       
    }

  distance3 = readDistance(trigPin3,echoPin3);

    if( (distance3<40) && (count_Done == 1) )
    {

      stop();


      //toggle led
      for(fc = 0 ;fc < 3 ;fc++)
      {
        digitalWrite(LedG, HIGH);
        delay(600);
        digitalWrite(LedG, LOW);
        delay(600);        
      }
      
      //Reset speed & flags
            set_speed(255);
            ec=0;
           count_Done=0;


      return true;
    }


  }else if(distance1 < 15){

    stop();
    digitalWrite(LedR, HIGH);
        delay(50);
        digitalWrite(LedR, LOW);

  }else if((distance1 >= 420) && (distance2 <220) ){

    forward();
    delay(60);
    stop();
    delay(60);

  }else{ 
    stop();
  }

    //return false until parking spot is detected
    return false;
  }

 void keep_Car_Distance()
  {
    distance1= readDistance(trigPin1,echoPin1);

  distance2 = readDistance(trigPin2,echoPin2);


  if(distance1 < 420 && distance1 > 15){

    if(distance2 >= 0 && distance2 <=35){

      if(distance2 > 25){
      right();
      delay(55);
      rc++;

    }
    else if(distance2 >= 0 && distance2 < 16){
    left();
    delay(65);
    lc++;

    }else if(distance2 >= 16 && distance2 <= 25){
    forward();
    delay(60);
    stop();
    delay(60);
    }else{
    left();
    delay(10000);
    }

    }

    if(lc == 2 ){
    right();
    delay(50);
    
    lc = 0;
    }

    if(rc == 2 ){
    left();
    delay(70);
    
    rc = 0;
    }

    if(distance2 > 35){
    forward();
    //count time
    }



  }else if(distance1 < 15 && distance1 >= 8 ){
  left();
  delay(50);

  }else if(distance1 > 420 ){
  forward();
  delay(60);
  }else{ 
    stop();
    }


  }


void blutooth_Control_And_Reading()
{

    readSensors(); 

if (Serial.available()) {

   command = Serial.read();

switch (command) {
      case 'F':
        set_speed(200);
        forward();
        break;

      case 'B':
        set_speed(200);
        backward();
        break;

      case 'L':
        set_speed(255);
        left();
        break;

      case 'R':
        set_speed(255);
        right();
        break;

        
      //Turn on Auto mode
      case 'A':
      Auto_Mode_Is_On = 1;
        break;


      //Turn off Auto mode & return to manual mode
      case 'M':
      Auto_Mode_Is_On = 0;
        break;


       default:              
       stop();
       break; 
    }
  }

//Auto Parking Mechanism

if(Auto_Mode_Is_On){

  readSensors();  

}



    delay(30);

}
//------------------------------------------------------------
//------------------------------------------------------------

void align_Vertically()
{

  
   //TODO && ((distance1-distance4) > 10 )

         readSensors();
      //turn right until car is aligned Vertically
      while((distance1 > 15 || distance4 > 15) )
      {

        digitalWrite(LedG, HIGH);

        right();
        delay(50);
       stop();
       delay(30);

       readSensors();
      }     


        right();
        delay(50);
       stop();

       
      digitalWrite(LedG, LOW);

      readSensors();
      while(  (distance3 - distance2) >= 2 )
    {

      while(( (distance3 - distance2) >= 2) && (distance3 > distance2))
      {
 
        right();
        delay(45);
       stop();
       delay(45);

       readSensors();
       }

       
        /*
       while( (distance2 - distance3) >= 1 && (distance2 > distance3))
       {
       //Stop the car if any obstacle appears suddenly 
       distance_Safety_Action();

        left();
       delay(30);
        stop();
        delay(30);

       readSensors();
       }
        */

        readSensors();
    }
}



void finish_Parking()
{

    readSensors();

   while( (distance1 - distance4) >= 1 ||  (distance4 - distance1) >= 1)
    {

      while( (distance1 - distance4) >= 1 && (distance1 > distance4))
      {
        //Stop the car if any obstacle appears suddenly 
       distance_Safety_Action();

        forward();
        delay(30);
        stop();
        delay(30);
        readSensors();
       }

       while( (distance4 - distance1) >= 1 && (distance4 > distance1))
       {

        //Stop the car if any obstacle appears suddenly 
       distance_Safety_Action(); 

        backward();
        delay(30);
        stop();
        delay(30);

       readSensors();
       }


      readSensors();
    }

          for(fc=0;fc<6;fc++)
      {
      digitalWrite(LedR, HIGH);
      digitalWrite(LedG, HIGH);
        delay(500);
       digitalWrite(LedR, LOW); 
        digitalWrite(LedG, LOW);
        delay(500);
      }

}

void distance_Safety_Action()
{
   readSensors();
  while(distance1 <= 3 || distance2 <= 3 || distance3 <= 3 || distance4 <= 3 )
  {

    //todo if distance1 < 3 move in opposite direction for a small amount
    stop();

    digitalWrite(LedR, HIGH);
     readSensors();
  }

   digitalWrite(LedR, LOW);
}

void parking_Safety_Action()
{
   readSensors();
  while(distance1 <= 1 || distance2 <= 1 || distance3 <= 1 || distance4 <= 1 )
  {

    //todo if distance1 < 3 move in opposite direction for a small amount
    stop();

    digitalWrite(LedR, HIGH);
     readSensors();
  }

   digitalWrite(LedR, LOW);
}

