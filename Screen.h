#ifndef Screen_h
#define Screen_h

#include <Arduino.h>

//Interface for all of the screen clases
class Screen {
  public:
    virtual char* getLine(byte) = 0; //output of the lines 0 and 1
    virtual void clicked() = 0;      //Button click event
    virtual void input(int) = 0;     //turning of the encoder event
    virtual void loopprocess() = 0;  //main loop of the class
};

#endif
