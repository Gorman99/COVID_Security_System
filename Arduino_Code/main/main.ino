#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <Servo.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 8;
const float normal_temp = 37.2;
const int relay_pin = 7;
int sensor = 2;  //PIR data pin 
long duration;
int distance;
int dis;
int val = 0;
int inByte = '1';

void setup() {   
    lcd.init();
    lcd.clear();         
    lcd.backlight(); 
    
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
    pinMode(buzzer, OUTPUT);
    pinMode(relay_pin, OUTPUT);
    pinMode(sensor, INPUT); 
    myservo.attach(9); 
    Serial.begin(9600);
    if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
      };
      lcd.setCursor(0,0);   
      lcd.print("System starting");  
      myservo.write(10);
      delay(1000);     
      lcd.clear(); 
  // put your setup code here, to run once:

}

void loop() {
   if (Serial.available() > 0) 
   {
      Serial.println("run"); 
      lcd.clear(); 
      lcd.backlight(); 
      lcd.setCursor(0,0);
      tone(buzzer, 1000);  
      delay(100); 
      noTone(buzzer); 
      lcd.print("Welcome to AIT");
      dis = measureDistance();     
      if(dis > 5){
        //check noise mask         
         lcd.clear(); 
         lcd.setCursor(0,0); 
         lcd.print("Please face the");   
         lcd.setCursor(0,1); 
         lcd.print("Camera");         
         delay(1000);
         Serial.println("start"); 
         inByte = (Serial.read());         
         if( inByte == '9'){
            lcd.clear(); 
            lcd.setCursor(0,0); 
            lcd.print("Please face temp");
            delay(1000);
            lcd.setCursor(0,1);  
            lcd.print("sensor ..");
            delay(1000);
            lcd.clear(); 
            lcd.setCursor(0,0); 
            lcd.print("Please face temp");
            float  ambient_Temp = mlx.readAmbientTempC();
            float temp = mlx.readObjectTempC();
            for (int i = 0; i <= 10; i++) {
              lcd.clear(); 
              lcd.setCursor(0,0); 
              lcd.print("Measuring ....");
              Serial.println("Measuring.....");
              delay(100);
              }
              if( ambient_Temp > temp){
                  lcd.clear(); 
                  lcd.setCursor(0,0); 
                  lcd.print("Face the sensor");
                  lcd.setCursor(0,1);
                  lcd.print("to measure again");
                  delay(200);
                  for (int i = 0; i <= 10; i++) {
                    lcd.clear(); 
                    lcd.setCursor(0,0); 
                    lcd.print("Measuring ....");
                    delay(100);
                  }
                  ambient_Temp = mlx.readAmbientTempC();
                  temp = mlx.readObjectTempC();
              }                     
          lcd.clear(); 
          lcd.setCursor(0,0); 
          lcd.print("Your Tempature");
          Serial.println("Your Tempature");
          lcd.setCursor(0,10); 
          lcd.print(temp);
          delay(1000);
          if(temp > normal_temp){
              tone(buzzer, 1000);
              lcd.clear(); 
              lcd.setCursor(0,0); 
              lcd.print("Temperature High");
              Serial.println("Temperature High");
              lcd.setCursor(0,1);
              lcd.print("Contact Doctor");                 
              delay(1000);
              noTone(buzzer); 
         }
          lcd.clear(); 
          lcd.setCursor(0,0); 
          lcd.print("Please wash");
          Serial.println("Please wash");
          lcd.setCursor(0,1); 
          lcd.print("your hands");
          delay(1000);
          val = digitalRead(sensor);   
          if (val == HIGH) {           
                lcd.clear(); 
                lcd.setCursor(0,0); 
                lcd.print("Be Alert for ");
                lcd.setCursor(0,1); 
                lcd.print("the sanitizer");
                delay(1000);
              }
          myservo.write(180);              
          delay(1500);
      } 
        else{
            tone(buzzer, 1000);
              lcd.clear(); 
              lcd.setCursor(0,0); 
              lcd.print("Wear Mask");
              Serial.println("Wear Mask");
              delay(1000);
              noTone(buzzer); 
        }
    }                 
      delay(1000);
      lcd.clear();
       Serial.println("end");
  
  }

}


int measureDistance (){  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH); 
  distance = duration * 0.034 / 2;
  Serial.println(distance);
  return distance;
}
