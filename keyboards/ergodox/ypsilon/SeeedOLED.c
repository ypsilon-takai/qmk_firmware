/*
 * SeeedOLED.c
 * SSD130x OLED Driver Library
 *
 * Copyright (c) 2011 seeed technology inc.
 * Author        :   Visweswara R
 * Create Time   :   Dec 2011
 *
 * Mod           : TAKAI Y
 * - change C++ to C
 * - add some functions to support buffer operetion
 * - 
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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <util/twi.h>

#include "SeeedOLED.h"
#include "SeeedOLED-basic-font.h"
#include "SeeedOLED-ErgoDox-logo.h"

//#include "../../../lib/twi.h"  // `TWI_FREQ` defined in "teensy-2-0.c"
#include "i2cmaster.h"

//#include "../../../main.h"
//#include "../../../lib-other/pjrc/usb_keyboard/usb_keyboard.h"

#include <avr/pgmspace.h>

char addressingMode;
uint8_t i2c_initialized_oled = 0;
uint8_t oled_status = 0x20;

uint8_t SeeedOLED_sendCommand(uint8_t command) {
    oled_status = 0x20;
    oled_status = i2c_start(SeeedOLED_Address_WRITE);   if (oled_status) goto out;
    oled_status = i2c_write(SeeedOLED_Command_Mode);    if (oled_status) goto out;
    oled_status = i2c_write(command);                   if (oled_status) goto out;
        
out:  
    i2c_stop(); // End I2C communication
    return oled_status;
}

uint8_t SeeedOLED_sendData(uint8_t data) {
    oled_status = 0x20;
    oled_status = i2c_start(SeeedOLED_Address_WRITE);   if (oled_status) goto out;
    oled_status = i2c_write(SeeedOLED_Data_Mode);       if (oled_status) goto out;
    oled_status = i2c_write(data);                   if (oled_status) goto out;
        
out:  
    i2c_stop(); // End I2C communication
    return oled_status;
}

uint8_t SeeedOLED_sendDataS(uint8_t* data, uint8_t Length) {
    oled_status = 0x20;
    oled_status = i2c_start(SeeedOLED_Address_WRITE);   if (oled_status) goto out;
    oled_status = i2c_write(SeeedOLED_Data_Mode);       if (oled_status) goto out;
    for (uint8_t i = 0; i < Length; i++) {
        oled_status = i2c_write(data[i]);
        if(oled_status) goto out;
    }
out:
    i2c_stop(); // End I2C communication
    return oled_status;
}

uint8_t SeeedOLED_init(void) {
    
    SeeedOLED_clearDisplay();
    
    if(SeeedOLED_sendCommand(SeeedOLED_Normal_Display_Cmd))  //Set Normal Display (default)
        return 1;

    SeeedOLED_sendCommand(SeeedOLED_Segment_Normal_Cmd);
    SeeedOLED_sendCommand(SeeedOLED_Com_Normal_Cmd);

    
    return 0;
}

void SeeedOLED_setBrightness(uint8_t Brightness) {
    SeeedOLED_sendCommand(SeeedOLED_Set_Brightness_Cmd);
    SeeedOLED_sendCommand(Brightness);
}

void SeeedOLED_setHorizontalMode() {
    addressingMode = SeeedOLED_HORIZONTAL_MODE;
    SeeedOLED_sendCommand(0x20);          //addressing mode
    SeeedOLED_sendCommand(0x00);          //set horizontal addressing mode
}

void SeeedOLED_setPageMode() {
    addressingMode = SeeedOLED_PAGE_MODE;
    SeeedOLED_sendCommand(0x20);          //addressing mode
    SeeedOLED_sendCommand(0x02);          //set page addressing mode
}


void SeeedOLED_setTextXY(uint8_t Row, uint8_t Column) {
    SeeedOLED_sendCommand(0xB0 + Row);            //set page address
    SeeedOLED_sendCommand(0x00 + (8*Column & 0x0F));  //set column lower address
    SeeedOLED_sendCommand(0x10 + ((8*Column>>4)&0x0F));   //set column higher address
}

void SeeedOLED_setXY(uint8_t Row, uint8_t Column) {
    SeeedOLED_sendCommand(0xB0 + Row);            //set page address
    SeeedOLED_sendCommand(0x00 + (Column & 0x0F));  //set column lower address
    SeeedOLED_sendCommand(0x10 + ((Column>>4)&0x0F));   //set column higher address
}


void SeeedOLED_clearDisplay() {
    uint8_t i,j;
    SeeedOLED_sendCommand(SeeedOLED_Display_Off_Cmd);   //display off
    for(j=0;j<8;j++)
    { 
        SeeedOLED_setTextXY(j,0);    
        {
            for(i=0;i<16;i++)  //clear all columns
            {
                SeeedOLED_putChar(' ');    
            }
        }
    }
    SeeedOLED_sendCommand(SeeedOLED_Display_On_Cmd);    //display on
    SeeedOLED_setTextXY(0,0);    
}

void SeeedOLED_putChar(uint8_t C) {
    if(C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
    {
        C=' '; //Space
    }   
    uint8_t i=0;
    for(i=0;i<8;i++)
    {
        //read bytes from code memory
        SeeedOLED_sendData(pgm_read_byte(&BasicFont[C-32][i])); //font array starts at 0, ASCII starts at 32. Hence the translation
    }
}

void SeeedOLED_putString(const char *string) {
    uint8_t i=0;
    while(string[i])
    {
        SeeedOLED_putChar(string[i]);     
        i++;
    }
}

void SeeedOLED_putHalfByteAsHex(uint8_t c) {
    uint8_t cc = c&0x0F;
    if(cc<=9)
        SeeedOLED_putChar('0'+cc);
    else
        SeeedOLED_putChar('A'+cc-10);
}

void SeeedOLED_putByteAsHex(uint8_t c) {
    SeeedOLED_putHalfByteAsHex(c>>4);
    SeeedOLED_putHalfByteAsHex(c);
}

uint8_t SeeedOLED_putNumber(long long_num) {
    uint8_t char_buffer[10]="";
    uint8_t i = 0;
    uint8_t f = 0;

    if (long_num < 0) 
    {
        f=1;
        SeeedOLED_putChar('-');
        long_num = -long_num;
    } 
    else if (long_num == 0) 
    {
        f=1;
        SeeedOLED_putChar('0');
        return f;
    } 

    while (long_num > 0) 
    {
        char_buffer[i++] = long_num % 10;
        long_num /= 10;
    }

    f=f+i;
    for(; i > 0; i--)
    {
        SeeedOLED_putChar('0'+ char_buffer[i - 1]);
    }
    return f;

}

uint8_t SeeedOLED_putFloatD(float floatNumber,uint8_t decimal) {
    unsigned int temp=0;
    float decy=0.0;
    float rounding = 0.5;
    uint8_t f=0;
    if(floatNumber<0.0)
    {
        SeeedOLED_putString("-");
        floatNumber = -floatNumber;
        f +=1;
    }
    for (uint8_t i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    floatNumber += rounding;
  
    temp = floatNumber;
    f += SeeedOLED_putNumber(temp);
    if(decimal>0)
    {
        SeeedOLED_putChar('.');
        f +=1;
    }
    decy = floatNumber-temp;//decimal part, 
    for(uint8_t i=0;i<decimal;i++)//4 
    {
        decy *=10;// for the next decimal
        temp = decy;//get the decimal
        SeeedOLED_putNumber(temp);
        decy -= temp;
    }
    f +=decimal;
    return f;
}

uint8_t SeeedOLED_putFloat(float floatNumber) {
    uint8_t decimal=2;
    unsigned int temp=0;
    float decy=0.0;
    float rounding = 0.5;
    uint8_t f=0;
    if(floatNumber<0.0)
    {
        SeeedOLED_putString("-");
        floatNumber = -floatNumber;
        f +=1;
    }
    for (uint8_t i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    floatNumber += rounding;
  
    temp = floatNumber;
    f += SeeedOLED_putNumber(temp);
    if(decimal>0)
    {
        SeeedOLED_putChar('.');
        f +=1;
    }
    decy = floatNumber-temp;//decimal part, 
    for(uint8_t i=0;i<decimal;i++)//4 
    {
        decy *=10;// for the next decimal
        temp = decy;//get the decimal
        SeeedOLED_putNumber(temp);
        decy -= temp;
    }
    f +=decimal;
    return f;
}

void SeeedOLED_drawBitmap(uint8_t *bitmaparray, int bytes) {
    char localAddressMode = addressingMode;
    if(addressingMode != SeeedOLED_HORIZONTAL_MODE)
    {
        //Bitmap is drawn in horizontal mode  
        SeeedOLED_setHorizontalMode();
    }

    for(int i=0;i<bytes;i++)
    {
        SeeedOLED_sendData(pgm_read_byte(&bitmaparray[i]));
    }

    if(localAddressMode == SeeedOLED_PAGE_MODE)
    {
        //If pageMode was used earlier, restore it.
        SeeedOLED_setPageMode(); 
    }
  
}

void SeeedOLED_setHorizontalScrollProperties(bool direction,uint8_t startPage, uint8_t endPage, uint8_t scrollSpeed) {
/*
  Use the following defines for 'direction' :

  Scroll_Left            
  Scroll_Right           

  Use the following defines for 'scrollSpeed' :

  Scroll_2Frames     
  Scroll_3Frames
  Scroll_4Frames
  Scroll_5Frames 
  Scroll_25Frames
  Scroll_64Frames
  Scroll_128Frames
  Scroll_256Frames

*/

    if(Scroll_Right == direction)
    {
        //Scroll Right
        SeeedOLED_sendCommand(0x26);
    }
    else
    {
        //Scroll Left  
        SeeedOLED_sendCommand(0x27);

    }
    SeeedOLED_sendCommand(0x00);
    SeeedOLED_sendCommand(startPage);
    SeeedOLED_sendCommand(scrollSpeed);
    SeeedOLED_sendCommand(endPage);
    SeeedOLED_sendCommand(0x00);
    SeeedOLED_sendCommand(0xFF);
}

