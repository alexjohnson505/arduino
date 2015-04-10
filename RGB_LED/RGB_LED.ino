
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

#define maxVal 250

int index;
int mode;
unsigned long time;
unsigned long modeStartedAt;

/*
  
 Mode Settings:
 mode can be one of:
 
 -1 = Waiting
 0 = Left Turn
 1 = Right Turn
 2 = Stop
 3 = Hazards
 */

void setup() {

  // Define Input PINS
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

  // Init LED values
  int L_REDVAL = 0;
  int L_GREENVAL = 0;
  int L_BLUEVAL = 0;
  int R_REDVAL = 0;
  int R_GREENVAL = 0;
  int R_BLUEVAL = 0;

  // Watch buttons
  if (digitalRead(button_left) == HIGH) {     

    // Toggle Next Mode
    mode = mode + 1;
    mode = mode % 4;

    modeStartedAt = time;

    // Serial.println("Mode Updated:");
    // Serial.println(mode);
    Serial.println(mode);

  } 
  else {

  }


  // Off
  if (mode == -1){
  }

  // Left Turn
  if (mode == 0){

    // 10s limit
    if (time - modeStartedAt > 10000){
      mode = -1;
    }

    L_REDVAL = maxVal;
    R_REDVAL = maxVal;

    // if (time % 1000 > 500){
    //    L_REDVAL = maxVal;
    // } else {
    //    L_REDVAL = 0;
    //}
  }

  // Right Turn
  if (mode == 1){

    // 10s limit
    if (time - modeStartedAt > 10000){
      mode = -1;
    }

    if (time % 1000 > 500){
      L_BLUEVAL = maxVal;
      R_BLUEVAL = maxVal;
    } 
    else {
      L_BLUEVAL = 0;
      R_BLUEVAL = 0;
    }
  }

  // Stop
  if (mode == 2){
    L_BLUEVAL = maxVal;
    R_BLUEVAL = maxVal;
  }

  // Hazards
  if (mode == 3){
    L_REDVAL = 155;
    L_BLUEVAL = 155;
    L_GREENVAL = 155;
    R_REDVAL = 155;
    R_BLUEVAL = 155;
    R_GREENVAL = 155;
  }

  // Apply Changes to LEDs
  analogWrite(L_REDPIN,   L_REDVAL);
  analogWrite(L_BLUEPIN,  L_BLUEVAL);
  analogWrite(L_GREENPIN, L_GREENVAL);
  analogWrite(R_REDPIN,   R_REDVAL);
  analogWrite(R_BLUEPIN,  R_BLUEVAL);
  analogWrite(R_GREENPIN, R_GREENVAL);

  // wait a second so as not to send massive amounts of data
  delay(400);

}


