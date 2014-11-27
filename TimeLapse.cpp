#include "TimeLapse.h"

TimeLapse::TimeLapse(void (*nswScr)(byte), void (*nTrigg)(bool), byte nposInArray){
  swScr = nswScr;
  trigg = nTrigg;
  
  posInArray = nposInArray;
  
  temptxt = new char[17];
}


char* TimeLapse::getLine(byte line){
  //Zuweisen der Anzeige zu temptxt (Aufteilung der Millisekundenzahl in H,m,s und hundertstel
  if(line == 0){
    byte hours = time/1000/60/60;
    byte minutes = (time/1000/60)%60;
    byte seconds = (time/1000)%60;
    byte centiseconds = (time/10)%100;
    
    snprintf(temptxt, 17, "   %1u:%02u:%02u.%02u", hours, minutes, seconds, centiseconds);
  }else if(!paused) //Darstellen von Pause und Resume
    strcpy(temptxt,"     Pauselol  ");
  else
    strcpy(temptxt,"     Resume    X");
  
  //Anzeigen des Cursors
  if(positions[pos]/16 == line)
    temptxt[positions[pos] % 16] = 1;
    
  return temptxt;
}

//Verarbeitung der Clickevents
void TimeLapse::clicked(){
  switch(pos){
    case 0:  //Pause/Resume verarbeiten
      paused = !paused;
      lasttime = millis();
      trigg(!paused);
      break;
    case 1:  //Exit zo Options
      swScr(posInArray - 1);
      break;
  }
}

//Verarbeitung der Encoderdrehund
void TimeLapse::input(int b){
  if(paused) //Wenn Pausiert 0 und 1 möglich, sonst nur 0
    pos = ((pos + b) % POS_NUM + POS_NUM) % POS_NUM; //Hässliche Lösung um einen Positiven Modulo zu erhalten.
  else
    pos = 0;
}

void TimeLapse::start(short ntime, short nsteps){
  swScr(posInArray);    //Anzeigen des Intervalometers
  pos = 0;              //Setzen des Cursers auf 0 um unschöne Bugs zu vermeiden
  time = ntime;         //Zuweisen der zu belichtenden Zeit
  lasttime = millis();  //Starte timer
  paused = false;       //Verhindern das der Timer gepaused startet
  trigg(true);          //Starten der Belichtung
}

void TimeLapse::loopprocess(){
  /*if(!paused){
    long tempmill = millis();
    time -= (tempmill - lasttime);  //füge zeitdiffernz zu
    lasttime = tempmill;
    if(time < 0){  //Wenn aus
      trigg(false);
      swScr(posInArray - 1);
    }
  }*/
}
