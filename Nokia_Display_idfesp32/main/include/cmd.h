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
#ifndef _CMD_H
#define _CMD_H

#define NOKIA105_NOP                     (0x00)
#define NOKIA105_SWRESET                 (0x01)
#define NOKIA105_RDDIDIF                 (0x04)
#define NOKIA105_RDDST                   (0x09)
#define NOKIA105_RDDPM                   (0x0A)
#define NOKIA105_RDDMADCTL               (0x0B)
#define NOKIA105_RDDCOLMOD               (0x0C)
#define NOKIA105_RDDIM                   (0x0D)
#define NOKIA105_RDDSM                   (0x0E)
#define NOKIA105_RDDSDR                  (0x0F)
#define NOKIA105_SLPIN                   (0x10)
#define NOKIA105_SPLOUT                  (0x11)
#define NOKIA105_PTLON                   (0x12)
#define NOKIA105_NORON                   (0x13)
#define NOKIA105_INVOFF                  (0x20)
#define NOKIA105_INVON                   (0x21)
#define NOKIA105_GAMSET                  (0x26)
#define NOKIA105_DISPOFF                 (0x28)
#define NOKIA105_DISPON                  (0x29)
#define NOKIA105_CASET                   (0x2A)
#define NOKIA105_PASET                   (0x2B)
#define NOKIA105_RAMWR                   (0x2C)
#define NOKIA105_RAMRD                   (0x2E)
#define NOKIA105_RGBSET                  (0x2D)
#define NOKIA105_PLTAR                   (0x30)
#define NOKIA105_VSCRDEF                 (0x33)
#define NOKIA105_TEOFF                   (0x34)
#define NOKIA105_TEON                    (0x35)
#define NOKIA105_MADCTL                  (0x36)
#define NOKIA105_VSCRSADD                (0x37)
#define NOKIA105_IDMOFF                  (0x38)
#define NOKIA105_IDMON                   (0x39)
#define NOKIA105_COLMOD                  (0x3A)
#define NOKIA105_RDID1                   (0xDA)
#define NOKIA105_RDID2                   (0xDB)
#define NOKIA105_RDID3                   (0xDC)
#define NOKIA105_SETOSC                  (0xB0)
#define NOKIA105_SETPWCTR1               (0xB1)
#define NOKIA105_SETPWCTR2               (0xB2)
#define NOKIA105_SETPWCTR3               (0xB3)
#define NOKIA105_SETPWCTR4               (0xB4)
#define NOKIA105_SETPWCTR5               (0xB5)
#define NOKIA105_SETDISCTRL              (0xB6)
#define NOKIA105_SETFRMCTRL              (0xB7)
#define NOKIA105_SETDISCYCCTRL           (0xB8)
#define NOKIA105_SETINVCTRL              (0xB9)
#define NOKIA105_RGBBPCTR                (0xBA)
#define NOKIA105_SETRGBIF                (0xBB)
#define NOKIA105_SETDODC                 (0xBC)
#define NOKIA105_SETINTMODE              (0xBD)
#define NOKIA105_SETPANEL                (0xBE)
#define NOKIA105_SETOTP                  (0xC7)
#define NOKIA105_SETONOFF                (0xC0)
#define NOKIA105_SETEXTCMD               (0xC1)
#define NOKIA105_SETGAMMAP               (0xC2)
#define NOKIA105_SETGAMMAN               (0xC3)

#endif
