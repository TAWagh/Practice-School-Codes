/*
 * Wafflr:Food Automation system 
 * Component Testing - IR SENSOR
 * 
 * /--------CONNECTIONS---------/
    Red wire: +5V
    Green wire: GND
    Yellow wire: Digital output
    
    OR
    
    Brown wire: +5V
    Blue wire: GND
    Black wire: Digital output 
 *
 *
 */
 
 #define IR 9

void setup() {
  pinMode(IR, INPUT_PULLUP); // Comment this line if you have already used a pull-up resistor
  Serial.begin(9600);
}

void loop() {
  int a;
  a=digitalRead(IR);
  Serial.println(a);
  
  if(a == 0){
    Serial.println(" Waffle detected");
  }  
  else{
    Serial.println("clear");
  }
  
  delay(500);
}
