const int LED_R = 3;
const int LED_G = 5;
const int LED_B = 6;
const int LED_R2 = 9;

char incomingChar = ' ';
int incomingByte = 0;
int count = 0;
char current_pixels[4];
char Accumulated[4096];
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
  while ((Serial.available() > 0) && (flag<=4096) && (!finished_transm))
  {
       incomingChar = Serial.read(); 
       Accumulated[flag++] = incomingChar;
       if (flag >= 4096)
       {
          digitalWrite(12,HIGH);
          finished = 1;
          break;
       }    
  }
  
  if ((finished) && (flag >= 4096))
  {
    for(int i=0; i<4096; ++i)
    {
      Serial.write(Accumulated[i]);
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
