


int LED_1 = 12;  //6
int LED_2 = 13;  //7
int LED_3 = 15;  //8

int BTN_1 = 0;  //3
int BTN_2 = 4;  //2
int BTN_3 = 5;  //1

int ENC_1 = 16;  //0
int ENC_2 = 2;  //4

int LIGHT = A0; //A0

int oldA = 0;
int oldB = 0;

//game logic
/**
 * 0 - just started
 * 1,2,3 - first, second and third tasks completed.
 */
int state = 0;

int btn1state = LOW;
int btn2state = LOW;
int btn3state = LOW;

int task1counter = 0;

int led1blink = false;
int led2blink = false;
int led3blink = false;

float light = 0;
float baseLight = 0;

void setup() {

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_3, INPUT);

  pinMode(ENC_1, INPUT);
  pinMode(ENC_2, INPUT);

  //set base light so game work in many conditions
  baseLight = analogRead(LIGHT);

  blinkLedsFast();

  Serial.begin(115200);
  //Serial.println("AZZ-AZZ-AZZ");

}

void loop() {
  int encChange = getEncTurn();
  if (state == 0 && encChange!=0){
    //Serial.println("X >> " + String(task1counter));
    task1counter += abs(encChange);
    if(task1()) {
      state = 1;
    } else {
      led1blink = true;
    }
  }

  //buttons
  if (digitalRead(BTN_1) != btn1state) {
    led2blink = true;
    btn1state = digitalRead(BTN_1);
  }
  if (digitalRead(BTN_2) != btn2state) {
    led2blink = true;
    btn2state = digitalRead(BTN_2);
  }
  if (digitalRead(BTN_3) != btn3state) {
    led2blink = true;
    btn3state = digitalRead(BTN_3);
  }

  float t = analogRead(LIGHT); 
  if (abs(t-light) > 100) {
    //Serial.println("TTT: " + String(t) );
    light = t;
    led3blink = true;
  }

  if(state == 1 && task2()) {
    state = 2;
  } else if(state == 2 && !task2()) {
    state = 1;
  } else if(state == 2 && task3()) {
    state = 3;
  }

  if (state < 1 && led1blink) {
    blinkLed(LED_1);
    led1blink = false;
  }
  if (state < 2 && led2blink) {
    blinkLed(LED_2);
    led2blink = false;
  }
  if (state < 3 && led3blink) {
    blinkLed(LED_3);
    led3blink = false;
  }

  switch(state) {
    case 0:
      //leds off
    break;
    case 1:
      digitalWrite(LED_1, HIGH);  
    break;
    case 2:
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, HIGH);
    break;
    case 3:
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, HIGH);
      digitalWrite(LED_3, HIGH);
    break;
  }

  /*
  if (digitalRead(BTN_1) == HIGH) {
    digitalWrite(LED_1, HIGH); 
  } else {
     digitalWrite(LED_1, LOW); 
  }

  if (digitalRead(BTN_2) == HIGH) {
    digitalWrite(LED_2, HIGH); 
  } else {
     digitalWrite(LED_2, LOW); 
  }

  if (digitalRead(BTN_3) == HIGH) {
    digitalWrite(LED_3, HIGH); 
  } else {
     digitalWrite(LED_3, LOW); 
  }
  */

  delay(1);
}

boolean task1() {
  if (task1counter > 30) {
    return true;
  } else {
    return false;
  }
}

boolean task2() {
  if (btn1state == HIGH
      && btn2state == LOW
      && btn3state == HIGH) {
    return true;
  } else {
    return false;
  }
}

boolean task3() {
  return light-baseLight > 300;
}

/**
 * return -1,0,1
 * if rotating too fast or slow provide invalida result (sometimes)
 */
int getEncTurn() {
  int a = digitalRead(ENC_1);
  int b = digitalRead(ENC_2);
  int result = 0;
  if (oldA == LOW && a==HIGH) {
    result = (oldB * 2 - 1);
  }
  oldA = a;
  oldB = b;
  return result;
}



void blinkLedsFast() {
  for (int i = 0; i < 3; i++) {
    enableLeds();
    delay(100); 
    disableLeds();
    delay(100); 
   }
}

void disableLeds(){
  digitalWrite(LED_1, LOW);  
  digitalWrite(LED_2, LOW); 
  digitalWrite(LED_3, LOW); 
}

void enableLeds(){
  digitalWrite(LED_1, HIGH);  
  digitalWrite(LED_2, HIGH); 
  digitalWrite(LED_3, HIGH); 
}

void blinkLed(int led) {
  digitalWrite(led, HIGH); 
  delay(100); 
  digitalWrite(led, LOW);  
  delay(100); 
}

