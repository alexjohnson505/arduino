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

*/

// Define PIN numbers
#define BUTTON_LEFT   2 // LEFT   (blue)
#define BUTTON_CENTER 3 // RESET  (green)
#define BUTTON_RIGHT  4 // RIGHT  (orange)

// Beeper
#define TONE_PIN 5 // brown

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
// Used for resetting turn indicators
// after 10 seconds
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
  
  // Reset mode after 10 seconds
  if ((timeInMode > 1000 * 10) && (mode != -1)) {
    
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
  
  // Did the user HOLD "stop". 
  // "hazards mode" (2) uses a 1,000 ms press
  // of the stop button, requires the following conditions
  //  - Middle button pressed
  //  - Not currently turning
  //  - Pressed for at least 1 second
  
  if (buttons[1] == HIGH && mode == -1 && timeInMode > 1000) {
  
     // Change Mode
     Serial.println("CENTER BUTTON HOLD");
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
   
  // Default
  if (mode == -1 ) {
    
    left[0] = 50;
    left[1] = 50;
    
    right[0] = 50;
    right[1] = 50;
  }
  
  // LEFT TURN
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
  if (mode == 1) {

    right[0] = LED_MAX;
    
    // Blink state
    if (blink) {
      right[0] = 200;
      right[1] = LED_MAX;
      beep(400, 30, 0);
    } 
  }
  
  // STOP
  if (mode == 2) {
    
    left[0] = LED_MAX;
    right[0] = LED_MAX;

  }
  
  // HAZARDS
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

  /****************************
           DEBUGGING
   ****************************/
   
  String output = "( " + String(buttons[0]) + " , " + String(buttons[1]) + " , " + String(buttons[2]) + " ) | Mode: " + String(mode) + " | Time in Mode: " + String(timeInMode);
  Serial.println(output);
  
  // blink change
  blink = !blink;
  
  // wait so as not to send massive amounts of data
  // Each loop = 1/10 seconds
  delay(400);
    
  // Apply Changes to LEDs
  refresh(left, right);
  
}


/****************************
        ADJUST LEDs
 ****************************/
   
void refresh(int left[], int right[]){  
  analogWrite(L_REDPIN,   left[0]);
  analogWrite(L_GREENPIN, left[1]);
  analogWrite(R_REDPIN,   right[0]);
  analogWrite(R_GREENPIN, right[1]);
}

/****************************
          BEEP
 ****************************/
   
void beep(int freq, int ms, int pause){
  tone(TONE_PIN, freq);
  delay(ms);
  noTone(TONE_PIN);
  delay(pause);
}
