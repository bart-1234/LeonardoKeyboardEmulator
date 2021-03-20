// Leonardo Keyboard Emulator version 1.0

#include <Keyboard.h>

struct io_port_t;                                         // The structure that holds all port information is announced here and defined later.
typedef void (*process_key_pointer)(io_port_t *io_port);  // A type definition for a pointer to the code that has to handel the processing of the key.
void process_char_command_port(io_port_t *io_port);       // The code to process a simple char key is announced here and defined later.

struct io_port_t {
  String Key;
  String Command;
  int PortNr;
  int Mode;
  int DebounceTics;
  char command_char;
  char command_char2;
  char command_char3;
  char command_char4;
  process_key_pointer process_key; // A pointer to the code that has to handel the processing of the key
} ;

// define constants that are used in the program, makes changes easier
#define DEFAULT_DEBOUNCE_TICS 100                     // default debounce tics
#define IOPORTS 18                                    // the number of io ports to initialize
// define the io ports to initialze and use, put them in an array that is easier to maintain
// currently 18 io ports are defined

// create an array that holds the io ports
io_port_t io_ports[IOPORTS] = {                       // Array that contains all io port definitions. note the array starts at 0 and ends at IOPORTS-1
  {"Arrow Left", "X_min", 0, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD8, 0xD8, 0xD8, 0xD8, &process_char_command_port},
  {"Arrow Right", "X_plus", 1, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD7, 0xD7, 0xD7, 0xD7, &process_char_command_port},
  {"Arrow Up", "Y_plus", 2, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xDA, 0xDA, 0xDA, 0xDA, &process_char_command_port},
  {"Arrow Down", "Y_min", 3, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD9, 0xD9, 0xD9, 0xD9, &process_char_command_port},
  {"Page Up", "Z_plus", 4, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD3, 0xD3, 0xD3, 0xD3, &process_char_command_port},
  {"Page Down", "Z_min", 5, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD6, 0xD6, 0xD6, 0xD6, &process_char_command_port},
  {"Home", "A_plus", 6, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD2, 0xD2, 0xD2, 0xD2, &process_char_command_port},
  {"End", "A_min", 7, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD5, 0xD5, 0xD5, 0xD5, &process_char_command_port},
  {"Ctrl", "ctrl", 8, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x80, 0x80, 0x80, &process_char_command_port},
  {"Shift", "shift", 9, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x81, 0x81, 0x81, 0x81, &process_char_command_port},
  {"Continuous Jog", "Jog_cont", 10, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x80, 0x81, 'n', &process_char_command_port},
  {"Jog step 0.01", "Jog_001", 11, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 'r', &process_char_command_port},
  {"Jog step 0.1", "Jog_01", 12, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 's', &process_char_command_port},
  {"Jog step 1", "Jog_1", 13, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 't', &process_char_command_port},
  {"Home sequence", "Home", A2, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x80, 0x80, 'h', &process_char_command_port},
  {"Reset", "Reset", A3, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x80, 0x80, 'r', &process_char_command_port},
  {"Toggle Main Auto menu", "Main_Auto", A4, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS},
  {"Mdi Menu", "Mdi", A5, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x80, 0x80, 0xC7, &process_char_command_port}
};

// initialize the ports
void init_io_ports()
{
  for (int i = 0; i < IOPORTS; i++)                    // for each io port
  {
    pinMode( io_ports[i].PortNr, io_ports[i].Mode);   // set the mode
  }
}

// return the state of a port
bool get_port_state(io_port_t *io_port) {
  return digitalRead(io_port->PortNr);
}

char get_keyboard_input(io_port_t *io_port) {
  return (io_port->command_char);
}
char get_keyboard_input2(io_port_t *io_port) {
  return (io_port->command_char2);
}
char get_keyboard_input3(io_port_t *io_port) {
  return (io_port->command_char3);
}
char get_keyboard_input4(io_port_t *io_port) {
  return (io_port->command_char4);
}

// process a char command port
void process_char_command_port(io_port_t *io_port) {
  if (get_port_state(io_port) == LOW) {
    Keyboard.press(get_keyboard_input(io_port));
    Keyboard.press(get_keyboard_input2(io_port));
    Keyboard.press(get_keyboard_input3(io_port));
    Keyboard.press(get_keyboard_input4(io_port));
  }
  if (get_port_state(io_port) == HIGH) {
    Keyboard.release(get_keyboard_input(io_port));
    Keyboard.release(get_keyboard_input2(io_port));
    Keyboard.release(get_keyboard_input3(io_port));
    Keyboard.release(get_keyboard_input4(io_port));
  }
}

// process the command ports
void process_char_command_ports()
{
  for (int i = 0; i < IOPORTS; i++)                       // for each io port 
    if (io_ports[i].process_key != NULL)                  // If there is a processing procedure defined
      io_ports[i].process_key(&io_ports[i]);              // Use the specified procedure to process the port
}

int Main_Auto = A4;

uint8_t MainAutoKeyState = HIGH;           // the current state of the key's to send
uint8_t MainAutoButtonState;             // the current reading from the input pin
uint8_t MainAutoLastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  init_io_ports();       // initialize all io ports
  Keyboard.begin();
}

void loop() {
  // next commands are controlled by the JOG joysticks.

  process_char_command_ports();

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
}
