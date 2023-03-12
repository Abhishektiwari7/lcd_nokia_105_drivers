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
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for memset
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "lcd.h"
#include "fonts.h"
#include "cmd.h"
#include "lspi.h"                    //spi Custom Library, import here your code

spi_device_handle_t spi3;           //spi handler
ledc_timer_config_t ledc_timer;     //ledc pwm
ledc_channel_config_t ledc_channel; //ledc pwm
//------------------------------------------------------------------adafruit gfx added---------
Nokia105::Nokia105( uint8_t  SID, uint8_t SCLK, uint8_t  RST, uint8_t CS) {                              //for low memory limitation no gfx
if (SID == 0 && SCLK == 0 && RST == 0 && CS == 0 ) {
  SPIDEVICE_SDA   = 23; //Mosi
  SPIDEVICE_SCK   = 18;
  SPIDEVICE_RES   = 19; 
  SPIDEVICE_CS    = 15;
  hwSPI = 1;      //Hardware SPI
 } else {
  SPIDEVICE_SDA   = SID; //Mosi
  SPIDEVICE_SCK   = SCLK;
  SPIDEVICE_RES   = RST; 
  SPIDEVICE_CS    = CS;
  hwSPI = 0;      //Bit Bang SPI
 }
}

void Nokia105::writeNokiaSPI (const char data,const char level) {
writeSpi (data,level);
//ESP_LOGI(TAG, "write nokia SPI.");
// if ( hwSPI == 0) {
//   //ESP_LOGI(TAG, "//Bitbang// ");
//   LCD_SCK_Low();          
//   LCD_CS_Low();
//   //----------------high 8 bit data----------------------------
//   if (level == 'C'|| level == 'c') {              //command 1st bit 0
//     LCD_SDA_Low();
//   } else if (level == 'D'|| level == 'd') {       //command 1st bit 1
//     LCD_SDA_High();
//   } else {
//     ESP_LOGI(TAG, "Invalid SEND spi");
//     return;
//   }
//   LCD_SCK_High();
//   LCD_SCK_Low();  
//   uint8_t j = 0x80;                         
//   for (uint8_t i = 0; i < 8;i++) {
//     if (data & j) {                                   
//       LCD_SDA_High();   
//       LCD_SCK_High();
//       LCD_SCK_Low();
//     } else {
//       LCD_SDA_Low(); 
//       LCD_SCK_High();
//       LCD_SCK_Low();
//     }
//     j=j>>1;                           
//   }
//   LCD_CS_High();
// } else if (hwSPI == 1) {
//     /*why not sending comple 16bit color in one go?
//     1st bit for data, necessarry for every 8 bit data, frame like: 1 color 8 bit 1 color 8 bit total 18 bit frame.
//     i can only send : 1 color 16 bit. problem in 16 bit color it need to be seperated by 1 bit data at its 1st place.
//     solution using address kind of technique->set 1st and 9th bit address data as 1. so in between only 16 bit color.
//     t.length = 9 * 2;length in bit
//     2 byte per pixel so Total Data length in bit : 1st bit data/command next 8 bits for data three times because 16bit pixel mode display. 
//     refference pag no.141
//     4 byte data
//     t.tx_data[0] = 8 bit data
//     t.tx_data[1] = 8 bit data
//     t.tx_data[2] = 8 bit data
//     t.tx_data[3] = 8 bit data
//     idea: get 16bit color add 1 bit at 1st and 10th place in 16bit data. then set length 18. 
//       SPI_SWAP_DATA_TX(0x145, 9); test me //https://1-rt--net-jp.translate.goog/mobility/archives/7026?_x_tr_enc=1&_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en-GB
//       But be more careful when sending 16 bit (or 32 bit) data.
//       ESP32 is designed in [little endian] . For example, when 16-bit data 0xABCD is stored in the transmission buffer, it is stored in the order of CD AB in memory .
//       Therefore, even if you intended to send 0xABCD, it is actually sent in the order of CDAB .
//       As a countermeasure, the [ SPI_SWAP_DATA_TX] and [ SPI_SWAP_DATA_RX] macros are provided to convert the data order.

//       queue application https://github.com/espressif/esp-idf/blob/master/examples/peripherals/spi_master/lcd/main/spi_master_example_main.c
//     */
//     //ESP_LOGI(TAG, "Hardware Spi");
//     spi_device_acquire_bus(spi3, portMAX_DELAY);  // When using SPI_TRANS_CS_KEEP_ACTIVE, bus must be locked/acquired

//     spi_transaction_t t;							//create a spi transaction
//     memset(&t, 0, sizeof(t));					//Zero out of the transaction
//     t.flags = SPI_TRANS_USE_TXDATA;		//Use SPI_TRANS_USE_TXDATA Flag
//     t.length = 9;									    /*length in bit//2 byte per pixel so Total 
//                                         Data length in bit : 1st bit data/command 
//                                         next 8 bits for data three times because 
//                                         16bit pixel mode display. refference pag no.141 
//                                       */
//     if (level == 'C'|| level == 'c') {        //command 1st bit 0
//         t.cmd = 0x0;
//     } else if (level == 'D'|| level == 'd') { //command 1st bit 1
//         t.cmd = 0x1;
//     } else {
//         ESP_LOGI(TAG, "Invalid SEND spi");
//         return;
//     }

//     t.tx_buffer=(char *)data;                               //Data, remain 8 bit information //void pointer txbuffer
//     esp_err_t ret = spi_device_polling_transmit(spi3, &t);  //Transmit data. local struct copy of spi_transaction -> t copy to spi3
//     //esp_err_t ret =spi_device_queue_trans(spi3, &t, 0); //slow
//     assert(ret == ESP_OK);							                    //Check no issues
  
//     spi_device_release_bus(spi3); // Release bus
//   } else if(hwSPI == 2) {
//     ESP_LOGI(TAG, "Nothing");
//   }
}
//---------------------------working on reading device id--------------------------------------
// uint32_t lcd_get_id( ) {
//     //get_id cmd
//     //lcd_cmd(spi, 0x04);

