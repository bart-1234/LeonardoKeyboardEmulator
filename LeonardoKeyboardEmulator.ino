// Leonardo Keyboard Emulator version 1.0
#include <Keyboard.h>

enum port_states_t {
  port_state_U,  // Undefined
  port_state_UA, // Undefined, changing to Active
  port_state_UI, // Undefined, changing to Inactive
  port_state_A,  // Active
  port_state_AI, // Active, changing to Inactive
  port_state_I,  // Inactive
  port_state_IA  // Inactive, changing to Active
};

String str_port_state(port_states_t port_state) {
  switch (port_state)
  {
    case port_state_U:  return "U";
    case port_state_UI:  return "UI";
    case port_state_UA:  return "UA";
    case port_state_I:  return "I";
    case port_state_IA:  return "IA";
    case port_state_A:  return "A";
    case port_state_AI:  return "AI";
    default: return "p?";
  }
}

#define ACTIVE port_state_A    // LOW
#define INACTIVE port_state_I  // HIGH

enum key_states_t { // The states a key can have
  key_state_U,  // Undefined
  key_state_I,  // Inactive
  key_state_IP, // Inactieve, Inactive and Processed
  key_state_A,  // Active
  key_state_AP // Active and Processed
};   // The states a key can have

String str_key_state(key_states_t key_state) {
  switch (key_state)
  {
    case key_state_U: return "U";
    case key_state_I: return "I";
    case key_state_IP: return "IP";
    case key_state_A: return "A";
    case key_state_AP: return "AP";
    default: return "k?";
  }
}

struct io_port_t;                                                         // The structure that holds all port information is announced here and defined later.
typedef void (*process_key_pointer)(io_port_t *io_port);                  // A type definition for a pointer to the code that has to handel the processing of the key.
void process_char_command_port(io_port_t *io_port);                       // The code to process a simple char key is announced here and defined later.
void process_double_command_port(io_port_t *io_port);                     // The code to process a double char key is announced here and defined later.
void process_triple_command_port(io_port_t *io_port);                     // The code to process a triple char key is announced here and defined later.
void process_quadruple_command_port(io_port_t *io_port);                  // The code to process a quadruple char key is announced here and defined later.
void register_port_state(io_port_t *io_port, port_states_t port_state);   // The code to register (not process) a key state change. Register = saving the new state and the time it happened
void print_state(String ID, String state);                                // Debug routine
void print_state(String ID, byte state);                                  // Debug routine

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
  port_states_t port_state;        // The last know state of the port, must be set to Undefined at Initialisation
  key_states_t key_state;           // The last know state of a key, must be set to Undefined at Initialisation
  unsigned long time_port_change;   // The time the port_state was changed
} ;

// define constants that are used in the program, makes changes easier
#define DEFAULT_DEBOUNCE_TICS 100  // default debounce tics
#define IOPORTS 18                 // the number of io ports to initialize

// define the io ports to initialze and use, put them in an array that is easier to maintain
// create an array that holds the io ports
io_port_t io_ports[IOPORTS] = {                       // Array that contains all io port definitions. note the array starts at 0 and ends at IOPORTS-1
  {"Arrow Left", "X_min", 0, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD8, 0, 0, 0, &process_char_command_port},                   // Single command, "0" means no action.
  {"Arrow Right", "X_plus", 1, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD7, 0, 0, 0, &process_char_command_port},
  {"Arrow Up", "Y_plus", 2, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xDA, 0, 0, 0, &process_char_command_port},
  {"Arrow Down", "Y_min", 3, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD9, 0, 0, 0, &process_char_command_port},
  {"Page Up", "Z_plus", 4, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD3, 0, 0, 0, &process_char_command_port},
  {"Page Down", "Z_min", 5, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD6, 0, 0, 0, &process_char_command_port},
  {"Home", "A_plus", 6, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD2, 0, 0, 0, &process_char_command_port},
  {"End", "A_min", 7, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0xD5, 0, 0, 0, &process_char_command_port},
  {"Ctrl", "ctrl", 8, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0, 0, 0, &process_char_command_port},
  {"Shift", "shift", 9, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x81, 0, 0, 0, &process_char_command_port},
  {"Continuous Jog", "Jog_cont", 10, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x81, 'n', 0, &process_triple_command_port},     // Triple command
  {"Jog step 0.01", "Jog_001", 11, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 'r', &process_quadruple_command_port}, // Quadruple command
  {"Jog step 0.1", "Jog_01", 12, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 's', &process_quadruple_command_port},
  {"Jog step 1", "Jog_1", 13, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0x82, 0x81, 't', &process_quadruple_command_port},
  {"Home sequence", "Home", A2, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 'h', 0, 0, &process_double_command_port},             // Double command
  {"Reset", "Reset", A3, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 'r', 0, 0, &process_double_command_port},
  {"Toggle Main Auto menu", "Main_Auto", A4, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0, 0, 0, 0, NULL},
  {"Mdi Menu", "Mdi", A5, INPUT_PULLUP, DEFAULT_DEBOUNCE_TICS, 0x80, 0xC7, 0, 0, &process_double_command_port}
};

