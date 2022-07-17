#include "lcd.h"
#include "fonts.h"
#include "cmd.h"

Nokia105::Nokia105(int8_t SDA, int8_t SCLK, int8_t RST, int8_t CS) {
  SPIDEVICE_SDA   = SDA; //Mosi
  SPIDEVICE_SCK  = SCLK;
  SPIDEVICE_RES   = RST; //misO
  SPIDEVICE_CS = CS;
  hwSPI = false;
}

void Nokia105::LCD_Write_Cmd (unsigned char Cmd) {
  if (hwSPI) {
   //hardware spi add
  } else {
    uint8_t i,j;
    LCD_SCK_Low();          
    LCD_CS_Low();
    LCD_SDA_Low();
    LCD_SCK_High();
    LCD_SCK_Low();  
    j = 0x80;                         
    for (i = 0; i < 8;i++) {
      if (Cmd & j) {                                   
        LCD_SDA_High();   
        LCD_SCK_High();
        LCD_SCK_Low();
      } else {
        LCD_SDA_Low(); 
        LCD_SCK_High();
        LCD_SCK_Low();
      }
      j=j>>1;                           
    }
    LCD_CS_High();
  }
}

void Nokia105:: LCD_Write_Data (unsigned char Data) {
 if (hwSPI) {
   //hardware spi add
  } else {
    uint8_t i,j;
    LCD_SCK_Low();          
    LCD_CS_Low();
    LCD_SDA_High();
    LCD_SCK_High();
    LCD_SCK_Low();
    j = 0x80;                          
    for (i = 0; i < 8;i++) {
      if (Data & j) {                                   
        LCD_SDA_High();
        LCD_SCK_High();
        LCD_SCK_Low();
      } else {
        LCD_SDA_Low();
        LCD_SCK_High();
        LCD_SCK_Low();
      }
      j=j>>1;                          
    }
    LCD_CS_High();
  }
}

void Nokia105:: LCD_Clear(void) {
unsigned int i;
LCD_Write_Cmd(NOKIA105_CASET);     // x-addres 0to0x83 //Ox2a
LCD_Write_Data(NOKIA105_NOP);  //xsta:BIT0-7 //0x00
LCD_Write_Data(NOKIA105_NOP);  //xend:BIT0-7
LCD_Write_Data(NOKIA105_NOP);  //xsta:BIT0-7
LCD_Write_Data(0x83);  //xend:BIT0-7

LCD_Write_Cmd(NOKIA105_PASET);     // y-address 0to0xa1 0x2b
LCD_Write_Data(NOKIA105_NOP);  //Ysta:BIT0-7
LCD_Write_Data(NOKIA105_NOP);  //xend:BIT0-7
LCD_Write_Data(NOKIA105_NOP);  //xsta:BIT0-7
LCD_Write_Data(0xa1);  //xend:BIT0-7

LCD_Write_Cmd(NOKIA105_RAMWR);     //RAMWR //0x2c
  
for(i=0; i < totalPixals; i++) {
  LCD_Write_Data(NOKIA105_NOP);
  LCD_Write_Data(NOKIA105_NOP);
}
}

void Nokia105:: LCD_Init(void) {
pinMode(SPIDEVICE_CS, OUTPUT);
pinMode(SPIDEVICE_RES, OUTPUT);
pinMode(SPIDEVICE_SDA, OUTPUT);
pinMode(SPIDEVICE_SCK, OUTPUT);

LCD_RES_Low();
delay(10);
LCD_RES_High();
delay(10);
LCD_Write_Cmd(NOKIA105_SPLOUT); //delay,sleep out//0x11
delay(10);
LCD_Write_Cmd(NOKIA105_COLMOD);          //Interface pixel format:bit1,2,3 //0x3a, color mode
LCD_Write_Data(0x05);         // 8 bit color
//LCD_Write_Data(0x55);          //16 bit color mode NOT WORKING
LCD_Write_Cmd(NOKIA105_DISPON);          //Display on,0x29=ON,0x28=OFF
LCD_Clear();
}

