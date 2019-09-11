#include <Arduino.h>
#include <Servo.h>
#include <OneButton.h>
#include <TimerOne.h>

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
    break;
  }
  case FIND_RIGHT:
  {
    right--;
    Serial.print("Right: ");
    Serial.println(right);
    break;
  }
  case SWEEP:
  {
    sweepDelay++;
    Serial.print("Sweep delay: ");
    Serial.println(sweepDelay);
    break;
  }
  case JUMP:
  {
    Timer1.pwm(pwmPin, left / 4.0);
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
    break;
  case FIND_RIGHT:
    right++;
    Serial.print("Right: ");
    Serial.println(right);
    break;
  case SWEEP:
    sweepDelay--;
    if (sweepDelay < 1)
    {
      sweepDelay = 1;
    }
    Serial.print("Sweep delay: ");
    Serial.println(sweepDelay);
    break;
  case JUMP:
    Timer1.pwm(pwmPin, right / 4.0);
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
    break;
  case FIND_RIGHT:
    currentState = SWEEP;
    currentPos = right;
    direction = -1;
    Serial.println("Sweeping");
    break;
  case SWEEP:
    currentState = JUMP;
    Serial.println("Jumping...");
    break;
  case JUMP:
    currentState = FIND_LEFT;
    Serial.println("Finding left...");
    Serial.print("Left: ");
    Serial.println(left);
    break;
  }
}