/*
 * Wafflr:Food Automation system 
 * Component Testing - DRV8825 2.2A Stepper Motor Driver
 * 
 * /--------CONNECTIONS---------/
   - ENA, MS1, MS2, MS3, VDD: NC
   - RES,SLP : 5V
   - STP : 3pin
   - DIR : 2pin
   - GND : GND from arduino
   - 1B 1A : coil 1
   - 2A 2B : coil 2
   - GND: from SMPS
   - VMT: 12V
 *
 *
 * Warning!!!!
 * #Do not give power to driver if the motor is not connected
 * #Do not forget to connect a 100uF bypass cap across 12V power supply
 * #Always set current limiting pot
 * 
 */

//The codes rotates the motor at an angle of 72 (360/5) degrees 

#define stepPin 3
#define en 2

void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(en, OUTPUT);
  

  Serial.begin(9600);

  Serial.println("Press 1 to dispense");
}
void loop(){
  
  char in = '0';
  if (Serial.available() > 0) {
    in = Serial.read();
   }
  if (in== '1'){
      digitalWrite(stepPin, LOW);
      //rotate 72 degrees
      for(int i = 0; i < 40; i++)
      {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(3000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(3000);
      }
      digitalWrite(stepPin, HIGH);
  }
}
