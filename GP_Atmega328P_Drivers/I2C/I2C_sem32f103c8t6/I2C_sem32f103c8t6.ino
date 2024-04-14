#include <Wire.h>

#define SLAVE_ADDRESS 0x10 // Define the I2C slave address


  int i ; 
  // Assuming buffer contains the 32-bit number
  uint8_t buffer[256]; 

  struct DATA {
  uint32_t distance1;
  uint32_t distance2;
  uint32_t distance3;
  uint32_t distance4;
  uint8_t DriverState;
} I2C_DATA;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  Wire.begin(SLAVE_ADDRESS); // Initialize I2C communication as slave
  Wire.onReceive(receiveEvent); // Register receive event callback

}

void loop() {

  // Place your code here if needed
}

void receiveEvent(int numBytes) {
  while (Wire.available()) { // Loop through all received bytes
  for(i=0 ; i<17 ; i++)
  {
    buffer[i] = Wire.read(); // Read received data
  }

  // Combine the four 8-bit chunks into a single 32-bit number
    I2C_DATA.distance1 = ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) | ((uint32_t)buffer[2] << 8) | buffer[3];
    I2C_DATA.distance2 = ((uint32_t)buffer[4] << 24) | ((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[6] << 8) | buffer[7];
    I2C_DATA.distance3 = ((uint32_t)buffer[8] << 24) | ((uint32_t)buffer[9] << 16) | ((uint32_t)buffer[10] << 8) | buffer[11];
    I2C_DATA.distance4 = ((uint32_t)buffer[12] << 24) | ((uint32_t)buffer[13] << 16) | ((uint32_t)buffer[14] << 8) | buffer[15];
    I2C_DATA.DriverState = buffer[16]; 


    Serial.print("distance1:  ");
    Serial.print(I2C_DATA.distance1);
    Serial.print(" ;;");

    Serial.print("distance2:  ");
    Serial.print(I2C_DATA.distance2);
    Serial.println(" ;;");

    Serial.print("distance3:  ");
    Serial.print(I2C_DATA.distance3);
    Serial.print(" ;;");

    Serial.print("distance4:  ");
    Serial.print(I2C_DATA.distance4);
    Serial.println(" ;;");

  
    Serial.print("DriverState:  ");
    Serial.println(I2C_DATA.DriverState);
    Serial.println(" ;;");

    
    
  }
}
