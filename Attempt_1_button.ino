/*
  Debounce

  Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
  press), the output pin is toggled from LOW to HIGH or HIGH to LOW. There's a
  minimum delay between toggles to debounce the circuit (i.e. to ignore noise).

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached from pin 2 to +5V
  - 10 kilohm resistor attached from pin 2 to ground

  - Note: On most Arduino boards, there is already an LED on the board connected
    to pin 13, so you don't need any extra components for this example.

  created 21 Nov 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Limor Fried
  modified 28 Dec 2012
  by Mike Walters
  modified 30 Aug 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Debounce
*/

// constants won't change. They're used here to set pin numbers:
const int PlusButtonPin = 2;    // the number of the pushbutton pin for Plus
const int MinusButtonPin = 8;    // the number of the pushbutton pin for Minus
const int GreenLedPin = 13;      // the number of the Green LED pin
const int YellowLedPin = 7;      // the number of the Green LED pin
const int RedLedPin = 4;      // the number of the Green LED pin
const int trigPinClose = 5;      //  Motion sensor pins closest to arduino
const int echoPinClose = 6;      //  Motion sensor pins closest to arduino

const int trigPinFar = 10;      //  Motion sensor pins
const int echoPinFar = 11;      //  Motion sensor pins

// Variables will change:
int GreenLedState = HIGH;         // the current state of the output pin
int YellowLedState = LOW;         // the current state of the output pin
int RedLedState = LOW;         // the current state of the output pin

long durationClose;
int distanceClose;

long durationFar;
int distanceFar;

int PlusButtonState;             // the current reading from the input pin
int lastPlusButtonState = LOW;   // the previous reading from the input pin
int MinusButtonState;             // the current reading from the input pin
int lastMinusButtonState = LOW;   // the previous reading from the input pin
int count = 0;               // keeps track of the number of times the button has been pressed
int lastSensorStateClose = LOW;
int lastSensorStateFar = LOW;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(PlusButtonPin, INPUT);
  pinMode(MinusButtonPin, INPUT);
  
  pinMode(GreenLedPin, OUTPUT);
  pinMode(YellowLedPin, OUTPUT);
  pinMode(RedLedPin, OUTPUT);
  // set initial LED state
  digitalWrite(GreenLedPin, GreenLedState);

  pinMode(trigPinClose, OUTPUT); // Sets the trigPin closest to arduino as an Output
  pinMode(echoPinClose, INPUT); // Sets the echoPin closest to arduino as an Input
  Serial.begin(9600); // Starts the serial communication for sensor closest to arduino

  pinMode(trigPinFar, OUTPUT); // Sets the trigPin far away from arduino as an Output 
  pinMode(echoPinFar, INPUT); // Sets the echoPin far away from arduino as an Input
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  // read the state of the switch into a local variable:
  int readingPlus = digitalRead(PlusButtonPin);
  int readingMinus = digitalRead(MinusButtonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingPlus != lastPlusButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  
  if (readingMinus != lastMinusButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingPlus != PlusButtonState) {
      PlusButtonState = readingPlus;

      // only toggle the LED if the new button state is HIGH
      if (PlusButtonState == HIGH) {
        count++;  
      }

      
    }
    
    if (readingMinus != MinusButtonState) {
       MinusButtonState = readingMinus;
      if (MinusButtonState == HIGH) {
        count--;  
      }
      
    }
    if (count < 0) {
       count = 0;
    }   
  }

  //Light Control
    //handles the lotopen case
    if (count < 3) {
         GreenLedState = HIGH;
         YellowLedState = LOW; 
         RedLedState = LOW;     
     }

     //lot is close to empty
     else if (count > 2 and count < 6) {
       GreenLedState = LOW;
       YellowLedState = HIGH; 
       RedLedState = LOW; 
     }
     //lot is empty
     else {
       GreenLedState = LOW;
       YellowLedState = LOW; 
       RedLedState = HIGH; 
     }
  
  // set the LED:
  digitalWrite(GreenLedPin, GreenLedState);
  digitalWrite(YellowLedPin, YellowLedState);
  digitalWrite(RedLedPin, RedLedState);
  
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastPlusButtonState = readingPlus;
  lastMinusButtonState = readingMinus;

//Handles the Close Sensor
  // Clears the trigPin
  digitalWrite(trigPinClose, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinClose, HIGH);
  delayMicroseconds(1);
  digitalWrite(trigPinClose, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationClose = pulseIn(echoPinClose, HIGH);
  // Calculating the distance
  distanceClose = durationClose*0.034/2;
  // Prints the distance on the Serial Monitor


 //Handles the Far Sensor
  // Clears the trigPin for the far sensor
  digitalWrite(trigPinFar, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinFar, HIGH);
  delayMicroseconds(1);
  digitalWrite(trigPinFar, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationFar = pulseIn(echoPinFar, HIGH);
  // Calculating the distance
  distanceFar = durationFar*0.034/2;
  // Prints the distance on the Serial Monitor

  //CLOSE sensor tests
//  if (distanceClose < 10 and lastSensorStateClose == LOW) {
//    
//    lastSensorStateClose = HIGH;
//    count++;
//  }
//  
//  if (distanceClose > 10 ){//and lastSensorStateClose == LOW) {
//    lastSensorStateClose = LOW;
//  }
//  
////FAR Sensor tests
//   if (distanceFar < 10 and lastSensorStateFar == LOW) {
//    
//    lastSensorStateClose = HIGH;
//    count++;
//  }
//  
//  if (distanceFar > 10 ){//and lastSensorStateFar == LOW) {
//    lastSensorStateClose = LOW;
//  }
//  
  //Serial.print("Distance: ");
  //Serial.println(distance);
  
}