void Nokia105:: LCD_SET_XY(unsigned char x, unsigned char y)
{
  LCD_Write_Cmd(NOKIA105_CASET);     // x-addres//0x2a
  LCD_Write_Data(NOKIA105_NOP);  //xsta:BIT0-7
  LCD_Write_Data(x+2);   //xsta:BIT0-7
  LCD_Write_Data(NOKIA105_NOP);  //xend:BIT0-7
  LCD_Write_Data(0x83);  //xend:BIT0-7
  
    LCD_Write_Cmd(NOKIA105_PASET);     // y-address //0x2b
  LCD_Write_Data(NOKIA105_NOP);  //xend:BIT0-7
  LCD_Write_Data(y);       //Ysta:BIT0-7
  LCD_Write_Data(NOKIA105_NOP);  //xend:BIT0-7
  LCD_Write_Data(0xA1);  //xend:BIT0-7

  LCD_Write_Cmd(NOKIA105_RAMWR);   //RAMWR//0x2c
}


void Nokia105::setWindow1 (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  uint8_t t0, t1;
  switch(rotation) {
    case 0:
    //no rotation
    break;
    
    case 1:
      t0 = WIDTH - 1 - y1;
      t1 = WIDTH - 1 - y0;
      y0 = x0;
      x0 = t0;
      y1 = x1;
      x1 = t1;
      break;
    case 2:
      t0 = x0;
      x0 = WIDTH  - 1 - x1;
      x1 = WIDTH  - 1 - t0;
      t0 = y0;
      y0 = HEIGHT - 1 - y1;
      y1 = HEIGHT - 1 - t0;
      break;
    case 3:
      t0 = HEIGHT - 1 - x1;
      t1 = HEIGHT - 1 - x0;
      x0 = y0;
      y0 = t0;
      x1 = y1;
      y1 = t1;
      break;
  }

  LCD_Write_Cmd(NOKIA105_CASET); // Column addr set//0x2a
  LCD_Write_Data(0); LCD_Write_Data(x0);   // X start
  LCD_Write_Data(0); LCD_Write_Data(x1);   // X end

  LCD_Write_Cmd(NOKIA105_PASET); // Page addr set 0x2b
  LCD_Write_Data(0); LCD_Write_Data(y0);   // Y start
  LCD_Write_Data(0); LCD_Write_Data(y1);   // Y end

  LCD_Write_Cmd(NOKIA105_RAMWR);//0x2c
}


void Nokia105:: onePixel(int16_t x, int16_t y, uint16_t color) {
if ((x < 0) || (x >= HEIGHT) || (y < 0) || (y >= WIDTH))
    return;

setWindow1(x, y, x, y);
LCD_Write_Data(color >> 8);
LCD_Write_Data(color);
}


void Nokia105:: image1d (int w, int h, int shiftX,int shiftY, const uint16_t image[] ) {
int l = 0;
for (int y = 0; y < h; y++) { //h
  for (int x = 0; x < w; x++) { //w
    onePixel( x+shiftX, y+shiftY, pgm_read_word(&(image[l])));
    l++;
    }
  }
}
//const uint16_t (image[][80] //parsing of 2d array like this
/* void Nokia105:: image2d (int w, int h, int shiftX,int shiftY, const uint16_t image[][] ) {
int l = 0;
for (int y = 0; y < h; y++) { //h
  for (int x = 0; x < w; x++) { //w
    onePixel(x+shiftX , y+shiftY, pgm_read_word(&(image[y][x])));
    l++;
    }
  }
}
*/
void Nokia105:: drawtext(unsigned char c, unsigned char x, unsigned char y ,uint16_t color) {
     unsigned char k,Mline,Ctemp;
    LCD_SET_XY(x,y);
    c -= 0x20; //asic value conversion
    
    for (Mline=0; Mline<16; Mline++) {
    Ctemp=text[c][Mline];
    for(k=0;k<8;k++) {
      if(Ctemp&0x80) {
        //LCD_Write_Data(0x07); //green color //fixed green color
        //LCD_Write_Data(0xe0); //orange
        LCD_Write_Data(Ctemp>>8); //customizable
        LCD_Write_Data(Ctemp);
      } else {
        LCD_Write_Data(0x00>>8); //BACKGOUND
        LCD_Write_Data(0x00);
      }
      Ctemp=Ctemp>>1;
      
    }
    LCD_SET_XY(x,++y); 
   }
}


