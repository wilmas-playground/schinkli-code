#include<Arduino.h>

struct Pumpi
{
  
  Pumpi(int anschluss)
  {
    pin = anschluss;
  }
  
  void ein()
  {
    digitalWrite(pin, HIGH);
  }
  void aus()
  {
    digitalWrite(pin, LOW);
  }
    
private:
  uint8_t pin;
  
};

struct Oregano
{
  Oregano(String namen, Pumpi& pumpi) :
  _name (namen),
  _pump (pumpi)
  {};
  
  //todo: implement the safe way, add debug msgs 
  void set_hour(int hour){ _stunde = hour; };
  void set_dauer(int dauer){ _dauer = dauer * 1000; }
  uint32_t get_dauer(){ return _dauer; }
  void set_day(String day) { _tag = day; };

  void pumpe_aktivieren()
  {
    _pump.ein();
    _startzeit = millis();
    _running = true;
    Serial.println("Hochbeet Pumpe eingeschaltet");
  };
  void pumpe_deaktivieren()
  {
    _pump.aus();
    _running = false;
    Serial.println("Hochbeet Pumpe ausgeschaltet");
  };
    
  const String _name;
  
  int _stunde = -1; // Stunde in der eingestaltet wird
  String _tag = ""; 
  bool _aktiv = false;
  bool _running = false;
  unsigned long _startzeit;
private:

  Pumpi& _pump;
  uint32_t _dauer = 0;   // in millisekunden

};
