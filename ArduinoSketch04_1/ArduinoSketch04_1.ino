/*
 PowerSwitch Tail test
 language: Wiring/Arduino
*/
const int switchOne = 2; // the first PowerSwitch Tail control pin 
const int switchTwo = 3; // the second PowerSwitch Tail control pin

void setup() {
  pinMode(switchOne, OUTPUT); // Configure the pins 
  pinMode(switchTwo, OUTPUT);

  digitalWrite(switchOne, LOW); // Make sure they are off 
  digitalWrite(switchTwo, LOW);
}

void loop() {
  // turn on first power strip, turn off the second 
  digitalWrite(switchOne, HIGH);
  digitalWrite(switchTwo, LOW);
  delay(2000);

  // turn on second power strip, turn off the first
  digitalWrite(switchTwo, HIGH);
  digitalWrite(switchOne, LOW);
  delay(2000);
}
