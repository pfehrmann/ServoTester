# ServoTester
Tool for simple servo calibration

## Usage
If you have to find the parameters for the Adafruit 12bit PWM board for a servo, connect the servo and strat finding the left and right maximums. The values will be written to both the LCD display and the Serial output.

If you have to find the values for Timer1, divide the value displayed by 4 (Timer1 uses values from 0..1024, Aafruit PWM from 0..4096).

## Connections
To use the tester, plug the PWM pin of the servo into pin 13 of the Arduiuino. Plug the positive and negative wires of the servo into the corresponding ones of the Arduino. If you can, connect a battery to the servo since the servo can drain a lot of power and make sure to have common ground.