// initialize the ports
void init_io_ports()
{
  for (int i = 0; i < IOPORTS; i++)                   // for each io port
  {
    pinMode( io_ports[i].PortNr, io_ports[i].Mode);   // set the mode
    io_ports[i].port_state = port_state_U;            // set to undefined
    io_ports[i].key_state = key_state_U;              // set to undefined
    register_port_state(&io_ports[i], port_state_U);  // register the port state. this will not set the debounce timer because the uninitialized port_state = 0 = port_state_U.
  }
}

void register_port_state(io_port_t *io_port, port_states_t port_state) {
  if (io_port->port_state != port_state)    // register only if the port state has changed
  {
    io_port->port_state = port_state;       // register the new state
    io_port->time_port_change = millis();   // register the time it happend
  }
}

void print_state(String ID, String state) {
  Serial.print(ID); Serial.print(state);
}
void print_state(String ID, byte state) {
  Serial.print(ID); Serial.print(state);
}

// return the state of a port
port_states_t get_port_state(io_port_t *io_port) {
  bool state = digitalRead(io_port->PortNr);
  print_state(" old port state: ", str_port_state(io_port->port_state));
  switch (io_port->port_state)
  {
    case port_state_U:  // Undefined
      if (state == HIGH) io_port->port_state = port_state_UI;   // changing to Inactive
      else io_port->port_state = port_state_UA;               // changing to Active
      break;
    case port_state_UI: // Undefined, changing to Inactive
      if (state == HIGH) io_port->port_state = port_state_I;  // changed to Inactive
      else io_port->port_state = port_state_UA;               // bounce, changing to Active
      break;
    case port_state_UA:
      if (state == HIGH) io_port->port_state = port_state_UI; // bounce, changing to Inactive
      else io_port->port_state = port_state_A;                // changed to Active
      break;
    case port_state_A:                                        // Active
      if (state == HIGH) io_port->port_state = port_state_AI; // changing to Inactive
      break;
    case port_state_AI:
      if (state == HIGH) io_port->port_state = port_state_I;  // changed to Inactive
      else io_port->port_state = port_state_A;                // bounce, changed to Active
      break;
    case port_state_I:                                        // Inactive
      if (state == LOW) io_port->port_state = port_state_IA;  // changing to Active
      break;
    case port_state_IA: // Inactive, changing to Active
      if (state == LOW) io_port->port_state = port_state_A;   // changed to Active
      else io_port->port_state = port_state_I;                // bounce, changed to Inactive
      break;
    default:
      print_state("Unprocessed port state: ", str_port_state(io_port->port_state));  // signal a fault condition
      io_port->port_state = port_state_ port
}

// This procedure is the workhorse of debouncing
// It returns the state of a key.
// The state of a key depends on the state of the port (HIGH or LOW), how  long is was in this state and many other things (when fully implemented).
// For now, whe just return the port_state for testing
key_states_t get_key_state(io_port_t *io_port) {
  port_states_t port_state = get_port_state(io_port);
  print_state(" old key state: ", str_key_state(io_port->key_state));
  switch (io_port->key_state)                                           // process depending on the previous key state
  {
    case key_state_U:
      if ((port_state) == INACTIVE)  io_port->key_state = key_state_I;  // change from U to I, Could also be IP to avoid processing inactive key at power on
      if ((port_state) == ACTIVE) io_port->key_state = key_state_A;     // change from U to A
      break;
    case key_state_A:                                                   // this could/should be removed because the A state is (should be) processed and changed to AP by the process_char_command_port, etc routine
    case key_state_AP:
      if ((port_state) == INACTIVE) io_port->key_state = key_state_I;   // change from U to I
      break;
    case key_state_I:                                                   // this could/should be removed because the I state is (should be) processed and changed to IP by the process_char_command_port, etc routine
    case key_state_IP:
      if ((port_state) == ACTIVE) io_port->key_state = key_state_A;     // change from IP to A
      break;
    default: //
      print_state("Unprocessed key state: ", str_key_state(io_port->key_state));  // signal a fault condition
      io_port->key_state = key_state_U;                                 // must not get here so register undefined to be safe
      break;
  }
  print_state(" new key state: ", str_key_state(io_port->key_state));
  return io_port->key_state;                                         // return the new key state
}

// process a char command port
void process_char_command_port(io_port_t *io_port) {
  if (get_key_state(io_port) == key_state_A) {
    Keyboard.press(io_port->command_char);
    io_port->key_state = key_state_AP;          // set the state to AP to show it is handled and won't be handled again until the port gets inactive
  }
  if (get_key_state(io_port) == key_state_I) {
    Keyboard.release(io_port->command_char);
    io_port->key_state = key_state_IP;          // set the state to IP to show it is handled and won't be handled again until the port gets active
  }
}

// process a double command port
void process_double_command_port(io_port_t *io_port) {
  if (get_key_state(io_port) == key_state_A) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    delay(100);
    Keyboard.releaseAll();
    io_port->key_state = key_state_AP;          // set the state to AP to show it is handled and won't be handled again until the port gets inactive
  }
}

