#include <Wire.h>
#include <SeeedRFID.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HDC1000.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_ADXL345_U.h>

#define DEBUG
#define PIN_SOS 3
#define PIN_TAG_ON 4
#define SCREEN_RESET 15
#define PIN_SCREEN_ON 2
#define BUFFER_SIZE 100 //GPS
#define GPS_Serial Serial1
#define SIGFOX_Serial Serial3

RFIDdata tag; 
SeeedRFID RFID_Serial = SeeedRFID();
Adafruit_SSD1306 display(SCREEN_RESET);
Adafruit_HDC1000 HDC1000SENSOR = Adafruit_HDC1000();
Adafruit_HMC5883_Unified MAGNETOMETERSENSOR = Adafruit_HMC5883_Unified(12345);


int flag_display=1, flag_rfid=1;
char str[BUFFER_SIZE];     // buffer array for data recieve over serial port
char mess[50];

float headingDegrees=0;
bool stop_tag_on= false, sos = false;
unsigned int count=0,cptag=0;               // counter for buffer array
byte temperature=0,humidity=0,accel=0;
unsigned long TAG_ID[6], foundNMEA, gotNMEA, gotGGA, gotRMC;

void setup(){
  
  #ifdef DEBUG
	Serial.begin(9600);
  delay(1000);
  Serial.println("Debug activated");
  #endif

  display.begin();
  display.clearDisplay();
  
  HDC1000SENSOR.begin();
  GPS_Serial.begin(9600);
  SIGFOX_Serial.begin(9600);
  MAGNETOMETERSENSOR.begin();

  pinMode(PIN_SCREEN_ON,INPUT);
  pinMode(PIN_SOS,INPUT);
  pinMode(PIN_TAG_ON,INPUT);
  pinMode(SCREEN_RESET,OUTPUT);
  attachInterrupt(PIN_TAG_ON, tag_on, CHANGE);
  attachInterrupt(PIN_SCREEN_ON, dream_on, CHANGE);
  attachInterrupt(PIN_SOS, sos_pls, CHANGE);
}
 
void loop(){ 
  if(flag_display == 1){
    display.ssd1306_command(SSD1306_DISPLAYON);
    HDC1000();
    COMPASS();
    LCDdisplay();
  } else{
    display.ssd1306_command(SSD1306_DISPLAYOFF);   
  }

  if(flag_rfid == 2){
    RFID();
  }
  
  #ifdef DEBUG
  debug();
  delay(200);
  #endif
}

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

void sos_pls(){
  
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  int i =0;
  char *latHEX;
  char *lonHEX;
  if (interrupt_time - last_interrupt_time > 200){
        sos = true;
        GPS();
        /*for(i = 0;i <sizeof (lat);i++){
          latHEX[i] = lat[i];   
        }*/
        
  }
  last_interrupt_time = interrupt_time;
}

