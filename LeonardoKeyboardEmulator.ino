// Leonardo Keyboard Emulator version 1.0

#include <Keyboard.h>

struct io_port_t {
  String ID;
  int PortNr;
  int Mode;
  int DebounceTics;
} ;

// define constants that are used in the program, makes changes easier
#define DEFAULT_DEBOUNCE_TICS 100                     // default debounce tics
#define IOPORTS 3                                     // the number of io ports to initialize
 
io_port_t test_port={"X_min",0,INPUT_PULLUP};         // showing how to define a variable structure

// define the io ports to initialze and use, put them in an array that is easier to maintain
// currently 3 io ports are defined

// create an array that holds the io ports
io_port_t io_ports[IOPORTS]={                         // Array that contains all io port definitions. note the array starts at 0 and ends at IOPORTS-1
  {"X_min",0,INPUT_PULLUP,DEFAULT_DEBOUNCE_TICS},
  {"X_plus",1,INPUT_PULLUP,DEFAULT_DEBOUNCE_TICS},
  {"Y_plus",2,INPUT_PULLUP,DEFAULT_DEBOUNCE_TICS}};

void init_io_ports()
{
  for (int i=0;i<IOPORTS;i++)                          // for each io port
  {
    pinMode( io_ports[i].PortNr, io_ports[i].Mode);   // set the mode
  }
}


int X_min = 0;
int X_plus = 1;
int Y_plus = 2;
int Y_min = 3;
int Z_plus = 4;
int Z_min = 5;
int A_plus = 6;
int A_min = 7;
int ctrl = 8;
int shift = 9;
int Jog_cont = 10;
int Jog_001 = 11;
int Jog_01 = 12;
int Jog_1 = 13;
int Home = A2;
int Reset = A3;
int Main_Auto = A4;
int Mdi = A5;

uint8_t MainAutoKeyState = HIGH;           // the current state of the key's to send
uint8_t MainAutoButtonState;             // the current reading from the input pin
uint8_t MainAutoLastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
   init_io_ports();       // initialize all io ports
  
  pinMode( X_min, INPUT_PULLUP);
  pinMode( X_plus, INPUT_PULLUP);
  pinMode( Y_plus, INPUT_PULLUP);
  pinMode( Y_min, INPUT_PULLUP);
  pinMode( Z_plus, INPUT_PULLUP);
  pinMode( Z_min, INPUT_PULLUP);
  pinMode( A_plus, INPUT_PULLUP);
  pinMode( A_min, INPUT_PULLUP);
  pinMode( ctrl, INPUT_PULLUP);
  pinMode( shift, INPUT_PULLUP);
  pinMode( Jog_cont, INPUT_PULLUP);
  pinMode( Jog_001, INPUT_PULLUP);
  pinMode( Jog_01, INPUT_PULLUP);
  pinMode( Jog_1, INPUT_PULLUP);
  pinMode( Home, INPUT_PULLUP);
  pinMode( Reset, INPUT_PULLUP);
  pinMode( Main_Auto, INPUT_PULLUP);
  pinMode( Mdi, INPUT_PULLUP);

  Keyboard.begin();
}

