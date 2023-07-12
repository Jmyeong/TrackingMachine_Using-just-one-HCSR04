#include <Servo.h>
#include <NewPing.h>

Servo servo;
NewPing sonar(10, 11, 200);

int present = 90, n=0;
int Det_angle_L,Det_angle_R;
const int trig = 10;
const int echo = 11;
const int threshold = 50;
long duration;
float distance;
int flag = 0;
float check_L, check_R;

char dis_print[10] = "";
char detect[10] = "User detected!";

void setup() {
  servo.attach(9);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  servo.write(90);
}

void loop() {
    distance = sonar.ping_cm();
    Serial.print("present motor angle : ");
    Serial.print(present);
    Serial.print("  Detecting ");
    Serial.println(distance);
    
    for(;(distance > threshold) & (flag == 0); ){ //Find User
      // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ Detecting Left side ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      Det_angle_L = present - n;
      if((present - n) < 0) Det_angle_L = 0;
      for(int i = present;i>Det_angle_L;i--){
        servo.write(i); //Left side detection based on 90 degrees  (expanding scope gradually)
        delay(10);
      }
      delay(10);
      check_L = sonar.ping_cm();
      Serial.print("Checking...  ");
      Serial.println(check_L);
      
      //Escape the loop if user detection
      if(check_L < 50){
        flag = 1;
        present = Det_angle_L;
        Serial.print("Checked in ");
        Serial.println(check_L);
        break;
      }

      // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡDetecting Right side ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
      Det_angle_R = present + n;
      if((present + n) > 180) Det_angle_R = 180;
      //Right side detection based on 90 degrees  (expanding scope gradually)
      for(int i = present;i<Det_angle_R;i++){
        servo.write(i); 
        delay(10);
      }
      
      delay(10);
      //duration = pulseIn(echo,HIGH);
      check_R = sonar.ping_cm();
      Serial.print("Checking...  ");
      Serial.println(check_R);
      
      //Escape the loop if user detection
      if(check_R < 50){ 
        flag = 1;
        present = Det_angle_R;
        Serial.print("Checked in ");
        Serial.println(check_R);
        break;
      }
      n += 10;
    }
    //User detection checked
    if(flag){
      n = 0;
      Serial.print(detect); 
      Serial.print("Present Motor angle : "); 
      Serial.println(present); 
      flag = 0; 
      delay(10);
    }
}
