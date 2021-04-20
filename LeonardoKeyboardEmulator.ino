
// Leonardo Keyboard Emulator version 1.0

#include <Keyboard.h>

//enum port_states_t {
//  ACTIVE,
//  INACTIVE
//};

#define ACTIVE LOW
#define INACTIVE HIGH

enum key_states_t { // The states a key can have
  key_state_U,  // Undefined
  key_state_UI, // Undefined, Changing to Inactive
  key_state_UA, // Undefined, changing to Active
  key_state_I,  // Inactive
  key_state_IA, // Inactieve, changing to Active
  key_state_A,  // Active
  key_state_AP, // Active and Processed
  key_state_API, // Active and Processed, changing to Inactive
  key_state_IP,  // Inactieve, Inactive and Processed
  key_state_IPA, // Inactive and Processed, changing to Active
  key_state_AI,  // Actieve, changing to Inactive
};   // The states a key can have

struct io_port_t;                                                         // The structure that holds all port information is announced here and defined later.
typedef void (*process_key_pointer)(io_port_t *io_port);                  // A type definition for a pointer to the code that has to handel the processing of the key.
void process_double_command_port(io_port_t *io_port);                     // The code to process a double char key is announced here and defined later.
void process_triple_command_port(io_port_t *io_port);                     // The code to process a triple char key is announced here and defined later.
void register_key_state(io_port_t *io_port, key_states_t key_state);      // The code to register (not process) a key state change. Register = saving the new state and the time it happened

struct io_port_t {                 // The structure with it's members are defined here.
  String Key;                      // Name of the key
  String Command;                  // Command in Eding.
  int PortNr;                      // Port on the Arduino
  int Mode;                        // Port mode
  int DebounceTics;                // Number of tics used to prevent boucning of the keys.
  char command_char;               // First character
  char command_char2;              // Second character
  char command_char3;              // Third character
  char command_char4;              // Fourth character
  process_key_pointer process_key; // A pointer to the code that has to handel the processing of the key

  // all members after this line must be initialized outside the creation of the array io_ports
  key_states_t key_state;           // The last know state of a key, must be set to Undefined at Initialisation
  unsigned long time_key_change;    // The time the key_state was changed
} ;

// define constants that are used in the program, makes changes easier
#define DEFAULT_DEBOUNCE_TICS 100  // default debounce tics
#define IOPORTS 11                 // the number of io ports to initialize

// define the io ports to initialze and use, put them in an array that is easier to maintain
// create an array that holds the io ports
io_port_t io_ports[IOPORTS] = {                       // Array that contains all io port definitions. note the array starts at 0 and ends at IOPORTS-1
  {"Ctrl+1", "X_zero", 0, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 1, 0, &process_double_command_port},                   // Double command, "0" means no action.
  {"Ctrl+2", "Y_zero", 1, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 2, 0, &process_double_command_port},
  {"Ctrl+3", "Z_zero", 2, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 3, 0, &process_double_command_port},
  {"Ctrl+4", "A_zero", 3, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 4, 0, &process_double_command_port},
  {"Ctrl+Shift+F", "Feed_down", 4, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x81, 'f', &process_triple_command_port},
  {"Ctrl+Alt+F", "Feed_100", 5, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 'f', &process_triple_command_port},
  {"Ctrl+F", "Feed_up", 6, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 'f', 0, &process_double_command_port},
  {"Ctrl+(Shift)+D", "Spindle_CW", 7, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0, 0, 0, NULL},
  {"Ctrl+(Shift)+E", "Spindle_CCW", 8, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0, 0, 0, NULL},
  {"Ctrl+K", "Coolant", 9, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 'k', 0, &process_double_command_port},
  {"Ctrl+M", "Air", 10, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 'm', 0, &process_double_command_port},

};

// initialize the ports
void init_io_ports()
{
  for (int i = 0; i < IOPORTS; i++)                    // for each io port
  {
    pinMode( io_ports[i].PortNr, io_ports[i].Mode);   // set the mode
    register_key_state(&io_ports[i], key_state_U);    // set the default key_state
  }
}

void register_key_state(io_port_t *io_port, key_states_t key_state) {
  if (io_port->key_state != key_state)    // register only if the key state has changed
  {
    io_port->key_state = key_state;       // register the new state
    io_port->time_key_change = millis();  // register the time it happend
  }
}

// return the state of a port
bool get_port_state(io_port_t *io_port) {
  return digitalRead(io_port->PortNr);
}

// signal an error by sending the error number to the keyboard. Char '0' is added to make the error number readable.
void signal_error(byte error_number)
{
  error_number += '0'; //show the error as char starting at 0
  Keyboard.press(error_number);
  delay(1000);                  //avoid flooding the keyboard
  Keyboard.releaseAll();
}

