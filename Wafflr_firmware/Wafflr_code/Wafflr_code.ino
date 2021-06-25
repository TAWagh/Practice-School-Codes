/*
 * 
 * Proect Name: Wafflr: Food Automation system 
 * Author: Titiksha Wagh
 * Intellectual Property of lvlAlpha
 *
 *
 *Note: - function for fluid dispensing has not been encorporated
 *
 */
 
//----------------IR SENSORS----------------//

#define IR_1 22   // below vertial stacker
#define IR_2 23   // airfryer enter
#define IR_3 24   // airfryer exit
#define IR_4 25   // condiment 1
#define IR_5 26   // condiment 2
#define IR_6 27   // condiment 3
#define IR_7 28   // exit

//----------------THERMOCOUPLE----------------//

#define SCK 30
#define SO 31
#define CS 32

//---------------STEPPER MOTORS--------------//

//circular convayer
#define DIR 2
#define STEP 3
#define EN 4

//solid condiment
#define DIR_1 5
#define STEP_1 6
#define EN_1 7

//-----------------DC MOTORS----------------//

//vertical stacker
#define IN1 9
#define IN2 10

//fluff condiment
#define IN3 11
#define IN4 12

//------------------RELAYS------------------//

#define R1 33   // for airfryer 
#define R2 34   // for cooling fan

//------------------DELAYS------------------//

//in ms
int cooking_time=120000;
int cooling_time= 7000; 

//---------------INPUT FLAGS----------------//

char s, l, f;   // s-solid, l-liquid, f-fluff

//------------------------------------------//
String alertMsg="";
//------------------------------------------//

///////// FUNCTIONS ////////////


void move_convayor(int s, int t){
  digitalWrite(EN,HIGH);
  Serial.write("MO1_1");
  for(int i=0; i < s; i++){
    digitalWrite(STEP, HIGH);
    delayMicroseconds(t);
    digitalWrite(STEP, LOW);
    delayMicroseconds(t);
  }
  digitalWrite(EN,LOW);
  Serial.write("MO1_0");
}

void dispense_fluff(){
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW);
   Serial.write("MO3_1");
   delay(5000);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, LOW);
   Serial.write("MO3_0");
   delay(1000);
}

void dispense_liq(){
  Serial.write("MO4_1");
  //add code
  Serial.write("MO4_0");
}

void dispense_solid(){
  digitalWrite(EN_1, LOW);
  Serial.write("MO5_1");
  for(int i = 0; i < 40; i++){
    digitalWrite(STEP_1, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP_1, LOW);
    delayMicroseconds(2000);
  }
  digitalWrite(EN_1, HIGH);
  Serial.write("MO5_0");
  delay(1000);
}


float thermocouple(){
  int v = 0;
  float temp;
  v = spiRead();
  if (v == -1) {
    alertMsg= "thermocoulpe not connected";
  }
  else {
    temp = v * 0.25;
  }
  delay(10);
  return temp;
}


int spiRead() {

  int rawTemp = 0;
  int i;
  
  digitalWrite(CS,LOW);
  delay(2);
  digitalWrite(CS,HIGH);
  delay(200);
  
  digitalWrite(CS,LOW); //Pull CS low to start reading
  
  // one clock cycle to discard dummy bit 15
  digitalWrite(SCK,HIGH); 
  delay(1);
  digitalWrite(SCK,LOW);
  
  //Read bits 14-0 
  for (i=14; i>=0; i--) {
  digitalWrite(SCK,HIGH);
  rawTemp = rawTemp + (digitalRead(SO) << i);
  digitalWrite(SCK,LOW);
  }
  
  //if bit D2 HIGH no sensor
  if ((rawTemp & 0x04) == 0x04) return -1;
  
  // shift right three places to get rid of b0,b1 and b2
  return rawTemp >> 3;
}

void heating(){
  float temp;
  int t1,t2;
  digitalWrite(R2, LOW); // turn ON airfrier
  Serial.write("RL2_1");
  t1= millis();
  do{
    t2=millis();
    temp= thermocouple();
    if (temp>200){
      digitalWrite(R2, HIGH); // turn OFF airfrier
       Serial.write("RL2_0");
    }
    if (temp<180){
      digitalWrite(R2, LOW); // turn ON airfrier
       Serial.write("RL2_1");
    }
  }while((t2-t1)< cooking_time);  

  digitalWrite(R2, HIGH); // turn OFF airfrier
  Serial.write("RL2_0");
}

