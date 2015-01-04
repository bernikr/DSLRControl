#include "TimeLapseOptions.h"

#define SAV_LOC 0
TimeLapseOptions::TimeLapseOptions(void (*nswScr)(byte), int (*readEeprom)(int), void (*writeEeprom)(int, int), TimeLapse* nTimeLap){
  time = readEeprom(SAV_LOC);
  steps = readEeprom(SAV_LOC + 2);
  
  save = writeEeprom;
  swScr = nswScr;
  
  timeLap = nTimeLap;
  
  temptxt = new char[17];
}

char* TimeLapseOptions::getLine(byte line){
  
  //Zuweisen der Anzeige zu temptxt
  if(line == 0)
    snprintf(temptxt, 17, " T:%03u  S:%+04i", time, steps);
  else
    strcpy(temptxt,"  Start  Save  X");
  
  //Anzeigen des Cursors
  if(positions[pos]/16 == line)
    temptxt[positions[pos] % 16] = sel ? 2 : 1;
    
  return temptxt;
}

//Verarbeitung der Clickevents
void TimeLapseOptions::clicked(){
  switch(pos){
    case 0:
    case 1:
      sel = !sel;
      break;
    case 2:
      timeLap->start(time, steps);
      break;
    case 3:
      save(SAV_LOC, time);
      save(SAV_LOC + 2, steps);
      break;
    case 4:
      swScr(0);
      break;
  }
}

//Verarbeitung der Encoderdrehund
void TimeLapseOptions::input(int b){
  if(sel) //Wenn Time oder Steps selected verändere diese
    switch(pos){
      case 0:
        time += b;
        while(time > 999) time -= 1000;
        while(time < 0) time += 1000;
        break;
      case 1:
        steps += b;
        while(steps > 999) steps -= 1999;
        while(steps < -999) steps += 1999;
        break;
    }
  else //Sonst ändere die curserposition
    pos = ((pos + b) % POS_NUM + POS_NUM) % POS_NUM; //Hässliche Lösung um einen Positiven Modulo zu erhalten.
}


