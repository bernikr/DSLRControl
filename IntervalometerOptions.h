#ifndef IntervalometerOptions_h
#define IntervalometerOptions_h

#include <Arduino.h>
#include "Screen.h"
#include "Intervalometer.h"

class IntervalometerOptions : public Screen{
  #define POS_NUM 7
  private:
    const byte positions[POS_NUM] = {2, 4, 7, 10, 17, 24, 30}; //Mögliche Positionen des Cursers
    char pos;                  //Aktuelle Position des Cursers
    bool sel;                  //Zustand des Cursers (True = ausgewählt)
    long time;                 //Aktuell ausgewälte Zeit
    void (*save)(int, long);   //Funktionspointer zum Speichern im EEPROM  
    void (*swScr)(byte);       //Funktionspointer zum Aufrufen anderer Bildschrime
    Intervalometer* interv;    //Objektpointer zum Intervalometer, insbesondere zum aufrugen von interv->start(int)
    char* temptxt;             //Variable zum Speichern des Ausgabetextes (Pointer wird von getline zurückgegeben)
  public:
    IntervalometerOptions(void (byte), long (int), void (int, long) , Intervalometer*);  //Parameter: showScreen pointer, EEPROMreadLong pointer, EEPROMwriteLong pointer, Intervalometer Objektpointer
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess(){}
};

#endif
