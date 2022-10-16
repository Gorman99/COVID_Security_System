#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <Servo.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int buzzer = 8;
const float normal_temp = 37.2;
const int relay_pin = 3;
int sensor = 2;  //PIR data pin
int val = 0;
long measured_Distance;
int inByte = '1';

void setup() {   
    lcd.init();
    lcd.clear();         
    lcd.backlight();   
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
      Serial.println("run"); 
      lcd.clear(); 
      lcd.backlight(); 
      lcd.setCursor(0,0);
      tone(buzzer, 1000);  
      delay(100); 
      noTone(buzzer); 
      lcd.print("Welcome to AIT");
      delay(2000);
      measured_Distance = measureDistance();      
      if(measured_Distance < 20){
        int lock = 0;
        clearLCD();
        while(lock != 1) {
            delay(1000);
            clearLCD();
            lcd.print("Please face the");   
            lcd.setCursor(0,1); 
            lcd.print("Camera");         
            delay(1000);
            Serial.println("start"); 
            inByte = (Serial.read());    
            if( inByte == '9'){
                lcd.clear(); 
                lcd.print("Please face temp");
                delay(1000);
                lcd.setCursor(0,1);  
                lcd.print("sensor ..");
                delay(1000);
                lcd.clear(); 
                lcd.print("Please face temp");
                float  ambient_Temp = mlx.readAmbientTempC();
                float temp = mlx.readObjectTempC();
                for (int i = 0; i <= 10; i++){
                    clearLCD();
                    lcd.print("Measuring ....");
                    Serial.println("Measuring.....");
                    delay(100);                            
                }
                if( ambient_Temp > temp){
                    clearLCD();
                    lcd.print("Face the sensor");
                    lcd.setCursor(0,1);
                    lcd.print("to measure again");
                    delay(200);
                    for (int i = 0; i <= 10; i++) {
                        clearLCD();
                        lcd.print("Measuring ....");
                        delay(100);
                    }
                    ambient_Temp = mlx.readAmbientTempC();
                    temp = mlx.readObjectTempC();
                }
                clearLCD();
                lcd.print("Your Tempature");
                Serial.println("Your Tempature");
                lcd.setCursor(0,10); 
                lcd.print(temp);
                delay(1000);
                if(temp > normal_temp)
                {
                    tone(buzzer, 1000);
                    clearLCD();
                    lcd.print("Temperature High");
                    Serial.println("Temperature High");
                    lcd.setCursor(0,1);
                    lcd.print("Contact Doctor");                 
                    delay(1000);
                    noTone(buzzer); 
                    lock = 1;
                    break;
                }
                clearLCD();
                lcd.print("Please wash");
                Serial.println("Please wash");
                lcd.setCursor(0,1); 
                lcd.print("your hands");
                delay(1000);
                val = digitalRead(sensor);   
                if (val == HIGH) {           
                    clearLCD();
                    lcd.print("Be Alert for ");
                    lcd.setCursor(0,1); 
                    lcd.print("the sanitizer");
                    delay(1000);
                    myservo.write(180);                                
                    delay(1500);
                }
            }
            else{
            tone(buzzer, 1000);
              clearLCD();
              lcd.print("Wear Mask");
              Serial.println("Wear Mask");
              delay(1000);
              noTone(buzzer); 
            }
            lock = 1;  
        }        
      }  
     delay(2000);
     lcd.clear();
    }




long measureDistance (){   
  long duration, distance;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   distance = duration / 29 / 2;
  Serial.println(distance);
  return distance;
}

void clearLCD(){
  lcd.clear(); 
  lcd.setCursor(0,0); 
}
