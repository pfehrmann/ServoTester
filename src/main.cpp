#include <Arduino.h>
#include <Servo.h>
#include <OneButton.h>
#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
OneButton buttonLeft(9, true);
OneButton buttonRight(7, true);
OneButton buttonCenter(8, true);
const int pwmPin = 10;

typedef enum
{
  FIND_LEFT,
  FIND_RIGHT,
  SWEEP,
  JUMP
} states;

void leftClick();
void centerClick();
void rightClick();

int countLeft = 0;
int countRight = 0;

void leftLongClick()
{
  if (countLeft == 0)
  {
    leftClick();
  }
  countLeft++;
  countLeft %= 75;
}

void rightLongClick()
{
  if (countRight == 0)
  {
    rightClick();
  }
  countRight++;
  countRight %= 75;
}

states currentState = FIND_LEFT;

int left = 300;
int right = 600;
int sweepDelay = 10;
int direction = 1;
int currentPos = left;

void setup()
{
  Serial.begin(115200);
  buttonLeft.attachClick(leftClick);
  buttonCenter.attachClick(centerClick);
  buttonRight.attachClick(rightClick);

  buttonLeft.attachDuringLongPress(leftLongClick);
  buttonRight.attachDuringLongPress(rightLongClick);

  Timer1.initialize(16666);

  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Find left");
  lcd.setCursor(0, 1);
  lcd.print("Timer1: ");
  lcd.print(left);
  lcd.setContrast(127);
  lcd.setBacklight(127);
}

void loop()
{
  buttonLeft.tick();
  buttonRight.tick();
  buttonCenter.tick();

  switch (currentState)
  {
  case FIND_LEFT:
    Timer1.pwm(pwmPin, left / 4.0);
    break;
  case FIND_RIGHT:
    Timer1.pwm(pwmPin, right / 4.0);
    break;
  case SWEEP:
    currentPos += direction;
    if (currentPos == left || currentPos == right)
    {
      direction *= -1;
      delay(600);
    }
    Timer1.pwm(pwmPin, currentPos / 4.0);
    delay(sweepDelay);
    break;
  case JUMP:
    break;
  }
}

void leftClick()
{
  switch (currentState)
  {
  case FIND_LEFT:
  {
    left--;
    Serial.print("Left: ");
    Serial.println(left);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Find left");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(left);
    break;
  }
  case FIND_RIGHT:
  {
    right--;
    Serial.print("Right: ");
    Serial.println(right);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Find right");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(right);
    break;
  }
  case SWEEP:
  {
    sweepDelay++;
    Serial.print("Sweep delay: ");
    Serial.println(sweepDelay);
    lcd.setCursor(0, 1);
    lcd.print("               ");
    lcd.setCursor(0, 1);
    lcd.print("Sweep delay ");
    lcd.print(sweepDelay);
    break;
  }
  case JUMP:
  {
    Timer1.pwm(pwmPin, left / 4.0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Jumping");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(left);
    break;
  }
  }
}

void rightClick()
{
  switch (currentState)
  {
  case FIND_LEFT:
    left++;
    Serial.print("Left: ");
    Serial.println(left);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Find left");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(left);
    break;
  case FIND_RIGHT:
    right++;
    Serial.print("Right: ");
    Serial.println(right);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Find right");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(right);
    break;
  case SWEEP:
    sweepDelay--;
    if (sweepDelay < 1)
    {
      sweepDelay = 1;
    }
    Serial.print("Sweep delay: ");
    Serial.println(sweepDelay);
    lcd.setCursor(0, 1);
    lcd.print("               ");
    lcd.setCursor(0, 1);
    lcd.print("Sweep delay ");
    lcd.print(sweepDelay);
    break;
  case JUMP:
    Timer1.pwm(pwmPin, right / 4.0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Jumping");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(right);
    break;
  }
}

void centerClick()
{
  switch (currentState)
  {
  case FIND_LEFT:
    currentState = FIND_RIGHT;
    Serial.println("Finding right...");
    Serial.print("Right: ");
    Serial.println(right);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Find right");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(right);
    break;
  case FIND_RIGHT:
    currentState = SWEEP;
    currentPos = right;
    direction = -1;
    Serial.println("Sweeping");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sweeping");
    lcd.setCursor(0, 1);
    lcd.print("Sweep delay ");
    lcd.print(sweepDelay);
    break;
  case SWEEP:
    currentState = JUMP;
    Serial.println("Jumping...");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Jumping");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(left);
    break;
  case JUMP:
    currentState = FIND_LEFT;
    Serial.println("Finding left...");
    Serial.print("Left: ");
    Serial.println(left);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Find left");
    lcd.setCursor(0, 1);
    lcd.print("Timer1: ");
    lcd.print(left);
    break;
  }
}