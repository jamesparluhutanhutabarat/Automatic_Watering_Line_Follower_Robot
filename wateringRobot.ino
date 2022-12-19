// PIN DRIVER
#define in1 9
#define in2 8
#define in3 7
#define in4 6
#define enA 10
#define enB 5

// PIN ULTRASONIC
#define echoPin 12
#define trigPin 13
const int Relay = A5;

// VAR ULTRASONIC
long duration;
int distance;

// VAR KEC.MOBIL
int M1_Speed = 80; // speed of motor 1
int M2_Speed = 80; // speed of motor 2
int LeftRotationSpeed = 250;  // Left Rotation Speed
int RightRotationSpeed = 250; // Right Rotation Speed

int kering=0; // Kondisi Arduino
int now_h=0; // Kondisi ESP32

// MILIS
unsigned long previousMillis = millis();

// VARIABEL TASK
int initiate = 0;
int siram = 0;

// VARIABEL SLAVE
char dataSlave;
//int data; // Integer ESP32

 void setup() {
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(Relay , OUTPUT);
  
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);

  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);

  pinMode(A0, INPUT); // initialize Left sensor as an input
  pinMode(A1, INPUT); // initialize Right sensor as an input
}

void forward()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, M1_Speed);
  analogWrite(enB, M2_Speed);
}

void backward()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, M1_Speed);
  analogWrite(enB, M2_Speed);
}

void right()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, LeftRotationSpeed);
  analogWrite(enB, RightRotationSpeed);
}

void left()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, LeftRotationSpeed);
  analogWrite(enB, RightRotationSpeed);
}

void Stop()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
}

void jalan(){
  int LEFT_SENSOR = digitalRead(A0);
  int RIGHT_SENSOR = digitalRead(A1);
  if(RIGHT_SENSOR==1 && LEFT_SENSOR==1) {
    forward(); //FORWARD
   }else if(RIGHT_SENSOR==1 && LEFT_SENSOR==0) {
    right(); //Move Right
   }else if(RIGHT_SENSOR==0 && LEFT_SENSOR==1) {
    left(); //Move Left
   }else if(RIGHT_SENSOR==0 && LEFT_SENSOR==0) {
    Stop();  //STOP
 }
}

void watering()
{
  digitalWrite(Relay,HIGH);
  delay(3000);
  digitalWrite(Relay,LOW);
}

void baca_h(){
  if(Serial.available()){
    dataSlave = Serial.read();
    now_h = dataSlave - '0';
    Serial.println(now_h);
}
}

void loop() {
  unsigned long currentMillis = millis();
  
  //TEST
  //now_h = 1; 
  //siram = 0;   
  //initiate = 1;  
  
  // ULTRASONIC TRIG
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  // ULTRASONIC ECHO
  duration=pulseIn(echoPin,HIGH);
  distance=(duration*0.034/2);
  
  baca_h(); //now_h=1 ADALAH KERING

  // FUNGSI JALAN
  jalan();

// TASK JALAN KARENA TRIGGER HUMIDITY
  if((now_h==1) && (initiate==1) && (siram==0) && (currentMillis - previousMillis >= 1000))//Maju sedikit 1s
  {
   forward();
   initiate=0; 
   siram=1; 
   previousMillis = currentMillis;
  }

  // TASK SIRAM
  if((5<=distance) && (distance<=8) && (now_h==1)&& (initiate==0) &&  (siram==1))// Siram 2s
  {
   Stop();
   watering();
   baca_h();
   if (now_h==0){
    siram=0;
   }
  }
  
// TEST
//  now_h = 0; 
//  delay(1000);
}
