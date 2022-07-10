#ifndef lcd
#define lcd
#include "Arduino.h"
#include "fonts.h"
 //https://stackoverflow.com/questions/2660484/what-are-0x01-and-0x80-representative-of-in-c-bitwise-operations
        /*| MSB |     |     |     |     |     |     | LSB |
          |  1  |  0  |  1  |  1  |  0  |  0  |  1  |  1  |   Input
          |  1  |  1  |  0  |  0  |  1  |  1  |  0  |  1  |   Output
          |  1  |  0  |  0  |  0  |  0  |  0  |  0  |  0  |   0x80
          |  0  |  0  |  0  |  0  |  0  |  0  |  0  |  1  |   0x01
          |  0  |  1  |  0  |  0  |  0  |  0  |  0  |  0  |   (0x80 >> 1)
          |  0  |  0  |  0  |  0  |  0  |  0  |  1  |  0  |   (0x01 << 1)*/

//#define SPIDEVICE_CS    10
//#define SPIDEVICE_RES   12          //miso
//#define SPIDEVICE_SDA   11          //Mosi
//#define SPIDEVICE_SCK   13

#define WIDTH          128
#define HEIGHT         167           //old->160
#define nextLineEdge   128          //Show_Str
#define spaceBetweenScanLines  16   //Show_Str, 2 lines ke beech ka distance
#define fullLengthVertical    160   // 164 characters on display
#define rotation        0 //SCREEN ROTATION 0 by default
#define rotateBitmap90  0 //1-> no rotaion 90,0-> yes rorate 90
#define LOG             1 //to activate serial
#define totalPixals     WIDTH*HEIGHT  //21384

#define LCD_RES_High()   { /*digitalWrite(SPIDEVICE_RES,HIGH);*/ PORTB = PORTB | B00010000; } 
#define LCD_RES_Low()    { /*digitalWrite(SPIDEVICE_RES,LOW);*/ PORTB = PORTB & B11101111;  }
#define LCD_CS_High()    { /*digitalWrite(SPIDEVICE_CS,HIGH);*/ PORTB = PORTB | B00010100;  }  
#define LCD_CS_Low()     { /*digitalWrite(SPIDEVICE_CS,LOW);*/  PORTB = PORTB & B11111011;  }
#define LCD_SDA_High()   { /*digitalWrite(SPIDEVICE_SDA,HIGH);*/PORTB = PORTB | B00001000;  } 
#define LCD_SDA_Low()    { /*digitalWrite(SPIDEVICE_SDA,LOW);*/PORTB = PORTB & B11110111;   }
#define LCD_SCK_High()   { /*digitalWrite(SPIDEVICE_SCK,HIGH);*/ PORTB = PORTB | B00100000; }
#define LCD_SCK_Low()    { /*digitalWrite(SPIDEVICE_SCK,LOW);*/PORTB = PORTB & B11011111;   }

//16 bit colors

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

#define textColor            GREEN  //text color
//int colorPallete[] = {BLACK,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,WHITE};
/**********************************************************************/
  /*!
    @brief    Prints the current contents of the canvas to Serial
    @param    rotated  true to print according to the current GFX rotation,
    false to print to the native rotation of the canvas (or unrotated).
  */
  /**********************************************************************/

class Nokia105 {
	public:
  		Nokia105(int8_t SID, int8_t SCLK, int8_t RST, int8_t CS);
  		//Nokia105(int8_t RST, int8_t CS);

		void	LCD_Init(),
            	LCD_SET_XY(unsigned char x, unsigned char y),
    			setWindow1(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1),
   				onePixel(int16_t x, int16_t y, uint16_t color),
				image1d (int w, int h, int shiftX,int shiftY, const uint16_t image[] ),
				/*image2d (int w, int h, int shiftX,int shiftY, const uint16_t image[][80] ),*/
				drawtext(unsigned char c, unsigned char x, unsigned char y ,uint16_t color),
				fillRectangle (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
				smpteTest(),
				drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[],int16_t w, int16_t h, uint16_t color),
				backgroundColor(uint16_t c),
				colorPalletTest(),
				lineHorixontal(int16_t x, int16_t y, int16_t h, uint16_t color),
				lineVertical(int16_t x, int16_t y, int16_t w,uint16_t color),
				LCD_Show_Char8(unsigned char c,unsigned char x, unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor),
				LCD_Show_String8 (unsigned char *String,unsigned char x,unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor),
				Show_Str(uint8_t*str,uint8_t x,uint8_t y,uint16_t forgroundColor, uint16_t backgroundColor),
				LCD_Clear();

	private:
  		void	LCD_Write_Cmd(unsigned char c),
				LCD_Write_Data(unsigned char c);

  		bool hwSPI; //use gpio or spi interface hardware

  		int8_t	SPIDEVICE_CS,
				SPIDEVICE_RES,    //miso
				SPIDEVICE_SDA,	//Mosi
				SPIDEVICE_SCK;

  		//volatile uint8_t	*dataport,
  			//				*clkport,
  				//			*csport;

  	//	uint8_t	datapinmask,
  		//		clkpinmask,
  		//		cspinmask,
  		//		spi_save;
};
#endif
