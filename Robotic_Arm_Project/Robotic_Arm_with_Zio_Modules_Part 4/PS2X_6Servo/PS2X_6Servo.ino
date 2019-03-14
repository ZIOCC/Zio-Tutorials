#include <PS2X_lib.h>  //for v1.6
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN0  350 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX0  542 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN1  300 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX1  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN2  200 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX2  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN3  100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX3  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN4  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX4  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN5  120 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX5  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVO     0x40
// our servo # counter
uint8_t servonum0 = 0,servonum1 = 1,servonum2 = 2,servonum3 = 3,servonum4 = 4,servonum5 = 5;
uint8_t RY_Value=127,RX_Value=127,LY_Value=127,LX_Value=127;
int pulselen0=542,pulselen1=532,pulselen2=600,pulselen3=100,pulselen4=585,pulselen5=370;
uint8_t Flag_SERVO0=0,Flag_SERVO1=0,Flag_SERVO2=0,Flag_SERVO3=0,Flag_SERVO4=0,Flag_SERVO5=0; //1:++, 2:--
uint8_t delay_time=15;
/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection 
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

void Flag_SERVO_Init()
{
  Flag_SERVO0=0;Flag_SERVO1=0;Flag_SERVO2=0;Flag_SERVO3=0;Flag_SERVO4=0;Flag_SERVO5=0;
  }
void setup(){
 
  Serial.begin(115200);
  pwm.begin(SERVO);
  pwm.setPWMFreq(60,SERVO);  // Analog servos run at ~60 Hz updates
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
  Flag_SERVO_Init();
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return; 
  
  if(type == 2){ //Guitar Hero Controller
    ps2x.read_gamepad();          //read controller 
   
    if(ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if(ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if(ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if(ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if(ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed"); 

    if(ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");
    
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
      Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
      Serial.println("DOWN Strum");
 
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");
    
    if(ps2x.Button(ORANGE_FRET)) {     // print stick value IF TRUE
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
    } 
  }
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");      

    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      //Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      Flag_SERVO4=2;
      pulselen4++;
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.print("Right held this hard: ");
     // Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      Flag_SERVO5=2;
      pulselen5++;
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.print("LEFT held this hard: ");
      //Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      Flag_SERVO5=1;
      pulselen5--;
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      //Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      Flag_SERVO4=1;
      pulselen4--;
    }   

      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
       {
        Serial.println("L2 pressed");
        Flag_SERVO0=1;
        pulselen0--;
       }
      if(ps2x.Button(PSB_R2))
      {
        Serial.println("R2 pressed");
        Flag_SERVO0=2;
        pulselen0++;
      }
      if(ps2x.Button(PSB_TRIANGLE))
      {
        Serial.println("Triangle pressed"); 
        Flag_SERVO3=1;
        pulselen3--;
      }       
    if(ps2x.Button(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
    {
      Serial.println("Circle just pressed");
      Flag_SERVO2=2;
      pulselen2++;
    }
    if(ps2x.Button(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
    {
      Serial.println("X just changed");
      Flag_SERVO3=2;
      pulselen3++;
    }
    if(ps2x.Button(PSB_SQUARE))              //will be TRUE if button was JUST released
    {
      Serial.println("Square just released");
      Flag_SERVO2=1;
      pulselen2--;
    }    
    if(ps2x.Button(PSB_L1))
    {
      Serial.println("L1 pressed");
      Flag_SERVO1=1;
      pulselen1--;
      }
    if(ps2x.Button(PSB_R1))
    {
      Serial.println("R2 pressed");
      Flag_SERVO1=2;
      pulselen1++;
      }

      RY_Value=ps2x.Analog(PSS_RY);
//      RX_Value=ps2x.Analog(PSS_RX);
      LY_Value=ps2x.Analog(PSS_LY);
//      LX_Value=ps2x.Analog(PSS_LX);

//      Serial.print("Stick Values:");
//      Serial.print(RY_Value);
//      Serial.print("  ");
//      Serial.println(LY_Value);
      
      if(LY_Value<=RY_Value)
      delay_time=map(LY_Value,0,127,0,15);
      if(LY_Value>=RY_Value)
      delay_time=map(RY_Value,0,127,0,15);
      if(pulselen0>SERVOMAX0) 
      {pulselen0=SERVOMAX0;}
      if(pulselen0<SERVOMIN0) 
      {pulselen0=SERVOMIN0;}
      
      if(pulselen1>SERVOMAX1) 
      {pulselen1=SERVOMAX1;}
      if(pulselen1<SERVOMIN1) 
      {pulselen1=SERVOMIN1;}
    
      if(pulselen2>SERVOMAX2) 
      {pulselen2=SERVOMAX2;}
      if(pulselen2<SERVOMIN2) 
      {pulselen2=SERVOMIN2;}
      
      if(pulselen3>SERVOMAX3) 
      {pulselen3=SERVOMAX3;}
      if(pulselen3<SERVOMIN3) 
      {pulselen3=SERVOMIN3;}
  
      pwm.setPWM(servonum0, 0, pulselen0,SERVO);
      pwm.setPWM(servonum1, 0, pulselen1,SERVO);
      pwm.setPWM(servonum2, 0, pulselen2,SERVO);
      pwm.setPWM(servonum3, 0, pulselen3,SERVO);
      pwm.setPWM(servonum4, 0, pulselen4,SERVO);
      pwm.setPWM(servonum5, 0, pulselen5,SERVO);
//      Serial.print("pulselen0:");
//      Serial.println(pulselen0);
//      Serial.print("pulselen1:");
//      Serial.println(pulselen1);
//      Serial.print("pulselen2:");
//      Serial.println(pulselen2);
//      Serial.print("pulselen3:");
//      Serial.println(pulselen3);
//      Serial.print("pulselen4:");
//      Serial.println(pulselen4);
//      Serial.print("pulselen5:");
//      Serial.println(pulselen5);   
  }
   delay(delay_time);
//   Flag_SERVO_Init();
}
