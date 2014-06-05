#define BUTTON_GREEN   A0
#define BUTTON_YELLOW  A1
#define BUTTON_BLUE    A2
#define BUTTON_UP      6
#define BUTTON_DOWN    9

#define BUTTON BUTTON_YELLOW

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(BUTTON);
  Serial.println(buttonState);
  delay(100);
}



