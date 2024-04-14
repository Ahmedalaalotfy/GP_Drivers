

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






/////////////////////////////////////////////



