#define IR_1 8
#define IR_2 9

#define EN 2
#define STEP 3

#define R1 4
#define R2 5

#define TOPP 10

//------------------DELAYS------------------//

int cooking_time=10000;
int cooling_time=3000;

//------------------FLAGS-------------------//
char t;
//------------------------------------------//


////////FUNCTIONS////////

void move_convayor(int s){
  
  digitalWrite(EN,LOW);
  for(int i=0; i < s; i++){
    digitalWrite(STEP, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP, LOW);
  }
  digitalWrite(EN,HIGH);
}

void dispense_topping(){
  digitalWrite(TOPP, HIGH);
  delay(5000);
  digitalWrite(TOPP, LOW);
  }

//------------------------------------------//

void setup() { 
  
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  
  pinMode(STEP, OUTPUT);
  pinMode(EN, OUTPUT);

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  
  pinMode(TOPP, OUTPUT);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  
  Serial.begin(9600);
}

void loop(){
    
  t='0';
  
  while(!Serial.available())
  delay(1);
  t= Serial.read();
  //Serial.println(t);
  if ((t=='1'|| t=='0')== false){
   Serial.println("invalid input");
   return;
  }
  
  delay(1000);
  //VERTICAL STACKER
  while(digitalRead(IR_1) == 0){}
  Serial.println("Your Waffle is on the convayor");
  delay(2000);
   
  //HEATING AREA
  move_convayor(750); 
  digitalWrite(R1, LOW);
  Serial.println("We are cooking your waffle");
  delay (cooking_time);
  digitalWrite(R1, HIGH);

  //COOLING AREA
  move_convayor(750);
  digitalWrite(R2, LOW);
  Serial.println("Your waffle is cooling");
  delay (cooling_time);
  digitalWrite(R2, HIGH);

  //CONDIMENT
  Serial.println("Adding your favorite toppings");
  digitalWrite(EN,LOW);
  analogWrite(STEP, 128);
  while (digitalRead(IR_2) == 0){}
  analogWrite(STEP, 0);
  digitalWrite(EN,HIGH);

  delay (1000);
  if (t== '1'){  
     delay (100);
     move_convayor(250);
     dispense_topping();
     delay(2000);
  }
  
  //DISPENSE
  move_convayor(1500);
  Serial.println("Your order is ready!");
  delay(3000);
  Serial.println("Enjoy :)");
  delay(5000);
  Serial.println("end");
  
}
