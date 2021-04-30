// This #include statement was automatically added by the Particle IDE.
#include <LiquidCrystal.h>

// This #include statement was automatically added by the Particle IDE.
#include <Keypad_Particle.h>

int reelSwitch = 13;
int switchState;

LiquidCrystal lcd(5, 4, 3, 2, 1, 0);

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {12,11,10,9}; //row pins
byte colPins[COLS] = {8,7,6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int codeLength = 4;
char secretCode[codeLength] = {'0', '8', '5','3'};


int correctCount = 0;
boolean correctCode = false;
boolean armed = false;
bool success;
int deviceCheck;

int notify = 0;


void myHandler(const char *event, const char *data){
    
     armed = true;
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Armed");
    
}

void setup() {
  
    Serial.begin(9600);
    
    lcd.begin(16, 2);
      
    pinMode (reelSwitch, INPUT);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Disarmed");

  
    Particle.subscribe("Arming Device", myHandler);
    
    Particle.variable("deviceCheck", deviceCheck);
    Particle.variable("notify", notify);
}


void loop() {
    
    deviceCheck = armed;
    
    switchState = digitalRead(reelSwitch); // read the value of digital interface 2 and assign it to switchState
  
 
    if(armed){
        if(switchState == HIGH){
            
            notify++;
            
            if(notify == 1){
            
             bool notify = Particle.publish("deviceTripped", "true", 10);
            }
            
            if(notify == 120){
            
             bool notify2 = Particle.publish("potentialBreak", "true", 10);
            }
            
            checkCode();
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Please Enter");
            lcd.setCursor(0,1);
            lcd.print("Your Code");

             if (correctCode) {
                 lcd.clear();
                 //lcd.setCursor(0,0);
                 lcd.print("Disarmed");
                 bool success = Particle.publish("Disarm-Alarm", "true", 60);
                 correctCode = false;
                 armed = false;
                 correctCount = 0;
                 notify = 0;
                 
            }
        }
    }
}

void checkCode() {
    char key = keypad.getKey();
    
    if (key) { 
        
        if (key == secretCode[correctCount]) {
            correctCount++; 
        } else {
            correctCount = 0; 
        }
    }
    if (correctCount == codeLength) {
        correctCode = true;
    }
    delay(100);
}
