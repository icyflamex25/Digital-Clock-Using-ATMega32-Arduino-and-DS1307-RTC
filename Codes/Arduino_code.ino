#include <LiquidCrystal.h>
#include <RTClib.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
RTC_DS1307 rtc;

int Contrast=60;
void setup()
{
  Serial.begin(57600);
  analogWrite(9,Contrast);
  lcd.begin(16,2);
  rtc.begin();
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    lcd.print("Couldn't find RTC");
    Serial.flush();
    abort();
  } 
  else
  {
    Serial.println("RTC Found");
    lcd.print("RTC Found"); 
  }
  //rtc.adjust(DateTime(2022, 5, 31, 13, 11, 30));
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
void loop()
{
  const char dayInWords[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  const char monthInWords[13][4] = {" ", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
  DateTime now = rtc.now();

  int hour=now.hour();
  int minute=now.minute();
  int second=now.second();
  int dd = now.day();// day in number
  int mth = now.month();// month
  int yr = now.year();// year
  int day = now.dayOfTheWeek();
  lcd.setCursor(0,0);
  lcd.print(dd);
  lcd.print("-");
  lcd.print(monthInWords[mth]);
  lcd.print("-");
  lcd.print(yr);
  lcd.print(" ");
  lcd.print(dayInWords[day]);
  Serial.print(dayInWords[day]);
  //Serial.print(day[now.dayOfTheWeek()]);
  lcd.setCursor(0,1);
  lcd.print("Time:");
  if (hour < 10) lcd.print("0");
  lcd.print(hour);
  lcd.print(':');

  if (minute< 10) lcd.print("0");
  lcd.print(minute);
  lcd.print(':');

  if (second < 10) lcd.print("0");
  lcd.print(second);
}