//     spi_transaction_t t;
//     memset(&t, 0, sizeof(t));
//     t.length=8*3;
//     t.flags = SPI_TRANS_USE_RXDATA;

//     esp_err_t ret = spi_device_polling_transmit(spi3, &t);
//     assert( ret == ESP_OK );
//     return *(uint32_t*)t.rx_data;
//    // uint32_t lcd_id = lcd_get_id(spi);
//    // RDDST (0x09): Read Display Status 32bit add me SCRLAR: scroll area
//    //RDDID 0x04 =24bit
// }

void Nokia105:: hardwareSpiInit() {
spiInit(SPIDEVICE_SDA, SPIDEVICE_SCK, SPIDEVICE_CS,hwSPI);
// if (hwSPI == 0) {
//   ESP_LOGI(TAG, "intial spi");
//   gpio_reset_pin((gpio_num_t)SPIDEVICE_SDA);
//   gpio_reset_pin((gpio_num_t)SPIDEVICE_SCK);
//   gpio_reset_pin((gpio_num_t)SPIDEVICE_CS);
  
//   gpio_set_direction((gpio_num_t)SPIDEVICE_SDA,GPIO_MODE_OUTPUT);
//   gpio_set_direction((gpio_num_t)SPIDEVICE_SCK,GPIO_MODE_OUTPUT);
//   gpio_set_direction((gpio_num_t)SPIDEVICE_CS,GPIO_MODE_OUTPUT);
// } else if (hwSPI == 1) {
//     esp_err_t ret;
//     ESP_LOGI(TAG, "Initializing bus SPI %d...", SPI3_HOST);    

//     spi_bus_config_t buscfg;
//     memset(&buscfg, 0, sizeof(spi_bus_config_t));
//     buscfg.sclk_io_num = SPIDEVICE_SCK;
//     buscfg.mosi_io_num = SPIDEVICE_SDA;
//     buscfg.miso_io_num = -1;
//     buscfg.quadwp_io_num = -1; // Not used
//     buscfg.quadhd_io_num = -1; // Not used
//     buscfg.max_transfer_sz = 2*totalPixals;
    
//     ret = spi_bus_initialize(SPI3_HOST, &buscfg, 0); 
//     /*SPI_DMA_CH1      = 1,     ///< Enable DMA, select DMA Channel 1
//       SPI_DMA_CH2      = 2,     ///< Enable DMA, select DMA Channel 2
//       SPI_DMA_CH_AUTO  = 3,     ///< Enable DMA, channel is automatically selected by driver
//     */
//     ESP_ERROR_CHECK(ret);
    
//     spi_device_interface_config_t devcfg = {
//       .command_bits = 1,                     //1st bit number of bits , 9bit secret power
//       //.address_bits = 0,
//       //.dummy_bits = 0,
//       .mode = 0,                             //SPI mode 0
//       //.duty_cycle_pos = 0,
//       .clock_speed_hz = 26000000, //SPI_MASTER_FREQ_20M  //cpu:160 000 000 hzz(160mhz)
//       .spics_io_num = SPIDEVICE_CS,                   // chip select line
//       .flags = SPI_DEVICE_HALFDUPLEX,        //half duplex for 3 wire spi
//       .queue_size = 7,                      //queue of spi transaction
//       .pre_cb = NULL,                        //before transaction
//       .post_cb = NULL,                       //after transaction
//     };	

    
//     //ESP_LOGI(TAG, "Api bus intial");
//     ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &devcfg, &spi3)); //spi3 added in config to able to use spi
//     //ESP_LOGI(TAG, "Spi bus add device");
//     } else if (hwSPI == 2) {
//       //do nothing
//     }
}

