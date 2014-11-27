#ifndef TimeLapse_h
#define TimeLapse_h

#include <Arduino.h>
#include "Screen.h"

class TimeLapse : public Screen{
  #define POS_NUM 2
  private:
    const byte positions[POS_NUM] = {20,  30};  //Mögliche Curserpositionen
    char pos;             //Aktuelle Curserposition
    short time;           //Aktuelle Zeit
    short steps;          //Akutelle Steps
    char* temptxt;        //Variable zum Speichern des Ausgabetextes (Pointer wird von getline zurückgegeben)
    void (*swScr)(byte);  //Funktionspointer zum Darstellen eines anderen Screens
    void (*trigg)(bool);  //Funktionspointer zum Auslösen der Kamera
    bool paused;          //Läuft der Timer: True = Timer angehalten, False = Timer läuft
    byte posInArray;      //Die Position dieses Bildschirmes im Array (Wird von Start gebraucht, posInArray - 1 sollten die zugehörigen Optionen sein
    long lasttime;        //Zeit in Millisecunden beim Letzen loopdurchlauf (Wird für differez gebraucht)
  public:
    TimeLapse(void (byte), void (bool), byte);  //Parameter: showScreen pointer, Camera trigger Pointer, Eigene Position im Array
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess();
    void start(short, short);     //Funktion, welche zum Starten des Timers von den Optionen aufgerufen wird. Parameter: time, steps
};

#endif