void BIST (){
  unsigned int x=0;
  float AcsValue=0.0,Samples=0.0,avg=0.0,current=0.0;
  int comp_val;
  analogWrite(STEP, 0);
  for (int i = 0; i < 1000; x++){ //Get 1000 samples
  AcsValue = analogRead(A0);     //Read current sensor values   
  Samples = Samples + AcsValue;  //Add samples together
  delay (3); // let ADC settle before next sample 3ms
  }
  
  analogWrite(STEP, 0);
  
  avg=Samples/100; //Taking Average of Samples
  current = (2.5 - (avg * (5.0 / 1024.0)) )/0.185;   
  if (current < comp_val){
    Serial.print("Stepper motor not Motor not working");
    }
  
  }

/////////////////////////

void setup() { 
  
  pinMode(IR_1, INPUT_PULLUP);
  pinMode(IR_2, INPUT_PULLUP);
  pinMode(IR_3, INPUT_PULLUP);
  pinMode(IR_4, INPUT_PULLUP);
  pinMode(IR_5, INPUT_PULLUP);
  pinMode(IR_6, INPUT_PULLUP);
  pinMode(IR_7, INPUT_PULLUP);

  pinMode(CS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SO, INPUT);

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(EN, OUTPUT);

  pinMode(DIR_1, OUTPUT);
  pinMode(STEP_1, OUTPUT);
  pinMode(EN_1, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  
  Serial.begin(9600);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  
  digitalWrite(DIR,HIGH);
  digitalWrite(DIR_1,HIGH);

  digitalWrite(CS, HIGH);
  digitalWrite(SCK, LOW);
 
}

void loop(){
  
  //Re-initiaize varibles
  s,l,f='0';

  // Read input form serial buffer
  /*The serial input must be a 3 bit binary value denoting the selected condiments */
  Serial.println("Waiting for input...");
  while(!Serial.available())
  delay(1);
  f= Serial.read();
  delay(1);
  l= Serial.read();
  delay(1);
  s= Serial.read();
  /*Serial.println(s);
  Serial.println(l);
  Serial.println(f);*/

  if (((f=='1'|| f=='0')&&(l=='1'|| l=='0')&& (s=='1'|| s=='0'))== false){
   /*Serial.println("invalid input");*/
   return;
   }
   delay (2000);
 
  //VERTICAL STACKER
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  Serial.write("MO2_1");
  do{
  delay (1); //wait for waffle to fall
  }while (digitalRead(IR_1) == 1 );
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  Serial.write("MO2_0");
 /* Serial.println("Waffle detected on convayer");*/
  delay(2000);

   
  //HEATING AREA
  move_convayor(40,30000); 
  digitalWrite(R1, LOW);
  Serial.write("RL_1");
  heating();
  delay (cooking_time);
  digitalWrite(R1, HIGH);
  Serial.write("RL_0");

  
  //COOLING AREA
  digitalWrite(R2, LOW);
  Serial.write("RL2_1");
  move_convayor(30,30000);
  delay (cooling_time);
  digitalWrite(R2, HIGH);
  Serial.write("RL2_0");

  //CONDIMENT 1 - Fluff
  do{
    digitalWrite(STEP, HIGH);
    delayMicroseconds(30000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(30000);
  } while (digitalRead(IR_4) == 0);
  if (f==1){  
     move_convayor(5, 30000);
     dispense_fluff();
  }

  //CONDIMENT 2 - Liquid
  do{
    digitalWrite(STEP, HIGH);
    delayMicroseconds(30000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(30000);
    } while (digitalRead(IR_5) == 0);
  if (l==l){  
     move_convayor(5, 30000);
     dispense_fluff();
  }

  //CONDIMENT 3 - Solid
  do{
    digitalWrite(STEP, HIGH);
    delayMicroseconds(30000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(30000);
    } while (digitalRead(IR_6) == 0);
  if (s==1){  
     move_convayor(5, 30000);
     dispense_fluff();
  }

  //exit
  //move_convayor(50);
}
