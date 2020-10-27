#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

   
#include "SoftwareSerial.h"


SoftwareSerial esp(10, 11);// RX, TX


//String ssid ="SosTech";
//String password="sostec123";

String ssid ="Mumbejja";
String password="alsj4950";
String data;
String server = "192.168.43.27"; 
//String uri = "/feeder/setNotificationStatus.php?data=1";// our example is /esppost.php
String uri_short = "/wifi/scripts/camera_on.php?data=1";
//String uri_long = "/wifi/scripts/attendance.php?data=1";
String uri;
int val1=1;
uint8_t id;
//..............................................................


#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

int getFingerprintIDez();

// RXPIN is IN from sensor (GREEN wire)
// TXPIN is OUT from arduino  (WHITE wire)
#define RXPIN A13// change this to whatever
#define TXPIN A12// change this to whatever
SoftwareSerial mySerial(RXPIN, TXPIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

 int signin = 0;

const int buzzer = 9; //buzzer to arduino pin 9
//...................................................................
 
void setup()
{
  //uri = uri_short +  "&humidity="+ hum + "&temperature=" + temp;
    pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
   finger.begin(57600);
  esp.begin(9600);
  Serial.begin(9600);
 connectWifi();
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("welcome!");
  lcd.setCursor(2,1);
  lcd.println("please signin!");
  delay(5000);
 //  lcd.setCursor(0,2);
  //lcd.print("Arduino LCM IIC 2004");
   //lcd.setCursor(2,3);
  //lcd.print("Power By Ec-yuan!");

}
void loop() {
  // put your main code here, to run repeatedly:
   rtctime();
   lcd.clear();
     lcd.setCursor(3,0);
  lcd.print("   welcome!");
  lcd.setCursor(2,1);
  lcd.println("please signin!");  
  int myfinger = getFingerprintIDez();
  uri = uri_short +  "&value1="+ myfinger+"";
  Serial.println(uri);
  //Serial.println(finger.fingerID);
  Serial.println(myfinger);

 
  if(myfinger==1){
    lcd.clear();
      lcd.setCursor(1,0);
lcd.println( "welcome tr joan");
    if (signin == 0){signin = 1;}else{signin = 0;}
     lcd.setCursor(2,1);
    lcd.println("please wait...");
    httppost ();
   }
     
  //httppost ();
 


   if(myfinger==50){
    lcd.clear();
      lcd.setCursor(1,0);
lcd.println( "welcome  sir    ");
 getf();
  uint8_t readnumber();
 fingerenroll();
  tone(buzzer, 440); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  lcd.clear();
        lcd.setCursor(2,1);
    lcd.println("enrollment successful!");
    delay(3000);
   // fingerenroll();
   }
//  Serial.print("OFF");
//delay(50000);
  delay(3000);
}
void connectWifi() {
    esp.begin(9600);
    lcd.clear();
      lcd.clear();
      lcd.setCursor(1,0);
    lcd.println("Trying to connect");
    //esp.println("AT+CWMODE=3");
    delay(100);
    String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";
    esp.println(cmd);
    //esp.println("AT");
    String inData = esp.readStringUntil('\r');
    Serial.println("Got reponse from ESP8266: " + inData);
    
    delay(4000);
    if(esp.find("OK")) { lcd.println("Connected!");
    //httppost ();
    }
    else {

//      connectWifi();
lcd.clear();
       lcd.setCursor(1,0);
      lcd.println("Cannot connect to wifi"); 
}
}

int getFingerprintIDez() {
  finger.begin(57600);
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  return finger.fingerID;
}
void rtctime(){
  String ur1 = "/wifi/scripts/get_time.php";
   esp.begin(9600);
  Serial.println("Starting Conn");
  esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  
  if( esp.find("OK")) {
    lcd.clear();
   //lcd.println("date");
    Serial.println("TCP connection ready");
  } 
  
  delay(1000);
  
  String postRequest =
  
  "POST " + ur1 + " HTTP/1.0\r\n" +
  
  "Host: " + server + "\r\n" +
  
  "Accept: *" + "/" + "*\r\n" +
  
  "Content-Length: " + data.length() + "\r\n" +
  
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  
  "\r\n" + data;
  
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  
  esp.print(sendCmd);
  
  esp.println(postRequest.length() );
  
  delay(500);
  
  if(esp.find(">")) {
    lcd.clear();
      lcd.setCursor(1,0);
    //lcd.println("Sending..");
    esp.print(postRequest);
  
  if( esp.find("SEND OK")) { 
    Serial.println("Packet sent");
 
  while (esp.available()) {
  
    String tmpResp = esp.readString();
    
    Serial.println(tmpResp);
    lcd.setCursor(0,0);
  lcd.println(tmpResp);
delay(5000);

  }
  
  esp.println("AT+CIPCLOSE"); // close the connection
  
  }

}
  }void getf()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
}

  
  uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
void fingerenroll()                     // run over and over again
{
   id = readnumber();
  if(id==1){
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  lcd.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  //id = readnumber();
  }else if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  lcd.print("Enrolling ID #");
  Serial.println(id);
lcd.println(id);
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  
  lcd.print("Waiting for valid finger to enroll as #"); lcd.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      lcd.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
     lcd.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      lcd.println("Unknown error");
      break;
    }
  }
  

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
     lcd.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
     lcd.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
 lcd.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  lcd.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      lcd.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
     lcd.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      lcd.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
     lcd.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
     lcd.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
lcd.print("Creating model for #");  lcd.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    lcd.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    lcd.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
     lcd.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  lcd.print("ID "); lcd.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    lcd.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    lcd.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    lcd.println("Unknown error");
    return p;
  }

  return true;
}
void delit()                     // run over and over again
{
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  uint8_t id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }

  Serial.print("Deleting ID #");
  Serial.println(id);

  deleteFingerprint(id);
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }
}
void httppost () {

  esp.begin(9600);
  Serial.println("Starting Conn");
  esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  
  if( esp.find("OK")) {
  
    Serial.println("TCP connection ready");
  } 
  
  delay(1000);
  
  String postRequest =
  
  "POST " + uri + " HTTP/1.0\r\n" +
  
  "Host: " + server + "\r\n" +
  
  "Accept: *" + "/" + "*\r\n" +
  
  "Content-Length: " + data.length() + "\r\n" +
  
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  
  "\r\n" + data;
  
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  
  esp.print(sendCmd);
  
  esp.println(postRequest.length() );
  
  delay(500);
  
  if(esp.find(">")) {
    lcd.clear();
      lcd.setCursor(2,0);
    lcd.println("  Sending..   ");
    esp.print(postRequest);
  
  if( esp.find("SEND OK")) { 
    Serial.println("Packet sent");
     
  tone(buzzer, 440); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  lcd.clear();
      lcd.setCursor(3,0);
  lcd.println("Successful   ");
  lcd.setCursor(3,1);
  lcd.println("Thank you!   ");
  while (esp.available()) {
  
    String tmpResp = esp.readString();
    
    Serial.println(tmpResp);
  
  }
  
  esp.println("AT+CIPCLOSE"); // close the connection
  
  }

}
}