void loop() {
  // next commands are controlled by the JOG joysticks.

  //Arrow Left.
  if (digitalRead(X_min) == LOW) {
    Keyboard.press(0xD8);
  }
  if (digitalRead(X_min) == HIGH) {
    Keyboard.release(0xD8);
  }
  //Arrow Right.
  if (digitalRead(X_plus) == LOW) {
    Keyboard.press(0xD7);
  }
  if (digitalRead(X_plus) == HIGH) {
    Keyboard.release(0xD7);
  }
  //Arrow Up.
  if (digitalRead(Y_plus) == LOW) {
    Keyboard.press(0xDA);
  }
  if (digitalRead(Y_plus) == HIGH) {
    Keyboard.release(0xDA);
  }
  //Arrow Down.
  if (digitalRead(Y_min) == LOW) {
    Keyboard.press(0xD9);
  }
  if (digitalRead(Y_min) == HIGH) {
    Keyboard.release(0xD9);
  }
  // Page Up.
  if (digitalRead(Z_plus) == LOW) {
    Keyboard.press(0xD3);
  }
  if (digitalRead(Z_plus) == HIGH) {
    Keyboard.release(0xD3);
  }
  // Page Down.
  if (digitalRead(Z_min) == LOW) {
    Keyboard.press(0xD6);
  }
  if (digitalRead(Z_min) == HIGH) {
    Keyboard.release(0xD6);
  }
  //Home.
  if (digitalRead(A_plus) == LOW) {
    Keyboard.press(0xD2);
  }
  if (digitalRead(A_plus) == HIGH) {
    Keyboard.release(0xD2);
  }
  //End.
  if (digitalRead(A_min) == LOW) {
    Keyboard.press(0xD5);
  }
  if (digitalRead(A_min) == HIGH) {
    Keyboard.release(0xD5);
  }

  // Next commands define the JOG steps.
  //Ctrl+Shift+N
  if (digitalRead(Jog_cont) == LOW) {
    Keyboard.press(0x80);
    Keyboard.press(0x81);
    Keyboard.press('N');
    delay(100);
    Keyboard.releaseAll();
  }
  //Ctrl+Alt+Shift+R
  if (digitalRead(Jog_001) == LOW) {
    Keyboard.press(0x80);
    Keyboard.press(0x82);
    Keyboard.press(0x81);
    Keyboard.press('R');
    delay(100);
    Keyboard.releaseAll();
  }
  //Ctrl+Alt+Shift+S
  if (digitalRead(Jog_01) == LOW) {
    Keyboard.press(0x80);
    Keyboard.press(0x82);
    Keyboard.press(0x81);
    Keyboard.press('S');
    delay(100);
    Keyboard.releaseAll();
  }
  //Ctrl+Alt+Shift+T
  if (digitalRead(Jog_1) == LOW) {
    Keyboard.press(0x80);
    Keyboard.press(0x82);
    Keyboard.press(0x81);
    Keyboard.press('T');
    delay(100);
    Keyboard.releaseAll();
  }

  // Next commands are used by the JOG speed switch.
  //Ctrl
  if (digitalRead(ctrl) == LOW) {
    Keyboard.press(0x80);
  }
  if (digitalRead(ctrl) == HIGH) {
    Keyboard.release(0x80);
  }
  //Shift
  if (digitalRead(shift) == LOW) {
    Keyboard.press(0x81);
  }
  if (digitalRead(shift) == HIGH) {
    Keyboard.release(0x81);
  }


  // Next command is used next to home all axis'.
  //Ctrl+H
  if (digitalRead(Home) == LOW) {
    Keyboard.press(0x80);
    Keyboard.press('h');
    delay(100);
    Keyboard.releaseAll();
  }
  // Next commands are used next to the screen'.
  //Ctrl+R
  if (digitalRead(Reset) == LOW) {
    Keyboard.press(0x80);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
  }
  //Toggle between Main and Auto menu.
  uint8_t reading = digitalRead(Main_Auto);
  if (reading != MainAutoLastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != MainAutoButtonState) {
      MainAutoButtonState = reading;

      if (MainAutoButtonState == LOW) {
        MainAutoKeyState = !MainAutoKeyState;
        if (MainAutoKeyState) {
          // Alt+F1
          Keyboard.press(0x82);
          Keyboard.press(0xC2);
          delay(100);
          Keyboard.releaseAll();
        } else {
          // Alt+F4
          Keyboard.press(0x82);
          Keyboard.press(0xC5);
          delay(100);
          Keyboard.releaseAll();
        }
      }
    }
  }
  MainAutoLastButtonState = reading;

  //Ctrl+F6
  if (digitalRead(Mdi) == LOW) {
    Keyboard.press(0x80);
    Keyboard.press(0xC7);
    delay(100);
    Keyboard.releaseAll();
  }
}