void Nokia105:: displayClear(void) {
setDrawPosition(0,0);
for (unsigned int i = 0; i < totalPixals; i++) {
  writeNokiaSPI(NOKIA105_NOP,d);
  writeNokiaSPI(NOKIA105_NOP,d);
}
}

void Nokia105:: initDisplay(void) {
ESP_LOGI(TAG, "...........Init Display.........%d",hwSPI);
gpio_reset_pin((gpio_num_t)SPIDEVICE_RES);
gpio_set_direction((gpio_num_t)SPIDEVICE_RES,GPIO_MODE_OUTPUT);
hardwareSpiInit();//Spi Setup
        
LCD_RES_Low();
vTaskDelay(10 / portTICK_PERIOD_MS);
LCD_RES_High();
vTaskDelay(10 / portTICK_PERIOD_MS);
    
writeNokiaSPI(NOKIA105_SPLOUT,c);    // vTaskDelay,sleep out//0x11    
vTaskDelay(10 / portTICK_PERIOD_MS);

writeNokiaSPI(NOKIA105_COLMOD,c);    // Interface pixel format:bit1,2,3 //0x3a, color mode
writeNokiaSPI(0x05,d);               // 16 bit color
//writeNokiaSPI(0x02,d);             // 8 bit color

if (RGB2BGR == 0) {
  writeNokiaSPI(NOKIA105_MADCTL,c); // CMD_MADCTR
  writeNokiaSPI(0x00,d);               // RGB
}
else if (RGB2BGR == 1) {
  writeNokiaSPI(NOKIA105_MADCTL,c); // CMD_MADCTR
  writeNokiaSPI(0x08,d);               // BGR
}
// writeNokiaSPI(NOKIA105_INVON,c);    // inversion on
// writeNokiaSPI(NOKIA105_INVOFF,c);   // inversion off
// writeNokiaSPI(NOKIA105_GAMSET,c);   // gamma curve
// writeNokiaSPI(0x01,d);              // set: 0x01,0x02,0x04,0x08
writeNokiaSPI(NOKIA105_DISPON,c);      // Display on,0x29=ON,0x28=OFF
displayClear();
//uint32_t lcd_id = lcd_get_id();
 // printf("LCD ID: %08"PRIx32"\n", lcd_id);
//ESP_LOGI(TAG, "LCD ID: %08"PRIx32"\n",lcd_id);
}

