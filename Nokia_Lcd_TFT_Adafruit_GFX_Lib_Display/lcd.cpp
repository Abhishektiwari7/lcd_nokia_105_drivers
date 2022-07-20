#include "lcd.h"
#include "fonts.h"
#include "cmd.h"
#include "Adafruit_GFX.h"

//------------------------------------------------------------------adafruit gfx added---------
Nokia105::Nokia105(int8_t SDA, int8_t SCLK, int8_t RST, int8_t CS): Adafruit_GFX(WIDTH,HEIGHT) {
  SPIDEVICE_SDA   = SDA; //Mosi
  SPIDEVICE_SCK  = SCLK;
  SPIDEVICE_RES   = RST; //misO
  SPIDEVICE_CS = CS;
  hwSPI = false;
}

void Nokia105::writeNokiaCommand (unsigned char Cmd) {
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

void Nokia105:: writeNokiaData (unsigned char Data) {
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

void Nokia105:: displayClear(void) {
unsigned int i;
writeNokiaCommand(NOKIA105_CASET);    //x-addres 0 to 0x83 //Ox2a
writeNokiaData(NOKIA105_NOP);         //xsta:BIT0-7 //0x00
writeNokiaData(NOKIA105_NOP);         //xend:BIT0-7
writeNokiaData(NOKIA105_NOP);         //xsta:BIT0-7
writeNokiaData(0x83);                 //xend:BIT0-7

writeNokiaCommand(NOKIA105_PASET);    //y-address 0 to 0xa1 //0x2b
writeNokiaData(NOKIA105_NOP);         //Ysta:BIT0-7
writeNokiaData(NOKIA105_NOP);         //xend:BIT0-7
writeNokiaData(NOKIA105_NOP);         //xsta:BIT0-7
writeNokiaData(0xa1);                 //xend:BIT0-7

writeNokiaCommand(NOKIA105_RAMWR);     //RAMWR //0x2c
  
for(i=0; i < totalPixals; i++) {
  writeNokiaData(NOKIA105_NOP);
  writeNokiaData(NOKIA105_NOP);
}
}

void Nokia105:: initDisplay(void) {
pinMode(SPIDEVICE_CS, OUTPUT);
pinMode(SPIDEVICE_RES, OUTPUT);
pinMode(SPIDEVICE_SDA, OUTPUT);
pinMode(SPIDEVICE_SCK, OUTPUT);

LCD_RES_Low();
delay(10);
LCD_RES_High();
delay(10);
writeNokiaCommand(NOKIA105_SPLOUT); //delay,sleep out//0x11
delay(10);
writeNokiaCommand(NOKIA105_COLMOD);          //Interface pixel format:bit1,2,3 //0x3a, color mode
writeNokiaData(0x05);         // 8 bit color
writeNokiaCommand(NOKIA105_DISPON);          //Display on,0x29=ON,0x28=OFF
displayClear();
}

void Nokia105:: setDrawPosition(unsigned char x, unsigned char y) {
writeNokiaCommand(NOKIA105_CASET);      //x-addres//0x2a
writeNokiaData(NOKIA105_NOP);           //xsta:BIT0-7
writeNokiaData(x+2);                    //xsta:BIT0-7
writeNokiaData(NOKIA105_NOP);           //xend:BIT0-7
writeNokiaData(0x83);                   //xend:BIT0-7

writeNokiaCommand(NOKIA105_PASET);      //y-address //0x2b
writeNokiaData(NOKIA105_NOP);           //xend:BIT0-7
writeNokiaData(y);                      //Ysta:BIT0-7
writeNokiaData(NOKIA105_NOP);           //xend:BIT0-7
writeNokiaData(0xA1);                   //xend:BIT0-7

writeNokiaCommand(NOKIA105_RAMWR);      //RAMWR//0x2c
}


void Nokia105::setDrawPositionAxis (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
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

writeNokiaCommand(NOKIA105_CASET); // Column addr set//0x2a
writeNokiaData(0); writeNokiaData(x0);   // X start
writeNokiaData(0); writeNokiaData(x1);   // X end

writeNokiaCommand(NOKIA105_PASET); // Page addr set 0x2b
writeNokiaData(0); writeNokiaData(y0);   // Y start
writeNokiaData(0); writeNokiaData(y1);   // Y end

writeNokiaCommand(NOKIA105_RAMWR);//0x2c
}


void Nokia105:: drawPixel(int16_t x, int16_t y, uint16_t color) {
if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT))
    return;

setDrawPositionAxis(x, y, x, y);
writeNokiaData(color >> 8);
writeNokiaData(color);
}


