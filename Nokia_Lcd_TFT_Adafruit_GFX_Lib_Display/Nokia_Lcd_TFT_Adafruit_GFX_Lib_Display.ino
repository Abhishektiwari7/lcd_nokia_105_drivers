/* NOKIA TFT 105,C1-00,1616 DISPLAY 128x166, 65K COLORS, 1.8 INCH, CODE: ET-LCD-104176
  12 PIN
//-----------------------
  | 1:  GND             |
  | 2:  RESET           |
  | 3:  CS              |
  | 4:  GND             |
  | 5:  SDA -> MOSI     |
  | 6:  SCK             |
  | 7:  VDDI            |
  | 8:  VDD             |
  | 9:  GND             |
  | 10: LED-            |
  | 11: LED+            |
  | 12: GND             |
//-----------------------
*/
#ifdef ARDUINO_ARCH_AVR
const int _CS         = 10;
const int _RESET      = 12;
const int SCLK        = 13;
const int SID         = 11; //Mosi

#elif defined ARDUINO_ARCH_ESP32
const int _CS         = 18;
const int _RESET      = 19;
const int SCLK        = 23;
const int SID         = 21; //Mosi

#else 
    #error Processor not supported
#endif 


#include <Adafruit_GFX.h>
#include "lcd.h"
#include "image.h"

#define BAUD_RATE     9600
//Arduino uno: PWM Brightness pin 9: PB1: 15: OC1A
//ESP32: PWM Brightness pin 2
Nokia105 display( SID,  SCLK, _RESET, _CS);

void setup() {
if (LOG) {
  Serial.begin(BAUD_RATE);
  Serial.println("Display Debug");
}

display.initDisplay();
display.PWMinit();
display.setLcdBrightness(1000);  
display.setDrawPosition(128,160); 
display.backgroundColor(BLUE);
display.displayClear();
display.setLcdBrightness(1000); //16 BIT value only
}

void loop() {
for(int i = 1; i<55;i++){
displayFunctions(i);
display.setLcdBrightness(analogRead(4));
delay(3000);
display.displayClear();
}
//display.printDigit(analogRead(A2),10,10,GREEN,BLACK);
}

