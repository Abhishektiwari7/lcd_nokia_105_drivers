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
#pragma once

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

//----------------------Set or Clear of bit-----digitalwrite is very slow----------------------------
#define LCD_RES_High()   { gpio_set_level((gpio_num_t)SPIDEVICE_RES, 1); }
#define LCD_RES_Low()    { gpio_set_level((gpio_num_t)SPIDEVICE_RES, 0); }

//---------------------------PWM----------------------------------------------------------------------
const int BACK_LIGHT = 4;

// // Set the LEDC peripheral configuration
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_HIGH_SPEED_MODE
#define LEDC_OUTPUT_IO          BACK_LIGHT         // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_9_BIT  //LEDC_TIMER_13_BIT //for 5khz Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (100000)          //(5000) // Frequency in Hertz. Set frequency at 5 kHz

//----------------------------macros to Manipulate display-----------------------------------
#define WIDTH          130
#define HEIGHT         167             //old->160
#define nextLineEdge   128             //printString
#define spaceBetweenScanLines  16      //printString, 2 lines ke beech ka distance
#define fullLengthVertical    160      //164 characters on display
#define rotation        0              //SCREEN ROTATION 0 by default
#define rotateBitmap90  0              //1-> no rotaion 90,0-> yes rorate 90
#define LOG             1              //to activate serial
#define totalPixals     WIDTH*HEIGHT   //21384
#define RGB2BGR         1              //0: RGB,1: BGR, color seems to off.means: blue become red or vice versa. green remain same

//----------predefined 16 bit colors
#define BLACK             0x0000
#define NAVY              0x000F
#define DARKGREEN         0x03E0
#define DARKCYAN          0x03EF
#define MAROON            0x7800
#define PURPLE            0x780F
#define OLIVE             0x7BE0
#define LIGHTGREY         0xC618
#define DARKGREY          0x7BEF
#define BLUE              0x001F
#define GREEN             0x07E0
#define CYAN              0x07FF
#define RED               0xF800
#define MAGENTA           0xF81F
#define YELLOW            0xFFE0
#define WHITE             0xFFFF
#define ORANGE            0xFD20
#define GREENYELLOW       0xAFE5
#define PINK              0xF81F
static const char* TAG = "MyDisplayLogs"; //for monitor
//---------------------------nokia display functions libraries overlap with adafruit gfx library------

class Nokia105 final {
	public:
  Nokia105( uint8_t  SID, uint8_t SCLK, uint8_t RST, uint8_t CS);
	/**********************************************************************/
  /*!
    @brief    Pin defination
    @param    SPIDEVICE_CS, SPIDEVICE_RES, SPIDEVICE_SDA or Mosi, SPIDEVICE_SCK
    spi proceed by defined gpio.
  */
  /**********************************************************************/
	
  int countDigit(long long n); 
  /**********************************************************************/
  /*!
    @brief    Count number of digits
    @param    to get number on digits in input passing number 
              :) used to give space in float print
  */
  /**********************************************************************/

	void	initDisplay(),
  /**********************************************************************/
  /*!
    @brief    lcd initialize
    @param    
  */
  /**********************************************************************/
      
  PWMinit(),    
  /**********************************************************************/
  /*!
    @brief    start the inbuilt timers to generate pwm
    @param    
  */
  /**********************************************************************/

  setLcdBrightness(unsigned int PWM),    
  /**********************************************************************/
  /*!
    @brief    simple map the input 16 bit values to counter
    @param    
  */
  /**********************************************************************/

  setDrawPosition(unsigned char x, unsigned char y),
  /**********************************************************************/
  /*!
    @brief    set window cursor to push colors
    @param    x-> number of pixels in x axis or horizontal, y>x-> number of pixels in y axis or vertical
  */
  /**********************************************************************/
        
  setDrawPositionAxis(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1),
  /**********************************************************************/
  /*!
    @brief    set window cursor to push colors
    @param    x-> number of pixels in x axis or horizontal, y>x-> number of pixels in y axis or vertical
    x0: start position | x1: end position 
    y0: start position | y1: end position
  */
  /**********************************************************************/
  
  drawPixel(int16_t x, int16_t y, uint16_t color), 
	pushFastPixel(uint32_t length, const void* color),
  /**********************************************************************/
  /*!
    @brief    as function name says, it drae 1 pixel on screen
    @param    x: horizonal position, y: vertical position,color: 16 bit color in hex
    only drawPixel added along with lcd.cpp->height and width
              length : total number of pixel being send.
  */
  /**********************************************************************/
	
