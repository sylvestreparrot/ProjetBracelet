void LCDdisplay(){
  if(sos == true){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.setCursor(5,30);
    display.print("SOS SENT !");

    display.display();
    delay(10000);
    sos = false;
  } else{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    
    display.setCursor(0,0);
    display.print(temperature); 
    display.print((char)247);display.print("C");
    
    display.setCursor(0,50);   
    display.print(humidity);
    display.println("%");
    
    display.setCursor(90,50);
    display.print(cptag);
    display.print("/6");
    
    display.setCursor(60,0);
    display.drawCircle(64,37,20, WHITE);
    CompassDraw();
    
    display.display();
  }
}

