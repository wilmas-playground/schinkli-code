#include <Arduino.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "models.h"

const char *ssid     = "shesoulV3";
const char *password = "supereight";

const long utcOffsetInSeconds = 3600;

String daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// NTP client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// systems
Pumpi _p1 {D0};
Oregano hochbeet {"Hochbeet", _p1};

// function flags
bool done_for_today = false;

void setup(){

  // Test setup
  hochbeet._tag = "Sunday";
  hochbeet._stunde = 18;
  hochbeet.set_dauer(5);
  // end

  Serial.begin(115200);

  hochbeet.pumpe_deaktivieren();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {

  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.println(timeClient.getFormattedTime());

  if(daysOfTheWeek[timeClient.getDay()] == hochbeet._tag)
  {
    if(timeClient.getHours() == hochbeet._stunde && !done_for_today)
    {
      if(!hochbeet._running)
      {
        hochbeet.pumpe_aktivieren();
      }
      else if(millis()-hochbeet._startzeit >= hochbeet.get_dauer())
      {
        hochbeet.pumpe_deaktivieren();
        done_for_today = true;
      }
      
    }
  }
  else
  {
    done_for_today = false;
  }
  
  delay(2000);
}