	image1d (uint16_t w, uint16_t h, uint16_t shiftX,uint16_t shiftY, const uint16_t * image ),
	/**********************************************************************/
  /*!
    @brief    Pin defination
    @param    
  */
  /**********************************************************************/
	
	/*image2d (int w, int h, int shiftX,int shiftY, const uint16_t image[][80] ),*/
  /**********************************************************************/
  /*!
    @brief    Pin defination
    @param    
  */
  /**********************************************************************/
				
	printDigitInteger(int32_t Inumber, int16_t x, int16_t y,uint16_t forgroundColor,uint16_t backgroundColor),
  /**********************************************************************/
  /*!
    @brief    digit print working upto 999,999,999 only unsigned integers 
    @param    
  */
  /**********************************************************************/

  printDigitFloat(double fnumber,uint8_t digits, int16_t x, int16_t y,uint16_t forgroundColor,uint16_t backgroundColor),
  /**********************************************************************/
  /*!
    @brief    float digit print function
    @param    fnumber is float number. digits is how many digit after 
              point need to print on screen.
  */
  /**********************************************************************/

	drawtext(unsigned char c, unsigned char x, unsigned char y ,uint16_t color),
  /**********************************************************************/
  /*!
    @brief    as per function name. it draw the text but it is in beta.
    @param    
  */
  /**********************************************************************/
				
	fillRectangle (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
  /**********************************************************************/
  /*!
    @brief    rectanglle  shape color 
    @param    
  */
  /**********************************************************************/
	
	smpteTest(),
  /**********************************************************************/
  /*!
    @brief    colorfull rectangles
    @param    
  */
  /**********************************************************************/
	
	printBitmap(int16_t x, int16_t y, const uint8_t bitmap[],int16_t w, int16_t h, uint16_t color),
	/**********************************************************************/
  /*!
    @brief    bitmap 
    @param    
  */
  /**********************************************************************/
	
	backgroundColor(uint16_t c),
  /**********************************************************************/
  /*!
    @brief    fill the screen by passing the color value
    @param    
  */
  /**********************************************************************/
	
	colorPalletTest(),
  /**********************************************************************/
  /*!
    @brief    colors flash on screen
    @param   
  */
  /**********************************************************************/
	
	lineHorixontal(int16_t x, int16_t y, int16_t h, uint16_t color),
  /**********************************************************************/
  /*!
    @brief    horizontal line
    @param    
  */
  /**********************************************************************/
	
	lineVertical(int16_t x, int16_t y, int16_t w,uint16_t color),
  /**********************************************************************/
  /*!
    @brief    vertical line
    @param    
  */
  /**********************************************************************/
				
  circle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
  /**********************************************************************/
  /*!
    @brief    draw circle
    @param    
  */
  /**********************************************************************/
	
	printSingleChar(unsigned char c,unsigned char x, unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor),
  /**********************************************************************/
  /*!
    @brief    single charatcer only
    @param   
  */
  /**********************************************************************/
	
	printStringChar ( char *String,unsigned char x,unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor),
  /**********************************************************************/
  /*!
    @brief    string of data without next line feature
    @param    .
  */
  /**********************************************************************/
	
	printString(char *str,uint8_t x,uint8_t y,uint16_t forgroundColor, uint16_t backgroundColor),
  /**********************************************************************/
  /*!
    @brief    Print String of character with next line feature
    @param    
  */
  /**********************************************************************/
	
	displayClear();
  /**********************************************************************/
  /*!
    @brief    Display Clear
    @param    
  */
  /**********************************************************************/

	
  private:
  void	writeNokiaSPI(const char data,const char level),
  /**********************************************************************/
  /*!
    @brief    write SPI command/Data to nokia display
    @param    8 bit data plus command/data bit
  */
  /**********************************************************************/
  
  hardwareSpiInit();
  /**********************************************************************/
  /*!
    @brief    write SPI data to nokia display
    @param    
  */
  /**********************************************************************/
  
  uint8_t hwSPI; //use gpio or spi interface hardware
  const char c = 'c';
  const char d = 'd'; //command, data level indication
  const char C = 'C';
  const char D = 'D'; //command, data level indication
  uint8_t	SPIDEVICE_CS,
				  SPIDEVICE_RES,  //miso
				  SPIDEVICE_SDA,	//Mosi
				  SPIDEVICE_SCK; 
  //------Hardware spi-------
};
