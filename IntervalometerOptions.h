#ifndef IntervalometerOptions_h
#define IntervalometerOptions_h

#include <Arduino.h>
#include "Screen.h"
#include "Intervalometer.h"

class IntervalometerOptions : public Screen {
#define POS_NUM 7
  private:
    const byte positions[POS_NUM] = {2, 4, 7, 10, 17, 24, 30}; //possible curser positions
    char pos;                  //curser position at the moment
    bool sel;                  //is the cursor in selected mode
    long time;                 //selected time
    void (*save)(int, long);   //function pointer to save in EEPROM
    void (*swScr)(byte);       //function pointer to display other screens
    Intervalometer* interv;    //Objectpointer to the Intervalometer object
    char* temptxt;             //variable to save the output line
  public:
    IntervalometerOptions(void (byte), long (int), void (int, long) , Intervalometer*);  //Parameters: showScreen pointer, EEPROMreadLong pointer, EEPROMwriteLong pointer, Intervalometer object pointer
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess() {}
};

#endif
