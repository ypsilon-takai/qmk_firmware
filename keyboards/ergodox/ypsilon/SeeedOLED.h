/*
 * SeeedOLED.h
 * SSD130x OLED Driver Library
 *
 * Copyright (c) 2011 seeed technology inc.
 * Author        :   Visweswara R
 * Create Time   :   Dec 2011
 * Change Log    :
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SeeedOLED_data_H
#define SeeedOLED_data_H

#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
//#include "../matrix.h"

// --------------------------------------------------------------------

uint8_t oled_clearLineBuf(uint8_t* buf);
uint8_t oled_DispLogo(void);

uint8_t oled_init(void);
uint8_t oled_updateDisplay(uint8_t* linebuf_1, uint8_t* linebuf_2, uint8_t* modbuf);

void update_rotator(uint8_t* line_buf, uint8_t index);
void set_numlock_image(uint8_t* line_buf);
void set_capslock_image(uint8_t* line_buf);


// SeeedOLED dimension
#define SeeedOLED_Max_X                 127 //128 Pixels
#define SeeedOLED_Max_Y                 63  //64  Pixels

// I2C address
#define SeeedOLED_Address               0x3c
#define SeeedOLED_Address_WRITE         ( (SeeedOLED_Address<<1) | 0 )
#define SeeedOLED_Address_READ          ( (SeeedOLED_Address<<1) | 1 )

// SeeedOLED Instruction set addresses
#define SeeedOLED_PAGE_MODE             0x01
#define SeeedOLED_HORIZONTAL_MODE       0x02

#define SeeedOLED_Command_Mode          0x80
#define SeeedOLED_Data_Mode             0x40
#define SeeedOLED_Segment_Normal_Cmd    0xA0
#define SeeedOLED_Segment_Remap_Cmd     0xA1
#define SeeedOLED_Com_Normal_Cmd        0xC0
#define SeeedOLED_Com_Remap_Cmd         0xC8
#define SeeedOLED_Display_Off_Cmd       0xAE
#define SeeedOLED_Display_On_Cmd        0xAF
#define SeeedOLED_Normal_Display_Cmd    0xA6
#define SeeedOLED_Inverse_Display_Cmd   0xA7
#define SeeedOLED_Activate_Scroll_Cmd   0x2F
#define SeeedOLED_Dectivate_Scroll_Cmd  0x2E
#define SeeedOLED_Set_Brightness_Cmd    0x81

#define Scroll_Left             0x00
#define Scroll_Right            0x01

#define Scroll_2Frames          0x7
#define Scroll_3Frames          0x4
#define Scroll_4Frames          0x5
#define Scroll_5Frames          0x0
#define Scroll_25Frames         0x6
#define Scroll_64Frames         0x1
#define Scroll_128Frames        0x2
#define Scroll_256Frames        0x3

uint8_t SeeedOLED_init(void);

void SeeedOLED_setNormalDisplay(void);
void SeeedOLED_setInverseDisplay(void);

uint8_t SeeedOLED_sendCommand(uint8_t command);
uint8_t SeeedOLED_sendData(uint8_t Data);
uint8_t SeeedOLED_sendDataS(uint8_t* Data, uint8_t Length);

void SeeedOLED_setPageMode(void);
void SeeedOLED_setHorizontalMode(void);

void SeeedOLED_setTextXY(uint8_t Row, uint8_t Column);

void SeeedOLED_clearDisplay(void);

void SeeedOLED_setBrightness(uint8_t Brightness);

void SeeedOLED_putChar(uint8_t c);
void SeeedOLED_putString(const char *String);
void SeeedOLED_putByteAsHex(uint8_t c);

uint8_t SeeedOLED_putNumber(long n);
uint8_t SeeedOLED_putFloatD(float floatNumber,uint8_t decimal);
uint8_t SeeedOLED_putFloat(float floatNumber);

void SeeedOLED_drawBitmap(uint8_t *bitmaparray,int bytes);

void SeeedOLED_setHorizontalScrollProperties(bool direction,uint8_t startPage, uint8_t endPage, uint8_t scrollSpeed);

void SeeedOLED_activateScroll(void);
void SeeedOLED_deactivateScroll(void);

// buffer support functions for seeed oled
uint8_t buf_set_num(uint8_t num, uint8_t* buf, uint8_t pos);
uint8_t buf_set_char(char c, uint8_t* buf, uint8_t pos);
uint8_t buf_set_str(uint8_t* str, uint8_t size, uint8_t* buf, uint8_t pos);
uint8_t buf_set_byte_as_hex(uint8_t c, uint8_t* buf, uint8_t pos);

uint8_t layer_set_num(uint8_t num, uint8_t* upper_buf, uint8_t* lower_buf, uint8_t pos);

/* extern SeeedOLED SeeedOled;  // SeeedOLED object  */

#endif


