int trigPin = 12;    // Trigger
int echoPin = 13;    // Echo
long duration, cm;

#define MR_Ctrl 2  //Motor A(Right) - Direction pin
#define MR_PWM 9   //Motor A(Right) - Speed pin
#define ML_Ctrl 4  //Motor B(Left) - Direction pin
#define ML_PWM 5   //Motor B(Left) - Speed pin


void setup() {
  Serial.begin (9600); 
  
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT); 

  pinMode(ML_Ctrl, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(MR_Ctrl, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
}

void loop() {  

  //CheckSensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // Convert time to distance
  cm = (duration/2) / 29.1;     
  delay(50);

  if (cm>=2 && cm<=15){
    //Turn right
    digitalWrite(ML_Ctrl,LOW);
    analogWrite(ML_PWM,255);
    digitalWrite(MR_Ctrl,HIGH);
    analogWrite(MR_PWM,255);
  } else{
    //Forward
    digitalWrite(ML_Ctrl,LOW);
    analogWrite(ML_PWM,255);  
    digitalWrite(MR_Ctrl,LOW);
    analogWrite(MR_PWM,245);  
  } 
}
