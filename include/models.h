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
  
  void set_hour(int hour)
  { 
    if(hour < 24) {
      _stunde = hour;
    }
    else
    {
      _stunde = 0;
    }
    Serial.print("Giessen um ");
    Serial.print(_stunde);
    Serial.println(":00 Uhr");
  };

  void set_dauer(int dauer){
    if(dauer > 600)
    {
      dauer = 600;
    }
    _dauer = dauer * 1000;
    Serial.print("Pumpe ");
    Serial.print(dauer);
    Serial.println(" sekuden einschalten");
  };

  uint32_t get_dauer(){ return _dauer; }
  
  void set_day(String day) 
  { 
    _tag = day;
    Serial.print(_name);
    Serial.print(" am ");
    Serial.print(_tag);
    Serial.println(" giessen"); 
  };

  void pumpe_aktivieren()
  {
    _pump.ein();
    _startzeit = millis();
    _running = true;
    Serial.print("Pumpe ");
    Serial.print(_name);
    Serial.println(" eingeschaltet");
  };

  void pumpe_deaktivieren()
  {
    _pump.aus();
    _running = false;
    Serial.print("Pumpe ");
    Serial.print(_name);
    Serial.println(" ausgeschaltet");
  };
    
  const String _name;
  
  int _stunde = -1; // Stunde in der eingestaltet wird
  String _tag = ""; 
  bool _aktiv = false;
  bool _running = false;
  unsigned long _startzeit;

private:

  Pumpi& _pump;
  uint32_t _dauer = 0; // in millisekunden

};
