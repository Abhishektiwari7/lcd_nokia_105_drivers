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

//Custom Spi Library Specific for display control
#include <string.h> //for memset
#include <stdint.h> //unint
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_err.h"

#include "driver/spi_master.h"

#include "lspi.h"
#include "lcd.h" //totalpixels

//----------------------Set or Clear of bit-----digitalwrite is very slow----------------------------
//--too slow for me ;)
// #define LCD_CS_High()    { gpio_set_level((gpio_num_t)_SPIDEVICE_CS, 1);  } 
// #define LCD_CS_Low()     { gpio_set_level((gpio_num_t)_SPIDEVICE_CS, 0);  } 
// #define LCD_SDA_High()   { gpio_set_level((gpio_num_t)_SPIDEVICE_SDA,1);  } 
// #define LCD_SDA_Low()    { gpio_set_level((gpio_num_t)_SPIDEVICE_SDA,0);  } 
// #define LCD_SCK_High()   { gpio_set_level((gpio_num_t)_SPIDEVICE_SCK,1);  } 
// #define LCD_SCK_Low()    { gpio_set_level((gpio_num_t)_SPIDEVICE_SCK,0);  }
//------Lets manipulate registers directly--------------
#define GPIO_OUT_W1TS_REG (0x3ff44008) //to set HIGH
#define GPIO_OUT_W1TC_REG (0x3ff4400C) //to set LOW
#define LCD_CS_High()    { REG_WRITE(GPIO_OUT_W1TS_REG, BIT(_SPIDEVICE_CS));   }
#define LCD_CS_Low()     { REG_WRITE(GPIO_OUT_W1TC_REG, BIT(_SPIDEVICE_CS));   } 
#define LCD_SDA_High()   { REG_WRITE(GPIO_OUT_W1TS_REG, BIT(_SPIDEVICE_SDA));  } 
#define LCD_SDA_Low()    { REG_WRITE(GPIO_OUT_W1TC_REG, BIT(_SPIDEVICE_SDA));  } 
#define LCD_SCK_High()   { REG_WRITE(GPIO_OUT_W1TS_REG, BIT(_SPIDEVICE_SCK));  } 
#define LCD_SCK_Low()    { REG_WRITE(GPIO_OUT_W1TC_REG, BIT(_SPIDEVICE_SCK));  }

//-------------------end---------------------------------
uint8_t	_SPIDEVICE_CS,     //chip select activelow
        _SPIDEVICE_SDA,	   //Mosi /miso
        _SPIDEVICE_SCK,    //clock
        _hwSPI;            // 0:bit bang spi,1: hardware spi,2: none
spi_device_handle_t _spi;  //spi handler

