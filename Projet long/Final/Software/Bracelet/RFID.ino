void RFID(){
  int countSameIds=0;
  
  if(RFID_Serial.isAvailable()){
    tag = RFID_Serial.data();
    
  }
    
  unsigned int TAG_ID_temp = (RFID_Serial.cardNumber());

  if ((cptag != 6) && (TAG_ID_temp != 0)){
    for (int scanIDs=0;scanIDs<6;scanIDs++){
      if (TAG_ID[scanIDs] == TAG_ID_temp)
        countSameIds++;
    }
    
    if (countSameIds > 0){
      TAG_ID_temp = 0;
    } 
    else {
      cptag++;
      TAG_ID[cptag] = TAG_ID_temp;
      sprintf(mess,"AT$SS=%d \r \n", TAG_ID[cptag]);
      SIGFOX_Serial.write(mess); //SEND VIA SIGFOX
      TAG_ID_temp = 0;
      
      stop_tag_on = true;
    }
  }
}
