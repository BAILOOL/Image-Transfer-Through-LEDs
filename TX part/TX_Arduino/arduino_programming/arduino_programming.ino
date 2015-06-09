const int LED_R = 7;
const int LED_G = 8;
const int LED_B = 9;
const int LED_R2 = 10;

char incomingChar = ' ';
int incomingByte = 0;

const int SIZE = 4096;
char Accumulated[SIZE];
int flag = 0;
int finished = 0;
int finished_transm = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial){
    delay(500);
  }
  delay(2000);
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_R2, OUTPUT);
  
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
 
}

void loop() {
  while ((Serial.available() > 0) && (flag<=SIZE) && (!finished_transm))
  {
       incomingChar = Serial.read(); 
       Accumulated[flag++] = incomingChar;
       if (flag >= SIZE)
       {
          digitalWrite(12,HIGH);
          finished = 1;
          break;
       }    
  }
  
  if ((finished) && (flag >= SIZE))
  {
    int i = 0;
    while (i<=SIZE)
    {
      if (Accumulated[i] == '1') analogWrite(LED_R,255);
      else if (Accumulated[i] == '0') analogWrite(LED_R,6);
      else analogWrite(LED_R,0);
      
      if (Accumulated[i+1] == '1') analogWrite(LED_G,255);
      else if (Accumulated[i+1] == '0') analogWrite(LED_G,6);
      else analogWrite(LED_G,0);
      
      if (Accumulated[i+2] == '1') analogWrite(LED_B,255);
      else if (Accumulated[i+2] == '0') analogWrite(LED_B,6);
      else analogWrite(LED_B,0);
      
      if (Accumulated[i+3] == '1') analogWrite(LED_R2,255);
      else if (Accumulated[i+3] == '0') analogWrite(LED_R2,6); 
      else analogWrite(LED_R2,0);
      i += 4;
      
      delay(6);
      
      analogWrite(LED_R,0);
      analogWrite(LED_G,0);
      analogWrite(LED_B,0);
      analogWrite(LED_R2,0);
      delay(700);
      
    }
    
    digitalWrite(12,LOW);
    finished = 0;
    finished_transm = 1;
  }
  
  
  if (finished_transm)
  {
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);
  }
}
