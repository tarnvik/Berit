#include <Servo.h>
Servo myservo;  

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

  digitalWrite(trigPin, LOW);
  delayMicroseconds(100);

  myservo.attach(3); //Connect servo pin and servo obejct 
  myservo.write(90); //Turn servo to 90 degree position (Forward)
}

void loop() {
  
  cm = Sensor();    

  if (cm > 0 && cm < 4){
    Backward();
  } else if (cm>=4 && cm<=12){
    Stop();

    myservo.write(0); //sensor turn right
    delay(250); //wait for sensor

    cm = Sensor();
  
    if (cm > 25){
      myservo.write(90); //sensor turn forward
      delay(250); //wait for sensor

      Right();
      delay(175);

      cm = Sensor();
    
      while(cm < 15){
        if (cm < 3) {
          Backward();
        } else {
          Right();
        }          
        cm = Sensor();          
      }
    } else{
    
      cm = Sensor();        
      myservo.write(90);
      delay(250);    
      cm = Sensor();

      Left();      
      delay(175);
    
      while(cm < 15){
        if (cm < 3){
          Backward();
        } else {
          Left();
        }
        cm = Sensor();           
      }
    }
  } else {    
    Forward();
  }
}

long Sensor (){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long cm = (duration/2) / 29.1;     
  delay(50);
  return cm;
}

void Left(){
  digitalWrite(ML_Ctrl,HIGH);
  analogWrite(ML_PWM,255);
  digitalWrite(MR_Ctrl,LOW);
  analogWrite(MR_PWM,255); 
}

void Right(){
  digitalWrite(ML_Ctrl,LOW);
  analogWrite(ML_PWM,255);
  digitalWrite(MR_Ctrl,HIGH);
  analogWrite(MR_PWM,255);       
}

void Forward(){
  digitalWrite(ML_Ctrl,LOW);
  analogWrite(ML_PWM,255);
  digitalWrite(MR_Ctrl,LOW);
  analogWrite(MR_PWM,255);       
}

void Stop(){
  digitalWrite(ML_Ctrl,HIGH);
  analogWrite(ML_PWM,255);
  digitalWrite(MR_Ctrl,HIGH);
  analogWrite(MR_PWM,255);
  delay(50); 
  digitalWrite(ML_Ctrl,LOW);
  analogWrite(ML_PWM,0);
  digitalWrite(MR_Ctrl,LOW);
  analogWrite(MR_PWM,0); 
}

void Backward(){
  digitalWrite(ML_Ctrl,HIGH);
  analogWrite(ML_PWM,255);
  digitalWrite(MR_Ctrl,HIGH);
  analogWrite(MR_PWM,255); 
}
