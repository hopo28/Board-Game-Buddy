//Its an Arduino Duemilanove w/ATmega328

#include <Rainbowduino.h>
#include <stdint.h>
#include <Entropy.h>

#define BUT_ROLL_PIN 3 //D2
#define BUT_MODE_PIN A0
#define PIEZO_PIN 2//needs to be a pwm one i think
#define AUC_MIN 10
#define AUC_MAX 45
#define PRESS_SHORT 30//in milliseconds, for debouncing
#define ROLL_DELAY 100

#define DEBUG 1//serial debug

boolean rollPressFlag = false;
unsigned long rollPressedTimer = 0;
boolean modePressFlag = false;
unsigned long modePressedTimer = 0;
boolean mute = false;
char timerLength = 0; // 0 means random between auc limits

//countdown animation
unsigned char animation[][2] = {{0,2},{0,3},{0,4},{0,5},{1,6},{2,7},{3,7},{4,7},{5,7},{6,6},{7,5},{7,4},{7,3},{7,2},{6,1},{5,0},{4,0},{3,0},{2,0},{1,1}};

byte standbyCounter = 0;

extern unsigned char diceFont[][8];
byte Brightness = 31;//starts dim

// modes are: Standby,1D6,2D6,Timer Set,Timer,Brightness,Mute
char mode = 0;//curent mode
const char numModes = 7;

void setup()
{
  Rb.init();
  Entropy.initialize();
  pinMode(BUT_ROLL_PIN, INPUT_PULLUP);
  pinMode(BUT_MODE_PIN, INPUT_PULLUP);
  pinMode(PIEZO_PIN, OUTPUT);
  doDisplay();
  if (DEBUG)
    Serial.begin(9600);
}


void loop()
{ 
  checkRoll();
  checkMode();
}


void checkMode()
{
  if(digitalRead(BUT_MODE_PIN) == LOW)//is being pressed
  {
    modePressFlag = true;
    modePressedTimer = millis();//log time
  }
  else if(modePressFlag == true)//was pressed and now unpressed
  {
    if((millis()-modePressedTimer) < PRESS_SHORT)
    {
      //do sod all, time too short
    }
    else
    {
      mode++;//advance mode counter
      if(mode >= numModes)
      {
        mode = 0;
      }
      modePressFlag = false;
      Rb.blankDisplay();
      switch(mode)
      {
        case 1: //1D6
          drawMyChar(6+6+6,0,0,0xFFFFFF);
          drawMyChar(6+6+1,0,0,0xFFFFFF);
          break;
        case 2: //2D6
          drawMyChar(6+6,0,0,0xFFFFFF);
          drawMyChar(6+1,0,0,0xFFFFFF);
          drawMyChar(6,0,0,0xFFFFFF);
          drawMyChar(1,0,0,0xFFFFFF);
          break;    
        default: doDisplay(); break;//modes other than rolls end up here
      }
    }
  }
}


void checkRoll()
{//actually do the roll
  if(digitalRead(BUT_ROLL_PIN) == LOW)//is being pressed
  {
    rollPressFlag = true;
    rollPressedTimer = millis();//log time
  }
  else if(rollPressFlag == true)//was pressed and now unpressed
  {
    if((millis()-rollPressedTimer) < PRESS_SHORT)
    {
      //do sod all, time too short
    }
    else    
    {
      switch(mode)
      {
        case 0: standbyCounter++; if(standbyCounter>=3){standbyCounter=0;} doDisplay(); break;//standby
        case 3:// Set timer
            timerLength = timerLength + 15;//advance by 15 seconds
            if(timerLength > 60)
              timerLength = 0;
            doDisplay();
            break; 
        case 4: doTimer(); break; // Timer
        case 5: // Brightness
            Brightness = Brightness + 32;//will rollover
            doDisplay();
            for(int i = 0; i < ((Brightness+1)/32); i++)
            {
              Rb.setPixelXY(0, i, reduceBrightness(0x00FF00, Brightness));
            }
            break;
        case 6: mute = !mute; doDisplay(); break; // Mute
        default: doDisplay(); break; // Standby, 1D6 and 2D6 end up here
      }
      rollPressFlag = false;
    }
  }
}

