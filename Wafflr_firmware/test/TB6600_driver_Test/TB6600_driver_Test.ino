/*
 * Wafflr:Food Automation system 
 * Component Testing - TB6600 4A Stepper Motor Driver
 * 
 * /--------CONNECTIONS---------/
   Signal
   - VCC : 5V
   - CLK- : pin7
   - CW- : pin6
   - EN- : pin5
   High voltage
   - B- : Blue
   - B+ : Yellow
   - A- : Red
   - A+ : Green
   - GND : Common ground
   - VCC: 12V
   Switches
   - SW [1-4] : 0010
 *
 *
 * Note: This code simply rotates the motor at 5rpm. The step number can be observesd in the serial monitor
 */

#define en 5
#define dirPin 6
#define stepPin 7

int c=0;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(en, OUTPUT);

  Serial.begin(9600);
  
  digitalWrite(en, HIGH);
  digitalWrite(dirPin, HIGH); // We only need to rotate motor in one direction (anticlockwise)
}

void loop() {
  // one pulse is one step (rising edge)
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(stepPin, LOW);
  
  // to display step number
  c++;
  Serial.println(c);
  if (c==200)
    c=0;

  //speed
  delay(60); //(60ms -> 5rpm)
  
}