void Nokia105:: fillRectangle (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= WIDTH) || (y >= HEIGHT)) return; // Fully off right or bottom
  int16_t x2, y2;
  if(((x2 = x + w - 1) < 0) ||
     ((y2 = y + h - 1) < 0)) return; // Fully off left or top
  if(x2 >= WIDTH)  w = WIDTH  - x; // Clip right
  if(x < 0) { w += x; x = 0; }       // Clip left
  if(y2 >= HEIGHT) h = HEIGHT - y; // Clip bottom
  if(y < 0) { h += y; y = 0; }       // Clip top

  setWindow1(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;
  int32_t i  = (int32_t)w * (int32_t)h;

  while(i--) {
     LCD_Write_Data(hi);
     LCD_Write_Data(lo);
  }
}

void Nokia105:: smpteTest() {
  fillRectangle(0,0,18,128,WHITE); // HEIGHTYPOS, WIDTHXPOS, STARTYPOS, STARTXPOS, ENDY,ENDX
  fillRectangle(18,0,36,128,BLUE); // HEIGHT, WIDTH
  fillRectangle(36,0,54,128,RED); // HEIGHT, WIDTH
  fillRectangle(54,0,72,128,GREEN); // HEIGHT, WIDTH
  fillRectangle(72,0,90,128,CYAN); // HEIGHT, WIDTH
  fillRectangle(90,0,108,128,MAGENTA); // HEIGHT, WIDTH
  fillRectangle(108,0,126,128,YELLOW); // HEIGHT, WIDTH
  fillRectangle(126,0,190,128,BLACK); // HEIGHT, WIDTH
}

void Nokia105:: drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[],int16_t w, int16_t h, uint16_t color) {  
//int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
int16_t byteWidth = 16.8;

uint8_t byte = 0;

for (int16_t j = 0; j < h; j++, y++) {
  for (int16_t i = 0; i < w; i++) {
    if (i & 7)
      byte <<= 1;
    else
      byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
    if (byte & 0x80) {
      if (rotateBitmap90){
        onePixel(x + i, y, color);
        } else
        onePixel(y, x + i, color);
    }
  }
}
}

void Nokia105:: backgroundColor(uint16_t c) {
uint8_t x, y, hi = c >> 8, lo = c;
setWindow1(0, 0, HEIGHT-1, WIDTH-1);

for(y=WIDTH; y>0; y--) {
  for(x=HEIGHT; x>0; x--) {
    LCD_Write_Data(hi);
    LCD_Write_Data(lo);
  }
}
}

void Nokia105:: colorPalletTest() {
int colorPallete[] = {WHITE,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,NAVY,DARKGREEN,DARKCYAN,MAROON,PURPLE,OLIVE,LIGHTGREY,DARKGREY,ORANGE,PINK};
  for(int i=0; i < sizeof(colorPallete); i++) {
    //Generate complete frame
      for (int y = 0; y < HEIGHT-1; y++) {
      for (int x = 0; x < WIDTH-1; x++) {
        LCD_Write_Data(colorPallete[i]>>8); //16 bit color chunks me jaengy
        LCD_Write_Data(colorPallete[i]);
      }
    }
    LCD_Clear(); //by this no over lapping  
  }  
}


void Nokia105:: circle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  
  onePixel(x0, y0 + r, color);
  onePixel(x0, y0 - r, color);
  onePixel(x0 + r, y0, color);
  onePixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    onePixel(x0 + x, y0 + y, color);
    onePixel(x0 - x, y0 + y, color);
    onePixel(x0 + x, y0 - y, color);
    onePixel(x0 - x, y0 - y, color);
    onePixel(x0 + y, y0 + x, color);
    onePixel(x0 - y, y0 + x, color);
    onePixel(x0 + y, y0 - x, color);
    onePixel(x0 - y, y0 - x, color);
  }
}

void Nokia105::  printDigit( int a, unsigned char x, unsigned char y,uint16_t forgroundColor,uint16_t backgroundColor) {
//convet int to string to character   
/*uint8_t sizeofinput = HEIGHT*WIDTH ;
char connverter[sizeofinput];
String str;

str = String(a);
str.toCharArray(connverter,sizeofinput); 
Show_Str(connverter,x,y,forgroundColor,backgroundColor);
*/

uint8_t sizeofinput = HEIGHT*WIDTH ; //need to update //255
char connverter[sizeofinput];
String str;
str = String(a);
str.toCharArray(connverter,sizeofinput);
//padding to avoid overlap text

Show_Str(connverter,x,y,forgroundColor,backgroundColor);
if (a < 10) { //1 digit
  for(int i = 1; i<4; i++) 
  Show_Str(" ",x+(i*8),y,backgroundColor,backgroundColor);
} else if ( a >= 10 && a < 100 )  { //2 digit
  Show_Str(" ",x+16,y,backgroundColor,backgroundColor);
} else if ( a >= 100 && a < 1000 )  { //3 digit
  Show_Str(" ",x+24,y,backgroundColor,backgroundColor);
}  else if ( a >= 1000 && a < 10000 )  { //4 digit
  Show_Str(" ",x+32,y,backgroundColor,backgroundColor);
}
}

