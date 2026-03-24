#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <string.h>
#include <stdlib.h>

const byte ROWS = 4; 
const byte COLS = 4; 
char buttonKey[4][4] = {
  {'7','8','9','+'},
  {'4','5','6','-'},
  {'1','2','3','x'},
  {'0','C','=','/'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {6, 7, 8, 9};

Keypad customKeypad = Keypad( makeKeymap(buttonKey), rowPins, colPins, ROWS, COLS); 
LiquidCrystal_I2C lcd(0x27,16,2); 

void setup()
{
  lcd.init();                      
  lcd.backlight();
  Serial.begin(115200);
  lcd.clear();
}

String location=""; // luu dau

void remove_location(int j , int index, float value[]) // xoa phan tu sau khi thuc hien tinh toan
{
  for (int i = j; i < index; i++)
  {
    int k = i + 1;
    location[i] = location[i+1];
    value[i + 1] = value[i + 2];
  }
}

void loop()
{
  bool kt = true; // kiem tra du dieu kien lap
  String strings="" ; // luu value_key tu matrix
  location="";
  int dem=0; // dem so phan tu nhap vao
  float value[30] = {0}; // luu gia tri so
  while(kt == true)
  {
    char value_key = customKeypad.getKey(); // lay gia tri tu matrix
    if (value_key)
    {
      if(value_key == 'C' ) 
      {
        kt = false;
        lcd.clear();
        break;
      }
      if (value_key== '=')
      {
        break;
      }
      delay(30);
      dem++; 
      strings = strings + (String)value_key; // luu value_key vao strings
      lcd.print(value_key); 
      if(dem > 16) // cuon man hinh khi co >16 ki tu
      {
        lcd.scrollDisplayLeft();
      }
    }
  }
  if(strings == "------2024------") // special code
  {
    lcd.clear();
    lcd.print("welcome 2024");
    kt = false;
  }
  if (kt == true)
  {
    int index=0;
    // tach so va dau, sau do luu so vao value_key và luu dau vào location
    for (int i = 0; i < dem; i++)
    {
      if (strings[i] == '+' || strings[i] == '-' || strings[i] == 'x' || strings[i] == '/' )
      {
        location += strings[i];
        index++;
      }
      else if (strings[i] != '+' && strings[i] != '-' && strings[i] != 'x' && strings[i] != '/')
      {
        int k = (int)strings[i] - 48; // doi so tu string ---> float
        value[index] = value[index] * 10 + k;
      }
    }
    // thuc hien nhan/chia
    for (int i = 0; i < index; i++)
    {
      if (location[i] == 'x')
      {
        value[i] *= value[i + 1];
        index--;
        remove_location(i,index, value);
        i--;
      }
      else if (location[i] == '/')
      {
        value[i] /= value[i + 1];
        index--;
        remove_location( i, index, value);
        i--;
      }
    }
    // thuc hien cong/tru
    for (int x = 0; x < index; x++)
    {
      if (location[x] == '+')
      {
        value[x] += value[x + 1];
        index--;
        remove_location(x, index, value);
        x--;
      }
      else if (location[x] == '-')
      {
        value[x] -= value[x + 1];
        index--;
        remove_location(x, index, value);
        x--;
      }
    }
    // in ket qua.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("solved:");
    lcd.setCursor(0,1);
    lcd.print(value[0]);
  }
  delay(1000);
}