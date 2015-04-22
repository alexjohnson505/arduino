/*
  
  Alex Johnson & Amanda Yee
  Physical Computing - Spring 2015
  Bike Indicators

  Goal: Designed to operate on a set
  of LEDS attached to the back of a 
  bike. Using buttons, toggle between
  modes of LED output. Mode are:

     -1 = Waiting
     0 = Left Turn (Blinking Red Left)
     1 = Right Turn (Blinking Red Right)
     2 = Stop (Solid Left/Right Red)
     3 = Hazards (Left/Right Blinking Red)

*/

// Define PIN numbers

#define BUTTON_LEFT   2 // blue
#define BUTTON_CENTER 3 // green
#define BUTTON_RIGHT  4 // orange

#define TONE_PIN 22 // brown

#define L_REDPIN   11
#define L_GREENPIN 10

#define R_REDPIN   8
#define R_GREENPIN 7

#define LED_MAX 200

#define BLINK_SPEED 400

// Store the state of the program
int mode;

// milliseconds since program run
unsigned long time;

// milliseconds at which the mode started
unsigned long modeStartedAt;

// millisenconds since the last mode change
unsigned long timeInMode;

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
  
  // Start global counter
  time = 0;

  // Init mode
  mode = -1;

}

void loop() {
  
  // update milliseconds 
  // since program started
  time = millis();
  
  int buttons[] = { 0, 0, 0 };

  // read the button state
  buttons[0]  = digitalRead(BUTTON_LEFT);
  buttons[1]  = digitalRead(BUTTON_CENTER);
  buttons[2]  = digitalRead(BUTTON_RIGHT);

  // Did the user press "left turn"?
  if (buttons[0] == HIGH) {   

    // Reset counter
    modeStartedAt = time;
     
    Serial.println("LEFT BUTTON");
    mode = 0;
    beep(900, 10, 0);
  }

  // Did the user press "right turn"?
  if (buttons[2] == HIGH) {     
    
    // Reset counter
    modeStartedAt = time;
    
    Serial.println("RIGHT BUTTON");
    mode = 1;
    beep(900, 10, 0);
  }

  // Did the user press "stop"?
  if (buttons[1] == HIGH) {    
    
    // Reset counter
    modeStartedAt = time;
    
    Serial.println("CENTER BUTTON");
    mode = 2;
    beep(500, 20, 0);
  }

  // Did the user HOLD "stop"
  if (buttons[1] == HIGH && mode == 2) {
    
     Serial.println("CENTER BUTTON HOLD");
     mode = 3;
     beep(900, 90, 0);
  }
  
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
     beep(400, 60, 0);
  }

  // Init LED values. Store each LED strip 
  // seperately as: { Red, Green, Blue }
  
  int left[] = { 0, 0, 0 };
  int right[] = { 0, 0, 0 };

  // Default
  if (mode == -1 ) {
    Serial.println("ZERO MODE");
    
    left[0] = LED_MAX;
    left[1] = LED_MAX;
    left[2] = LED_MAX;
    
    right[0] = LED_MAX;
    right[1] = 100;
    right[2] = LED_MAX;
  }
  
  // LEFT TURN
  if (mode == 0) {
    
    left[0] = LED_MAX;
    
    // Blink state
    if (blink(modeStartedAt)) {
      left[0] = 100;
      left[1] = 100;
    }
  }
  
  // RIGHT TURN
  if (mode == 1) {

    right[0] = LED_MAX;
    
    // Blink state
    if (blink(modeStartedAt)) {
      right[0] = 100;
      right[1] = 100;
    } 
  }
  
  // STOP
  if (mode == 2) {
    
    left[0] = LED_MAX;
    right[0] = LED_MAX;

  }
  
  // HAZARDS
  if (mode == 3) {
    
    left[0] = LED_MAX;
    
    right[0] = LED_MAX;

    // Blink state
    if (blink(modeStartedAt)) {
      left[0] = 0;
      right[0] = 0;
      
      left[2] = LED_MAX;
      right[2] = LED_MAX;
    } 

  }

  // Debug  
  String output = "( " + String(buttons[0]) + " , " + String(buttons[1]) + " , " + String(buttons[2]) + " ) | Mode: " + String(mode);
  Serial.println(output);
  
  // wait so as not to send massive amounts of data
  // Each loop = 1/10 seconds
  delay(400);
    
  // Apply Changes to LEDs
  refresh(left, right);
  
}

// Apply Changes to LEDs
void refresh(int left[], int right[]){
   
  analogWrite(13, 250);
  
  analogWrite(L_REDPIN,   left[0]);
  analogWrite(L_GREENPIN, left[2]);
  
  analogWrite(R_REDPIN,   right[0]);
  analogWrite(R_GREENPIN, right[2]);
}


void beep(int freq, int ms, int pause){
  tone(TONE_PIN, freq);
  delay(ms);
  noTone(TONE_PIN);
  delay(pause);
}

// Is it time for the second state yet?
boolean blink(int clock){
  
  // Time since start of loop
  int loop_time = BLINK_SPEED * 2;
  
  // Are we in the second state?
  return ((clock % loop_time) > BLINK_SPEED);
 
}



