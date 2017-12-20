const int buttonPin = 10;   
const int ledPin =  9;    // pin to write LED state
const int ledState = 8;   // pin to read LED state

int buttonState = 0;      // variable for reading the button status
int lastButtonState = 0;  // variable to remember last button state to detect rising edges
long t = millis();

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the button pin and LED state as inputs:
  pinMode(buttonPin, INPUT);
  pinMode(ledState, INPUT);
  Serial.begin(9600);
}

void loop() {
  if(millis()-t > 50){
    // read the state of the button:
    buttonState = digitalRead(buttonPin);
    
    // check if the button is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH && lastButtonState == LOW) {
      Serial.println("switch");
      if (digitalRead(ledState) == LOW){
        // turn LED on:
        digitalWrite(ledPin, HIGH);
      } else {
        // turn LED off:
        digitalWrite(ledPin, LOW);
      }
    }
    lastButtonState = digitalRead(buttonPin);
    t = millis();
  }
}
