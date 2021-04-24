/*************************************
 * Program : Alarm otomatis
 * Input   : 4 Tombol push on, Module RTC DS1307 
 * Output  : LCD 2x16, BUZZER
 * by Sarjana Electro
 * ***********************************/

#include <dht.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#include<EEPROM.h>
#include <RTClib.h>
 
//LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
RTC_DS1307 RTC;
int temp,inc,hours1,minut,addR=11,temp1=3,mode=0;
int tSeconds=0, tMinutes=0, hours=0;  
//this line, along with another line in void timerFunction(), is where you can adjust the amount of time that is counted down in                                                                    //the timer function
int centiseconds=0, sSeconds=0, sMinutes=0;
int next=6;    
int INC=5;
int set_mad=4;
int shw_dat=3;
int buzzer=7;
int HOUR, MINUT, SECOND=0;
int tempe=0, humi=0;
dht DHT;

#define DHT11_PIN A0

void setup(){

   Wire.begin();
   RTC.begin();
   lcd.begin(16,2);
   
   pinMode(INC, INPUT);
   pinMode(next, INPUT);
   pinMode(set_mad, INPUT);
   pinMode(shw_dat, INPUT);
   pinMode(buzzer, OUTPUT);
   digitalWrite(next, HIGH);
   digitalWrite(set_mad, HIGH);
   digitalWrite(INC, HIGH);
   digitalWrite(shw_dat, HIGH);
 
   lcd.setCursor(0,0);
   lcd.print("      ALARM     ");
   lcd.setCursor(0,1);
   lcd.print("SARJANA ELECTRO");
   delay(1000);  
   lcd.setCursor(0,0);
   lcd.print("      TIMER     ");
   delay(1000);  
   lcd.setCursor(0,0);
   lcd.print("   STOP WATCH   ");
   delay(2000);
   if(!RTC.isrunning())
  {
   RTC.adjust(DateTime(__DATE__,__TIME__));
  }
}

void loop()
{
  
  int temp=0,val=1,temp4;
  DateTime now = RTC.now();
  DHT.read11(DHT11_PIN);
  humi = DHT.humidity;
  tempe =  DHT.temperature;
   
  if(digitalRead(shw_dat) == 0)
   {mode++;}

  if(digitalRead(shw_dat) == 0 && mode==1)      //set Alarm time
   { 
     lcd.setCursor(0,0);
     lcd.print("   SET ALARM    ");
     lcd.setCursor(0,1);
     lcd.print("                ");
    delay(2000);
    //defualt();int temp,inc,hours1,minut,add=11;
   while(1)
   {   
    
    if(digitalRead(set_mad) == 0)
    {
    alarm();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" ALARM TIME SET ");
     lcd.setCursor(9,1);
     lcd.print(HOUR=hours1,DEC); 
     lcd.print(":");
     lcd.print(MINUT=minut,DEC);
     lcd.print(":");
     lcd.print(SECOND=now.second(),DEC);
     CheckTime();
     delay(1000);
    }
    if(digitalRead(INC) == 0)
    break;
   }
   }

 if(digitalRead(shw_dat) == 0 && mode==3)      //set Timer
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    SET TIMER     ");
    while(1)
    {
    if(digitalRead(set_mad) == 0)
    {
    timerFunction();
    }
   if(digitalRead(INC) == 0)
   break;
    }
   }
