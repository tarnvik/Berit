#include <Servo.h>
Servo myservo;  

int trigPinL = 18;    
int echoPinL = 19;   
int trigPinR = 10;    
int echoPinR = 11;
int trigPinF = 12;    
int echoPinF = 13;   

struct sensorValues
{
   long front;
   long left;
   long right;
};

#define MR_Ctrl 2  //Motor A(Right) - Direction pin
#define MR_PWM 9   //Motor A(Right) - Speed pin
#define ML_Ctrl 4  //Motor B(Left) - Direction pin   
#define ML_PWM 5   //Motor B(Left) - Speed pin

sensorValues queue [60];

long loops = 0;

void setup() {
  Serial.begin (9600);  
  
  pinMode(trigPinF, OUTPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(trigPinR, OUTPUT);

  pinMode(echoPinF, INPUT);
  pinMode(echoPinL, INPUT);
  pinMode(echoPinR, INPUT);
 
  pinMode(ML_Ctrl, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(MR_Ctrl, OUTPUT);
  pinMode(MR_PWM, OUTPUT);

  digitalWrite(trigPinF, LOW);
  digitalWrite(trigPinL, LOW);
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(100);

  myservo.attach(3); //Connect servo pin and servo obejct 
  myservo.write(90); //Turn servo to 90 degree position (Forward)
}

bool AmIMoving(){
  long max = -99999;
  long min = 99999;
  for(int i = 0; i <= 32; i++){
    if (queue[i].front > max){
      max = queue[i].front;
    }
    if (queue[i].front < min) {
      min = queue[i].front;
    }
  }
  if(abs(min-max)<5){
    Serial.print("vi står still");
    Serial.println();
    return false;
  }
  return true;
}

void loop() {
  sensorValues distance = Sensor();  
  Add(distance);

  //Check if standing still
  loops++;
  if(loops > 60 && !AmIMoving()){
    loops = 0;
    Backward();
    delay(200);
    Right();
    delay(50);
    Stop();
    return;
  }

  if (distance.front > 0 && distance.front < 4){
    Backward();
  } else if(distance.front > 28) {
    ForwardWithLeftRightAdjustment(distance);
  } else if (distance.right >= distance.front && distance.right > distance.left){
    Right();
  } else if (distance.left >= distance.front && distance.left > distance.right){
    Left();
  } else if (distance.front >= distance.left && distance.front >= distance.right){
    ForwardWithLeftRightAdjustment(distance);
  } else if (distance.left == distance.right){
    Left();
  } else {
    Backward();
  }

  //Console tests 
  
  //Serial.print(queue[0].front);
  //Serial.println();
  
  /*Serial.print("Front: ");
  Serial.print(distance.front);
  Serial.println();
  Serial.print("Left: ");
  Serial.print(distance.left);
  Serial.println();
  Serial.print("Right: ");
  Serial.print(distance.right);
  Serial.println();*/
}

sensorValues Sensor (){
  digitalWrite(trigPinF, LOW);
  digitalWrite(trigPinL, LOW);
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinF, HIGH);
  digitalWrite(trigPinL, HIGH);
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinF, LOW);
  long durationF = pulseIn(echoPinF, HIGH);
  digitalWrite(trigPinL, LOW);
  long durationL = pulseIn(echoPinL, HIGH);
  digitalWrite(trigPinR, LOW);
  long durationR = pulseIn(echoPinR, HIGH);
  
  long distanceF = (durationF/2) / 29.1;
  long distanceL = (durationL/2) / 29.1; 
  long distanceR = (durationR/2) / 29.1;     
  delay(50);
  sensorValues distance = {distanceF, distanceL, distanceR};
  return distance;
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

void ForwardWithLeftRightAdjustment (sensorValues distance){
  Forward();
  long distanceDiff = abs(distance.left - distance.right);
  if(distanceDiff > 5 && distance.right > distance.left){
    Right();
    delay(15);
  } else if (distanceDiff > 5 && distance.left > distance.right){
    Left();
    delay(15);
  }
  Forward();
}

void Add(sensorValues item){
  for(int i = 58; i >= 0; --i){
    queue[i+1] = queue[i];
  }
  queue[0] = item;
}

void Stop(){
  /*digitalWrite(ML_Ctrl,HIGH);
  analogWrite(ML_PWM,255);
  digitalWrite(MR_Ctrl,HIGH);
  analogWrite(MR_PWM,255);
  delay(50);*/ 
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
