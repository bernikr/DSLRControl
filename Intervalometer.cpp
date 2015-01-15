#include "Intervalometer.h"

Intervalometer::Intervalometer(void (*nswScr)(byte), void (*nTrigg)(bool), byte nposInArray) {
  swScr = nswScr;
  trigg = nTrigg;

  posInArray = nposInArray;

  temptxt = new char[17];
}


char* Intervalometer::getLine(byte line) {
  if (line == 0) {
    byte hours = time / 1000 / 60 / 60;
    byte minutes = (time / 1000 / 60) % 60;
    byte seconds = (time / 1000) % 60;
    byte centiseconds = (time / 10) % 100;

    snprintf(temptxt, 17, "   %1u:%02u:%02u.%02u", hours, minutes, seconds, centiseconds);
  } else if (!paused) //Darstellen von Pause und Resume
    strcpy(temptxt, "     Pause      ");
  else
    strcpy(temptxt, "     Resume    X");

  //show the cursor
  if (positions[pos] / 16 == line)
    temptxt[positions[pos] % 16] = 1;

  return temptxt;
}

void Intervalometer::clicked() {
  switch (pos) {
    case 0:  //Pause/Resume
      paused = !paused;
      lasttime = millis();
      trigg(!paused);
      break;
    case 1:  //Exit
      swScr(posInArray - 1);
      break;
  }
}

void Intervalometer::input(int b) {
  if (paused) //If paused 0 and 0 possible, else only 0
    pos = ((pos + b) % POS_NUM + POS_NUM) % POS_NUM; //Ugly solution to get the positive modulo
  else
    pos = 0;
}

//Intervalometer started
void Intervalometer::start(long ntime) {
  swScr(posInArray);    //Display intervalometer
  pos = 0;              //curser on pos 0
  time = ntime;
  lasttime = millis();  //start timer
  paused = false;       //not paused
  trigg(true);          //start the exposure
}

void Intervalometer::loopprocess() {
  if (!paused) {
    long tempmill = millis();
    time -= (tempmill - lasttime);  //add time difference
    lasttime = tempmill;
    if (time < 0) { //If the time is over
      trigg(false);
      swScr(posInArray - 1);
    }
  }
}
