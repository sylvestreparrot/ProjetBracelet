//Prints the information on Serial
#ifdef DEBUG
void debug(){
  Serial.println("--------------------------------------------------------------------------------------");
  Serial.print("Temperature: ");
  Serial.print(temperature); 
  Serial.print("C | Humidity: ");
  Serial.print(humidity);
  Serial.print("% | Heading: ");
  Serial.print(headingDegrees);
  Serial.print(" Degrees | Z Acceleration: ");
  Serial.print(accel);
  Serial.println("m/s^2");
  Serial.print("RFID ID: ");
  Serial.println(TAG_ID[cptag]);
}
#endif