// This procedure is the workhorse of debouncing
// It returns the state of a key.
// The state of a key depends on the state of the port (HIGH or LOW), how  long is was in this state and many other things (when fully implemented).
// For now, whe just return the port_state for testing
key_states_t get_key_state(io_port_t *io_port) {
  bool port_state = get_port_state(io_port);
  key_states_t new_key_state = key_state_U;                     // initialize to the default value
  //  if (port_state == HIGH) return key_state_I;
  //  else return key_state_A;
  switch (io_port->key_state)                                  // process depending on the previous key state
  {
    case key_state_U:
      if ((port_state) == INACTIVE) new_key_state = key_state_UI;  // change from U to UI when inactve  maybe bouncing
      else new_key_state = key_state_UA;                       // change from U to UA when active   maybe bouncing
      break;
    case key_state_UI:
      if ((port_state) == INACTIVE) new_key_state = key_state_I;   // change from UI to I when inactive  stable I
      else new_key_state = key_state_UA;                       // change from UI to UA when active   bouncing
      break;
    case key_state_UA:
      if ((port_state) == INACTIVE) new_key_state = key_state_UI;  // change from UA to UI                bouncing
      else new_key_state = key_state_A;                        // change from UA to A                 stable A
      break;
    case key_state_A:
      if ((port_state) == INACTIVE) new_key_state = key_state_AI;  // change from A to AI                 maybe bouncing
      else new_key_state = key_state_A;                        // no change                           stable A
      break;
    case key_state_AI:
      if ((port_state) == INACTIVE) new_key_state = key_state_I;  // change from A to AI                 maybe bouncing
      else new_key_state = key_state_A;                        // no change                           stable A
      break;
    case key_state_I:
      if ((port_state) == INACTIVE) new_key_state = key_state_I;   // no change                           stable I
      else new_key_state = key_state_IA;                       // change from I to IA                 maybe bouncing
      break;
    case key_state_IA:
      if ((port_state) == INACTIVE) new_key_state = key_state_I;   // no change                           stable I
      else new_key_state = key_state_A;                       // change from I to IA                 maybe bouncing
      break;
    case key_state_API:
      if ((port_state) == INACTIVE) new_key_state = key_state_I;   // change from AP to I                 maybe bouncing
      else new_key_state = key_state_AP;                        // change from AP to A                 maybe bouncing
      break;
    case key_state_IPA:
      if ((port_state) == INACTIVE) new_key_state = key_state_IP;   // change from IP to I                 maybe bouncing
      else new_key_state = key_state_A;                        // change from IP to A                 maybe bouncing
      break;

    default: //
      signal_error(io_port->key_state);                        // signal a fault condition, the error number is the enum member. First member is 0, second 1, etc
      new_key_state = key_state_U;                             // must not get here so register undefined to be safe
      break;
  }
  register_key_state(io_port, new_key_state);                  // register the new key state
  return new_key_state;                                        // return the new key state
}

// process a double command port
void process_double_command_port(io_port_t *io_port) {
  if (get_key_state(io_port) == key_state_AP) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    delay(100);
    Keyboard.releaseAll();
  }
}

// process a triple command port
void process_triple_command_port(io_port_t *io_port) {
  if (get_key_state(io_port) == key_state_AP) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    Keyboard.press(io_port->command_char3);
    delay(100);
    Keyboard.releaseAll();
  }
}

// process the command ports
void process_command_ports()
{
  for (int i = 0; i < IOPORTS; i++)                       // for each io port
    if (io_ports[i].process_key != NULL)                  // If there is a processing procedure defined
      io_ports[i].process_key(&io_ports[i]);              // Use the specified procedure to process the port
}

int Spindle_CW = 7;
int Spindle_CCW = 8;

uint8_t CwKeyState = HIGH;           // the current state of the key's to send
uint8_t CwButtonState;             // the current reading from the input pin
uint8_t CwLastButtonState = LOW;   // the previous reading from the input pin
uint8_t CcwKeyState = HIGH;           // the current state of the key's to send
uint8_t CcwButtonState;             // the current reading from the input pin
uint8_t CcwLastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  init_io_ports();       // initialize all io ports
  Keyboard.begin();
}

void loop() {
  process_command_ports();


  // Next commands are used to turn the Spindle On/Off in CW or CCW direction.
  uint8_t reading = digitalRead(Spindle_CW);
  if (reading != CwLastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != CwButtonState) {
      CwButtonState = reading;

      if (CwButtonState == LOW) {
        CwKeyState = !CwKeyState;
        if (CwKeyState) {
          //Ctrl+D.
          Keyboard.press(0x80);
          Keyboard.press('d');
          delay(100);
          Keyboard.releaseAll();

        } else {
          //Ctrl+Shift+D.
          Keyboard.press(0x80);
          Keyboard.press(0x81);
          Keyboard.press('d');
          delay(100);
          Keyboard.releaseAll();
        }
      }
    }
  }
  CwLastButtonState = reading;

  uint8_t reading2 = digitalRead(Spindle_CCW);
  if (reading2 != CcwLastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading2 != CcwButtonState) {
      CcwButtonState = reading2;

      if (CcwButtonState == LOW) {
        CcwKeyState = !CcwKeyState;
        if (CcwKeyState) {
          //Ctrl+E
          Keyboard.press(0x80);
          Keyboard.press('e');
          delay(100);
          Keyboard.releaseAll();
        } else {
          //Ctrl+Shift+E
          Keyboard.press(0x80);
          Keyboard.press(0x81);
          Keyboard.press('e');
          delay(100);
          Keyboard.releaseAll();
        }
      }
    }
  }
  CcwLastButtonState = reading2;
}
