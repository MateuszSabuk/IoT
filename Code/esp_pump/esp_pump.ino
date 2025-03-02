void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(5, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(8, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(10000);                      // wait for a second
  digitalWrite(5, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(8, LOW);   // turn the LED off by making the voltage LOW
  delay(10000);                      // wait for a second
}
