void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
}

void loop() {
  // Check if data is available
  if (Serial.available() > 0) {
    // Read the incoming byte
    byte data_byte = Serial.read();
    int data_int = data_byte;

    // Print the received data
    Serial.print("Received: ");
    Serial.println(data_int);

    if (data_int == 2) {
      digitalWrite(11, HIGH);
    }
    else if (data_int == 1) {
      while (data_int == 1) {
        digitalWrite(11, HIGH);
        delay(50);
        digitalWrite(11, LOW);
        delay(200);
        if (Serial.available() > 0) {
          data_byte = Serial.read();
          data_int = data_byte;
          Serial.print("Received: ");
          Serial.println(data_int);
        }
    } 
    }
    else {
      digitalWrite(11, LOW);
    }
  }
}
