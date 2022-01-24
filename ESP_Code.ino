
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "BITSFORGE-KOCHI";
char wifiPassword[] = "wpass3055";


char username[] = "f8d31710-6b63-11eb-b767-3f1a8f1211ba";
char password[] = "c086b49e459e9d5dec9047a5313e3b3543708ba5";
char clientID[] = "805d0de0-a7ea-11eb-b767-3f1a8f1211ba";

unsigned long lastMillis = 0;
String p,q,r;
int flag=1;

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();
 if(Serial.available()>0)
 {
  if(Serial.read()=='$')
  {
     p=Serial.readStringUntil(',');
     q=Serial.readStringUntil(',');
     r=Serial.readStringUntil('*');
  }
 }
  Cayenne.virtualWrite(7,p.toInt());
  Cayenne.virtualWrite(8,q.toInt());
  Cayenne.virtualWrite(9,r.toInt());
}
