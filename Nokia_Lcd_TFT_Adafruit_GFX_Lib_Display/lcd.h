#ifndef _LCD_H
#define _LCD_H
//----------------------Set or Clear of bit-----digitalwrite is very slow----------------------------
#ifdef ARDUINO_ARCH_AVR
#define LCD_RES_High()   { PORTB = PORTB | B00010000; /*digitalWrite(SPIDEVICE_RES,HIGH);*/} 
#define LCD_RES_Low()    { PORTB = PORTB & B11101111; /*digitalWrite(SPIDEVICE_RES,LOW);*/ }
#define LCD_CS_High()    { PORTB = PORTB | B00010100; /*digitalWrite(SPIDEVICE_CS,HIGH);*/ }  
#define LCD_CS_Low()     { PORTB = PORTB & B11111011; /*digitalWrite(SPIDEVICE_CS,LOW);*/  }
#define LCD_SDA_High()   { PORTB = PORTB | B00001000; /*digitalWrite(SPIDEVICE_SDA,HIGH);*/} 
#define LCD_SDA_Low()    { PORTB = PORTB & B11110111; /*digitalWrite(SPIDEVICE_SDA,LOW);*/ }
#define LCD_SCK_High()   { PORTB = PORTB | B00100000; /*digitalWrite(SPIDEVICE_SCK,HIGH);*/}
#define LCD_SCK_Low()    { PORTB = PORTB & B11011111; /*digitalWrite(SPIDEVICE_SCK,LOW);*/ }

/*int portSPIDEVICE_RES = portOutputRegister(digitalPinToPort(SPIDEVICE_RES));
int portSPIDEVICE_CS =  portOutputRegister(digitalPinToPort(SPIDEVICE_CS));
int portSPIDEVICE_SDA = portOutputRegister(digitalPinToPort(SPIDEVICE_SDA));
int portSPIDEVICE_SCK = portOutputRegister(digitalPinToPort(SPIDEVICE_SCK));
int maskSPIDEVICE_SCK = digitalPinToBitMask(SPIDEVICE_SCK);
int maskSPIDEVICE_RES = digitalPinToBitMask(SPIDEVICE_RES);
int maskSPIDEVICE_CS =  digitalPinToBitMask(SPIDEVICE_CS);
int maskSPIDEVICE_SDA = digitalPinToBitMask(SPIDEVICE_SDA);

#define LCD_RES_High()   { digitalPinToPort(SPIDEVICE_RES) |=   (1<<digitalPinToBitMask(SPIDEVICE_RES));   }//B00010000; }//digitalWrite(SPIDEVICE_RES,HIGH);} 
#define LCD_RES_Low()    { digitalPinToPort(SPIDEVICE_RES) &=   ~(1<<digitalPinToBitMask(SPIDEVICE_RES));  }//B11101111; }//digitalWrite(SPIDEVICE_RES,LOW); }
#define LCD_CS_High()    { digitalPinToPort(SPIDEVICE_CS)  |=   (1<<digitalPinToBitMask(SPIDEVICE_CS));    }//B00010100; }//digitalWrite(SPIDEVICE_CS,HIGH); }  
#define LCD_CS_Low()     { digitalPinToPort(SPIDEVICE_CS)  &=   ~(1<<digitalPinToBitMask(SPIDEVICE_CS));   }//B11111011; }//digitalWrite(SPIDEVICE_CS,LOW);  }
#define LCD_SDA_High()   { digitalPinToPort(SPIDEVICE_SDA) |=   (1<<digitalPinToBitMask(SPIDEVICE_SDA));   }//B00001000; }//digitalWrite(SPIDEVICE_SDA,HIGH);} 
#define LCD_SDA_Low()    { digitalPinToPort(SPIDEVICE_SDA) &=   ~(1<<digitalPinToBitMask(SPIDEVICE_SDA));   }//B11110111; }//digitalWrite(SPIDEVICE_SDA,LOW);}
#define LCD_SCK_High()   { digitalPinToPort(SPIDEVICE_SCK) |=   (1<<digitalPinToBitMask(SPIDEVICE_SCK));   }//B00100000; }//digitalWrite(SPIDEVICE_SCK,HIGH);}
#define LCD_SCK_Low()    { digitalPinToPort(SPIDEVICE_SCK) &=   ~(1<<digitalPinToBitMask(SPIDEVICE_SCK));  }//B11011111; }//digitalWrite(SPIDEVICE_SCK,LOW); }
*/