void doDisplay()
{
  char dice1 = Entropy.random(1,7);
  char dice2 = Entropy.random(1,7);
  uint32_t dice1Col = getColourFromNumber(dice1);
  uint32_t dice2Col = getColourFromNumber(dice2);
  Rb.blankDisplay();
  switch(mode)
  {
    case 0: // standby screen
      if(standbyCounter == 0){drawMyChar(21,0,0,0xFF0000);}// R
      if(standbyCounter == 1){drawMyChar(21,0,0,0x00FF00);}// G
      if(standbyCounter == 2){drawMyChar(21,0,0,0x0000FF);}// B
      break;
    case 1:// 1D6
        //do little animation
        for(int i = 1; i <= 6; i++)
        {
          drawMyChar(i + 12,0,0,getColourFromNumber(i));
          delay(ROLL_DELAY);//make it look like its struggling to calculate
          Rb.blankDisplay();
        }
        drawMyChar(dice1 + 12,0,0,dice1Col);//1D6
        break;
    case 2:// 2D6
        delay(200);//give the impression its doing something
        if(dice1 == dice2)
        {
          dice1Col = dice2Col = 0xFFFFFF;//if a double, make it white
        }
        //do little animation
        for(int i = 1; i <= 6; i++)
        {
          drawMyChar((7 - i),0,0,getColourFromNumber(i));//2D6(1)
          drawMyChar(i + 6,0,0,getColourFromNumber(i));//2D6(2)
          delay(ROLL_DELAY);//make it look like its struggling to calculate
          Rb.blankDisplay();
        }
        drawMyChar(dice1,0,0,dice1Col);//2D6(1)
        drawMyChar(dice2 + 6,0,0,dice2Col);//2D6(2)
        break;
    case 3:// Timer Set
        switch(timerLength)
        {
          case 0: drawMyChar(28,0,0,0xFFFFFF); break;//draw Random
          case 15: drawMyChar(24,0,0,0xFFFFFF); break;//draw 15
          case 30: drawMyChar(25,0,0,0xFFFFFF); break;//draw 30
          case 45: drawMyChar(26,0,0,0xFFFFFF); break;//draw 45
          case 60: drawMyChar(27,0,0,0xFFFFFF); break;//draw 60
          default: timerLength = 0; drawMyChar(28,0,0,0xFFFFFF); break;//draw Random
        }
        break;
    case 4: drawMyChar(19,0,0,0xFFFFFF); break; // Timer
    case 5: drawMyChar(20,0,0,0xFFFFFF); break; // Brightness
    case 6:// Mute
        drawMyChar(22,0,0,0xFFFFFF);//draw speaker
        if(mute)//if muted
        drawMyChar(23,0,0,0xFF0000);//draw slash
        break;
    default: /*do nothing*/ break;
  }
}

void doTimer()
{//perform the countdown
  unsigned long startTime = millis();//get time now
  unsigned long time = timerLength;
  if(time == 0)
    time = Entropy.random(AUC_MIN,AUC_MAX);//get random time length
  unsigned long endTime = startTime + (time*1000);//calculate end time
  Rb.blankDisplay();
  drawMyChar(19,0,0,0x00FF00);//in green
  byte pixel = 0;
  while(millis() < endTime)
  {//do animation
    drawMyChar(19,0,0,0x00FF00);//in green
    Rb.setPixelXY(animation[pixel][0], animation[pixel][1], reduceBrightness(0xFF0000, Brightness));//black out a pixel
    delay(40);//10ms for display etc
    pixel++;
    if(pixel >= 20)
      pixel = 0;
    if(time != 0)
    {//if not random time, beep for last 3 seconds
      if((endTime-millis() > 3000) && (endTime-millis() < 3050))
        shortBip();
      if((endTime-millis() > 2000) && (endTime-millis() < 2050))
        shortBip();
      if((endTime-millis() > 1000) && (endTime-millis() < 1050))
        shortBip();
    }
  }
  byte tempBrightness = Brightness;//save brightness
  Brightness = 255;//full brightness
  //Flash it a bit
  for(int i = 0; i < 3; i++)
  {
    drawMyChar(19,0,0,0xFF0000);//in red
    bip();//make a noise
//    delay(200);//pause for a bit
    Rb.blankDisplay();
    delay(100);//pause for a bit
  }
  //long on
  drawMyChar(19,0,0,0xFF0000);//in red
  timeEndBeep();//1 second sound
  delay(1000);//pause for a bit longer
  Brightness = tempBrightness;//restore brightness
  doDisplay();
}

// = = = = = = = = = SUPPORT FUNCTIONS = = = = = = = = = = =

uint32_t getColourFromNumber(char number)
{//based on monopoly property colours
  uint32_t retVal = 0;
  switch(number)
  {
    case 1: retVal = 0x00FFFF; break; //CYAN
    case 2: retVal = 0xFF00FF; break; //MAGENTA
    case 3: retVal = 0xFF5500; break; //ORANGE
    case 4: retVal = 0xFF0000; break; //RED
    case 5: retVal = 0xFFFF00; break; //YELLOW
    case 6: retVal = 0x00FF00; break; //GREEN
    default: retVal = 0xFFFFFF; break; //WHITE 
  }
  return retVal;
}

uint32_t reduceBrightness(uint32_t colorRGB, byte reduction)
{
  colorRGB = (colorRGB & 0x00FFFFFF);
  byte B = (colorRGB & 0x0000FF); //channel Blue
  colorRGB = (colorRGB >> 8);
  byte G =(colorRGB & 0x0000FF); //channel Green
  colorRGB = (colorRGB >> 8);
  byte R =(colorRGB & 0x0000FF); //channel Red
//apply brightness correction
  R = map(R, 0, 255, 0, reduction);
  G = map(G, 0, 255, 0, reduction);
  B = map(B, 0, 255, 0, reduction);
  return ((uint32_t)R << 16) | ((uint32_t)G <<  8) | B;
}

void drawMyChar(unsigned char ascii,unsigned int poX, unsigned int poY,  uint32_t colorRGB)
{//this just uses numbers, not the 0x20 offset
  for(unsigned char i=0;i<8;i++)
  {
    unsigned char temp = pgm_read_byte(&diceFont[ascii][i]);
    for(unsigned char f=0;f<8;f++)
    {
      if((temp>>f)&0x01)
      {
        Rb.setPixelXY(poY+f, poX+i, reduceBrightness(colorRGB, Brightness));
      }
    }
  }
}


