#include <Stepper.h>

#define PIN_R 8
#define PIN_G 9
#define PIN_B 10

#define PIN_INPUT A7
#define PIN_OUTPUT 6

#define but 36
#define but2 37

#define stepsPerRevolution 90

float rgbled_r = 255.0,
  rgbled_g = 0.0,
  rgbled_b = 0.0,
  rainbow_step = 0.25;

bool buting = 1, buting1 = 1, error;

bool opening = false, rgbled_k = true;

Stepper moveStepper(stepsPerRevolution, 22,24,26,28);
Stepper platformStepper(stepsPerRevolution, 23,25,27,29);

void setup() {
  Serial.begin(9600);

  pinMode(PIN_INPUT, INPUT_PULLUP);
  pinMode(PIN_OUTPUT, OUTPUT);

  moveStepper.setSpeed(30);
  platformStepper.setSpeed(15);

  pinMode(but, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  delay(100);
}

void loop() {
  rgbled_k = !rgbled_k;
  if (rgbled_k) {
    if (rgbled_r == 255) {
      rgbled_r = 255 - rainbow_step;
      rgbled_g = rainbow_step;
    } else if (rgbled_g == 255) {
      rgbled_g = 255 - rainbow_step;
      rgbled_b = rainbow_step;
    } else if (rgbled_b == 255) {
      rgbled_b = 255 - rainbow_step;
      rgbled_r = rainbow_step;
    } else if (rgbled_r && rgbled_g) {
      rgbled_r -= rainbow_step;
      rgbled_g += rainbow_step;
    } else if (rgbled_g && rgbled_b) {
      rgbled_g -= rainbow_step;
      rgbled_b += rainbow_step;
    } else if (rgbled_b && rgbled_r) {
      rgbled_b -= rainbow_step;
      rgbled_r += rainbow_step;
    }
  }
  if (analogRead(PIN_INPUT) > 300) {
    setColor(255, 0, 0);
    if (opening) open();
    else close();
    opening = !opening;
    digitalWrite(PIN_OUTPUT, HIGH);
    delay(300);
    digitalWrite(PIN_OUTPUT, LOW);
  }
  setColor(rgbled_r, rgbled_g, rgbled_b);
  delay(5);
}

void setColor(int rColor, int gColor, int bColor) {
  analogWrite(PIN_R, 255 - rColor);
  analogWrite(PIN_G, 255 - gColor);
  analogWrite(PIN_B, 255 - bColor);
}

void open() {
  delay(3000);
  // Serial.println(digitalRead(but2));
  // while (buting == 1) {
  //   Serial.println("Going A");
  //   moveStepper.step(2);
  //   buting = digitalRead(but);
  // }
  // Serial.println("Stop");
}

void close() {
  delay(3000);
  // platformStepper.step(-120);
  // platformStepper.step(120);
  // moveStepper.step(-180);
  
  // buting1 = 1;
  // buting = 1;
  
  // while (buting1 == 1) {
  //   Serial.println("Going B");
  //   moveStepper.step(-2);
  //   buting1 = digitalRead(but2);
  // }
  // Serial.println("Finish");
}
