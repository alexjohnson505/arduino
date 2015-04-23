/*
  
  Alex Johnson & Amanda Yee
  Physical Computing - Spring 2015
  Bike Indicators

  Goal: Designed to operate on a set
  of LEDS attached to the back of a 
  bike. Using buttons, toggle between
  modes of LED output. Modes are:

     -1 = Waiting
     0 = Left Turn (Blinking Red Left)
     1 = Right Turn (Blinking Red Right)
     2 = Hazards (Left/Right Blinking Red)
     
  The left and right buttons activate the
  corresponding turn signal. These turn signals
  will automatically turn off after 10 seconds.
  
  The center button, when pressed will reset
  the current mode to the default "waiting"
  status. A long-press will active HAZARDS mode,
  enabling a state where both LED arrows blink
  until the user de-activates the state.

*/

// Define PIN numbers
#define BUTTON_LEFT   2 // LEFT   (blue)
#define BUTTON_CENTER 3 // RESET  (green)
#define BUTTON_RIGHT  4 // RIGHT  (orange)

// Beeper
#define TONE_PIN 5 // (brown)

// Left Arrow
#define L_REDPIN   11
#define L_GREENPIN 10

// Right Arrow
#define R_REDPIN   8
#define R_GREENPIN 7

// Max value for LED
#define LED_MAX 250

// Current mode
int mode;

// time in ms since program run
unsigned long time;

// time in ms at which the current mode began
unsigned long modeStartedAt;

// millisenconds since the last mode change
// Used for resetting turn indicators after 10s
unsigned long timeInMode;

// For the turn indicators, alternate
// between two different states.
boolean blink;

// Initialize the program
void setup() {

  // Define Button PINS
  pinMode(BUTTON_LEFT, INPUT);    
  pinMode(BUTTON_CENTER, INPUT);    
  pinMode(BUTTON_RIGHT, INPUT);    

  // Define Left LED Pins
  pinMode(L_REDPIN, OUTPUT);
  pinMode(L_GREENPIN, OUTPUT);

  // Define Right LED Pins
  pinMode(R_REDPIN, OUTPUT);
  pinMode(R_GREENPIN, OUTPUT);

  // Open Serial Communication
  Serial.begin(9600); 
  
  // Start global time counter
  time = 0;

  // Initialize mode
  mode = -1;
  
  // Initialize blink state
  blink = false;

}

void loop() {
  
  /****************************
        TIME COUNTING
   ****************************/
   
  // update milliseconds 
  // since program started
  time = millis();
  
  // milliseconds in current mode
  timeInMode = time - modeStartedAt;
  
  // Reset turn indicators after
  // a total of ten seconds have passed
  if ((timeInMode > 1000 * 10) && (mode == 0 || mode == 1 )) {
    
    // Debug
    Serial.println("10 Seconds in mode. Resetting.");

    // Reset Mode
     mode = -1;
     
     // Reset counter
     modeStartedAt = time;
     
     // Alert user of change
     beep(800, 90, 0);
  }
  
  /****************************
            INPUT
   ****************************/
    
  // What buttons are selected
  int buttons[] = { 0, 0, 0 };

  // read the button state
  buttons[0]  = digitalRead(BUTTON_LEFT);
  buttons[1]  = digitalRead(BUTTON_CENTER);
  buttons[2]  = digitalRead(BUTTON_RIGHT);

  /****************************
           SET MODE
   ****************************/
   
  // Did the user press "left turn"?
  if (buttons[0] == HIGH) {   

    // Reset counter
    modeStartedAt = time;

    // Change Mode
    Serial.println("LEFT BUTTON");    
    mode = 0;
    beep(1200, 30, 0);
  }

  // Did the user press "right turn"?
  if (buttons[2] == HIGH) {     
    
    // Reset counter
    modeStartedAt = time;

    // Change Mode
    Serial.println("RIGHT BUTTON");
    mode = 1;
    beep(1200, 30, 0);
  }
  
  // Did the user HOLD "stop"?
  // "HAZARDS" mode (2) uses a 1 second press of the
  // stop button, and requires the following conditions
  //  - Middle button pressed
  //  - Not currently turning
  //  - Pressed for at least 1 second
  
  if (buttons[1] == HIGH && mode == -1 && timeInMode > 1000) {
  
     // Change Mode
     Serial.println("HAZARDS");
     mode = 2;
     
     // Double Beep
     beep(600, 90, 60);
     beep(600, 90, 0);

  // Did the user press "reset"?
  } else if (buttons[1] == HIGH) {
    
    // Reset counter
    modeStartedAt = time;

    // Change Mode    
    Serial.println("RESET");
    mode = -1;
    beep(1200, 60, 0);
  }

  /****************************
        ADJUST LEDs
   ****************************/
  
  // Init LED values. Store each LED strip 
  // seperately as: { Red, Green, Blue } 
  int left[] = { 0, 0, 0 };
  int right[] = { 0, 0, 0 };
   
  // Default.
  // Gentle yellow "running lights"
  if (mode == -1 ) {
    
    left[0] = 50;
    left[1] = 50;
    
    right[0] = 50;
    right[1] = 50;
  }
  
  // LEFT TURN
  // Left arrow blinks between red and yellow
  if (mode == 0) {
    
    left[0] = LED_MAX;
    
    // Blink state
    if (blink) {
      
      left[0] = 200;
      left[1] = LED_MAX;
      beep(400, 30, 0);
    }
  }
  
  // RIGHT TURN
  // Right arrow blinks between red and yellow
  if (mode == 1) {

    right[0] = LED_MAX;
    
    // Blink state
    if (blink) {
      
      right[0] = 200;
      right[1] = LED_MAX;
      beep(400, 30, 0);
    } 
  }
  
  // HAZARDS
  // Both arrows blink between
  // red and yellow
  if (mode == 2) {
    
    left[0] = LED_MAX;
    right[0] = LED_MAX;

    // Blink state
    if (blink) {
      
      left[1] = LED_MAX;
      right[1] = LED_MAX;
      
      beep(900, 60, 0);
    } 

  }
  
  // blink change.
  // Alternate between loops
  blink = !blink;

  // Each loop = 2/5 of a second
  // Slow the iteration speed, as to
  // simplify the button, beeping, and
  // blinking logic.
  delay(400);
    
  // Apply Changes to LEDs
  refresh(left, right);
  
  /****************************
           DEBUGGING
   ****************************/
   
  // String output = "( " + String(buttons[0]) + " , " + String(buttons[1]) + " , " + String(buttons[2]) + " ) | Mode: " + String(mode) + " | Time in Mode: " + String(timeInMode);
  // Serial.println(output);
  
}


/****************************
        ADJUST LEDs
 ****************************/

// Apply changes to LED output
void refresh(int left[], int right[]){  
  analogWrite(L_REDPIN,   left[0]);
  analogWrite(L_GREENPIN, left[1]);
  analogWrite(R_REDPIN,   right[0]);
  analogWrite(R_GREENPIN, right[1]);
}

/****************************
          BEEP
 ****************************/
// Produce a sound.
//  - freq = pitch of sound
//  - ms   = length of sound
//  - puse = delay after sound

void beep(int freq, int ms, int pause){
  tone(TONE_PIN, freq);
  delay(ms);
  noTone(TONE_PIN);
  delay(pause);
}