void SeeedOLED_activateScroll() {
    SeeedOLED_sendCommand(SeeedOLED_Activate_Scroll_Cmd);
}

void SeeedOLED_deactivateScroll() {
    SeeedOLED_sendCommand(SeeedOLED_Dectivate_Scroll_Cmd);
}

void SeeedOLED_setNormalDisplay() {
    SeeedOLED_sendCommand(SeeedOLED_Normal_Display_Cmd);
}

void SeeedOLED_setInverseDisplay() {
    SeeedOLED_sendCommand(SeeedOLED_Inverse_Display_Cmd);
}


/* SeeedOLED SeeedOled;  // Preinstantiate Objects */

int rotator_index;
uint8_t prev_keyboard_leds;
uint8_t prev_keyboard_modifier_keys;
uint8_t prev_keyboard_keys[6];
uint32_t prev_layers_state;

uint8_t layerline_buf_1[128]; // layer line1 buffer
uint8_t layerline_buf_2[128]; // layer line2 buffer
uint8_t modifierline_buf[128];   // modifier line buffer

uint8_t oled_clearLineBuf(uint8_t* buf) {
    for(int i=0; i<128; i++){  // clear
        buf[i] = 0;
    }
    return 128;
}

uint8_t oled_DispLogo(void) {
    SeeedOLED_drawBitmap((uint8_t*)ErgodoxLogo, ErgodoxLogoSize);
    return 0;
}


