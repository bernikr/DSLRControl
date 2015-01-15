#include "TimeLapse.h"

TimeLapse::TimeLapse(void (*nswScr)(byte), void (*nTrigg)(bool), void (*nMov)(byte), void (*nready)(bool), byte nposInArray) {
  swScr = nswScr;
  trigg = nTrigg;
  mov = nMov;
  rState = nready;

  posInArray = nposInArray;

  temptxt = new char[17];
}


char* TimeLapse::getLine(byte line) {
  if (line == 0)
    snprintf(temptxt, 17, " T:%03u  S:%+04i", time, steps);
  else if (!paused) //Darstellen von Pause und Resume
    strcpy(temptxt, "     Pause  ");
  else
    strcpy(temptxt, "     Resume    X");

  //cursor
  if (positions[pos] / 16 == line)
    temptxt[positions[pos] % 16] = 1;

  return temptxt;
}

void TimeLapse::clicked() {
  switch (pos) {
    case 0:
      paused = !paused;
      lasttime = millis();
      rState(!paused);
      break;
    case 1:
      swScr(posInArray - 1);
      break;
  }
}

void TimeLapse::input(int b) {
  if (paused)
    pos = ((pos + b) % POS_NUM + POS_NUM) % POS_NUM;
  else
    pos = 0;
}

void TimeLapse::start(short ntime, short nsteps) {
  swScr(posInArray);    //show the time lapse
  pos = 0;              //reset cursor pos
  consttime = ntime;
  conststeps = nsteps;
  paused = false;
  rState(true);
  newloop();
}

void TimeLapse::loopprocess() {
  long tempmill = millis();
  if (!paused) {
    if ((tempmill - lasttime) >= 1000) { //If a second is over
      time -= (tempmill - lasttime) / 1000;     //subtract it from time
      lasttime += (tempmill - lasttime) / 1000 * 1000; //and add it to lasttime
    }
  }
  if (time <= 0) { //when the time is over
    newloop();
  }
  if ((starttime + TRMILLIS) < tempmill) { //end the exposure after TRMILLIS
    trigg(false);
  }
}

void TimeLapse::newloop() {
  time = consttime;     //reset time
  steps = conststeps;   //reset steps
  lasttime = millis();  //start timer
  starttime = lasttime;
  trigg(true);          //start the exposure
}
