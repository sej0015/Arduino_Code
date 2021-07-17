void setup() {
  // put your setup code here, to run once:
  pinMode(30,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(30, HIGH);
  delay(300);
  digitalWrite(30, LOW);
  delay(300);
}
