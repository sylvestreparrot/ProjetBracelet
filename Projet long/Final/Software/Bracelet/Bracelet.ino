#include <Wire.h>
#include <SeeedRFID.h>          //RFID reader library
#include <Adafruit_GFX.h>       //Screen
#include <Adafruit_SSD1306.h>   //library
#include <Adafruit_Sensor.h>    //General Adafruit sensor library
#include <Adafruit_HDC1000.h>   //Temperature & Humidity sensor library
#include <Adafruit_HMC5883_U.h> //Compass library

#define DEBUG                   //Comment it to remove debug info
#define PIN_SOS 3               //SOS button pin
#define PIN_TAG_ON 4            //RFID tag button pin
#define PIN_SCREEN_ON 2         //Screen command button pin
#define SCREEN_RESET 15         //Screen reset pin
#define BUFFER_SIZE 100         //Buffer size for the GPS function
#define GPS_Serial Serial1      //GPS is using Serial1 of the Teensy LC
#define SIGFOX_Serial Serial3   //Sigfox module is using the Serial3 of the Teensy LC

RFIDdata tag;                                                                   //Creating the variable in which we will put the RFID data
SeeedRFID RFID_Serial = SeeedRFID();                                            //Creating the RFID Serial (it uses the Serial2 of the Teensy LC)
Adafruit_SSD1306 display(SCREEN_RESET);                                         //Creating the screen object
Adafruit_HDC1000 HDC1000SENSOR = Adafruit_HDC1000();                            //Creating the temperature/humidity sensor object
Adafruit_HMC5883_Unified MAGNETOMETERSENSOR = Adafruit_HMC5883_Unified(12345);  //Creating the compass object


int flag_display=1, flag_rfid=1;                                
char str[BUFFER_SIZE];
char mess[50];
float headingDegrees=0;
bool stop_tag_on= false, sos = false;
unsigned int count=0,cptag=0;       
byte temperature=0,humidity=0,accel=0;
unsigned long TAG_ID[6], foundNMEA, gotNMEA, gotGGA, gotRMC;

void setup(){
  
  #ifdef DEBUG
	Serial.begin(9600);
  delay(1000);
  Serial.println("Debug activated");
  #endif

  display.begin();                                      //Screen
  display.clearDisplay();                               //initialisation
  
  HDC1000SENSOR.begin();                                //Temperature/Humidiy sensor initialisation
  GPS_Serial.begin(9600);                               //GPS initialisation
  SIGFOX_Serial.begin(9600);                            //Sigfox initialisation
  MAGNETOMETERSENSOR.begin();                           //Compass initialisation

  pinMode(PIN_SCREEN_ON,INPUT);                         //Pin 2 as input
  pinMode(PIN_SOS,INPUT);                               //Pin 3 as input
  pinMode(PIN_TAG_ON,INPUT);                            //Pin 4 as input
  pinMode(SCREEN_RESET,OUTPUT);                         //Pin 15 as output
  attachInterrupt(PIN_TAG_ON, tag_on, CHANGE);          //Execute the function tag_on() each time a change of state happens on the pin 4
  attachInterrupt(PIN_SCREEN_ON, dream_on, CHANGE);     //Execute the function dream_on() each time a change of state happens on the pin 2
  attachInterrupt(PIN_SOS, sos_pls, CHANGE);            //Execute the function sos_pls() each time a change of state happens on the pin 3
}


//Main loop
void loop(){
  //If flag_display is equal to 1
  if(flag_display == 1){
    //Turn on the screen
    display.ssd1306_command(SSD1306_DISPLAYON);
    //Execute sensors functions and display what is needed with the function LCDDisplay()
    HDC1000();
    COMPASS();
    LCDdisplay();
  }
  //Else turn off the screen and do nothing
  else{
    display.ssd1306_command(SSD1306_DISPLAYOFF);   
  }

  //If flag_rfid is equal to 2
  if(flag_rfid == 2){
    //Do a RFID scan
    RFID();
  }
  
  #ifdef DEBUG
  debug();
  delay(200);
  #endif
}

//When enter in this interrupt change the value of flag_display
void dream_on(){
  
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > 300){
    if (flag_display == 1){
      flag_display = 2;
      #ifdef DEBUG
      Serial.println("Screen Off");
      #endif
    } else {
      flag_display = 1;
      #ifdef DEBUG
      Serial.println("Screen On");
      #endif
    }
  }
  last_interrupt_time = interrupt_time;
}


//When enter in this interrupt change the value of flag_rfid
void tag_on(){
  
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  #ifdef DEBUG
  Serial.print("RFID Scanning : ");
  #endif
        
  if (interrupt_time - last_interrupt_time > 300){
   
    if ( flag_rfid == 1){
       flag_rfid = 2;
      #ifdef DEBUG
      Serial.println("Scan on");
      #endif
    } else {
      flag_rfid = 1;
      #ifdef DEBUG
      Serial.println("Scan off");
      #endif
    }
  }

  last_interrupt_time = interrupt_time;
  
}

//When enter in this interrupt get the GPS coordinates
void sos_pls(){
  
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200){
        sos = true;
        GPS();
  }
  last_interrupt_time = interrupt_time;
}