void Nokia105:: lineHorixontal(int16_t x, int16_t y, int16_t h, uint16_t color) {

  if((x < 0) || (x >= HEIGHT)           // Fully off left or right //note i flipped the height and width in code
             || (y >= WIDTH)) return; // Fully off bottom
  int16_t y2 = y + h - 1;
  if(y2 < 0) return;                    // Fully off top
  if(y2 >= WIDTH) h = WIDTH - y;    // Clip bottom //note i flipped the height and width in code
  if(y < 0) { h += y; y = 0; }          // Clip top
  setWindow1(x, y, x, y+h-1);

  uint8_t hi = color >> 8, lo = color;
  while (h--) {
    LCD_Write_Data(hi);
    LCD_Write_Data(lo);
  }
}


void Nokia105:: lineVertical(int16_t x, int16_t y, int16_t w,uint16_t color) {
  if((y < 0) || (y >= WIDTH)         // Fully off top or bottom//note i flipped the height and width in code
             || (x >= HEIGHT)) return; // Fully off right
  int16_t x2 = x + w - 1;
  if(x2 < 0) return;                   // Fully off left
  if(x2 >= HEIGHT) w = HEIGHT - x;     // Clip right //note i flipped the height and width in code
  if(x < 0) { w += x; x = 0; }         // Clip left
  setWindow1(x, y, x+w-1, y);

  uint8_t hi = color >> 8, lo = color;
  while (w--) {
    LCD_Write_Data(hi);
    LCD_Write_Data(lo);
  }
}




void Nokia105:: LCD_Show_Char8(unsigned char c,unsigned char x, unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor) {
    unsigned char k,Mline,Ctemp;
    LCD_SET_XY(x,y);
    c -= 0x20;
    
    for (Mline=0; Mline<16; Mline++) {
    Ctemp=font8x16[c][Mline];
    for(k=0;k<8;k++) {
      if(Ctemp&0x01) {
        /*LCD_Write_Data(0x07); //green color //fixed green color
        LCD_Write_Data(0xe0); //orange*/
        LCD_Write_Data(forgroundColor>>8); //customizable
        LCD_Write_Data(forgroundColor);
      } else {
        LCD_Write_Data(backgroundColor>>8); //BACKGOUND
        LCD_Write_Data(backgroundColor);
      }
      Ctemp=Ctemp>>1;
    }
    LCD_SET_XY(x,++y); 
   }
}
void Nokia105:: LCD_Show_String8 (unsigned char *String,unsigned char x,unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor) {
while ( * String ) {
  LCD_Show_Char8( *String++,x,y,forgroundColor,backgroundColor);
  x+=8;     
  }
}

void Nokia105:: Show_Str(uint8_t*str,uint8_t x,uint8_t y,uint16_t forgroundColor, uint16_t backgroundColor) {                              
while(*str!=0) { 
  if (x > nextLineEdge) {        // old->112          
     y += spaceBetweenScanLines; //old->16
     x = 0;                     //reset the position to rewrite from start position of x
    if (LOG){
      Serial.println("RESET X = 0 ");
    }
       
   }                 
   if (y > fullLengthVertical) //old->144 //OVERFLOW EXIT OR ERROR ADD UP
    break;      
    
   /*if (*str == 13) {         
     y+=spaceBetweenScanLines;
     x = 0;
     str++;
     Serial.println("STR == 13 ");
     Serial.println(*str); 
    } else {
      Serial.println("lcd show char8: ");
      Serial.println(*str);
      LCD_Show_Char8(*str,x,y);
    }*/ //OLD METHOD, FUTURE RESEARCH
    if(LOG) {
      Serial.println("lcd show char8: ");
      Serial.println(*str);
    }
    LCD_Show_Char8(*str,x,y,forgroundColor,backgroundColor);
    str++; 
    x+=8; 
  }
} 