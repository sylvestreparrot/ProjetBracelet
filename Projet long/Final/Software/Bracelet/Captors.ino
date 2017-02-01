void HDC1000 (){
  temperature = HDC1000SENSOR.readTemperature();
  humidity = HDC1000SENSOR.readHumidity();
}

//--------------------------------------------------------------------------------------------------------------

void COMPASS(){
  sensors_event_t getmagmesure; 
  MAGNETOMETERSENSOR.getEvent(&getmagmesure);

  getmagmesure.magnetic.x;
  getmagmesure.magnetic.y;
  getmagmesure.magnetic.z;

  float heading = atan2(getmagmesure.magnetic.y, getmagmesure.magnetic.x);

  if(heading < 0)
    heading += 2*PI;

  headingDegrees = heading * 180/M_PI; 
}

//--------------------------------------------------------------------------------------------------------------

void CompassDraw(){
  if((headingDegrees >= 348.75) || (headingDegrees<11.25)){display.drawLine(64,37,64,17, WHITE);}      //----NORD
  else if((headingDegrees >= 11.25) && (headingDegrees<33.75)){display.drawLine(64,37,69,22, WHITE);}  //NNE
  else if((headingDegrees >= 33.75) && (headingDegrees<56.25)){display.drawLine(64,37,74,27, WHITE);}  //NE
  else if((headingDegrees >= 56.25) && (headingDegrees<78.75)){display.drawLine(64,37,79,32, WHITE);}  //ENE
  else if((headingDegrees >= 78.75) && (headingDegrees<101.25)){display.drawLine(64,37,84,37, WHITE);} //----EST
  else if((headingDegrees >= 101.25) && (headingDegrees<123.75)){display.drawLine(64,37,79,42, WHITE);}//ESE
  else if((headingDegrees >= 123.75) && (headingDegrees<146.25)){display.drawLine(64,37,74,47, WHITE);}//SE
  else if((headingDegrees >= 146.25) && (headingDegrees<168.75)){display.drawLine(64,37,69,52, WHITE);}//SSE
  else if((headingDegrees >= 168.75) && (headingDegrees<191.25)){display.drawLine(64,37,64,57, WHITE);}//----SUD
  else if((headingDegrees >= 191.25) && (headingDegrees<213.75)){display.drawLine(64,37,59,52, WHITE);}//SSO
  else if((headingDegrees >= 213.75) && (headingDegrees<236.25)){display.drawLine(64,37,54,47, WHITE);}//SO
  else if((headingDegrees >= 236.25) && (headingDegrees<258.75)){display.drawLine(64,37,49,42, WHITE);}//OSO
  else if((headingDegrees >= 258.75) && (headingDegrees<281.25)){display.drawLine(64,37,44,37, WHITE);}//----OUEST
  else if((headingDegrees >= 281.25) && (headingDegrees<303.75)){display.drawLine(64,37,49,32, WHITE);}//ONO
  else if((headingDegrees >= 303.75) && (headingDegrees<326.25)){display.drawLine(64,37,54,27, WHITE);}//NO
  else if((headingDegrees >= 326.25) && (headingDegrees<348.75)){display.drawLine(64,37,59,22, WHITE);}//NNO
}

//--------------------------------------------------------------------------------------------------------------