// process a triple command port
void process_triple_command_port(io_port_t *io_port) {
  if (get_key_state(io_port) == key_state_A) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    Keyboard.press(io_port->command_char3);
    delay(100);
    Keyboard.releaseAll();
    io_port->key_state = key_state_AP;          // set the state to AP to show it is handled and won't be handled again until the port gets inactive
  }
}

// process a quadruple command port
void process_quadruple_command_port(io_port_t *io_port) {
  if (get_key_state(io_port) == key_state_A) {
    Keyboard.press(io_port->command_char);
    Keyboard.press(io_port->command_char2);
    Keyboard.press(io_port->command_char3);
    Keyboard.press(io_port->command_char4);
    delay(100);
    Keyboard.releaseAll();
    io_port->key_state = key_state_AP;          // set the state to AP to show it is handled and won't be handled again until the port gets inactive
  }
}

// process the command ports
void process_command_port(io_port_t *io_port)
{
  if (io_port->process_key != NULL)                  // If there is a processing procedure defined
  {
    io_port->process_key(io_port);              // Use the specified procedure to process the port
  }
}
// process the command ports
void process_command_ports()
{
  for (int i = 0; i < IOPORTS; i++)                       // for each io port
    process_command_port(&io_ports[i]);                   // process the port
}

void test_port(io_port_t *io_port) {
  print_state("Testing PortNr: ", io_port->PortNr);
  Serial.println("");
  // first test only port, than only key, than only process
  for (;;)
  {
    port_states_t port_state = get_port_state(io_port);
    //  key_states_t key_state = get_key_state(io_port);
    //process_command_port(io_port);
    Serial.println("");
    delay(1000);
  }
}

int Main_Auto = A4;

uint8_t MainAutoKeyState = HIGH;           // the current state of the key's to send
uint8_t MainAutoButtonState;             // the current reading from the input pin
uint8_t MainAutoLastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  delay(2000);
  init_io_ports();       // initialize all io ports
  Keyboard.begin();
  test_port(&io_ports[5]);
}


void loop() {
  process_command_ports();

  //
  //  //Toggle between Main and Auto menu.
  //  uint8_t reading = digitalRead(Main_Auto);
  //  if (reading != MainAutoLastButtonState) {
  //    // reset the debouncing timer
  //    lastDebounceTime = millis();
  //  }
  //
  //  if ((millis() - lastDebounceTime) > debounceDelay) {
  //    // whatever the reading is at, it's been there for longer than the debounce
  //    // delay, so take it as the actual current state:
  //
  //    // if the button state has changed:
  //    if (reading != MainAutoButtonState) {
  //      MainAutoButtonState = reading;
  //
  //      if (MainAutoButtonState == LOW) {
  //        MainAutoKeyState = !MainAutoKeyState;
  //        if (MainAutoKeyState) {
  //          // Alt+F1
  //          Keyboard.press(0x82);
  //          Keyboard.press(0xC2);
  //          delay(100);
  //          Keyboard.releaseAll();
  //        } else {
  //          // Alt+F4
  //          Keyboard.press(0x82);
  //          Keyboard.press(0xC5);
  //          delay(100);
  //          Keyboard.releaseAll();
  //        }
  //      }
  //    }
  //  }
  //  MainAutoLastButtonState = reading;
}