void Nokia105:: setDrawPosition(unsigned char x, unsigned char y) {
writeNokiaSPI(NOKIA105_CASET,c);      //x-addres//0x2a
writeNokiaSPI(NOKIA105_NOP,d);           //xsta:BIT0-7
writeNokiaSPI(x,d);                    //xsta:BIT0-7
writeNokiaSPI(NOKIA105_NOP,d);           //xend:BIT0-7
writeNokiaSPI(WIDTH,d);                   //xend:BIT0-7

writeNokiaSPI(NOKIA105_PASET,c);      //y-address //0x2b
writeNokiaSPI(NOKIA105_NOP,d);           //xend:BIT0-7
writeNokiaSPI(y,d);                      //Ysta:BIT0-7
writeNokiaSPI(NOKIA105_NOP,d);           //xend:BIT0-7
writeNokiaSPI(HEIGHT,d);                   //xend:BIT0-7

writeNokiaSPI(NOKIA105_RAMWR,c);      //RAMWR//0x2c
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

writeNokiaSPI(NOKIA105_CASET,c); // Column addr set//0x2a
writeNokiaSPI(0,d); writeNokiaSPI(x0,d);   // X start
writeNokiaSPI(0,d); writeNokiaSPI(x1,d);   // X end

writeNokiaSPI(NOKIA105_PASET,c); // Page addr set 0x2b
writeNokiaSPI(0,d); writeNokiaSPI(y0,d);   // Y start
writeNokiaSPI(0,d); writeNokiaSPI(y1,d);   // Y end

writeNokiaSPI(NOKIA105_RAMWR,c);//0x2c
}


void Nokia105:: drawPixel(int16_t x, int16_t y, uint16_t color) {
if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT))
    return;
uint8_t hi = color >> 8, lo = color;
setDrawPositionAxis(x, y, x, y);
writeNokiaSPI(hi,d);
writeNokiaSPI(lo,d);
}

void Nokia105:: pushFastPixel(uint32_t length, const void* color) { //fast image processing
uint16_t *data = (uint16_t*)color;
uint8_t hi,lo;
while (length--) {
hi = *data >> 8;
lo = *data;
writeNokiaSPI(hi,d);
writeNokiaSPI(lo,d);
data++;
}
}

void Nokia105:: image1d (uint16_t w, uint16_t h, uint16_t shiftX,uint16_t shiftY, const uint16_t * image ) {
uint16_t  buffer[w];
setDrawPositionAxis(shiftX, shiftY, shiftX + w - 1, shiftY + h - 1 );
for (int32_t y = 0; y < h; y++) { //h
  for (int32_t x = 0; x < w; x++) { //w
    buffer[x] = pgm_read_word(&image[y * w + x]); //line by line data of image store in buuffer then send.
  }
  pushFastPixel(w, buffer); //tft_espi
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
// void Nokia105:: drawtext(unsigned char c, unsigned char x, unsigned char y ,uint16_t color) {
// unsigned char k,Mline,Ctemp;
// setDrawPosition(x,y);

// c -= 0x20; //asic value conversion

// for (Mline = 0; Mline < 16; Mline++) {
//   Ctemp = text[c][Mline];
//   for(k = 0; k < 8; k++) {
//     if(Ctemp & 0x80) {
//       writeNokiaSPI(color>>8,d); 
//       writeNokiaSPI(color,d);
//     } else {
//       writeNokiaSPI(0x00>>8,d); 
//       writeNokiaSPI(0x00,d);
//     }
//     Ctemp=Ctemp>>1;
//   }
//   setDrawPosition(x,++y); 
// }
// }


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
   writeNokiaSPI(hi,d);
   writeNokiaSPI(lo,d);
}
}

