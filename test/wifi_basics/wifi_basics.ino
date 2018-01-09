void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
}

void loop() {
  if (Serial1.available()) {
    byte b = Serial1.read();
    Serial.write(b);
  }

  if (Serial.available()) {
    byte c = Serial.read();
    Serial1.write(c);
  }
}

