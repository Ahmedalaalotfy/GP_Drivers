void readSensors(){



    if (Serial.available() != 0 )
    {
     Tab_CMD = Serial.read() ; 
    }



       SerialData_Send.distance1 = readDistance(trigPin1, echoPin1);
     SerialData_Send.distance2 = readDistance(trigPin2, echoPin2);
    SerialData_Send.distance3 = readDistance(trigPin3, echoPin3);
   SerialData_Send.distance4 = readDistance(trigPin4, echoPin4);
    SerialData_Send.DriverState = Tab_CMD ; 



    // Serial.print("distance1:  ");
    // Serial.print(SerialData_Send.distance1);
    // Serial.print(" ;;");

    // Serial.print("distance2:  ");
    // Serial.print(SerialData_Send.distance2);
    // Serial.println(" ;;");

    //  Serial.print("distance3:  ");
    // Serial.print(SerialData_Send.distance3);
    // Serial.print(" ;;");

    //    Serial.print("distance4:  ");
    // Serial.print(SerialData_Send.distance4);
    // Serial.println(" ;;");

    //        Serial.print("State:  ");
    // Serial.print(SerialData_Send.DriverState);
    // Serial.println(" ;;");


    
  }


  ///////////////////////////////////////////////////
  //////////////////////////////////////////////////


  // Function to send data over I2C
void sendData(const SerialData &dataToSend) {
  byte* ptr = (byte*)&dataToSend;
  for (size_t i = 0; i < sizeof(dataToSend); i++) {
    Wire.write(*ptr++);
  }
}

void receiveData() {
  // Request data from the slave Arduino
  Wire.requestFrom(slaveAddress, sizeof(Sending_Flag));

  // Wait until data is available
  while (Wire.available() < sizeof(Sending_Flag)) {
    // Wait
  }

  // Read the received data into the structure
  byte* ptr = (byte*)&Sending_Flag;
  for (size_t i = 0; i < sizeof(Sending_Flag); i++) {
    *ptr++ = Wire.read();
  }

  // Print received data for debugging
  Serial.print("Received data from slave: ");
  Serial.print("Data1: ");
  Serial.println(Sending_Flag);

}
