/*************************************************** 
  This is the project of Biometric attandance systen

  Biometric sensor takes scan the finger print to enroll user or mark attandance
 ****************************************************/

#include <Adafruit_Fingerprint.h>  //Biometric sensor Library
#include <LiquidCrystal.h>          // LCD Library


// For UNO 
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)


/* Define the LCD Pins to ardunio board*/
SoftwareSerial mySerial(2, 3);
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define buzzer 5 //define buzzer pin
#define LED 7    //define LED pin

uint8_t id;

void setup()  
{
            Serial.begin(9600);
            lcd.begin(16, 2);
            pinMode(buzzer, OUTPUT);
            pinMode(LED, OUTPUT);
            digitalWrite(buzzer, LOW);
            while (!Serial);  // For Yun/Leo/Micro/Zero/...
            delay(100);
            Serial.println("Fingerprint sensor enrollment");
            lcd.setCursor(0, 0);
            lcd.print("Fingerprint sensor enrollment");
            lcd.setCursor(0, 1);
              lcd.print("  enrollment!");
              delay(2000);
          
            // set the data rate for the sensor serial port
            finger.begin(57600);
            
            if (finger.verifyPassword()) {
                  Serial.println("Found finger print sensor!");
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Found finger");
                  lcd.setCursor(0, 1);
                  lcd.print(" print sensor!");
                  digitalWrite(buzzer, HIGH);
                  delay(500);
                  digitalWrite(buzzer, LOW);
            } else {
                  Serial.println("\nDid not find fingerprint sensor :(");
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("fingerprint ");
                  lcd.setCursor(0, 1);
                  lcd.print("sensor not found");
                  digitalWrite(buzzer, HIGH);
                  delay(1000);
                  digitalWrite(buzzer, LOW);
                  while (1) { delay(1); }
            }
          
            finger.getTemplateCount();
            Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println("\n templates");
            Serial.println("\nWaiting for valid finger...");
            lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Waiting for ");
              lcd.setCursor(0, 1);
              lcd.print("valid finger...");
}

uint8_t readnumber(void) {
        uint8_t num = 0;
        
        while (num == 0) {
          while (! Serial.available());
          num = Serial.parseInt();
        }
        return num;
}
void loop()
                {
                  uint8_t num = 0;
                  digitalWrite(LED, HIGH);
                  Serial.println("\nPress Enroll or Search!!!");
                  lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Press Enroll or");
                    lcd.setCursor(0, 1);
                    lcd.print("Search!!!");
                    digitalWrite(buzzer, HIGH);
                delay(500);
                digitalWrite(buzzer, LOW);
                  while (num == 0) {
                        while (! Serial.available());
                        num = Serial.parseInt();
                  }
                  if(num==1) //1 enroll, 2 search
                            loop1();
                  else if(num==2)
                            loop2();
                  else
                    Serial.println("\nERROR!!!!!!");
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("ERROR!!!!!!");
   
}

/* Loop 2 to mark/search Attandance*/

void loop2()                     // run over and over again
{
  uint8_t p=100;
  uint8_t k=0;
  while(p!=FINGERPRINT_NOFINGER){
                      p=getFingerprintIDez();
                      if(Serial.available())
                        k=Serial.parseInt();
                       if(k==9)
                        break;
                      if(p==FINGERPRINT_OK)
                        break;
                      if(p==FINGERPRINT_PACKETRECIEVEERR)
                        break;
                      if(p==FINGERPRINT_IMAGEFAIL)
                        break;
                      delay(500);            //don't ned to run this at full speed.
                      }
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("\nImage taken");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("\nNo finger detected");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image taken");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("\nCommunication error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("\nImaging error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Imaging error");
      return p;
    default:
      Serial.println("\nUnknown error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("\nImage converted");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("\nImage too messy");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("\nCommunication error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("\nCould not find fingerprint features");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Could not find ");
    lcd.setCursor(0, 1);
    lcd.print("fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("\nCould not find fingerprint features");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid Image");
      return p;
    default:
      Serial.println("\nUnknown error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("\nFound a print match!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found a print ");
    lcd.setCursor(0, 1);
    lcd.print("match!!!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("\nCommunication error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("\nDid not find a match");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Match not found");
    return p;
  } else {
    Serial.println("\nUnknown error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); 
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found ID #");
    lcd.print(finger.fingerID);
  Serial.print(finger.fingerID); 
  Serial.print(" with confidence of ");
    lcd.setCursor(0, 1);
    lcd.print("confidence");
    lcd.print(finger.confidence);
  Serial.println(finger.confidence);
  delay(1000);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found ID #");
    lcd.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  lcd.setCursor(0, 1);
    lcd.print("confidence");
    lcd.print(finger.confidence);
  Serial.println(finger.confidence);
  delay(1000);
  return finger.fingerID; 
}


/* Loop 1 to enroll/register Biometricn impression*/
void loop1()                     // run over and over again
{
  Serial.println("\nReady to enroll a fingerprint!");
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready to enroll a");
    lcd.setCursor(0, 1);
    lcd.print("fingerprint!");
  Serial.println("\nPlease type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enrolling ID #");
    lcd.print(id);
  delay(200);
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.println("\nWaiting for valid finger to enroll as #"); Serial.println(id);
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting fr valid");
    lcd.setCursor(0, 1);
    lcd.print("finger as # ");
    lcd.print(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("\nImage taken");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("\n.");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("\nCommunication error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("\nImaging error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Imaging error");
      break;
    default:
      Serial.println("\nUnknown error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("\nImage converted");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("\nImage too messy");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("\nCommunication error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("\nCould not find fingerprint features");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Finger not found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("\nCould not find fingerprint features");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Finger not found");
      return p;
    default:
      Serial.println("\nUnknown error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
      return p;
  }
  
  Serial.println("\nRemove finger");
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID  ");
    lcd.print(id);
  p = -1;
  Serial.println("\nPlace same finger again");
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place same ");
    lcd.setCursor(0, 1);
    lcd.print("finger again ");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("\nImage taken");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("\nCommunication error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error ");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("\nImaging error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Imaging error ");
      break;
    default:
      Serial.println("\nUnknown error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error ");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("\nImage converted");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image converted ");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("\nImage too messy");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Image too messy ");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("\nCommunication error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("\nCould not find fingerprint features");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Finger not found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("\nCould not find fingerprint features");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("finger not found");
      return p;
    default:
      Serial.println("\nUnknown error");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error ");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("\nPrints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("\nCommunication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("\nFingerprints did not match");
    return p;
  } else {
    Serial.println("\nUnknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("\nStored!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stored! ");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("\nCommunication error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("\nCould not store in that location");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Could not store ");
    lcd.setCursor(0, 0);
    lcd.print("in that location ");
    
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("\nError writing to flash");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Flash writing error");
    return p;
  } else {
    Serial.println("\nUnknown error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
    return p;
  }   
}
