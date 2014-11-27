#ifndef Screen_h
#define Screen_h

#include <Arduino.h>

//Interface für Screen-Klassen
class Screen{
  public:
    virtual char* getLine(byte) = 0; //Ausgabe der Zeilen 0 und 1
    virtual void clicked() = 0;      //Button Click Event
    virtual void input(int) = 0;     //Drehen des Encoderrades (Richtung durch Vorzeichen bestimmt)
    virtual void loopprocess() = 0;  //Funktion die sooft wie möglich aufgerufen werden sollte, um möglichst genaue Steuerung zu ermöglichen.  
};

#endif
