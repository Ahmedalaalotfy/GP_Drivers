
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
    SerialData receivedData;
    receiveData(receivedData);
    processReceivedData(receivedData); 
  } else {
    // Handle incorrect data length
    while (Wire.available()) {
      Wire.read();
    }
  }
}



  //**********************************************************/**********************************************************
//**********************************************************/**********************************************************
//**********************************************************/**********************************************************




// Function to process received data
void processReceivedData(const SerialData &receivedData) {

    Serial.print("distance1:  ");
    Serial.print(receivedData.distance1);
    Serial.print(" ;;");

    Serial.print("distance2:  ");
    Serial.print(receivedData.distance2);
    Serial.println(" ;;");

     Serial.print("distance3:  ");
    Serial.print(receivedData.distance3);
    Serial.print(" ;;");

       Serial.print("distance4:  ");
    Serial.print(receivedData.distance4);
    Serial.println(" ;;");

  
    Serial.print("DriverState:  ");
    Serial.println(receivedData.DriverState);
    Serial.println(" ;;");

     SerialData1 = receivedData ;
}
