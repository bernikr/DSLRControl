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
#define buttonint 0 //Interrupt 0 is pin 2
#define camerapin 12
#define redled 5
#define greenled 6
#define backlight 9

//Custom Chars
byte arrow[8] = {
  0b00000,
  0b01100,
  0b01110,
  0b01111,
  0b01111,
  0b01110,
  0b01100,
  0b00000
};
byte hollowarrow[8] = {
  0b00000,
  0b01100,
  0b01010,
  0b01001,
  0b01001,
  0b01010,
  0b01100,
  0b00000
};

//state variable of the ready state (=yellow led)
boolean rState = false;

//state variable of the button
volatile bool buttonpressed = false;

//state variable that gets true after the startup (Is needed in order that the initializing of the pullup resitor doesn't trigger an interrupt.)
bool afterstartup = false;

////Screens
//Init of the screen objects
MainMenu mainMenu = MainMenu(&switchScreen, 1, 3);
TimeLapse timeLap = TimeLapse(&switchScreen, &triggerCamera, &moveStepper, &readyState, 2);
TimeLapseOptions timeLapOpt = TimeLapseOptions(&switchScreen, &EEPROMReadInt, &EEPROMWriteInt, &timeLap);
Intervalometer interv = Intervalometer(&switchScreen, &triggerCamera, 4);
IntervalometerOptions intervOpt = IntervalometerOptions(&switchScreen, &EEPROMReadLong, &EEPROMWriteLong, &interv);
//Creating an array including all of the screens
Screen *scrs[5] = {&mainMenu, &timeLapOpt, &timeLap, &intervOpt, &interv};
//state varibale of the current screen in the array
byte curscr = 0;

void setup() {
  //Init of the 2x16 LCD and creating the two arrows
  lcd.begin(16, 2);
  lcd.createChar(1, arrow);
  lcd.createChar(2, hollowarrow);

  //Init of the stepper
  st.setSpeed(30);

  //Init of the button
  attachInterrupt(buttonint, clicked, RISING);
  digitalWrite(buttonpin, HIGH); //Internal pullup resistor

  //Camera Pin
  pinMode(camerapin, OUTPUT);

  //Leds as output
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);

  //Wait a moment to let the LCD finish its startup
  delay(300);

  //Startup finished
  afterstartup = true;

  //Activate the red LED
  digitalWrite(redled, HIGH);
}

void loop() {
  //run and show the current screen
  scrs[curscr]->loopprocess();
  showScreen(scrs[curscr]);

  //process the encoder input
  if (enc.read() / 4 != 0) {
    scrs[curscr]->input(-enc.read() / 4);
    enc.write(0);
  }

  //process the button press
  if (buttonpressed) {
    buttonpressed = false;
    scrs[curscr]->clicked();
  }

  //Wait a moment, so that the LCD isn't updated to often
  delay(100);
}

//Function to display a screen
void showScreen(Screen *s) {
  lcd.clear();
  lcd.print(s->getLine(0));
  lcd.setCursor(0, 1);
  lcd.print(s->getLine(1));
  lcd.setCursor(16, 1);
}

//simple function to change the current screen (Pointer is given to the screen objects so that they can start another screen.)
void switchScreen(byte newscrnum) {
  curscr = newscrnum;
}

//Function to trigger the camera
void triggerCamera(bool t) {
  digitalWrite(greenled, t || rState); //green led is on when the camera is or when the led should be yellow in order to mix it with red
  digitalWrite(redled, !t);  //red led indicates that the camera isn't triggered currently
  digitalWrite(camerapin, t);
}

//Toggle the yellow state
void readyState(bool s) {
  rState = s;
  digitalWrite(greenled, s);
  digitalWrite(redled, true);
}

//function called by the button interrupt
void clicked() {
  if (afterstartup) buttonpressed = true;
}

//function to move the stepper (only implemented at the time to give a pointer to the TimeLapse object
void moveStepper(byte steps) {
}