void displayFunctions(int test) {
  switch (test) {
  
  case 1:
    display.colorPalletTest();
    break;
  
  case 2:
    display.smpteTest();
    break;
  
  case 3:
    display.fillRectangle(10,10,18,128,WHITE);   // HEIGHTYPOS, WIDTHXPOS, STARTYPOS, STARTXPOS, ENDY,ENDX
    break;
  
  case 4:
    display.printString("abhishek tiwari",0,50,RED,YELLOW);  //charactare, horizontal position, vertical position
    break;
  
  case 5:
    display.printString("0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz123456789abcdefghijklmnop",0,2,PURPLE,BLACK);
    break;
  
  case 6:
   display.printBitmap(0,0,bitImage,128,100,RED); //x axis, y axis, bit map,bitmap size
    break;
  
  case 7:
    display.printBitmap(0,50,bitImage,128,100,ORANGE); //x axis, y axis, bit map,bitmap size
    break;
  
  case 8:
    display.printString("! @ # $ % ^ & *  () _ - + = < > ,",0,2,GREEN,BLACK);
    break;
  
  case 9:
    display.printString("",0,2,GREEN,BLACK);
    break;
  
  case 10:
    display.printString("blank             ",0,10,GREEN,BLACK);
    break;
  
  case 11:
    display.printString("[]",0,20,GREEN,BLACK);
    break;
  
  case 12:
    display.printStringChar("Smile, check 12234567889",2,80,GREEN,BLACK); //only 1 line, no next line like show str
    break;
  
  case 13:
    display.printSingleChar ('1',80,50,GREEN,BLACK);
    break;
  
  case 14:
    display.printSingleChar ('2',32,50,GREEN,BLACK);
    break;
  
  case 15:
    display.printSingleChar ('3',80,90,GREEN,BLACK);
    break;
  
  case 16:
    display.printSingleChar ('4',32,90,GREEN,BLACK);
    break;
  
  case 17:
    display.lineHorixontal(10,80,100,WHITE);     //VERTICAL,HORIZONTAL, LENGTH
    break;
  
  case 18:
    display.lineVertical(10,80,100,GREEN);      //PEHLA -- UPER LE JATA HAIN,DUSRA ++ ULTEY HATH KE TARAF LATA HAIN,
    break;
  
  case 19:
    display.drawPixel(100,100,RED);
    break;
  
  case 20:
    display.displayClear();
    break;
  
  case 21:
   display.image1d ( 125, 110, 0,0, ram );
    break;
  
  case 22:
   // display.image2d ( 120, 80, 0,0, abhishekme ); //working byt need to define array[][y]
    break;
  
  case 23:
    display.image1d ( 125, 110, 0,0, abhishek );
    break;
  
  case 24:
    display.drawtext('8',50,50,BLUE);
    break;
  
  case 25:
    display.circle(64, 83, 50, YELLOW);
    break;
  
  case 26:
    display.printDigit(100,10,10,GREEN,BLACK);
    break;
  
  case 27:
    display.printString("a",0,2,GREEN,BLACK);
    break;
    
  case 28:
    display.setTextSize(3);
    display.print("BIG Size");
    break;
  
  case 29:
    display.setTextColor(WHITE);
    display.print("Color");
    break;
  
  case 30:
    display.setCursor(2, 2);
    break;
  
  case 31:
    display.setTextColor(GREEN);
    display.setTextSize(1);
    display.print("using gfx library");
    break;
  
  case 32:
    display.drawFastVLine(50,50,50,GREEN); //worked
    break;
  
  case 33:
    display.drawFastHLine(50,50,50,GREEN); //worked
    break;
  
  case 34:
    display.drawRect(50,50,50,50,GREEN); //worked
    break;
  
  case 35: 
    display.fillRect(50,50,50,50,GREEN); //worked
    break;
  
  case 36:
    display.fillScreen(GREEN); //worked
    break;
  
  case 37:
    display.drawLine(2,0,20,20,GREEN); //worked
    break;
  
  case 38:
    display.drawCircle(50,50,10,GREEN); //worked
    break;
  
  case 39:
    display.drawCircleHelper(50,50,60,10,GREEN); //worked
    break;
  
  case 40:
    display.fillCircle(50,50,10,GREEN); ////worked
    break;
  case 41:
    display.fillCircleHelper(50,50,20,10,2,GREEN); //worked
    break;
    
  case 42:
    display.drawTriangle(50,40,70,50,80,90,GREEN);//worked
    break;
    
  case 43:
    display.fillTriangle(50,40,70,50,80,90,GREEN);  //worked
    break;

  case 44:
    display.drawRoundRect(50,50,50,50,5,GREEN); //worked
    break;
  
  case 45:
    display.fillRoundRect(50,50,70,70,5,GREEN); //worked
    break;
  
  case 46:
    display.drawBitmap(10, 10, bitImage, 128,100, GREEN); //worked
    break;
  
  case 47:
    display.drawBitmap(10, 10, bitImage, 128,100, ORANGE,BLACK); //worked
    break;
  
  case 48:
    display.drawBitmap(10, 10, bitImage, 128,100, PINK); //*bitmap //worked
    break;
  
  case 49:
    display.drawBitmap(10, 10, bitImage, 128,100, WHITE,BLACK); //worked
    break;
  
  case 50:
    display.drawRGBBitmap(10, 10, ram, 125,110);
    break;
  
  case 51:
    display.drawRGBBitmap(10, 10, ram, 125,110);
    break;
  
  case 52:
    display.drawChar(10, 10, 'a', GREEN,YELLOW, 5); //SIZE
    break;
  
  case 53:
    display.drawChar(1, 1, 'A', WHITE,GREEN, 5, 5); //STRECHdisplay.setTextColor(WHITE);
    break;
  
  case 54:
    //code
    break;
  
  case 56:
    //code
    break;
  
  case 57:
    //code
    break;
  
  case 58:
    //code
    break;
  
  case 59:
    //code
    break;
  
  case 60:
    //code
    break;
  
  case 61:
    //code
    break;
  
  case 62:
    //code
    break;
  
  case 63:
    //code
    break;
  
  case 64:
    //code
    break;

  default:
    // statements
    break;
  }
}
/* IMPORTANT LINK:
  1.https://forum.arduino.cc/t/nokia-c1-1616-display/931571/8 
  2.https://github.com/fire0shadow/Adafruit-SPFD54124B
  3.http://uglyduck.vajn.icu/PDF/lcd/Nokia1202/Nokia_1202_LCD.html
  4.https://github.com/fire0shadow/Adafruit-SPFD54124B/blob/master/examples/SPFD54124B.ino
  5.https://github.com/SodaqMoja/Sodaq_DS3231/issues/5
  6.https://www.avrfreaks.net/forum/atmega328p-nokia-1616-tft-color-streaking
  7.https://github.com/Bodmer/TFT_eSPI
  8.https://www.amobbs.com/thread-5099383-1-1.html
  9.https://chowdera.com/2021/07/20210726190916525c.html
  10.https://github.com/FeatherHunter/STM32F4/blob/master/HARDWARE/LCD/lcd.h
  11.https://roboticsbackend.com/arduino-fast-digitalwrite/
  12.https://docs.google.com/spreadsheets/d/11NiqGfhY6hEn82NVkdbyhqLUGae0Sy1m_vwhqrpiItQ/edit#gid=1863870689
  13.https://radiokot.ru/forum/viewtopic.php?f=61&t=78889&sid=bd6f4e0a31b3de5cc3a78866aa3be72b
  14.https://www.radiokot.ru/konkursCatDay2017/44/
  15.https://forum.arduino.cc/t/problem-with-nokia-1616-lcd-adafruit-library/695003
  16.https://arduino.ru/forum/apparatnye-voprosy/tft-nokia-1661
  17.https://github.com/Sisoog/Nokia-1616-1661-TFT-Library/releases
  18.https://github.com/cbm80amiga
  jackpot
  19.https://radiokot.ru/articles/53/
  20.https://we-easyelectronics-ru.translate.goog/lcd_gfx/podklyuchenie-displeya-nokia-1616-na-primere-lpc1343.html?_x_tr_sch=http&_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en-GB
  21.http://we.easyelectronics.ru/lcd_gfx/osvaivaem-displei-ot-mobilnyh-telefonov.html
  22.https://serdisplib-sourceforge-net.translate.goog/ser/nokcol_15g10.html?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en-GB&_x_tr_sch=http
  imagesprocessing
  23.https://avr64-com.translate.goog/1400/11/13/%D9%86%D9%85%D8%A7%DB%8C%D8%B4-%D8%B9%DA%A9%D8%B3-%D8%AF%D8%B1-lcd-nokia-1616-c1/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en-GB
  24.https://kbiva.wordpress.com/2015/07/05/nokia-6151-lcd/#more-1299
*/