#elif defined ARDUINO_ARCH_ESP32 
#define LCD_RES_High()   { GPIO.out_w1ts = ((uint32_t)1 << SPIDEVICE_RES);  } //digitalWrite(SPIDEVICE_RES,HIGH);} //PORTB = PORTB | B00010000;}
#define LCD_RES_Low()    { GPIO.out_w1tc = ((uint32_t)1 << SPIDEVICE_RES);  } //digitalWrite(SPIDEVICE_RES,LOW);  }//PORTB = PORTB & B11101111;  }
#define LCD_CS_High()    { GPIO.out_w1ts = ((uint32_t)1 << SPIDEVICE_CS);   } //digitalWrite(SPIDEVICE_CS,HIGH); }//PORTB = PORTB | B00010100;}
#define LCD_CS_Low()     { GPIO.out_w1tc = ((uint32_t)1 << SPIDEVICE_CS);   } //digitalWrite(SPIDEVICE_CS,LOW);   }//PORTB = PORTB & B11111011; }
#define LCD_SDA_High()   { GPIO.out_w1ts = ((uint32_t)1 << SPIDEVICE_SDA);  } //digitalWrite(SPIDEVICE_SDA,HIGH); }//PORTB = PORTB | B00001000; }
#define LCD_SDA_Low()    { GPIO.out_w1tc = ((uint32_t)1 << SPIDEVICE_SDA);  } //digitalWrite(SPIDEVICE_SDA,LOW);  }//PORTB = PORTB & B11110111;  }
#define LCD_SCK_High()   { GPIO.out_w1ts = ((uint32_t)1 << SPIDEVICE_SCK);  } // digitalWrite(SPIDEVICE_SCK,HIGH); }//PORTB = PORTB | B00100000;}
#define LCD_SCK_Low()    { GPIO.out_w1tc = ((uint32_t)1 << SPIDEVICE_SCK);  } //digitalWrite(SPIDEVICE_SCK,LOW);  }//PORTB = PORTB & B11011111;  }
#else 
    #error Processor not supported
#endif

#include "Arduino.h"
#include "fonts.h"
#include "Adafruit_GFX.h"
 //https://stackoverflow.com/questions/2660484/what-are-0x01-and-0x80-representative-of-in-c-bitwise-operations
        /*| MSB |     |     |     |     |     |     | LSB |
          |  1  |  0  |  1  |  1  |  0  |  0  |  1  |  1  |   Input
          |  1  |  1  |  0  |  0  |  1  |  1  |  0  |  1  |   Output
          |  1  |  0  |  0  |  0  |  0  |  0  |  0  |  0  |   0x80
          |  0  |  0  |  0  |  0  |  0  |  0  |  0  |  1  |   0x01
          |  0  |  1  |  0  |  0  |  0  |  0  |  0  |  0  |   (0x80 >> 1)
          |  0  |  0  |  0  |  0  |  0  |  0  |  1  |  0  |   (0x01 << 1)*/
/*
 * harcoded gpio for future updates
#define SPIDEVICE_CS    10
#define SPIDEVICE_RES   12          //miso
#define SPIDEVICE_SDA   11          //Mosi
#define SPIDEVICE_SCK   13
*/
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

//---------------------------nokia display functions libraries overlap with adafruit gfx library------

class Nokia105 : public Adafruit_GFX {
	public:
  Nokia105(int8_t SID, int8_t SCLK, int8_t RST, int8_t CS);
	/**********************************************************************/
  /*!
    @brief    Pin defination
    @param    SPIDEVICE_CS, SPIDEVICE_RES, SPIDEVICE_SDA or Mosi, SPIDEVICE_SCK
    spi proceed by defined gpio.
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

  setLcdBrightness(uint16_t PWM),    
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
	/**********************************************************************/
  /*!
    @brief    as function name says, it drae 1 pixel on screen
    @param    x: horizonal position, y: vertical position,color: 16 bit color in hex
    only drawPixel added along with lcd.cpp->height and width
  */
  /**********************************************************************/
	
	image1d (uint16_t w, uint16_t h, uint16_t shiftX,uint16_t shiftY, const uint16_t image[] ),
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
				
	printDigit(unsigned int a, int16_t x, int16_t y,uint16_t forgroundColor,uint16_t backgroundColor),
  /**********************************************************************/
  /*!
    @brief    digit print working upto 10,000 only unsigned integers 
    @param    
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
  void	writeNokiaCommand(unsigned char c),
  /**********************************************************************/
  /*!
    @brief    write SPI command to nokia display
    @param    
  */
  /**********************************************************************/
	
	writeNokiaData(unsigned char c);
  /**********************************************************************/
  /*!
    @brief    write SPI data to nokia display
    @param    
  */
  /**********************************************************************/

  bool hwSPI; //use gpio or spi interface hardware

  int8_t	SPIDEVICE_CS,
				      SPIDEVICE_RES,  //miso
				      SPIDEVICE_SDA,	//Mosi
				      SPIDEVICE_SCK;
};
#endif