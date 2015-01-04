#ifndef MainMenu_h
#define MainMenu_h

#include <Arduino.h>
#include "Screen.h"

class MainMenu : public Screen{
  private:
    void (*swScr)(byte);  //Funktionspointer zum Aufrufen anderer Bildschrime
    byte scrnum[2];       //Stellen der aufzurufenden Bildschrime im Array - Array
    bool mode;            //Aktuell selectierter Punkt (false = obere line)
    char* temptxt;        //Variable zum Speichern des Ausgabetextes (Pointer wird von getline zurückgegeben)
  public:
    MainMenu(void (byte), byte, byte); //Parameter:  showScreen pointer, Position des Ersten Screens im Array, Position des zweiten Screens
    char* getLine(byte);
    void clicked();
    void input(int);
    void loopprocess(){}
};

#endif
