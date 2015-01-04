#include "IntervalometerOptions.h"


#define SAV_LOC 4

IntervalometerOptions::IntervalometerOptions(void (*nswScr)(byte), long (*readEeprom)(int), void (*writeEeprom)(int, long), Intervalometer* ninterv){
  time = readEeprom(SAV_LOC);
  
  swScr = nswScr;
  save = writeEeprom;
  interv = ninterv;

  temptxt = new char[17];
}


char* IntervalometerOptions::getLine(byte line){
  //Zuweisen der Anzeige zu temptxt
  if(line == 0){
    byte hours = time/1000/60/60;
    byte minutes = (time/1000/60)%60;
    byte seconds = (time/1000)%60;
    byte centiseconds = (time/10)%100;
    
    snprintf(temptxt, 17, "   %1u:%02u:%02u.%02u", hours, minutes, seconds, centiseconds);
  }else
    strcpy(temptxt,"  Start  Save  X");
  
  //Anzeigen des Cursors
  if(positions[pos]/16 == line)
    temptxt[positions[pos] % 16] = sel ? 2 : 1;
    
  return temptxt;
}

//Verarbeitung der Clickevents
void IntervalometerOptions::clicked(){
  switch(pos){
    case 0:
    case 1:
    case 2:
    case 3:
      sel = !sel;
      break;
    case 4:
      interv->start(time);
      break;
    case 5:
      save(SAV_LOC, time);
      break;
    case 6:
      swScr(0);
      break;
  }
}

//Verarbeitung der Encoderdrehund
void IntervalometerOptions::input(int b){
  if(sel){ //Wenn Time oder Steps selected verändere diese
   switch(pos){
     case 0:
       time += b * 3600000;
       break;
     case 1:    
       time += b * 60000;
       break;
     case 2:
       time += b * 1000;
       break;
     case 3:
       time += b * 10;
       break;
   }
   if(time < 0) time = 0;
  }else //Sonst ändere die curserposition
    pos = ((pos + b) % POS_NUM + POS_NUM) % POS_NUM; //Hässliche Lösung um einen Positiven Modulo zu erhalten.
}
