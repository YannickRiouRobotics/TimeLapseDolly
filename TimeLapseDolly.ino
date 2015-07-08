
#include <avr/sleep.h>

// Motors Pinout
#define Phase1 9  // D9 ORANGE
#define Phase2 10// D10 ROUGE
#define Phase3 11// D11 BLEU 
#define Phase4 12 // D12 JAUNE

#define Focus A0
#define Shutter A1

#define LEDR 5
#define LEDV 6

void MakeAStep();
void InitDolly();
void Configuration();
void End();
void Quitting();

enum StateMachine {SConfiguration, SRunning, SEnd, SQuitting};

StateMachine s;
int time = 1, flag = 0, i = 0;
boolean loopconfig = true;

void setup()
{
  InitDolly();
}


void loop()
{


  switch(s)
  {
    
    case SConfiguration:
    Configuration();
    break;
    
    
    case SRunning:
    TakePhoto();
    delay(10);
    MakeAStep();
    digitalWrite(LEDV,!digitalRead(LEDV));
    break;
    
    case SEnd: 
    End();
    break;
    
    case SQuitting:
    Quitting();
    break;
    
  }
  
  
}



void MakeAStep()
{
   digitalWrite(Phase1,HIGH);
   digitalWrite(Phase2,LOW); 
   digitalWrite(Phase3,LOW); 
   digitalWrite(Phase4,LOW);
   Serial.println("1"); 
   //delay(500);
   delay(40);
   
   digitalWrite(Phase2,HIGH);
   digitalWrite(Phase1,LOW); 
   digitalWrite(Phase3,LOW); 
   digitalWrite(Phase4,LOW);
  Serial.println("2");  
   //delay(500);
   delay(40);
   
   digitalWrite(Phase3,HIGH);
   digitalWrite(Phase2,LOW); 
   digitalWrite(Phase1,LOW); 
   digitalWrite(Phase4,LOW); 
   Serial.println("3"); 
   //delay(500);
   delay(40);
   
   digitalWrite(Phase4,HIGH);
   digitalWrite(Phase2,LOW); 
   digitalWrite(Phase3,LOW); 
   digitalWrite(Phase1,LOW); 
   Serial.println("4"); 
 
   delay(40);
}

void InitDolly()
{
  Serial.begin(115200); // Begin Bluetooth Connection
  
  Serial.println("TimelapseDolly v2.0");
  Serial.println("Initialization...");
  pinMode(Phase1,OUTPUT); // Initialize the direction of the pins
  pinMode(Phase2,OUTPUT);
  pinMode(Phase3,OUTPUT);
  pinMode(Phase4,OUTPUT);

  pinMode(LEDV,OUTPUT);
  pinMode(LEDR,OUTPUT);
  
  pinMode(Focus,OUTPUT);
  pinMode(Shutter,OUTPUT);
  
 
  attachInterrupt(1, SwitchButton, CHANGE);

  attachInterrupt(0, EndSwitch, CHANGE); // Attach interrupt on end switch on pin D2
    
  
  Serial.println("Ready for configuration");
  
  s = SConfiguration;
}

void SwitchButton()
{
  if(flag == 0)
  {
   
  if(time <3)
  {
    time++;
  }
  else 
  {
    time= 0;
  }
  flag = 1;
  Serial.println(time);
  }
  
}


void EndSwitch()
{
  if(s == SConfiguration)
  {
    loopconfig = false;
  }
  else if(s == SRunning)
  {
    s = SEnd;
  }
}

void TakePhoto()
{
   digitalWrite(Focus, HIGH); 
   delay(400);
   digitalWrite(Shutter,HIGH);
   digitalWrite(Focus, LOW);
   delay(time*1000);
}

void Configuration()
{
  
  while(loopconfig)
  {
    if(flag == 1)
    {
      delay(400);
      flag = 0;
    }
    switch(time)
    {
     case 0:
     analogWrite(LEDR,0);
     analogWrite(LEDV,0);
     break;
   
   
     case 1: 
     analogWrite(LEDR,50);
     analogWrite(LEDV,0);
     break; 

    case 2: 
     analogWrite(LEDR,0);
     analogWrite(LEDV,50);
     break; 
     
     case 3: 
     analogWrite(LEDR,50);
     analogWrite(LEDV,50);
     break; 
    }
    
    
  }
  delay(500);
  s = SRunning;
 
 
  
}

void End()
{
   for(i=0;i<3;i++)
   {
     analogWrite(LEDR,50);
     analogWrite(LEDV,50);
     delay(500);
     analogWrite(LEDR,0);
     analogWrite(LEDV,0);
     delay(500);
   }
   
   s = SQuitting;
     
}


void Quitting()
{
    digitalWrite(LEDR,LOW);
    digitalWrite(LEDV,LOW);
    digitalWrite(Phase1,LOW);
    digitalWrite(Phase2,LOW);
    digitalWrite(Phase3,LOW);
    digitalWrite(Phase4,LOW);
  
  
 
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    while(1);
  
}
 
 
 
