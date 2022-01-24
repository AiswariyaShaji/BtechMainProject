/*
  CONNECTION
  Placing 3.3V jumper to top
  From left
  pin1 : 5V  pin2:GND  pin3:TXD  pin4:RXD

  No connection pins
  pin5:TOUCH  pin6:3.3VT
*/

#include <Adafruit_Fingerprint.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Fonts/FreeSans9pt7b.h>
#include <FreeDefaultFonts.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>


#define PI 3.1415926535897932384626433832795
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define  AQUA 0x04FF
#define PINK 0xF81F

#define mySerial Serial1
MCUFRIEND_kbv tft;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


#define up 37
#define down 36
#define select 35
#define back 34
#define can1 33
#define can2 32
#define can3  31
#define buzzer 42
#define max_voter 50

bool flag1 = false, flag2 = false, flag3 = false, flag = false, flag_delete = false, flag_enroll = false, flag_start = true;
int vote1, vote2, vote3;
int doneArray[max_voter] = {NULL};
long counter = 0, timer = 0;
int col[8], count = 0, count_delete = 0, count_enroll = 0;
uint8_t id;



int vote();
bool exitFlag = true, drawFlag = false;
void setup()
{
  Serial.begin(9600);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(select, INPUT);

  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(45);


  tft.fillScreen(BLACK);
  text(50, 120, 2, &FreeSerif12pt7b, CYAN, "VOTING");
  text(40, 170, 2, &FreeSerif12pt7b, CYAN, "MACHINE");
  delay(2000);
  tft.fillScreen(BLACK);
  text(90, 60, 1, &FreeSans12pt7b, WHITE, "Booting up...");

  col[0] = tft.color565(155, 0, 50);
  col[1] = tft.color565(170, 30, 80);
  col[2] = tft.color565(195, 60, 110);
  col[3] = tft.color565(215, 90, 140);
  col[4] = tft.color565(230, 120, 170);
  col[5] = tft.color565(250, 150, 200);
  col[6] = tft.color565(255, 180, 220);
  col[7] = tft.color565(255, 210, 240);
  for (int i = 8; i > 0; i--) {
    tft.fillCircle(150 + 40 * (cos(-i * PI / 4)), 160 + 40 * (sin(-i * PI / 4)), 7, col[0]); delay(35);
    tft.fillCircle(150 + 40 * (cos(-(i + 1)*PI / 4)), 160 + 40 * (sin(-(i + 1)*PI / 4)), 7, col[1]); delay(35);
    tft.fillCircle(150 + 40 * (cos(-(i + 2)*PI / 4)), 160 + 40 * (sin(-(i + 2)*PI / 4)), 7, col[2]); delay(35);
    tft.fillCircle(150 + 40 * (cos(-(i + 3)*PI / 4)), 160 + 40 * (sin(-(i + 3)*PI / 4)), 7, col[3]); delay(35);
    tft.fillCircle(150 + 40 * (cos(-(i + 4)*PI / 4)), 160 + 40 * (sin(-(i + 4)*PI / 4)), 7, col[4]); delay(35);
    tft.fillCircle(150 + 40 * (cos(-(i + 5)*PI / 4)), 160 + 40 * (sin(-(i + 5)*PI / 4)), 7, col[5]); delay(35);
    tft.fillCircle(150 + 40 * (cos(-(i + 6)*PI / 4)), 160 + 40 * (sin(-(i + 6)*PI / 4)), 7, col[6]); delay(35);
    tft.fillCircle(150 + 40 * (cos(-(i + 7)*PI / 4)), 160 + 40 * (sin(-(i + 7)*PI / 4)), 7, col[7]); delay(35);
  }

  while (!Serial);
  delay(100);
  //Serial.println("Voting Machine");
  delay(500);

  
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {   //commented as to check tft...uncomment once a working fingerprint module is available
    //Serial.println("Ready for Voting!");
  } else {
    //Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  //Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");

  vote1 = vote2 = vote3 = 0;
  tft.fillScreen(BLACK);
  

}

void loop()
{
  if (flag_start) {
    flag_start = false;
    tft.fillScreen(BLACK);
    text(9, 75, 1, &FreeSans9pt7b, CYAN, "1. VOTE");
    text(9, 125, 1, &FreeSans9pt7b, CYAN, "2. ENROLL");
    text(9, 175, 1, &FreeSans9pt7b, CYAN, "3. DELETE DATA");
    text(65, 20, 1, &FreeSans9pt7b, GREENYELLOW, "VOTING MACHINE");
    text(9, 235, 1, &FreeSans9pt7b, RED, "/\\ \\/: SELECTION");
    text(240, 235, 1, &FreeSans9pt7b, RED, "SEL: OK");
  }


  if (digitalRead(down) == 0)
  {
    while (digitalRead(down) == 0);
    drawFlag = true;
    count++;
    //Serial.println(count);
    if (count > 3)
    {
      count = 1;
    }

  }
  if (digitalRead(up) == 0)
  {
    while (digitalRead(up) == 0);
    drawFlag = true;
    count--;
    if (count < 1)
    {
      count = 3;
    }
  }
  if (digitalRead(select) == 0)
  {
    while (digitalRead(select) == 0);
    if (count == 1)text(9, 75, 1, &FreeSans9pt7b, PINK, "1. VOTE");
    else if (count == 2)text(9, 125, 1, &FreeSans9pt7b, PINK, "2. ENROLL");
    else if (count == 3)text(9, 175, 1, &FreeSans9pt7b, PINK, "3. DELETE DATA");
    delay(1000);
    flag = true;
  }


  //Serial.println(count);

  if (drawFlag) {
    drawFlag = false;
    tft.drawRoundRect(4, 55, 180, 30, 10, BLACK); //3
    tft.drawRoundRect(4, 105, 180, 30, 10, BLACK); //2
    tft.drawRoundRect(4, 155, 180, 30, 10, BLACK); //1
    if (count == 1)
    {

      tft.drawRoundRect(4, 55, 180, 30, 10, GREENYELLOW);
    }
    else if (count == 2)
    {

      tft.drawRoundRect(4, 105, 180, 30, 10, GREENYELLOW); //2
    }
    else
    {

      tft.drawRoundRect(4, 155, 180, 30, 10, GREENYELLOW); //3
    }
  }

  /*if (flag1 == 0) {
    Serial.println("Place your finger");
    flag1 = true;
    }
  */

  if (flag)
  {
    flag = false;
    tft.fillScreen(BLACK);
    flag_start = true;
    //while(digitalRead(select)==0);
    exitFlag = true;
    while (exitFlag) { 
      if (count == 1)
      {

        text(70, 120, 1, &FreeSerif12pt7b, ORANGE, "Place your finger");
        int result = getFingerprintIDez();

        if (result > 0 ) {
          if (doneArray[(result - 1)] == 0)
          {
            tft.fillScreen(BLACK);
            text(70, 120, 1, &FreeSerif12pt7b, ORANGE, "VOTER ID : " + String(result));
            delay(5000);
            int ret = vote();
            if (ret == 1 || ret == 2 || ret == 3) {
              doneArray[(result - 1)] = 1;
              exitFlag = false;
            }
            if (ret == 4)
            {

              //Serial.println("TIME OUT");
              exitFlag = false;


            }

          }
          else
          {
            tft.fillScreen(BLACK);
            text(70, 80, 1, &FreeSerif12pt7b, RED, "REJECTED");
            text(60, 95 , 2, &FreeSerif12pt7b, RED, "---------");
            text(30, 130, 1, &FreeSerif12pt7b, ORANGE, "You have already voted");
            text(70, 160, 1, &FreeSerif12pt7b, DARKGREY, "VOTER ID: " + String(result));
            delay(2000);
            digitalWrite(buzzer, HIGH);
            delay(250);
            digitalWrite(buzzer, LOW);
            tft.fillScreen(BLACK);
            exitFlag = false;

          }

        }
   
        delay(500);
      }
      else if (count == 2)
      {
        text(9, 235, 1, &FreeSans9pt7b, RED, "/\\ \\/: ID+ / ID-");
        text(240, 235, 1, &FreeSans9pt7b, RED, "SEL: OK");

        tft.fillScreen(BLACK);
        text(60, 60, 1, &FreeSans12pt7b, ORANGE, "SELECT YOUR ID");
        text(90, 90, 1, &FreeSans12pt7b, DARKGREY, "range(0-50)");
        showmsgXY(100, 140, 2, &FreeSans9pt7b, "ID :", AQUA);
        count_enroll = keypad();
        tft.fillScreen(BLACK);
        text(70, 100, 1, &FreeSerif12pt7b, ORANGE, "ENROLLMENT");
        text(90, 150, 1, &FreeSerif12pt7b, ORANGE, "Processing");
        delay(800);
        text(200, 150, 2, &FreeSerif12pt7b, ORANGE, ".");
        delay(800);
        text(220, 150, 2, &FreeSerif12pt7b, ORANGE, ".");
        delay(800);
        text(240, 150, 2, &FreeSerif12pt7b, ORANGE, ".");
        delay(1000);
        flag_enroll = false;

       
        id = count_enroll;
        while (!  getFingerprintEnroll() );
        tft.fillScreen(BLACK);
        exitFlag = false;
        //}

      }
      else
      {
        text(20, 35, 1, &FreeSans9pt7b, WHITE, "Are you sure to delete database?");
        text(130, 100, 1, &FreeSans9pt7b, WHITE, "YES");
        text(130, 145, 1, &FreeSans9pt7b, WHITE, "NO");
        if (digitalRead(down) == 0)
        {
          while (digitalRead(down) == 0);
          {
            flag_delete = true;
            count_delete++;
            //Serial.println(count);
            if (count_delete > 2)
            {
              count_delete = 1;
            }
          }
        }
        if (digitalRead(up) == 0)
        {
          while (digitalRead(up) == 0);
          {
            flag_delete = true;
            count_delete--;
            if (count_delete < 1)
            {
              count_delete = 2;
            }
          }
        }

        if (flag_delete) {
          flag_delete = false;
          tft.drawRoundRect(99, 80, 100, 30, 10, BLACK); //yes
          tft.drawRoundRect(99, 125, 100, 30, 10, BLACK); //NO
          if (count_delete == 1)
          {
            tft.drawRoundRect(99, 80, 100, 30, 10, CYAN);
          }
          if (count_delete == 2)
          {
            tft.drawRoundRect(99, 125, 100, 30, 10, CYAN); //2
          }
        }
        if (digitalRead(select) == 0)
        {
          tft.fillScreen(BLACK);
          while (digitalRead(select) == 0);
          ////while(digitalRead(back) || count==1) try this if tft doenot show place ur fing again.
          if (count_delete == 1)
          {

            finger.emptyDatabase();
            text(60, 100, 1, &FreeSans9pt7b, YELLOW, " Database Deleted !!");
            delay(2000);
            text(20, 130, 1, &FreeSans9pt7b, RED, "Press Reset button to restart !");
            while (1);


          }
          else
          {
            exitFlag = false;
          }
        }
      }


      //  flag1 == false;
    }
  }
}

int vote()
{

  counter = millis();
  timer = millis();
  int can1val = 0, can2val = 0, can3val = 0;
  tft.fillRect(15, 30, 160, 40, BLACK);
  tft.fillScreen(BLACK);
  //Serial.println("press the button to place your vote");
  text(50, 100, 1, &FreeSans9pt7b, LIGHTGREY, "PRESS THE BUTTON ");
  text(45, 130, 1, &FreeSans9pt7b, LIGHTGREY, "TO PLACE YOUR VOTE");
  delay(2000);
  while (1) {
    if (digitalRead(can1) == 0) {
      can1val = 1;
      tft.fillScreen(BLACK);
      break;
    }
    else if (digitalRead(can2) == 0) {
      tft.fillScreen(BLACK);
      can2val = 1;
      break;
    }
    else if (digitalRead(can3) == 0) {
      tft.fillScreen(BLACK);
      can3val = 1;
      break;
    }
    else if (millis() - counter > 15000)
    {
      tft.fillScreen(BLACK);
      break;
    }
  }
  if (can1val == 1)
  {
    vote1++;
   
    text(30, 120, 1, &FreeSans9pt7b, YELLOW, "VOTED FOR CANDIDATE 1");
    
    delay(2000);
  
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    tft.fillScreen(BLACK);
    Serial.println("$" + String(vote1) + "," + String(vote2) + "," + String(vote3) + "*");
    return 1;


  }
  else if (can2val == 1)
  {
    vote2++;
    //Serial.println("VOTED for can2");
    text(30, 120, 1, &FreeSans9pt7b, YELLOW, "VOTED FOR CANDIDATE 2");
    //while (digitalRead(down) == 1);
    delay(2000);
    //Serial.println("VOTE 2 : "+ String(vote2));
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    tft.fillScreen(BLACK);
    Serial.println("$" + String(vote1) + "," + String(vote2) + "," + String(vote3) + "*");
    return 2;
  }
  else if (can3val == 1)
  {
    vote3++;
    //Serial.println("VOTED for can3");
    text(30, 120, 1, &FreeSans9pt7b, YELLOW, "VOTED FOR CANDIDATE 3");
    //while (digitalRead(back) == 0);
    delay(2000);
    //Serial.println("VOTE 3 : "+ String(vote3));
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    tft.fillScreen(BLACK);
    Serial.println("$" + String(vote1) + "," + String(vote2) + "," + String(vote3) + "*");
    return 3;
  }
  else
  {

    tft.fillScreen(BLACK);
    text(40, 120, 2, &FreeSans9pt7b, RED, "TIME OUT!!");
    delay(2000);
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    tft.fillScreen(BLACK);
    //Serial.println("ERROR");
    return 4;


  }
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);

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

  
  return finger.fingerID;
}




