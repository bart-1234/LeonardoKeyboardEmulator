// Leonardo Keyboard Emulator version 1.0

#include <Keyboard.h>

struct io_port_t;                                         // The structure that holds all port information is announced here and defined later.
typedef void (*process_key_pointer)(io_port_t *io_port);  // A type definition for a pointer to the code that has to handel the processing of the key.
void process_char_command_port(io_port_t *io_port);       // The code to process a simple char key is announced here and defined later.
void process_double_command_port(io_port_t *io_port);     // The code to process a double char key is announced here and defined later.
void process_triple_command_port(io_port_t *io_port);     // The code to process a triple char key is announced here and defined later.
void process_quadruple_command_port(io_port_t *io_port);  // The code to process a quadruple char key is announced here and defined later.

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
} ;

// define constants that are used in the program, makes changes easier
#define DEFAULT_DEBOUNCE_TICS 100  // default debounce tics
#define IOPORTS 18                 // the number of io ports to initialize

// define the io ports to initialze and use, put them in an array that is easier to maintain
// create an array that holds the io ports
io_port_t io_ports[IOPORTS] = {                       // Array that contains all io port definitions. note the array starts at 0 and ends at IOPORTS-1
  {"Arrow Left", "X_min", 0, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD8,0,0,0, &process_char_command_port},                      // Single command, "0" means no action.
  {"Arrow Right", "X_plus", 1, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD7,0,0,0, &process_char_command_port},
  {"Arrow Up", "Y_plus", 2, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xDA,0,0,0, &process_char_command_port},
  {"Arrow Down", "Y_min", 3, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD9,0,0,0, &process_char_command_port},
  {"Page Up", "Z_plus", 4, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD3,0,0,0, &process_char_command_port},
  {"Page Down", "Z_min", 5, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD6,0,0,0, &process_char_command_port},     
  {"Home", "A_plus", 6, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD2,0,0,0, &process_char_command_port},
  {"End", "A_min", 7, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD5,0,0,0, &process_char_command_port},
  {"Ctrl", "ctrl", 8, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80,0,0,0, &process_char_command_port},
  {"Shift", "shift", 9, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x81,0,0,0, &process_char_command_port},
  {"Continuous Jog", "Jog_cont", 10, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x81, 'n',0, &process_triple_command_port},      // Triple command
  {"Jog step 0.01", "Jog_001", 11, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 'r', &process_quadruple_command_port}, // Quadruple command
  {"Jog step 0.1", "Jog_01", 12, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 's', &process_quadruple_command_port},
  {"Jog step 1", "Jog_1", 13, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 't', &process_quadruple_command_port},
  {"Home sequence", "Home", A2, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 'h',0,0, &process_double_command_port},               // Double command
  {"Reset", "Reset", A3, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 'r',0,0, &process_double_command_port},
  {"Toggle Main Auto menu", "Main_Auto", A4, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0, 0, 0, 0, NULL},
  {"Mdi Menu", "Mdi", A5, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0xC7,0,0, &process_double_command_port}
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
// process a char command port
void process_char_command_port(io_port_t *io_port) {
  if (get_port_state(io_port) == LOW) {
    Keyboard.press(io_port->command_char);
  }
  if (get_port_state(io_port) == HIGH) {
    Keyboard.release(io_port->command_char);
  }
}
  
// process a double command port
void process_double_command_port(io_port_t *io_port) {
  if (get_port_state(io_port) == LOW) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    delay(100);
    Keyboard.releaseAll();
  }
}

// process a triple command port
void process_triple_command_port(io_port_t *io_port) {
  if (get_port_state(io_port) == LOW) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    Keyboard.press(io_port->command_char3);
    delay(100);
    Keyboard.releaseAll();
  }
}

// process a quadruple command port
void process_quadruple_command_port(io_port_t *io_port) {
  if (get_port_state(io_port) == LOW) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    Keyboard.press(io_port->command_char3);
    Keyboard.press(io_port->command_char4);
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
  process_command_ports();


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
