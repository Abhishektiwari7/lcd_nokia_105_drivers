/**************************************************************************
  This is a library for Nokia 1.8" displays based on SPFD54124B drivers.
  Works with the Custom 1.8" Display Breakout board
  Check out the Github of Library for our tutorials and wiring diagrams.
  These displays use 9-bit SPI to communicate, 3 pins are required to
  interface.
  Abhishek Tiwari invests time and resources providing this open source code,
  please support Abhishek Tiwari and open-source hardware by Providing some Credits or mentions.
  Written by Abhishek Tiwari.

  This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **************************************************************************/

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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "lcd.h"
#include "image.h"

//---bitbang pins------------------------------
const int _CS    = 15;
const int _RESET = 19;
const int _SCLK  = 18;
const int _SID   = 23; // Mosi
//Backlight Default Gpio4 //PWM BRIGHTNESS
Nokia105 display( _SID,  _SCLK, _RESET, _CS);
//--------------------------------------------
//Nokia105 display( 0, 0, 0, 0); //for internal SPI hardware default vspi
void displayFunctions(int test) {
  switch (test) {

  case 1:
    display.colorPalletTest();
    break;

  case 2:
    display.smpteTest();
    break;

  case 3:
    display.fillRectangle(0,0,18,128,WHITE);   // HEIGHTYPOS, WIDTHXPOS, STARTYPOS, STARTXPOS, ENDY,ENDX
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
    display.printBitmap(0,50,bitImage,128,100,GREEN); //x axis, y axis, bit map,bitmap size
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
    display.printString("/{}[]",0,20,GREEN,BLACK);
    break;

  case 12:
    display.printStringChar("check no next line",2,80,GREEN,BLACK); //only 1 line, no next line like show str
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
    display.circle(64, 83, 50, YELLOW);
    break;

  case 21:
   display.image1d ( 125, 110, 0,0, ram );
    break;

  case 22:
   display.printString("a",0,2,GREEN,BLACK);
    break;

  case 23:
   display.image1d ( 125, 110, 0,0, abhishek );
    break;

  case 24:
   display.printDigitInteger(111000999,40,40,GREEN,BLACK); //limit 999,999,999
    break;

  case 25:
    display.displayClear();
    break;

  case 26: {
      float Fnum = 1234562.1234;
      uint8_t digits = 4;
      display.printDigitFloat(Fnum,digits,1,10,GREEN,BLACK);
    }
    break;

  case 27:
    // display.image2d ( 120, 80, 0,0, abhishekme ); //working by need to define array[][y]
    break;
  
  case 28:
    // display.drawtext('8',50,50,BLUE);
    break;
  
  default:
    // statements
    break;
  }
}
extern "C" void app_main(void) {

display.initDisplay();
display.PWMinit();
display.setLcdBrightness(100);   //max 512, duty cycle
display.setDrawPosition(128,160);
display.backgroundColor(BLUE);
display.displayClear();
ESP_LOGI(TAG, "APP main - Display Setup Completed, Functions are Activated");

while (1) {
  for(int i = 1; i < 28; i++) {
    displayFunctions(i);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
   display.displayClear();
 }
//display.setLcdBrightness(analogRead(4));
//display.printDigit(analogRead(A2),10,10,GREEN,BLACK);
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
