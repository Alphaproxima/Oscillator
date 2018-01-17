#include <MsTimer2.h>
#include <TimerOne.h>

bool flagPWM = false, flagLED = false;
long count = 0;
int led = 5;

void test() {
  flagPWM = !flagPWM;
  if (flagLED) {
    if (flagPWM)
      digitalWrite(led, HIGH);
    else
      digitalWrite(led, LOW);
  }
  else
    digitalWrite(led, LOW);

  /*For the test: 1s pikapika.*/
  if (count++ > 100000) {
    count = 0;
    flagLED = !flagLED;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  // MsTimer2::set(20,test);
  // MsTimer2::start();
  Timer1.initialize(64);
  Timer1.attachInterrupt(test);
  Timer1.start();
}

void loop() {
  int a = analogRead(A3);
  int b = analogRead(A2);
//  int c = analogRead(A1);

String temp = "{\"x\":\"";
    temp += a;
    temp +="\",\"y\":\"";
    temp += b;
    temp +="\"}";

	Serial.println(temp);
	
}
