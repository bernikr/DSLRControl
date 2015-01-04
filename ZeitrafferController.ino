#include <Encoder.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "MainMenu.h"
#include "TimeLapseOptions.h"
#include "TimeLapse.h"
#include "IntervalometerOptions.h"
#include "Intervalometer.h"

//CONFIG of Pins
LiquidCrystal lcd(7, 8, A3, A2, A1, A0);
Stepper st(90, 10, 11, A4, A5);
Encoder enc(3, 4);
#define buttonpin 2
#define buttonint 0 //Interrupt 0 ist Pin 2
#define camerapin 12
#define redled 6
#define greenled 5
#define backlight 9

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

//Statusvariable des Buttons
volatile bool buttonpressed = false;

//Erster Buttonclick erfolgt (interrupt feuert gleich)
volatile bool notfirstpress = false;

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
  
  //Einrichten des Buttons
  attachInterrupt(buttonint, clicked, RISING);
  digitalWrite(buttonpin, HIGH); //Internal pullup resistor
  
  //Kamera Pin
  pinMode(camerapin, OUTPUT);
  
  //Delay um Einrichtung des Monitors abzuwarten
  delay(300);
}

void loop() {
  scrs[curscr]->loopprocess();
  showScreen(scrs[curscr]);
  
  //Verarbeitung der Encoder Eingabe
  if(enc.read()/4 != 0){
    scrs[curscr]->input(-enc.read()/4);
    enc.write(0);
  }
  
  //Verarbeitung der Buttoneingabe
  if(buttonpressed){
    buttonpressed = false;
    scrs[curscr]->clicked();
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
  digitalWrite(camerapin, t);
}

void clicked(){
  if(notfirstpress)
    buttonpressed = true;
  else
    notfirstpress = true;
}

void moveStepper(byte steps){
}
