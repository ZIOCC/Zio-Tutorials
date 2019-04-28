/* 

*/
#include <EEPROM.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/******************************* OLED *******************************/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
/******************************* Keypad *******************************/
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

/*******************************  *******************************/
byte value;
uint8_t lock_status=0;  //0:lock-in  1:unlocked  2:change password
uint8_t password[6],password_input[6];
uint8_t P_NUM=0,error_num=0,unlocked_flag=0,password_lock_flag=0;
void setup(){
  Serial.begin(9600);
//Set up the password
/*
  for(uint8_t i=0;i<6;i++)
    {
    EEPROM.write(i, 0);
    }*/
    for(uint8_t i=0;i<6;i++)
    {
    password[i]=EEPROM.read(i);
    }
    Serial.println("password:");
    for(uint8_t i=0;i<6;i++)
    {
    Serial.print(password[i]);
    Serial.print(" ");
    }
    Serial.println();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,16);
  display.clearDisplay();
  display.print("system initialing...");
  display.display();
  delay(2000);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println("Please input six password,Ending with '#'!");
  display.print("     ");
  display.display();
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    //Serial.println(customKey);
    switch(lock_status)
    {
      case 0:
              {
                if(P_NUM<6)
                {
                  password_input[P_NUM++]=customKey-0X30;
                  display.setTextSize(2);
                  display.print("*");
                  display.display();
                }
                if(P_NUM==6&&customKey=='#')
                  {
                    Serial.println("input_password:");
                    for(uint8_t i=0;i<6;i++)
                    {
                    Serial.print(password_input[i]);
                    Serial.print(" ");
                    }
                    Serial.println();
                    if(password_compare(password_input))
                      {
                        lock_status = 1;
                        P_NUM=0;
                        unlocked_flag=1;
                        }
                    else
                     {
                      P_NUM=0;
                      error_num++;
                      if(error_num<3)
                        {
                        display.setTextSize(1);
                        display.clearDisplay();
                        display.setCursor(20,16);
                        display.println("password error!");
                        display.display();
                        delay(2000);
                        display.setCursor(0,0);
                        display.setTextSize(1);
                        display.clearDisplay();
                        display.println("Please input six password,Ending with '#'!");
                        display.print("     ");
                        display.display();
                        }
                      else
                      {
                        password_lock_flag=1;
                        }
                      }
                    }
                }break;
      case 1:
              {
                
                if(customKey=='*')
                  {
                    lock_status=2;
                    display.setCursor(0,0);
                    display.setTextSize(1);
                    display.clearDisplay();
                    display.println("Please input new password,Ending with '#'!");
                    display.print("     ");
                    display.display();
                    }
                }break;             
      case 2:
      {
        if(P_NUM<6)
        {
          password_input[P_NUM++]=customKey-0X30;
          display.setTextSize(2);
          display.print("*");
          display.display();
        }
        if(P_NUM==6&&customKey=='#')
          {
          for(uint8_t i=0;i<6;i++)
            {
            EEPROM.write(i, password_input[i]);
            password[i]=password_input[i];
            }
            display.setCursor(20,12);
            display.setTextSize(1);
            display.clearDisplay();
            display.println("Changed done!");
            display.display();
            P_NUM=0;
            lock_status=1;
            unlocked_flag=1;
          }
        }break;
      default:break;
      }
  }
 if(unlocked_flag)
 {
  display.setCursor(20,12);
  display.setTextSize(2);
  display.clearDisplay();
  display.println("Welcome!");
  display.display();
  delay(2000);
  display.setCursor(0,0);
  display.setTextSize(1);
  display.clearDisplay();
  display.println("Press '*'to change password");
  display.println("Ending with '#'!");
  display.display();
  unlocked_flag=0;
  }
 if(password_lock_flag) 
  {
    display.clearDisplay();
    display.setCursor(10,0);
    display.setTextSize(1);
    display.print("Input error 3 times");
    display.setCursor(20,16);
    display.println("password locked!");
    display.display();
    password_lock_flag=0;
    for(;;);
    }
}
bool password_compare(uint8_t *data)
{
  uint8_t flag=0;
  for(uint8_t i=0;i<6;i++)
    {
      if(data[i]!=password[i])
        {
          flag=1;
          break;
          }
      }
  if(flag)
    return false;
  else
    return true;
  }
