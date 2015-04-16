/*
  
  Alex Johnson & Amanda Yee
  Physical Computing - Spring 2015
  Bike Indicators
  
*/

// Define PIN numbers
#define button_left   11
#define button_center 12
#define button_right  13

#define L_REDPIN   7
#define L_GREENPIN 6
#define L_BLUEPIN  5

#define R_REDPIN   10
#define R_GREENPIN 9
#define R_BLUEPIN  8 

#define TONE_PIN 2

#define LED_MAX 250

#define BLINK_SPEED 500

// Store the state of the program
int mode;

// Record time
unsigned long time;
unsigned long modeStartedAt;
unsigned long timeInMode;

/*
 Mode Settings:
 mode can be one of:
 
   -1 = Waiting
   0 = Left Turn (Blinking Red Left)
   1 = Right Turn (Blinking Red Right)
   2 = Stop (Solid Left/Right Red)
   3 = Hazards (Left/Right Blinking Red)

   
 */

void setup() {

  // Define Button PINS
  pinMode(button_left, INPUT);    
  pinMode(button_center, INPUT);    
  pinMode(button_right, INPUT);    

  // Define Left LED Pins
  pinMode(L_REDPIN, OUTPUT);
  pinMode(L_GREENPIN, OUTPUT);
  pinMode(L_BLUEPIN, OUTPUT);

  // Define Right LED Pins
  pinMode(R_REDPIN, OUTPUT);
  pinMode(R_GREENPIN, OUTPUT);
  pinMode(R_BLUEPIN, OUTPUT);

  // Open Serial Communication
  Serial.begin(9600); 
  
  // Start global counter
  time = 0;

  // Init mode
  mode = -1;

}

void loop() {
  
  // Stores time since program has started
  time = millis();
  
  // Get button selected
  if (true) {
    
    // mode = -1, 0, 1, 2, 3
    modeStartedAt = time;
    // beep(900, 30, 0);
  }
  
  // Count ms in current mode
  timeInMode = time - modeStartedAt;
  
  // Reset mode after 10 seconds
  if (timeInMode > 1000 * 10) {

    // Reset Mode
     mode = -1;
     
     // Reset counter
     modeStartedAt = time;
     
     // Alert user of change
     beep(400, 30, 0);
  }

  // Init LED values. Store each LED strip 
  // seperately as: { Red, Green, Blue }
  
  int left[] = { 0, 0, 0 };
  int right[] = { 0, 0, 0 };

  // Do nothing
  if (mode == -1 ) {
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
   
  // Notify user
  // beep(900, 30, 2000);
  
  // wait so as not to send massive amounts of data
  // Each loop = 1/5s
  delay(200);

  // Apply Changes to LEDs
  refresh(left, right);
  
}

// Apply Changes to LEDs
void refresh(int left[], int right[]){
  
  analogWrite(L_REDPIN,   left[0]);
  analogWrite(L_BLUEPIN,  left[1]);
  analogWrite(L_GREENPIN, left[2]);
  
  analogWrite(R_REDPIN,   right[0]);
  analogWrite(R_BLUEPIN,  right[1]);
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
  return (clock % loop_time > BLINK_SPEED);
 
}



