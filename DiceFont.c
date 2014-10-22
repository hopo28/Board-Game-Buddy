/* -FONT FOR ELECTRONIC DICE-
 * First one is blank
 * Next 6 are 2D6(1) (top left)
 * Next 6 are 2D6(2) (bottom right)
 * Next 6 are 1D6 (full screen)
*/
#include <avr/pgmspace.h>

const unsigned char diceFont[][8] PROGMEM=
{//col1,col2,col3,col4,col5,col6,col7,col8
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //Blank
  {0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00}, //2D6(1) 1
  {0x01,0x00,0x10,0x00,0x00,0x00,0x00,0x00}, //2D6(1) 2
  {0x01,0x04,0x10,0x00,0x00,0x00,0x00,0x00}, //2D6(1) 3
  {0x11,0x00,0x11,0x00,0x00,0x00,0x00,0x00}, //2D6(1) 4
  {0x11,0x04,0x11,0x00,0x00,0x00,0x00,0x00}, //2D6(1) 5
  {0x15,0x00,0x15,0x00,0x00,0x00,0x00,0x00}, //2D6(1) 6
  {0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00}, //2D6(2) 1
  {0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x80}, //2D6(2) 2
  {0x00,0x00,0x00,0x00,0x00,0x08,0x20,0x80}, //2D6(2) 3
  {0x00,0x00,0x00,0x00,0x00,0x88,0x00,0x88}, //2D6(2) 4
  {0x00,0x00,0x00,0x00,0x00,0x88,0x20,0x88}, //2D6(2) 5
  {0x00,0x00,0x00,0x00,0x00,0xA8,0x00,0xA8}, //2D6(2) 6
  {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00}, //1D6 1
  {0x03,0x03,0x00,0x00,0x00,0x00,0xC0,0xC0}, //1D6 2
  {0x03,0x03,0x00,0x18,0x18,0x00,0xC0,0xC0}, //1D6 3
  {0xC3,0xC3,0x00,0x00,0x00,0x00,0xC3,0xC3}, //1D6 4
  {0xC3,0xC3,0x00,0x18,0x18,0x00,0xC3,0xC3}, //1D6 5
  {0xDB,0xDB,0x00,0x00,0x00,0x00,0xDB,0xDB}, //1D6 6
  {0x3C,0x52,0x91,0x91,0x9D,0x81,0x42,0x3C}, //Timer
  {0x92,0x54,0x00,0xD6,0x00,0x54,0x92,0x00}};//Brightness