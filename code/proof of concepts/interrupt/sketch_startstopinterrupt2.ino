#define LED_PIN 9
#define LED_PIN2 10
#define BUTTON_PIN 3
volatile byte ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blinkLed, RISING);
}

void loop() {
  digitalWrite(LED_PIN2, true);
}

void blinkLed() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}
