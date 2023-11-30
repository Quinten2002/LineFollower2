const byte ledPin = 13;
const byte buttonPin = 2;
 
 
volatile bool toggleState = false;
 
void checkSwitch() {
  // Check status of switch
  // Toggle LED if button pressed
 
  if (digitalRead(buttonPin) == LOW) {
    // Switch was pressed
    // Change state of toggle
    toggleState = !toggleState;
    // Indicate state on LED
    digitalWrite(ledPin, toggleState);
  }
}
 
void setup() {
  
  pinMode(ledPin, OUTPUT);
  
  pinMode(buttonPin, INPUT_PULLUP);
 
  
  attachInterrupt(digitalPinToInterrupt(buttonPin),checkSwitch, FALLING); 
}
 
void loop() {
  
  
  Serial.println("Delay Started");
  delay(5000);
  Serial.println("Delay Finished");
  Serial.println("..............");
}