uint8_t oled_init(void) {
    uint8_t ret = 0;
    rotator_index = 0;
    prev_keyboard_leds = 0;
    prev_keyboard_modifier_keys = -1;
    /*
    for(int i=0; i<16; ++i)
        prev_layers_state[i] = -1;
    for(int i=0; i<6; ++i)
        prev_keyboard_keys[i] = -1;
    */

    ret = SeeedOLED_init();

    SeeedOLED_setXY(3, 0);
    oled_DispLogo();

    oled_clearLineBuf(layerline_buf_1);
    oled_clearLineBuf(layerline_buf_2);
    oled_clearLineBuf(modifierline_buf);

    SeeedOLED_setHorizontalScrollProperties(Scroll_Left, 0, 4, Scroll_5Frames);
    
    return ret;
}

uint8_t layer_set_num(uint8_t num, uint8_t* upper_buf, uint8_t* lower_buf, uint8_t pos) {
    uint8_t *upper_img;
    uint8_t *lower_img;
    if (num == 1) {
        upper_img = BigNum_1_upper;
        lower_img = BigNum_1_lower;
    } else {
        upper_img = BigNum_2_upper;
        lower_img = BigNum_2_lower;
    }
    
    memcpy_P(upper_buf + pos, upper_img , 16);
    memcpy_P(lower_buf + pos, lower_img , 16);
    
    return 16;
}

