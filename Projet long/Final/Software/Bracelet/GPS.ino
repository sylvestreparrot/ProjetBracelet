void GPS(){
  foundNMEA = gotNMEA = false;
  count = 0;
    
  do {
    while (!GPS_Serial.available()) {} // wait forever (yeah, I know...)
    char c = GPS_Serial.read();        // read a char
    if (c == '$')                      // looking for start of NMEA string
      foundNMEA = true;
    if (foundNMEA) {                   // once found, begin saving characters
      str[count++] = c;
      if (c == '\n')                   // looking for end of NMEA string
        gotNMEA = true;
    }
  } while (!gotNMEA && (count < BUFFER_SIZE));

  if (gotNMEA) {
    char *lat, *lon;
    gotGGA = gotRMC = false;
    if ( !strncmp(str, "$GPGGA", 6) )
      gotGGA = true;
    else if ( !strncmp(str, "$GPRMC", 6) )
      gotRMC = true;
      
    if (gotGGA || gotRMC){
      //char *lat, *lon;
      strtok( str + 6, "," );   // skip hour
      
      if (gotRMC)
        strtok( NULL, "," );    // skip 'A'
      
      lat = strtok( NULL, "," );// store lat
      strtok( NULL, "," );      // skip 'N'
      lon = strtok( NULL, "," );// store lon
      sprintf(mess,"AT$SS=%d%d \r \n", atoi(lat), atoi(lon));
        
        SIGFOX_Serial.write(mess); //SEND VIA SIGFOX
      #ifdef DEBUG
        Serial.print("GPS: Latitude: ");
        Serial.print(lat);
        Serial.print(" | Longitude: ");
        Serial.println(lon);
        
      #endif
    }
  }
}
