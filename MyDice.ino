#include <Rainbowduino.h>
#include <stdint.h>
#include <Entropy.h>

#define BUT_ROLL_PIN 3 //D3
#define BUT_MODE_PIN A0
#define AUC_MIN 10 //was 10
#define AUC_MAX 60 //was 60

boolean rollPressFlag = false;
boolean modePressFlag = false;

extern unsigned char diceFont[][8];
byte Brightness = 31;//starts dim

// modes are: Standby,1D6,2D6,Timer,Brightness
char mode = 0;//curent mode
const char numModes = 5;

void setup()
{
  Rb.init();
  Entropy.initialize();
  pinMode(BUT_ROLL_PIN, INPUT_PULLUP);
  pinMode(BUT_MODE_PIN, INPUT_PULLUP);
//  Serial.begin(9600);
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
  }
  else if(modePressFlag == true)//was pressed and now unpressed
  {
    mode++;
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
      default: doDisplay(); break;//standby, timer, and brightness end up here
    }
  }
}

void checkRoll()
{
  if(digitalRead(BUT_ROLL_PIN) == LOW)//is being pressed
  {
    rollPressFlag = true;
  }
  else if(rollPressFlag == true)//was pressed and now unpressed
  {
    switch(mode)
    {
      case 3: doTimer(); break; // Timer
      case 4: // Brightness
          Brightness = Brightness + 32;//will rollover
          doDisplay();
          for(int i = 0; i < ((Brightness+1)/32); i++)
          {
            Rb.setPixelXY(0, i, reduceBrightness(0x00FF00, Brightness));
          }
          break;
      default: doDisplay(); break; //standby, 1D6 and 2D6 end up here
    }
    rollPressFlag = false;
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
    case 0: Rb.setPixelXY(0, 0, reduceBrightness(0xFF0000, 31)); break;// red pixel minimum brightness
    case 1:// 1D6
        delay(200);//make it look like its struggling to calculate
        drawMyChar(dice1 + 12,0,0,dice1Col);//1D6
        break;
    case 2:// 2D6
        delay(200);//give the impression its doing something
        if(dice1 == dice2)
        {
          dice1Col = dice2Col = 0xFFFFFF;//if a double, make it white
        }
        drawMyChar(dice1,0,0,dice1Col);//2D6(1)
        drawMyChar(dice2 + 6,0,0,dice2Col);//2D6(2)
        break;
    case 3: drawMyChar(19,0,0,0xFFFFFF); break; // Timer
    case 4: drawMyChar(20,0,0,0xFFFFFF); break; // Brightness
    default: /*do nothing*/ break;
  }
}

void doTimer()
{//perform the countdown
  char time = Entropy.random(AUC_MIN,AUC_MAX);
  Rb.blankDisplay();
  drawMyChar(19,0,0,0x00FF00);//in green
  unsigned long startTime = millis();
  while(millis() < (startTime + (time*1000)))
  {/*do nothing until it is time*/}
  byte tempBrightness = Brightness;//save brightness
  Brightness = 255;//full brightness
  //Flash it a bit
  drawMyChar(19,0,0,0xFF0000);//in red
  delay(200);//pause for a bit
  Rb.blankDisplay();
  delay(200);//pause for a bit
  drawMyChar(19,0,0,0xFF0000);//in red
  delay(200);//pause for a bit
  Rb.blankDisplay();
  delay(200);//pause for a bit
  drawMyChar(19,0,0,0xFF0000);//in red
  delay(200);//pause for a bit
  Rb.blankDisplay();
  delay(200);//pause for a bit
  drawMyChar(19,0,0,0xFF0000);//in red
  delay(2000);//pause for a bit
  Brightness = tempBrightness;//restore brightness
  doDisplay();
}

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
