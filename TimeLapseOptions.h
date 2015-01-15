#ifndef TimeLapseOptions_h
#define TimeLapseOptions_h

#include <Arduino.h>
#include "Screen.h"
#include "TimeLapse.h"

class TimeLapseOptions : public Screen {
#define POS_NUM 5
  private:
    const byte positions[POS_NUM] = {0, 7, 17, 24, 30};
    char pos;
    bool sel;
    short time;
    short steps;
    void (*save)(int, int);
    void (*swScr)(byte);
    char* temptxt;
    TimeLapse* timeLap;    //Object Pointer to the timeLapse object
  public:
    TimeLapseOptions(void (byte), int (int), void (int, int), TimeLapse*); //Parameters: showScreen pointer, EEPROMreadLong pointer, EEPROMwriteLong pointer, TimeLapse object pointer
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess() {}
};

#endif
