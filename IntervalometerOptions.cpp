#include "IntervalometerOptions.h"

#define SAV_LOC 4 //Save location in the EEPROM

IntervalometerOptions::IntervalometerOptions(void (*nswScr)(byte), long (*readEeprom)(int), void (*writeEeprom)(int, long), Intervalometer* ninterv) {
  time = readEeprom(SAV_LOC);

  swScr = nswScr;
  save = writeEeprom;
  interv = ninterv;

  temptxt = new char[17];
}


char* IntervalometerOptions::getLine(byte line) {
  if (line == 0) {
    byte hours = time / 1000 / 60 / 60;
    byte minutes = (time / 1000 / 60) % 60;
    byte seconds = (time / 1000) % 60;
    byte centiseconds = (time / 10) % 100;

    snprintf(temptxt, 17, "   %1u:%02u:%02u.%02u", hours, minutes, seconds, centiseconds);
  } else
    strcpy(temptxt, "  Start  Save  X");

  //show the cursor
  if (positions[pos] / 16 == line)
    temptxt[positions[pos] % 16] = sel ? 2 : 1;

  return temptxt;
}

void IntervalometerOptions::clicked() {
  switch (pos) {
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

void IntervalometerOptions::input(int b) {
  if (sel) { //If time is selected, change it
    switch (pos) {
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
    if (time < 0) time = 0;
  } else //else change the cursor pos
    pos = ((pos + b) % POS_NUM + POS_NUM) % POS_NUM;
}