uint8_t buf_set_num(uint8_t num, uint8_t* buf, uint8_t pos) {
    for (uint8_t i=0; i<8; i++) {
        memcpy(buf + (pos + i), &NumFont[num][i], 1);
    }
    return 8;
}

uint8_t buf_set_char(char c, uint8_t* buf, uint8_t pos) {
    uint8_t num = c - 'A';
    for (uint8_t i=0; i<8; i++) {
        memcpy(buf + (pos + i), &AlfaFont[num][i], 1);
    }
    return 8;
}

uint8_t buf_set_image(uint8_t* img, uint8_t size, uint8_t* buf, uint8_t pos) {
    memcpy(buf + pos, img, size);
    return 0;
}

uint8_t buf_set_image_P(const uint8_t* flash_img, uint8_t size, uint8_t* buf, uint8_t pos) {
    memcpy_P(buf + pos, flash_img, size);
    return 0;
}

uint8_t buf_set_str(uint8_t* str, uint8_t size, uint8_t* buf, uint8_t pos) {
    uint8_t retsize = 0;
    for(uint8_t i=0; i<size; i++) {
        uint8_t ret = buf_set_char(str[i], buf, pos + i);
        if (ret == 0) return 0;
        retsize += ret;
    }
    return retsize;
}

uint8_t buf_set_byte_as_hex(uint8_t c, uint8_t* buf, uint8_t pos) {
    uint8_t retsize = 0;
    retsize = buf_set_num(c >> 4, buf, pos);
    retsize += buf_set_num(c & 0x0f, buf, pos + retsize);
    return retsize;
}

//uint8_t oled_updateDisplay(bool* status_changed) {
uint8_t oled_updateDisplay() {    

    uint8_t ret = 0x20;

    SeeedOLED_setXY(0, 0);
    
    ret = i2c_start(SeeedOLED_Address_WRITE);        // begin I2C communication
    if(ret) goto out;

    ret = i2c_write(SeeedOLED_Data_Mode);            // data mode
    if(ret) goto out;

    for (uint8_t i = 0; i < 128; i++) {
        ret = i2c_write(layerline_buf_1[i]);
        if(ret) goto out;
    }
    for (uint8_t i = 0; i < 128; i++) {
        ret = i2c_write(layerline_buf_2[i]);
        if(ret) goto out;
    }
    for (uint8_t i = 0; i < 128; i++) {
        ret = i2c_write(modifierline_buf[i]);
        if(ret) goto out;
    }

out:
    i2c_stop(); // stop I2C transmission
    return ret;
}

// oled update
uint8_t oled_update(uint32_t default_layer_state, uint32_t layer_state, uint8_t keyboard_leds, bool keypressed) {
    bool updated = false;
    
    // Layer state
    if(layer_state !=  prev_layers_state){
        updated = true;

        oled_clearLineBuf(layerline_buf_1);
        oled_clearLineBuf(layerline_buf_2);

        uint8_t i=0;
        uint8_t idx = 20;        
        for(; i<=2; ++i){
            if (layer_state & (1<<i)) {
                idx += layer_set_num(i, layerline_buf_1, layerline_buf_2, idx);
                idx += 8;
            }
        }

        // backup prev state
        prev_layers_state = layer_state;
    }

    // LED state
    if (prev_keyboard_leds ^ keyboard_leds) {
        updated = true;
        
        oled_clearLineBuf(modifierline_buf);
        
        // numlock state
        if(keyboard_leds & (1<<0)){
            buf_set_image_P(NumlkImage, 16, modifierline_buf, 0);
        }
        // capslock state
        if(keyboard_leds & (1<<1)){
            buf_set_image_P(CaplkImage, 16, modifierline_buf, 20);
        }
        
        prev_keyboard_leds = keyboard_leds;
    }

    // if something is updated, rotate the wheel
    if (updated || keypressed) {
        // rotate the wheel
        buf_set_image_P(Rotator[rotator_index>>1], 8, layerline_buf_1, 0);
        rotator_index++;
        if(rotator_index>=16)
            rotator_index=0;

        // change display contents
        oled_updateDisplay();
    }
  
    return 0;
}

