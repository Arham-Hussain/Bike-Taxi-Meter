#include <LiquidCrystal.h>
#include<SoftwareSerial.h>
#include<TimeLib.h>
SoftwareSerial myGsm(5,6);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
void setup() {
  pinMode(3,INPUT);//This is for the hall sensor output
  pinMode(4,INPUT);//This is for the taxi to start 
  Serial.begin(9600);
  Serial.println("Setup executed");
  delay(250);
  myGsm.begin(9600);
  Serial.println("Started the Gsm");
  delay(250);
  lcd.begin(16,2);
  lcd.clear();
  lcd.blink();
  delay(250);
  Serial.println("Started the lcd");
  delay(250);

}


void loop() {
  int count=(-1);
  double distance=0.0,fare=0,distinrev=1.27;
  double tinitial,tfinal,t1=0,t2=0,t3=0,t4,velocity1=0,velocity2,acc;
  tinitial=now();
  if(digitalRead(3)==HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Welcome..!");
    delay(000);
     while(digitalRead(3)==HIGH)//Taxi mode is on
   {
        Serial.println("loop enter");
        if(digitalRead(4)==LOW)
      {
        count++;
        Serial.print("Count:");
        Serial.println(count);
        delay(1000);
        distance=distance+distinrev;//distance covered in meters
        Serial.print("distance:");
        Serial.println(distance);
        t4=t3;
        t3=t2;
        t2=t1;
        t1=now();
        Serial.print("time");
        Serial.println(t1);
        if(t1!=t2){
          velocity1=distinrev/(t1-t2); 
          Serial.print("Velocity:");
          Serial.println(velocity1);
        }
        if(t3!=t4){
          velocity2=distinrev/(t3-t4);
          Serial.print("Velocity2:");
          Serial.println(velocity2);
        }
        if(count>5){
          acc=(velocity1-velocity2)/(t1-t4);
          if(acc>=2){
            distance=distance+2*distinrev;
          }
        }
      }
      
  }
  tfinal=now();
  fare=distance*10+(tfinal-tinitial)*0.5;
  Serial.print("The fare");
  Serial.println(fare);//At the end of this right the fare to be sent to the mobile phone of the owner
  SendMessage(fare);
  lcd.setCursor(0,0);
  lcd.print("Fare:");
  lcd.print(fare);
  delay(10000);
  }
 else{
 Serial.println("Thank You");
 lcd.setCursor(0,1);
 lcd.print("Thank You !");
 delay(1500);  
 }
}

void SendMessage(double fare){
  myGsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  myGsm.println("AT+CMGS=\"+917997540706\"\r"); // Replace x with mobile number
  delay(1000);
  myGsm.println("The fare for your trip is :");
  myGsm.println(fare,9);
  myGsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  
}
 
  