void Nokia105:: smpteTest() {
fillRectangle(0,0,18,160,WHITE); // HEIGHTYPOS, WIDTHXPOS, STARTYPOS, STARTXPOS, ENDY,ENDX
fillRectangle(18,0,36,160,BLUE); // HEIGHT, WIDTH
fillRectangle(36,0,54,160,RED); // HEIGHT, WIDTH
fillRectangle(54,0,72,160,GREEN); // HEIGHT, WIDTH
fillRectangle(72,0,90,160,CYAN); // HEIGHT, WIDTH
fillRectangle(90,0,108,160,MAGENTA); // HEIGHT, WIDTH
fillRectangle(108,0,126,160,YELLOW); // HEIGHT, WIDTH
fillRectangle(126,0,190,160,BLACK); // HEIGHT, WIDTH
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
      byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]); //research required
    if (byte & 0x80) {
        drawPixel(x + i, y, color);
    }
  }
}
}

void Nokia105:: backgroundColor(uint16_t c) {
uint8_t x, y, hi = c >> 8, lo = c;
setDrawPositionAxis(0, 0, WIDTH-1, HEIGHT-1);
int length = totalPixals;
while (--length) {
    writeNokiaSPI(hi,d);
    writeNokiaSPI(lo,d);
}
}

void Nokia105:: colorPalletTest() {
int colorPallete[] = {WHITE,BLUE,RED,GREEN,CYAN,MAGENTA,YELLOW,NAVY,DARKGREEN,DARKCYAN,MAROON,PURPLE,OLIVE,LIGHTGREY,DARKGREY,ORANGE,PINK};
  for(int i=0; i < 10; i++) {
    backgroundColor(colorPallete[i]);
    //Generate complete frame
    //   for (int y = 0; y < HEIGHT-1; y++) {
    //   for (int x = 0; x < WIDTH-1; x++) {
    //     writeNokiaSPI(colorPallete[1]>>8,d); //16 bit color chunks me jaengy
    //     writeNokiaSPI(colorPallete[1],d);
    //   }
    // }
    vTaskDelay(800 / portTICK_PERIOD_MS);
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

int Nokia105:: countDigit(long long n) { // to get number on digits in input passing number :) used to give space in float print
if (n == 0) //only 1 digit
  return 1;
int count = 0;
while (n != 0) { //check until no number being left 
  n = n / 10;
  ++count;
}
return count;
}

void Nokia105::  printDigitInteger(int32_t Inumber,int16_t x, int16_t y,uint16_t forgroundColor,uint16_t backgroundColor) {
// a = a + 48;
// unsigned char b = a;
// printSingleChar (b,x,y,forgroundColor,backgroundColor);
//-------------------------thanks ben heck :)----------------
//Send up to a 9 digit decimal value to memory
	int zPad = 0;							//Flag for zero padding
	uint32_t divider = 100000000;			//Divider starts at 900 million = max decimal printable is 999,999,999

	for (int xx = 0 ; xx < 9 ; xx++) {		//9 digit number
		if (Inumber >= divider) {
			uint8_t intnum =  '0' + (Inumber / divider);
			printSingleChar (intnum,x,y,forgroundColor,backgroundColor);
      x=x+8;
      Inumber %= divider;
			zPad = 1;
		}
		else if (zPad || divider == 1) {			
			printSingleChar ('0',x,y,forgroundColor,backgroundColor);
      x=x+8;
		}
		divider /= 10;
	}
}

void Nokia105:: printDigitFloat(double fnumber,uint8_t digits, int16_t x, int16_t y,uint16_t forgroundColor,uint16_t backgroundColor) {
//-------check if given number is valid or not?--------------------------------------------
if (isnan(fnumber)) return printString("nan",x,y,backgroundColor,backgroundColor);
if (isinf(fnumber)) return printString("inf",x,y,backgroundColor,backgroundColor);
if (fnumber > 4294967040.0) return printString("ovf",x,y,backgroundColor,backgroundColor);  // constant determined empirically
if (fnumber <-4294967040.0) return printString("ovf",x,y,backgroundColor,backgroundColor);  // constant determined empirically

// Handle negative numbers
if (fnumber < 0.0) {
  printString("-",x,y,forgroundColor,backgroundColor);
  x = x+8; // character size on screen
  fnumber = -fnumber;
}

double rounding = 0.5;
for (uint8_t i = 0; i < digits; ++i) { // Round correctly so that print(1.999, 2) prints as "2.00"
  rounding /= 10.0;
}
fnumber += rounding;

unsigned long int_part = (unsigned long)fnumber; // Extract the integer part of the number
double remainder = fnumber - (double)int_part;

if (int_part == 0) { //if 1st interger has zero
printSingleChar ('0',x,y,forgroundColor,backgroundColor);
x = x+8; // character size on screen
} else {
  printDigitInteger(int_part,x,y,forgroundColor,backgroundColor); //print it
  x = (x +  ( countDigit(int_part) * 8 ) ); // character size on screen count digit to use give space
}

if (digits > 0) { // Print the decimal point, but only if there are digits beyond
  printString(".",x,y,forgroundColor,backgroundColor);
  x = x+8; // character size on screen
}

// Extract digits from the remainder one at a time
while (digits-- > 0) {
  remainder *= 10.0;
  unsigned int toPrint = (unsigned int)(remainder);
  printDigitInteger(toPrint,x,y,forgroundColor,backgroundColor);
  x = x+8;
  remainder -= toPrint; 
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
  writeNokiaSPI(hi,d);
  writeNokiaSPI(lo,d);
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
  writeNokiaSPI(hi,d);
  writeNokiaSPI(lo,d);
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
      writeNokiaSPI(forgroundColor>>8,d); //print color at that position
      writeNokiaSPI(forgroundColor,d);
    } else {
      writeNokiaSPI(backgroundColor>>8,d); //BACKGOUND color
      writeNokiaSPI(backgroundColor,d);
    }
    Ctemp=Ctemp>>1; //next bit of data of current row
  }
  setDrawPosition(x,++y); //increase y position
}
}

