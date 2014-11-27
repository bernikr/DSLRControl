#ifndef Intervalometer_h
#define Intervalometer_h

#include <Arduino.h>
#include "Screen.h"

class Intervalometer : public Screen{
  #define POS_NUM 2
  private:
    const byte positions[POS_NUM] = {20,  30};  //Mögliche Curserpositionen
    char pos;             //Aktuelle Curserposition
    long time;            //Aktuelle Zeit
    char* temptxt;        //Variable zum Speichern des Ausgabetextes (Pointer wird von getline zurückgegeben)
    void (*swScr)(byte);  //Funktionspointer zum Darstellen eines anderen Screens
    void (*trigg)(bool);  //Funktionspointer zum Auslösen der Kamera
    bool paused;          //Läuft der Timer: True = Timer angehalten, False = Timer läuft
    byte posInArray;      //Die Position dieses Bildschirmes im Array (Wird von Start gebraucht, posInArray - 1 sollten die zugehörigen Optionen sein
    long lasttime;        //Zeit in Millisecunden beim Letzen loopdurchlauf (Wird für differez gebraucht)
  public:
    Intervalometer(void (byte), void (bool), byte);  //Parameter: showScreen pointer, Camera trigger Pointer, Eigene Position im Array
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess();
    void start(long);     //Funktion, welche zum Starten des Timers von den Optionen aufgerufen wird.
};

#endif
