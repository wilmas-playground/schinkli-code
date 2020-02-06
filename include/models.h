#include<Arduino.h>

struct Plant
{
  Plant(String namen, int pump_pin) :
  _name (namen),
  _pump (pump_pin)
  {
    pinMode(_pump, OUTPUT);
  };
  
  /**
   * Stunde in der die Pflanze gegossen wird
   * 
   * @param hour Stunde von 0 bis 23
   */
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

  /**
   * Einschaltdauer der Pumpe setzen
   * 
   * @param dauer Dauer in sekunden
   */
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

  /**
   * Einschaltdauer der Pumpe abfragen
   * 
   * @return Dauer in milli sekunden
   */
  uint32_t get_dauer(){ return _dauer; }
  
  /**
   * Wochentag an dem gegossen wird
   * 
   * @param day Wochentag in Englisch
   */
  void set_day(String day) 
  { 
    _tag = day;
    Serial.print(_name);
    Serial.print(" am ");
    Serial.print(_tag);
    Serial.println(" giessen"); 
  };

  /// Pumpe einschalten
  void pumpe_aktivieren()
  {
    digitalWrite(_pump, HIGH);
    _startzeit = millis();
    _running = true;
    Serial.print("Pumpe ");
    Serial.print(_name);
    Serial.println(" eingeschaltet");
  };

  /// Pumpe ausschalten
  void pumpe_deaktivieren()
  {
    digitalWrite(_pump, HIGH);
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

  int _pump;
  uint32_t _dauer = 0; // in millisekunden

};
