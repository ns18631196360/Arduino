int ASignal1 = A0;
int signal2 = A1;

void setup() {
  // put your setup code here, to run once:
  pinMode(ASignal1, INPUT);
  pinMode(signal2, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(ASignal1));
  delay(5000);
  Serial.println(analogRead(signal2));
  delay(5000);
}
