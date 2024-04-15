  //-------- SENSORS functions --------

  // Read distance from sensors and save the readings in variables
  void readSensors(){

    distance1 = readDistance(trigPin1, echoPin1);
    distance2 = readDistance(trigPin2, echoPin2);
    distance3 = readDistance(trigPin3, echoPin3);
    distance4 = readDistance(trigPin4, echoPin4);

    /*
    bluetooth.print(distance1);
    bluetooth.print(",");
    bluetooth.print(distance2);
    bluetooth.print(",");
    bluetooth.print(distance3);
    bluetooth.print(",");
    bluetooth.print(distance4);
    bluetooth.print(";");
    */

  }



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