//void Nokia105:: printStringChar(unsigned char *String,unsigned char x,unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor) {
void Nokia105:: printStringChar( char *String,unsigned char x,unsigned char y,uint16_t forgroundColor, uint16_t backgroundColor) {
while ( * String ) {
  printSingleChar ( *String++,x,y,forgroundColor,backgroundColor);
  x+=8;     
}
}

//void Nokia105:: printString(uint8_t *str,uint8_t x,uint8_t y,uint16_t forgroundColor, uint16_t backgroundColor) {                              
void Nokia105:: printString(char *str,uint8_t x,uint8_t y,uint16_t forgroundColor, uint16_t backgroundColor) {                              
while(*str!=0) { 
  if (x > nextLineEdge) {        // old->112          
     y += spaceBetweenScanLines; //old->16
     x = 0;                     //reset the position to rewrite from start position of x
   }                 
   
   if (y > fullLengthVertical) { //old->144 //OVERFLOW EXIT OR ERROR ADD UP
    break;
   } 
          
    printSingleChar (*str,x,y,forgroundColor,backgroundColor);
    str++; 
    x+=8; 
  }
} 

void Nokia105:: PWMinit() {
//gpio_reset_pin((gpio_num_t)BACK_LIGHT);
//gpio_set_direction((gpio_num_t)BACK_LIGHT, GPIO_MODE_OUTPUT);
//gpio_set_level((gpio_num_t)BACK_LIGHT, 1); //backlight
ledc_timer.speed_mode       = LEDC_MODE;
ledc_timer.timer_num        = LEDC_TIMER;
ledc_timer.duty_resolution  = LEDC_DUTY_RES;
ledc_timer.freq_hz          = LEDC_FREQUENCY;  // Set output frequency at ? kHz
ledc_timer.clk_cfg          = LEDC_AUTO_CLK;
ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

ledc_channel.speed_mode     = LEDC_MODE;
ledc_channel.channel        = LEDC_CHANNEL;
ledc_channel.timer_sel      = LEDC_TIMER;
ledc_channel.intr_type      = LEDC_INTR_DISABLE;
ledc_channel.gpio_num       = LEDC_OUTPUT_IO;
ledc_channel.duty           = 0; // Set duty to 0%
ledc_channel.hpoint         = 0;
ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void Nokia105:: setLcdBrightness(unsigned int PWM) {
  if (PWM > 512) {
     PWM = 512; //max
   } else if (PWM < 20 ) {
      PWM = 20;  //min
   }
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, PWM));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}
