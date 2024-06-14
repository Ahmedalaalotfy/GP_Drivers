
//////////////////////////////   I2C   //////////////////////////////



/// Function to receive data and deserialize into structure
void receiveData(SerialData &receivedData) {
  byte* ptr = (byte*)&receivedData;
  for (size_t i = 0; i < sizeof(receivedData); i++) {
    *ptr++ = Wire.read();
  }
}



  //**********************************************************/**********************************************************
//**********************************************************/**********************************************************
//**********************************************************/**********************************************************


// Function to handle I2C receive event
void receiveEvent(int byteCount) {

  if (byteCount == sizeof(SerialData)) {
    receiveData(receivedData);
     SerialData1 = receivedData;

// if(i < 3)
// {
//   temp[i] = receivedData;
//   i++;
// }else{
//   i = 0;
// }


       processReceivedData(receivedData);
     newDataAvailable = true;
  } else {
    // Handle incorrect data length
    while (Wire.available()) {
      Wire.read();
    }
  }

}

// Function to handle the request event
// void requestEvent() {
//   // Respond to the master's request by sending the stored data
//   Wire.write((byte*)&I2c_Sending_Flag, sizeof(I2c_Sending_Flag));
// }

  //**********************************************************/**********************************************************
//**********************************************************/**********************************************************
//**********************************************************/**********************************************************

void processData()
{
   if (newDataAvailable) {

    // Process received data
  processReceivedData(receivedData);

    // Reset flag
    newDataAvailable = false;
  }
}



// Function to process received data
void processReceivedData(const SerialData &receivedData) {

    // Serial.print("distance1:  ");
    // Serial.print(receivedData.distance1);
    // Serial.print(" ;;");

    // Serial.print("distance2:  ");
    // Serial.print(receivedData.distance2);
    // Serial.println(" ;;");

    // Serial.print("distance3:  ");
    // Serial.print(receivedData.distance3);
    // Serial.print(" ;;");            

    //    Serial.print("distance4:  ");
    // Serial.print(receivedData.distance4);
    // Serial.println(" ;;");

  
    Serial.print("DriverState:  ");
    Serial.println(receivedData.DriverState);
    Serial.println(" ;;");

     SerialData1 = receivedData ;

// if(i < 3)
// {
//   temp[i] = receivedData;
//   i++;
// }else{
//   i = 0;
// }
   


}     



//********************************************************************************************************************
//****************************************** Ultrasonic Readings Functions  ******************************************
//********************************************************************************************************************

int average_reading(){

int average = 0 ,sum = 0 ,count=0;  

for(count = 0 ; count<3 ;count++ )
{
sum += temp[count].distance2;
}

average = sum / 3;


    Serial.print("average:  ");
    Serial.print(average);
    Serial.print(" ;;;;;; ");

return average;

}




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




 //for ultrasonic reading if needed

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

