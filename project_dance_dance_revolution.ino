#define NOTE_A3 220

const int sensorPins[4] = {0, 1, 2, 3}; //↑ → ↓ ←
float pinValue[4];
float calibratedValue[4];
const int threshold[4] = {10, 10, 10, 10};

int noteDuration = 4;
const int pinBuzzer = 8;
int dances[][16] = {{1,1,2,3,4,1,2,2,3,1,4,4,2,1,3,1}, {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4}, {1,3,2,4,1,3,2,4,1,3,2,4,1,3,2,4}};

float calibrate(int pin)
{
   float value = 0;
   for(int i = 0; i < 20; i++)
   {
      value += analogRead(pin);
      delay(100);
   }
   value /= 20;
   return value;
}

void setup() 
{
  // put your setup code here, to run once:
   Serial.begin(9600);
   
   pinMode(0, INPUT); 
   pinMode(1, INPUT); 
   pinMode(2, INPUT); 
   pinMode(3, INPUT); 
   pinMode(8, OUTPUT); 
   Serial.println("Loading...");
   for(int i = 0; i < 4; i++)
   {
      calibratedValue[i] = calibrate(sensorPins[i]);
      //Serial.println(calibratedValue[i]);
   }
  
   Serial.println("Instructions:\n0 - exit\n1 - Jingle Bells\n2 - Snowman\n3 - Pepas\n");
} 

void loop() 
{
// put your main code here, to run repeatedly:
  Serial.println("Choose your dance:");
  delay(5000);
  int chosenDance;
  
  if (Serial.available() > 0) 
  {
     chosenDance = Serial.read();
     chosenDance = chosenDance - '0';
  }
  
  if(chosenDance == 0 || chosenDance == -38) 
  {
    Serial.print("Bye bye!");
    delay(500);
    exit(0);
  }
  else 
  {
    chosenDance--;
    Serial.println("Start!");
  }
  
  int sizeDance = sizeof(dances[chosenDance])/sizeof(dances[chosenDance][0]); 
  int score = 0;
  
  for (int i = 0; i < sizeDance; i++)
  {
     switch (dances[chosenDance][i])
     {
        case 1:
          Serial.println("↑");
          break;
        case 2:
          Serial.println("→"); 
        case 3:
          Serial.println("↓");
          break;
        case 4:
          Serial.println("←");
          break;  
        default:
          Serial.println("");  
          break;
     }
     delay(1000);
     
     int danceStep = dances[chosenDance][i] - 1;
     pinValue[danceStep] = analogRead(sensorPins[danceStep]);
     pinValue[danceStep] -= calibratedValue[danceStep];
     
     if (pinValue[danceStep] < threshold[danceStep])
     {
       noteDuration = 1000/noteDuration;
       tone(pinBuzzer, NOTE_A3, 500);
       score -= 100;
     }  
     else
     {
       score += 100;
     }
     delay(500);
  }
  Serial.println("Score: ");
  Serial.println(score);
  delay(2000); 
}
