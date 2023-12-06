#define ENABLE 13
#define DIRA 12
#define DHTTYPE DHT11 
#define DHTPIN A5

#include "DHT.h"
#include "timer.h"

DHT dht(DHTPIN, DHTTYPE);
enum LED {INIT, ON} gstate = INIT;
enum DISPLAY2 {INIT2, TWO} gstate2 = INIT2;

const int LED = A4; 
int i = 0; // tick 

const char gSegPins[] = {5,6,7,8,9, A0, A1};
const int digitPins[] ={2, 3, };


void fan(int temperature) {
 
  switch (gstate){
    case INIT:
    if(temperature > 50){
    gstate = ON;
    }
  break; 

  case ON:
  if(temperature < 50){
  gstate = INIT;
  }
  break;
  }


  switch(gstate){
case INIT:
digitalWrite(DIRA,HIGH); //one way
 analogWrite(12,0);
 digitalWrite(LED, LOW);
break; 

case ON:
digitalWrite(DIRA,HIGH); //one way
 analogWrite(12,200);
 digitalWrite(LED, HIGH);
TimerSet(300);

break; 
}
 }

void displayTemperature(int temp) {
  // Convert temperature to an integer
  int tempInt = int(temp);

  // Loop through each digit of the temperature and display
  for (int i = 0; i < 2; ++i) {
    int digit = tempInt % 10; // Get the rightmost digit

   
   
tempInt /= 10; // Move to the next digit

    switch (gstate2){
    case INIT2:
    gstate2 = TWO;
  break; 

  case TWO:
  gstate2 = INIT2;
  break;
  }
  switch(gstate2){
case INIT2:
digitalWrite(2, HIGH);
displayNumTo7Seg(digit, 3);

break; 

case TWO: 

digitalWrite(3, HIGH);
displayNumTo7Seg(digit, 2);
   
break;
  }

   delayMicroseconds(9000);
   // Delay for stability (adjust if needed)
  }
}


void displayNumTo7Seg(unsigned int targetNum, int digitPin) {


    // A map of integers to the respective values needed to display
    // a value on one 7 seg digit.
    unsigned char encodeInt[10] = {
        // 0     1     2     3     4     5     6     7     8     9  C
        0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67,
    };
    // Make sure the target digit is off while updating the segments iteratively
    digitalWrite(digitPin, HIGH);


    // Update the segments
    for (int k = 0; k < 7; ++k) {
        digitalWrite(gSegPins[k], encodeInt[targetNum] & (1 << k));
    }


    // Turn on the digit again
    digitalWrite(digitPin, LOW);
}

void setup() {
    TimerSet(1000);
  TimerOn();

   pinMode(LED,OUTPUT);
 pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);

   for (int i = 0; i < 3; ++i) {
    pinMode(digitPins[i], OUTPUT);
  }
    for (int i = 0; i < 7; ++i) {
    pinMode(gSegPins[i], OUTPUT);
  }
Serial.println(F("DHTxx test!"));
  Serial.begin(9600);
  dht.begin();
}

void loop() {

 int temperature = dht.readTemperature(true);
  Serial.println(temperature); 
  Serial.print(F("°F  "));
  displayTemperature(temperature);
  fan(temperature);
  TimerSet(2000);
  
}