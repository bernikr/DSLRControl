#ifndef MainMenu_h
#define MainMenu_h

#include <Arduino.h>
#include "Screen.h"

class MainMenu : public Screen {
  private:
    void (*swScr)(byte);  //Function pointer to display other screens
    byte scrnum[2];       //Positions of the optionsscreens in the array
    bool mode;            //selected mode (false = upper line)
    char* temptxt;        //variable to save the output line
  public:
    MainMenu(void (byte), byte, byte); //Parameters:  showScreen pointer, position of the first screen in the array, position of the second screen
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess() {}
};

#endif
