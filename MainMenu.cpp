#include "MainMenu.h"

MainMenu::MainMenu(void (*nswScr)(byte), byte scrnum1, byte scrnum2){
  swScr = nswScr;
  scrnum[0] = scrnum1;
  scrnum[1] = scrnum2;
  mode = false;
  temptxt = new char[17];
}

char* MainMenu::getLine(byte line){
  //Zuweisen der Anzeige zu temptxt
  if(line == 0)
    strcpy(temptxt," Time Lapse.");
  else
    strcpy(temptxt," Interval.");
  
  //Anzeigen des Cursors
  if((mode ? 1 : 0) == line)
    temptxt[0] = 1;
  
  return temptxt;
}

void MainMenu::input(int b){
    mode = !mode;
}

void MainMenu::clicked(){
  swScr(scrnum[mode ? 1 : 0]);
}

