#include <Arduino.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

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

// web UI server
AsyncWebServer server(80);
const char* PARAM_INPUT_DAY_1 = "due_day_1";
const char* PARAM_INPUT_HOUR_1 = "due_hour_1";
const char* PARAM_INPUT_DURATION_1 = "due_duration_1";

// function flags
bool done_for_today = false;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h1> Schinkli's Bewaesserungssystem </h1>
  <h2> Hochbeet </h2>
  <form action="/get">
    <p> Wochentag: </p> 
    <input type="text" name="due_day_1">
    <input type="submit" value="speichern">
  </form>
  
  <form action="/get">
    <p> Startzeit[h]: </p>
    <input type="number" name="due_hour_1">
    <input type="submit" value="speichern">
  </form>
  
  <form action="/get">
    <p> Dauer[s]: </p>
    <input type="number" name="due_duration_1">
    <input type="submit" value="speichern">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup(){

  // Test setup
  hochbeet._tag = "Sunday";
  hochbeet._stunde = 18;
  hochbeet.set_dauer(5);
  // Test end

  Serial.begin(115200);

  hochbeet.pumpe_deaktivieren();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  // Send a GET request to <ESP_IP>/get?due_day_1=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    
    if (request->hasParam(PARAM_INPUT_DAY_1)) {
      hochbeet.set_day(request->getParam(PARAM_INPUT_DAY_1)->value());
    }
    if (request->hasParam(PARAM_INPUT_HOUR_1)) {
      hochbeet.set_hour(request->getParam(PARAM_INPUT_HOUR_1)->value().toInt());
    }
    if (request->hasParam(PARAM_INPUT_DURATION_1)) {
      hochbeet.set_dauer(request->getParam(PARAM_INPUT_DURATION_1)->value().toInt());
    }
    request->send_P(200, "text/html", index_html);
    
    Serial.println(inputMessage);
  });

  server.onNotFound(notFound);
  server.begin();
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