/************Subfn for enrolling**************/



uint8_t getFingerprintEnroll() {

  tft.fillScreen(BLACK);
  int p = -1;
  //Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  text(10, 120, 1, &FreeSans12pt7b, RED, "Place a valid finger. ID#" + String(count_enroll));
  delay(2000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        tft.fillScreen(BLACK);
        text(9, 60, 1, &FreeSans12pt7b, DARKGREY, "#");
        text(9, 100, 1, &FreeSans12pt7b, DARKGREY, "#");
        text(9, 140, 1, &FreeSans12pt7b, DARKGREY, "#");
        text(9, 180, 1, &FreeSans12pt7b, DARKGREY, "#");
        text(30, 60, 1, &FreeSerif12pt7b, LIGHTGREY, "Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        //Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  //Serial.println("Remove finger");
  text(30, 100, 1, &FreeSerif12pt7b, ORANGE, "Remove finger");
  delay(400);
  text(30, 100, 1, &FreeSerif12pt7b, LIGHTGREY, "Remove finger");
  delay(400);
  text(30, 100, 1, &FreeSerif12pt7b, ORANGE, "Remove finger");
  delay(400);
  text(30, 100, 1, &FreeSerif12pt7b, LIGHTGREY, "Remove finger");
  delay(400);
  text(30, 100, 1, &FreeSerif12pt7b, ORANGE, "Remove finger");
  delay(400);
  text(30, 100, 1, &FreeSerif12pt7b, LIGHTGREY, "Remove finger");
  delay(400);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //Serial.print("ID "); Serial.println(id);
  p = -1;
  //Serial.println("Place same finger again");
  text(30, 140, 1, &FreeSerif12pt7b, LIGHTGREY, "Place same finger again");
  delay(400);
  text(30, 140, 1, &FreeSerif12pt7b, ORANGE, "Place same finger again");
  delay(400);
  text(30, 140, 1, &FreeSerif12pt7b, LIGHTGREY, "Place same finger again");
  delay(400);
  text(30, 140, 1, &FreeSerif12pt7b, ORANGE, "Place same finger again");
  delay(400);
  text(30, 140, 1, &FreeSerif12pt7b, LIGHTGREY, "Place same finger again");
  delay(400);
  text(30, 140, 1, &FreeSerif12pt7b, ORANGE, "Place same finger again");
  delay(400);
  text(30, 140, 1, &FreeSerif12pt7b, LIGHTGREY, "Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        //Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  //Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
    text(30, 180, 1, &FreeSerif12pt7b, YELLOW, "Prints matched!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    text(30, 180, 1, &FreeSerif12pt7b, RED, "Fingerprints did not match");
    delay(2000);
    tft.fillScreen(BLACK);
    text(100, 120, 1, &FreeSerif12pt7b, WHITE, "Try Again !!");
    delay(2000);
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  //Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    tft.fillScreen(BLACK);
    //Serial.println("Stored!");
    text(100, 120, 1, &FreeSerif12pt7b, WHITE, "STORED");
    delay(2000);
    return 1;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }
}

void text(int x, int y, int sz, const GFXfont *f, int colour, String msg)
{
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(colour);
  tft.setTextSize(sz);
  tft.print(msg);
}
void showmsgXY(int x, int y, int sz, const GFXfont *f, String msg, int colour)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(colour);
  tft.setTextSize(sz);
  tft.print(msg);
}

int keypad() {
  long HOLD_DELAY = 1000;
  int max = 50, min = 1;
  unsigned long initTime = 0;
  bool flag = false, flagB = false;

  while (digitalRead(select)) {
    if (!digitalRead(up)) {
      flag = true;
      initTime = millis();
      while (!digitalRead(up)) {
        if (millis() - initTime > HOLD_DELAY) {
          flag = false;
          while (!digitalRead(up)) {
            if (count++ > max)count = min;
            delay(100);
            tft.fillRect(160, 110, 100, 100, BLACK);
            showmsgXY(170, 140, 2, &FreeSans9pt7b, String(count), AQUA);
          }
        }
      }
      if (flag) {
        if (++count > max)count = min;
        while (!digitalRead(up));
        delay(100);
        tft.fillRect(160, 110, 100, 100, BLACK);
        showmsgXY(170, 140, 2, &FreeSans9pt7b, String(count), AQUA);
      }
    }
    if (!digitalRead(down)) {
      flagB = true;
      initTime = millis();
      while (!digitalRead(down)) {
        if (millis() - initTime > HOLD_DELAY) {
          flagB = false;
          while (!digitalRead(down)) {
            if (count-- < min)count = max;
            delay(100);
            tft.fillRect(160, 110, 100, 100, BLACK);
            showmsgXY(170, 140, 2, &FreeSans9pt7b, String(count), AQUA);
          }
        }
      }
      if (flagB) {
        if (--count < min)count = max;
        while (!digitalRead(down));
        delay(100);
        tft.fillRect(160, 110, 100, 100, BLACK);
        showmsgXY(170, 140, 2, &FreeSans9pt7b, String(count), AQUA);
      }
    }
  }
  return (count);
}
