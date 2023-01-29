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
#pragma once
#ifndef _LSPI_H
#define _LSPI_H

#include <stdint.h> //unint

void writeSpi (const char _data,const char _level);
uint32_t readSpi (const char len);
uint32_t lcd_get_id();
void spiInit(uint8_t  _SID, uint8_t _SCLK, uint8_t _CS,uint8_t _hwSPI1);
#endif