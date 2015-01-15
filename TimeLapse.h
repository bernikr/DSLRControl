#ifndef TimeLapse_h
#define TimeLapse_h

#include <Arduino.h>
#include "Screen.h"

class TimeLapse : public Screen {
#define POS_NUM 2
#define TRMILLIS 100
  private:
    const byte positions[POS_NUM] = {20,  30}; 
    char pos;
    short time;
    short steps;
    short consttime;
    short conststeps;
    char* temptxt;
    void (*swScr)(byte);
    void (*trigg)(bool);  //function pointer to trigger camera
    void (*mov)(byte);    //function pointer to move stepper
    void (*rState)(bool); //function pointer to aktivate yellow state
    bool paused;
    byte posInArray;
    long lasttime;
    long starttime;
    void newloop();
  public:
    TimeLapse(void (byte), void (bool), void (byte), void (bool), byte);  //Parameters: showScreen pointer, camera trigger pointer, stepper mover pointer, readyState pointer, own position in array
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess();
    void start(short, short);     //function to start the time lapse, Parameters: time, steps
};

#endif