void Nokia105:: image1d (uint16_t w, uint16_t h, uint16_t shiftX,uint16_t shiftY, const uint16_t image[] ) {
int l = 0;
for (int y = 0; y < h; y++) { //h
  for (int x = 0; x < w; x++) { //w
    drawPixel( x+shiftX, y+shiftY, pgm_read_word(&(image[l])));
    l++;
    }
  }
}
//--------------------------------Working---------------------------------------------------
//const uint16_t (image[][80] //parsing of 2d array like this
/* void Nokia105:: image2d (int w, int h, int shiftX,int shiftY, const uint16_t image[][] ) {
int l = 0;
for (int y = 0; y < h; y++) { //h
  for (int x = 0; x < w; x++) { //w
    drawPixel(x+shiftX , y+shiftY, pgm_read_word(&(image[y][x])));
    l++;
    }
  }
}
*/
//--------------------------------beta test-------------------------------------------------
void Nokia105:: drawtext(unsigned char c, unsigned char x, unsigned char y ,uint16_t color) {
unsigned char k,Mline,Ctemp;
setDrawPosition(x,y);

c -= 0x20; //asic value conversion

for (Mline = 0; Mline < 16; Mline++) {
  Ctemp = text[c][Mline];
  for(k = 0; k < 8; k++) {
    if(Ctemp & 0x80) {
      writeNokiaData(color>>8); 
      writeNokiaData(color);
    } else {
      writeNokiaData(0x00>>8); 
      writeNokiaData(0x00);
    }
    Ctemp=Ctemp>>1;
  }
  setDrawPosition(x,++y); 
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

setDrawPositionAxis(x, y, x+w-1, y+h-1);

uint8_t hi = color >> 8, lo = color;
int32_t i  = (int32_t)w * (int32_t)h;

while(i--) {
   writeNokiaData(hi);
   writeNokiaData(lo);
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

void Nokia105:: printBitmap(int16_t x, int16_t y, const uint8_t bitmap[],int16_t w, int16_t h, uint16_t color) {  
int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
//int16_t byteWidth = 16.8;

uint8_t byte = 0;

for (int16_t j = 0; j < h; j++, y++) {
  for (int16_t i = 0; i < w; i++) {
    if (i & 7)
      byte <<= 1;
    else
      byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
    if (byte & 0x80) {
        drawPixel(x + i, y, color);
    }
  }
}
}

void Nokia105:: backgroundColor(uint16_t c) {
uint8_t x, y, hi = c >> 8, lo = c;
setDrawPositionAxis(0, 0, WIDTH-1, HEIGHT-1);

for( y = HEIGHT; y > 0; y--) {
  for(x = WIDTH; x > 0; x--) {
    writeNokiaData(hi);
    writeNokiaData(lo);
  }
}
}

void Nokia105:: colorPalletTest() {
int colorPallete[] = {WHITE,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,NAVY,DARKGREEN,DARKCYAN,MAROON,PURPLE,OLIVE,LIGHTGREY,DARKGREY,ORANGE,PINK};
  for(int i=0; i < sizeof(colorPallete); i++) {
    //Generate complete frame
      for (int y = 0; y < HEIGHT-1; y++) {
      for (int x = 0; x < WIDTH-1; x++) {
        writeNokiaData(colorPallete[i]>>8); //16 bit color chunks me jaengy
        writeNokiaData(colorPallete[i]);
      }
    }
    displayClear(); //by this no over lapping  
  }  
}


void Nokia105:: circle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
int16_t f = 1 - r;
int16_t ddF_x = 1;
int16_t ddF_y = -2 * r;
int16_t x = 0;
int16_t y = r;

drawPixel(x0, y0 + r, color);
drawPixel(x0, y0 - r, color);
drawPixel(x0 + r, y0, color);
drawPixel(x0 - r, y0, color);

while (x < y) {
  if (f >= 0) {
    y--;
    ddF_y += 2;
    f += ddF_y;
  }
  x++;
  ddF_x += 2;
  f += ddF_x;

  drawPixel(x0 + x, y0 + y, color);
  drawPixel(x0 - x, y0 + y, color);
  drawPixel(x0 + x, y0 - y, color);
  drawPixel(x0 - x, y0 - y, color);
  drawPixel(x0 + y, y0 + x, color);
  drawPixel(x0 - y, y0 + x, color);
  drawPixel(x0 + y, y0 - x, color);
  drawPixel(x0 - y, y0 - x, color);
}
}

void Nokia105::  printDigit(unsigned int a,int16_t x, int16_t y,uint16_t forgroundColor,uint16_t backgroundColor) {
//convet int to string to character the print data but overlapping problem occured when number 4 digit back to 2 digit need some display clear without blink effect   
/*uint8_t sizeofinput = HEIGHT*WIDTH ; //need to update //255
char connverter[sizeofinput];
String str = String(a);  
str.toCharArray(connverter,sizeofinput);
*/ //arduino heap memmory got mad at me//https://arduino.stackexchange.com/questions/42986/convert-int-to-char*
int count = int(log10(a) + 1);
char cstr[count];
itoa(a, cstr, 10); //https://cplusplus.com/reference/cstdlib/itoa/
//---------------------------------------upgrade required-------------------------------------------------------
printString(cstr,x,y,forgroundColor,backgroundColor);

//padding to avoid overlap text
if (a < 10) { //1 digit
  for(int i = 1; i<4; i++) 
  printString(" ",x+(i*8),y,backgroundColor,backgroundColor);
} else if ( a >= 10 && a < 100 )  { //2 digit
  printString(" ",x+16,y,backgroundColor,backgroundColor);
} else if ( a >= 100 && a < 1000 )  { //3 digit
  printString(" ",x+24,y,backgroundColor,backgroundColor);
}  else if ( a >= 1000 && a < 10000 )  { //4 digit
  printString(" ",x+32,y,backgroundColor,backgroundColor);
}

}

void Nokia105:: lineVertical(int16_t x, int16_t y, int16_t h, uint16_t color) {
if ((x < 0) || (x >= WIDTH ) || (y >= HEIGHT)) return; 
int16_t y2 = y + h - 1;

if (y2 < 0) return;

if (y2 >= HEIGHT) {
  h = HEIGHT - y;    // Clip bottom 
}

if (y < 0) {         // Clip top 
  h += y; y = 0; 
}

setDrawPositionAxis(x, y, x, y+h-1);

uint8_t hi = color >> 8, lo = color;
while (h--) {
  writeNokiaData(hi);
  writeNokiaData(lo);
}
}


void Nokia105:: lineHorixontal(int16_t x, int16_t y, int16_t w,uint16_t color) {
if((y < 0) || (y >= HEIGHT )|| (x >= WIDTH)) return; // Fully off top or bottom and Fully off right

int16_t x2 = x + w - 1;

if (x2 < 0) return;                   // Fully off left

if (x2 >= WIDTH) { // Clip right
  w = WIDTH - x;   
}
if (x < 0) { // Clip left
  w += x; x = 0; 
}         

setDrawPositionAxis(x, y, x+w-1, y);

uint8_t hi = color >> 8, lo = color;
while (w--) {
  writeNokiaData(hi);
  writeNokiaData(lo);
}
}

void Nokia105:: printSingleChar (unsigned char c,unsigned char x, unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor) {
unsigned char k,Mline,Ctemp;
setDrawPosition(x,y);
c -= 0x20;  //to get chracters from fonts

for (Mline = 0; Mline < 16; Mline++) { //font has 16 rows of data
  Ctemp = font8x16[c][Mline]; //one row extracted put in ctemp
  for(k = 0; k < 8; k++) { //ctemp, each rows has 8 bit of data of font or char
    if(Ctemp & 0x01) { //LSB, true: character bi present
      writeNokiaData(forgroundColor>>8); //print color at that position
      writeNokiaData(forgroundColor);
    } else {
      writeNokiaData(backgroundColor>>8); //BACKGOUND color
      writeNokiaData(backgroundColor);
    }
    Ctemp=Ctemp>>1; //next bit of data of current row
  }
  setDrawPosition(x,++y); //increase y position
}
}

void Nokia105:: printStringChar(unsigned char *String,unsigned char x,unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor) {
while ( * String ) {
  printSingleChar ( *String++,x,y,forgroundColor,backgroundColor);
  x+=8;     
}
}

void Nokia105:: printString(uint8_t*str,uint8_t x,uint8_t y,uint16_t forgroundColor, uint16_t backgroundColor) {                              
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
   if(LOG) {
     Serial.println("Single Char: ");
     Serial.println(*str);
   }
    printSingleChar (*str,x,y,forgroundColor,backgroundColor);
    str++; 
    x+=8; 
  }
} 

void Nokia105:: PWMinit() {
//https://forum.arduino.cc/t/pwm-pin-9-using-registers-solved/673183
DDRB |= 1<<DDB1; //PIN DIGITAL 9 AS OUTPUT

TCCR1A = 0; //DISABLE ALL FEATURE OF TIMERS
TCCR1B = 0; //DISABLE ALL FEATURE OF TIMERS

TIMSK1 = 0; //Interrupt Mask Register
TIFR1 = 0; // Interrupt Flag Register

ICR1 = 65535; //16 bit: 65535, TOP VALUE, Input Capture Flag

OCR1A = 0; //set Initial PWM =0

TCCR1B |= 1<<CS10; //max Frequency:~243hz/4.1millisecond, (CS12=0,CS11=0,CS10=1)N0 Prescalling,Control Register B

TCCR1A |= 1<<WGM11;  //mode 14
TCCR1B |= (1<<WGM13) | (1<<WGM12); //CTC1 (WGM13 bit set Fast PWM Mode)Clear OC1A on Compare Match, set OC1A at BOTTOM (non-inverting mode)

TCCR1A |= 1<<COM1A1;  // Enable Fast PWM on Pin 9: Set OC1A at BOTTOM and clear OC1A on OCR1A compare
}

void Nokia105:: setLcdBrightness(uint16_t PWM) {
OCR1A = map(PWM, 0, 1023, 0, 65535); //BOTTOM VALUE
}