#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_IR 0 // IR at Pin A0
#define PIN_SERVO 10
#define PIN_LED 9

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5 
float  dist, dist_raw;
float dist_prev=_DUTY_NEU;
float dist_ema=_DUTY_NEU;
Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  
  
  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;


  // Remove Next line !!!
  a_value = analogRead(PIN_IR);
  float dist = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;
  dist_raw=dist;
  // Read IR Sensor value !!!
  // Convert IR sensor value into distance !!!
  // we need distance range filter here !!!
  // we need EMA filter here !!!
  if (dist_raw<100){
    dist_raw = dist_prev;   
    digitalWrite(PIN_LED, 1); 
  } else if(dist_raw>250){
    dist_raw = dist_prev;   
    digitalWrite(PIN_LED, 1);     
  }else{
    digitalWrite(PIN_LED, 0);       // LED ON      
    dist_prev = dist_raw;
  }
  dist_ema = _EMA_ALPHA*dist_raw+(1-_EMA_ALPHA)*dist_ema;

  // map distance into duty
  // duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  duty=553 + (dist_ema-100)/150*1846;
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:"); Serial.print(_DUTY_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",dist:"); Serial.print(dist);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
  // Serial.print(" = ");
  // Serial.print((a_value / 1024.0) * 5.0);
  // Serial.print(" Volt => Duty : ");
  // Serial.print(duty);
  // Serial.println("usec");
}