if(digitalRead(shw_dat) == 0 && mode==4)      //set Stopwatch
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    STOPWATCH     ");
    
    while(1)
    {
    if(digitalRead(set_mad) == 0)
    {
    stopwatchFunction();
    }
   if(digitalRead(INC) == 0)
   break;
    }
   
    }
 if(mode>=5)
    {mode=0;}
 lcd.clear();
 lcd.setCursor(0,0);
 printDigits(HOUR=now.hour()); 
 lcd.print(":");
 printDigits(MINUT=now.minute());
 lcd.print(":");
 printDigits(SECOND=now.second());
 lcd.setCursor(0,1);
 printDigits(now.day());
 lcd.print("/");
 printDigits(now.month());
 lcd.print("/");
 lcd.print(now.year(),DEC);
  lcd.setCursor(10,0); 
  lcd.print("T:");
  lcd.print(tempe);
  lcd.print((char)223);
  lcd.setCursor(15,0); 
  lcd.print("C");
  lcd.setCursor(11,1);
  lcd.print("H:");
  lcd.print(humi);
  lcd.print("%");
  
  delay(1000);
}
/*Function to set alarm time and feed time into Internal eeprom*/
void alarm()                             
{
  int temp=1,minuts=0,hours=0,seconds=0;
  
    while(temp==1)
    {
      
     if(digitalRead(INC)==0)
     {
      HOUR++;
      if(HOUR==24)
      {
       HOUR=0;
      }
      while(digitalRead(INC)==0);
     }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Set Alarm Time ");
   //lcd.print(x); 
    lcd.setCursor(0,1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
    if(digitalRead(next)==0)
    {
      hours1=HOUR;
      EEPROM.write(addR++,hours1);
     temp=2;
     while(digitalRead(next)==0);
    }
    }
    
    while(temp==2)
    {
     if(digitalRead(INC)==0)
     {
      MINUT++;
      if(MINUT==60)
      {MINUT=0;}
      while(digitalRead(INC)==0);
     }
     // lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
      if(digitalRead(next)==0)
      {
       minut=MINUT;
       EEPROM.write(addR++, minut);
       temp=0;
       while(digitalRead(next)==0);
      }
     
    }
    delay(1000);
}
/* Function to check medication time */
void CheckTime()
{
  int tem[17];
  while(1)
  {
    DateTime now = RTC.now();
  lcd.setCursor(0,1);
 lcd.print(HOUR=now.hour(),DEC); 
 lcd.print(":");
 lcd.print(MINUT=now.minute(),DEC);
 lcd.print(":");
 lcd.print(SECOND=now.second(),DEC);
  for(int i=11;i<17;i++)
  {
    tem[i]=EEPROM.read(i);
  }
  if(HOUR == tem[11] && MINUT == tem[12]) 
  {
   for(int j=0;j<10;j++)
   {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    
    }
   hours1=0;
    minut=0;
    addR=11;
    return;
    
   }
 }
}
void timerFunction()  //the timer function was made with the help of this post: http://pastebin.com/f57045830
{
  int set=0;
    lcd.setCursor(4,1);
    //lcd.setCursor(0, 1);
    lcd.print("00:00:00");
  while(1)
  {


                   while(digitalRead(shw_dat)==1)
                    {
                      set=1;
                   if(digitalRead(set_mad)==0)  //if "Start/Stop" is pressed, the timer counts down
                      {
                        tSeconds++;
                        lcdOutput();
                        delay(300);
                        if(tSeconds==60)
                        {
                        tMinutes++;
                        tSeconds=0;
                        }
                       }
                 if(digitalRead(INC)==0)  //if "Start/Stop" is pressed, the timer counts down
                      {
                        tMinutes++;
                        lcdOutput();
                        delay(300);
                        if(tMinutes==60)
                        {
                        hours++;
                        tMinutes=0;
                        }
                      }
                  if(digitalRead(next)==0 )  //if "Start/Stop" is pressed, the timer counts down
                  {
                        hours++;
                        lcdOutput();
                        delay(300);
                        if(hours==24)
                        {
                        hours=0;
                        }
                  }      }
  if(digitalRead(shw_dat)==0 && set==1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    //lcd.setCursor(0, 1);
    lcd.print(" TIMER SET FOR  ");
    lcd.setCursor(4,1);
    //lcd.setCursor(0, 1);
    lcd.print("00:00:00");
            while(digitalRead(INC)==1)
            {
            static unsigned long lastTick = 0;
              if (tSeconds > 0)
                {
                if (millis() - lastTick >= 1000)
                {
                lastTick = millis();
                tSeconds--;
                lcdOutput();
                }
                }
            if (tMinutes > 0)
               {
              if (tSeconds <= 0)
              {
                tMinutes--;
                tSeconds = 60;
              }
              }
            if (hours > 0)
              {
              if (tMinutes <= 0)
              {
                hours--;
                  tMinutes = 60;
                }
              } 
            
              /*if(hours == 00 && tMinutes == 00 && tSeconds == 00)  //when timer ends, the alarm goes on
                {
             // while(digitalRead(shw_dat) == 1)  //the alarm will only go off until "Restart" is pressed
             // {
                set=2;
                lcd.setCursor(4, 1);
                lcd.print("00:00:00");
                  for(int i=0;i<2;i++)
                  {
                  digitalWrite(buzzer, HIGH);
                  delay(500);
                  digitalWrite(buzzer, LOW);
                  delay(500);
                  }
               // }
                }*/
                
           }
  }
  if(digitalRead(shw_dat) == 0)
               {
                set=0;
                mode=0;
                break;
                } 
  }
  
  }

void lcdOutput()  //this is just used to display the timer on the LCD
{
  lcd.setCursor(4, 1);
  printDigits(hours);
  lcd.setCursor(7, 1);
  printDigits(tMinutes);
  lcd.setCursor(10, 1);
  printDigits(tSeconds);
  delay(100);
}

void printDigits(int digits)  //this void function is really useful; it adds a "0" to the //beginning of the number, so that 5 minutes is displayed as "00:05:00", rather than "00:5 :00"
{
  if(digits < 10) 
  {
    lcd.print("0");
    lcd.print(digits);
  }
  else
  {
    lcd.print(digits);
  }
}
void stopwatchFunction()
{
 int count=1,sMin,sSec,sCen;
 
    lcd.setCursor(4,1);
    //lcd.setCursor(0, 1);
    lcd.print("00:00:00");
    
  while(1)
  {
  if(digitalRead(shw_dat) == LOW )
    {
      count=0;
      loop();
     
    }    
  if(digitalRead(next) == LOW)  //if the "Start/Stop" button is pressed, the time begins running
  {
    
  while(1)
    {
      int count=1;
      delay(6);
  lcd.setCursor(4, 1);
  printDigits(sMinutes);
  lcd.setCursor(7, 1);
  printDigits(sSeconds);
  lcd.setCursor(10, 1);
  printDigits(centiseconds);
    centiseconds++;
    sCen=centiseconds;
    if(centiseconds==100)
    {
      sSeconds++;
      sSec=sSeconds;
      centiseconds=0;
      if(sSeconds==60)
      {
        sMinutes++;
        sMin=sMinutes;
        sSeconds=0;        
        }
    }
    if(digitalRead(set_mad) == 0)
      {
        centiseconds = 0;
        sSeconds = 0;
        sMinutes = 0;
        break;
      }
if(digitalRead(INC) == LOW && count ==1)
    {
      while(1)
      {
     lcd.setCursor(4, 1);
  printDigits(sMinutes);
  lcd.setCursor(7, 1);
  printDigits(sSeconds);
  lcd.setCursor(10, 1);
  printDigits(centiseconds); 
     
      if(digitalRead(set_mad) == 0)
      {
        count=2;
        centiseconds = 0;
        sSeconds = 0;
        sMinutes = 0;
        break;
      }
      }
      }
      
      }
    }
  }
}
