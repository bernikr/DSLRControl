#include "TimeLapse.h"

TimeLapse::TimeLapse(void (*nswScr)(byte), void (*nTrigg)(bool), void (*nMov)(byte), byte nposInArray){
  swScr = nswScr;
  trigg = nTrigg;
  mov = nMov;
  
  posInArray = nposInArray;
  
  temptxt = new char[17];
}


char* TimeLapse::getLine(byte line){
  //Zuweisen der Anzeige zu temptxt (Aufteilung der Millisekundenzahl in H,m,s und hundertstel
  if(line == 0)
    snprintf(temptxt, 17, " T:%03u  S:%+04i", time, steps);
  else if(!paused) //Darstellen von Pause und Resume
    strcpy(temptxt,"     Pause  ");
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
  consttime = ntime;    //Zuweisen der zu belichtenden Zeit
  conststeps = nsteps;  //Zuweisen der Schritte
  paused = false;       //Verhindern das der Timer gepaused startet
  newloop();
}

void TimeLapse::loopprocess(){
  if(!paused){
    long tempmill = millis();
    if((tempmill - lasttime) >= 1000){ //Wenn eine Sekunde vergangen ist
      time -= (tempmill - lasttime)/1000;       //ziehe diese von time ab
      lasttime += (tempmill - lasttime)/1000*1000;   //und füge sie lasttime hinzu
    }
    if(time < 0){  //Wenn die Zeit aus ist
        trigg(false);
        newloop();
    }
  }
}

void TimeLapse::newloop(){
  time = consttime;     //zurücksetzten der time Variable
  steps = conststeps;   //zurücksetzten der steps Variable
  lasttime = millis();  //Starte timer
  trigg(true);          //Starten der Belichtung
}
