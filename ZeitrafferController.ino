//#include <Encoder.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "MainMenu.h"
#include "TimeLapseOptions.h"
#include "TimeLapse.h"
#include "IntervalometerOptions.h"
#include "Intervalometer.h"

//CONFIG of Pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Stepper st(90, 8, 9, 10, 11);
//Encoder enc(3, 4);
byte button = 2;

//Eigene Zeichen
byte pfeil[8] = {
  0b00000,
  0b01100,
  0b01110,
  0b01111,
  0b01111,
  0b01110,
  0b01100,
  0b00000
};
byte hohlpfeil[8] = {
  0b00000,
  0b01100,
  0b01010,
  0b01001,
  0b01001,
  0b01010,
  0b01100,
  0b00000
};

////Screens
//Erstellen der Verschiedenen Screen-Objekte
MainMenu mainMenu = MainMenu(&switchScreen, 1, 3);
TimeLapse timeLap = TimeLapse(&switchScreen, &triggerCamera, &moveStepper, 2);
TimeLapseOptions timeLapOpt = TimeLapseOptions(&switchScreen, &EEPROMReadInt, &EEPROMWriteInt, &timeLap);
Intervalometer interv = Intervalometer(&switchScreen, &triggerCamera, 4);
IntervalometerOptions intervOpt = IntervalometerOptions(&switchScreen, &EEPROMReadLong, &EEPROMWriteLong, &interv);
//Zusammenfassen der Screens in einem Screen-Pointer Array
Screen *scrs[5] = {&mainMenu, &timeLapOpt, &timeLap, &intervOpt, &interv};
//Statusvariable des Aktuellen Screens im Array
byte curscr = 0;

void setup() {
  //Aufsetzen des LCDs mit 2 Zeilen und 16 Zeichen und Erstellen des oben definierten Pfeilcharacters
  lcd.begin(16, 2);
  lcd.createChar(1, pfeil);
  lcd.createChar(2, hohlpfeil);
  
  //Einrichten der Schrittmotoren
  st.setSpeed(30);
  
  //DEV: Serielle Schnittstelle
  Serial.begin(9600);
  //DEV: LED
  pinMode(13, OUTPUT);
  
  //Delay um Einrichtung des Monitors abzuwarten
  delay(300);
}

void loop() {
  scrs[curscr]->loopprocess();
  showScreen(scrs[curscr]);
  
  //TEST
  if (Serial.available()) {
   signed char sertemp;
   while(Serial.available()){
     sertemp = Serial.read();
     if(sertemp == 0)
       scrs[curscr]->clicked();
     else
       scrs[curscr]->input(sertemp);
   }   
  }

  delay(100);
}

//Funktion zum Darstellen eines Screens
void showScreen(Screen *s){
  lcd.clear();
  lcd.print(s->getLine(0));
  lcd.setCursor(0, 1);
  lcd.print(s->getLine(1));
  lcd.setCursor(16, 1);
}

void switchScreen(byte newscrnum){
  curscr = newscrnum;
}

void triggerCamera(bool t){
  digitalWrite(13, t);
}

void moveStepper(byte steps){
}