//-----------------------------write SPI data/cmd --------------------------------
void writeSpi (const char _data,const char _level) {
//ESP_LOGI(TAG, "write nokia SPI.");
if ( _hwSPI == 0) {
  //ESP_LOGI(TAG, "//Bitbang// ");
  LCD_SCK_Low();          
  LCD_CS_Low();
  //----------------high 8 bit data----------------------------
  if (_level == 'C'|| _level == 'c') {              //command 1st bit 0
    LCD_SDA_Low();
  } else if (_level == 'D'|| _level == 'd') {       //command 1st bit 1
    LCD_SDA_High();
  } else {
    ESP_LOGI(TAG, "Invalid SEND spi");
    return;
  }
  LCD_SCK_High();
  LCD_SCK_Low();  
  uint8_t j = 0x80;                         
  for (uint8_t i = 0; i < 8;i++) {
    if (_data & j) {                                   
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
} else if (_hwSPI == 1) {
    /*why not sending comple 16bit color in one go?
    1st bit for data, necessarry for every 8 bit data, frame like: 1 color 8 bit 1 color 8 bit total 18 bit frame.
    i can only send : 1 color 16 bit. problem in 16 bit color it need to be seperated by 1 bit data at its 1st place.
    solution using address kind of technique->set 1st and 9th bit address data as 1. so in between only 16 bit color.
    t.length = 9 * 2;length in bit
    2 byte per pixel so Total Data length in bit : 1st bit data/command next 8 bits for data three times because 16bit pixel mode display. 
    refference pag no.141
    4 byte data
    t.tx_data[0] = 8 bit data
    t.tx_data[1] = 8 bit data
    t.tx_data[2] = 8 bit data
    t.tx_data[3] = 8 bit data
    idea: get 16bit color add 1 bit at 1st and 10th place in 16bit data. then set length 18. 
      SPI_SWAP_DATA_TX(0x145, 9); test me //https://1-rt--net-jp.translate.goog/mobility/archives/7026?_x_tr_enc=1&_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en-GB
      But be more careful when sending 16 bit (or 32 bit) data.
      ESP32 is designed in [little endian] . For example, when 16-bit data 0xABCD is stored in the transmission buffer, it is stored in the order of CD AB in memory .
      Therefore, even if you intended to send 0xABCD, it is actually sent in the order of CDAB .
      As a countermeasure, the [ SPI_SWAP_DATA_TX] and [ SPI_SWAP_DATA_RX] macros are provided to convert the data order.

      queue application https://github.com/espressif/esp-idf/blob/master/examples/peripherals/spi_master/lcd/main/spi_master_example_main.c
    */
    //ESP_LOGI(TAG, "Hardware Spi");
    spi_device_acquire_bus(_spi, portMAX_DELAY);  // When using SPI_TRANS_CS_KEEP_ACTIVE, bus must be locked/acquired

    spi_transaction_t t;							//create a spi transaction
    memset(&t, 0, sizeof(t));					//Zero out of the transaction
    t.flags = SPI_TRANS_USE_TXDATA;		//Use SPI_TRANS_USE_TXDATA Flag
    t.length = 9;									    /*length in bit//2 byte per pixel so Total 
                                        Data length in bit : 1st bit data/command 
                                        next 8 bits for data three times because 
                                        16bit pixel mode display. refference pag no.141 
                                      */
    if (_level == 'C'|| _level == 'c') {        //command 1st bit 0
        t.cmd = 0x0;
    } else if (_level == 'D'|| _level == 'd') { //command 1st bit 1
        t.cmd = 0x1;
    } else {
        ESP_LOGI(TAG, "Invalid SEND spi");
        return;
    }

    t.tx_buffer=(char *)_data;                               //Data, remain 8 bit information //void pointer txbuffer
    esp_err_t ret = spi_device_polling_transmit(_spi, &t);  //Transmit data. local struct copy of spi_transaction -> t copy to spi
    //esp_err_t ret =spi_device_queue_trans(spi, &t, 0); //slow
    assert(ret == ESP_OK);							                    //Check no issues
  
    spi_device_release_bus(_spi); // Release bus
  } else if(_hwSPI == 2) {
    ESP_LOGI(TAG, "Nothing");
  }
}

//---------------------------working on reading device id--------------------------------------

uint32_t readSpi (const char len) {
  spi_transaction_t t;
  memset(&t, 0, sizeof(t));
  t.length=8*len;
  t.flags = SPI_TRANS_USE_RXDATA;

  esp_err_t ret = spi_device_polling_transmit(_spi, &t);
  assert( ret == ESP_OK );
  return *(uint32_t*)t.rx_data;
}

uint32_t lcd_get_id( ) {
  writeSpi (0x04,'c'); //get_id cmd
  return *(uint32_t*)readSpi(3);  
  //uint32_t lcd_id = lcd_get_id(spi);
  // RDDST (0x09): Read Display Status 32bit add me SCRLAR: scroll area
  //RDDID 0x04 =24bit
}

//----------------------Init SPI----------------------------------------------------------------
void spiInit(uint8_t  _SID, uint8_t _SCLK, uint8_t _CS,uint8_t _hwSPI1) {
_hwSPI = _hwSPI1; //local to global
_SPIDEVICE_SDA   = _SID; //Mosi
_SPIDEVICE_SCK   = _SCLK;
_SPIDEVICE_CS    = _CS;
if (_hwSPI == 0) {
  ESP_LOGI(TAG, "Init BitBang SPI");
  gpio_reset_pin((gpio_num_t)_SPIDEVICE_SDA);
  gpio_reset_pin((gpio_num_t)_SPIDEVICE_SCK);
  gpio_reset_pin((gpio_num_t)_SPIDEVICE_CS);
  
  gpio_set_direction((gpio_num_t)_SPIDEVICE_SDA,GPIO_MODE_OUTPUT);
  gpio_set_direction((gpio_num_t)_SPIDEVICE_SCK,GPIO_MODE_OUTPUT);
  gpio_set_direction((gpio_num_t)_SPIDEVICE_CS,GPIO_MODE_OUTPUT);
} else if (_hwSPI == 1) {
    esp_err_t ret;
    ESP_LOGI(TAG, "Initializing bus SPI %d...", SPI3_HOST);    

    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(spi_bus_config_t));
    buscfg.sclk_io_num = _SPIDEVICE_SCK;
    buscfg.mosi_io_num = _SPIDEVICE_SDA;
    buscfg.miso_io_num = -1;
    buscfg.quadwp_io_num = -1; // Not used
    buscfg.quadhd_io_num = -1; // Not used
    buscfg.max_transfer_sz = 2*totalPixals;
    
    ret = spi_bus_initialize(SPI3_HOST, &buscfg, 0); 
    /*SPI_DMA_CH1      = 1,     ///< Enable DMA, select DMA Channel 1
      SPI_DMA_CH2      = 2,     ///< Enable DMA, select DMA Channel 2
      SPI_DMA_CH_AUTO  = 3,     ///< Enable DMA, channel is automatically selected by driver
    */
    ESP_ERROR_CHECK(ret);
    
    spi_device_interface_config_t devcfg = {
      .command_bits = 1,                     //1st bit number of bits , 9bit secret power
      //.address_bits = 0,
      //.dummy_bits = 0,
      .mode = 0,                             //SPI mode 0
      //.duty_cycle_pos = 0,
      .clock_speed_hz = 26000000, //SPI_MASTER_FREQ_20M  //cpu:160 000 000 hzz(160mhz)
      .spics_io_num = _SPIDEVICE_CS,                   // chip select line
      .flags = SPI_DEVICE_HALFDUPLEX,        //half duplex for 3 wire spi
      .queue_size = 7,                      //queue of spi transaction
      .pre_cb = NULL,                        //before transaction
      .post_cb = NULL,                       //after transaction
    };	

    
    //ESP_LOGI(TAG, "Api bus intial");
    ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &devcfg, &_spi)); //spi added in config to able to use spi
    //ESP_LOGI(TAG, "Spi bus add device");
    } else if (_hwSPI == 2) {
      //do nothing
    }
}
