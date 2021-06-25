#define CS 50 //chip select
#define SCK 51 //clock
#define SO 52 // Slave out


//define variables
int v = 0;
float temp;

void setup() {
  pinMode(CS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SO, INPUT);
    
  Serial.begin(9600);
    
  digitalWrite(CS, HIGH);
  digitalWrite(SCK, LOW);
}

void loop() {
  v = spiRead();
  if (v == -1) {
    Serial.println("thermocoulpe not connected");
  }
  else {
    temp = v * 0.25;
    Serial.println(temp);
  }
  delay(500);
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
