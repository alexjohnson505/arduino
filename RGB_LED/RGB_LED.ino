
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

#define TONE 2

#define LED_MAX 250

// Store the state of the program
int index;
int mode;

// Record time
unsigned long time;
unsigned long modeStartedAt;

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

  // Init Counter.
  index = 0;

  // Init mode
  mode = -1;

}

void loop() {

  // Stores time since program has started
  time = millis();

  // Init LED values. Store each LED strip 
  // seperately as: { Red, Green, Blue }
  
  int left[] = { 0, 0, 0 };
  int right[] = { 0, 0, 0 };

  // Apply Changes to LEDs
  refresh(left, right);
   
  // Notify user
  // beep();
  
  // wait so as not to send massive amounts of data
  delay(10);
  
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


void beep(){
  tone(2, 900);
  delay(50);
  noTone(2);
}




