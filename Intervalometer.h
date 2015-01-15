#ifndef Intervalometer_h
#define Intervalometer_h

#include <Arduino.h>
#include "Screen.h"

#define POS_NUM 2 //Number of curser positions

class Intervalometer : public Screen {
  private:
    const byte positions[POS_NUM] = {20,  30};  //possible curser positions
    char pos;             //curser position at the moment
    long time;            //remaining time
    char* temptxt;        //variable to save the output line
    void (*swScr)(byte);  //Function pointer to display another screen
    void (*trigg)(bool);  //Function pointer to trigger the camera
    bool paused;          //is the timer paused?
    byte posInArray;      //The Position of this screen in the array (Is needed by start(), posInArray - 1 must be the InvervalometerOptions)
    long lasttime;        //Time in ms at the last loop
  public:
    Intervalometer(void (byte), void (bool), byte);  //Parameters: showScreen pointer, camera trigger pointer, own position in array
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess();
    void start(long);     //start the timer (called by the options)
};

#endif
