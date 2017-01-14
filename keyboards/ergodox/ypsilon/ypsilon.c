#include "ypsilon.h"
#include "i2cmaster.h"
#include "SeeedOLED.h"
#include "timer.h"

bool i2c_initialized = 0;
uint8_t mcp23018_status = 0x20;

#define DISP_SLEEP 100000
bool display_on_state;
uint32_t disp_off_timer = 0;

bool is_shift_pressed (void) {
    return keyboard_report->mods == MOD_BIT(KC_LSFT) || keyboard_report->mods == MOD_BIT(KC_RSFT);
}

bool is_ctrl_pressed(void) {
    return keyboard_report->mods == MOD_BIT(KC_LCTL) || keyboard_report->mods == MOD_BIT(KC_RCTL);
}

bool is_alt_pressed(void) {
    return keyboard_report->mods == MOD_BIT(KC_LALT) || keyboard_report->mods == MOD_BIT(KC_RALT);
}

bool is_gui_pressed(void) {
    return keyboard_report->mods == MOD_BIT(KC_LGUI) || keyboard_report->mods == MOD_BIT(KC_RGUI);
}

void matrix_init_kb(void) {
   // keyboard LEDs (see "PWM on ports OC1(A|B|C)" in "teensy-2-0.md")
    TCCR1A = 0b10101001;  // set and configure fast PWM
    TCCR1B = 0b00001001;  // set and configure fast PWM

    // (tied to Vcc for hardware convenience)
    DDRB  &= ~(1<<4);  // set B(4) as input
    PORTB &= ~(1<<4);  // set B(4) internal pull-up disabled

    // unused pins - C7, D4, D5, D7, E6
    // set as input with internal pull-ip enabled
    DDRC  &= ~(1<<7);
    DDRD  &= ~(1<<7 | 1<<5 | 1<<4);
    DDRE  &= ~(1<<6);
    PORTC |=  (1<<7);
    PORTD |=  (1<<7 | 1<<5 | 1<<4);
    PORTE |=  (1<<6);

    ergodox_blink_all_leds();

    matrix_init_user();

    oled_init();
}

void ergodox_blink_all_leds(void)
{
    ergodox_led_all_off();
    ergodox_led_all_set(LED_BRIGHTNESS_HI);
    ergodox_right_led_1_on();
    _delay_ms(50);
    ergodox_right_led_2_on();
    _delay_ms(50);
    ergodox_right_led_3_on();
    _delay_ms(50);
    ergodox_right_led_1_off();
    _delay_ms(50);
    ergodox_right_led_2_off();
    _delay_ms(50);
    ergodox_right_led_3_off();
    ergodox_led_all_on();
    _delay_ms(1000);
    ergodox_led_all_off();
}

uint8_t init_mcp23018(void) {
    mcp23018_status = 0x20;

    // I2C subsystem
    if (i2c_initialized == 0) {
        i2c_init();  // on pins D(1,0)
        i2c_initialized++;
        _delay_ms(1000);
    }

    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0
    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(IODIRA);            if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00000000);        if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00111111);        if (mcp23018_status) goto out;
    i2c_stop();

    // set pull-up
    // - unused  : on  : 1
    // - input   : on  : 1
    // - driving : off : 0
    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(GPPUA);             if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00000000);        if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00111111);        if (mcp23018_status) goto out;

out:
    i2c_stop();

    return mcp23018_status;
}

// oled update
int rotator_index;
uint8_t prev_keyboard_leds;
uint8_t prev_keyboard_modifier_keys;
uint8_t prev_keyboard_keys[6];
uint32_t prev_layers_state;

uint8_t layerline_buf_1[128]; // layer line1 buffer
uint8_t layerline_buf_2[128]; // layer line2 buffer
uint8_t modifierline_buf[128];   // modifier line buffer

uint8_t oled_init(void) {
    uint8_t ret = 0;
    rotator_index = 0;
    prev_keyboard_leds = 0;
    prev_keyboard_modifier_keys = 0;

    display_on_state = true;
    disp_off_timer = timer_read32();
    /*
      for(int i=0; i<16; ++i)
      prev_layers_state[i] = -1;
      for(int i=0; i<6; ++i)
      prev_keyboard_keys[i] = -1;
    */

    ret = SeeedOLED_init();

    oled_DispLogo();

    oled_clearLineBuf(layerline_buf_1);
    oled_clearLineBuf(layerline_buf_2);
    oled_clearLineBuf(modifierline_buf);

    SeeedOLED_setHorizontalScrollProperties(Scroll_Left, 0, 4, Scroll_5Frames);
    
    return ret;
}

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
    if (prev_keyboard_leds ^ keyboard_leds ||
        prev_keyboard_modifier_keys ^ keyboard_report->mods) {
        updated = true;
        
        oled_clearLineBuf(modifierline_buf);
        
        // numlock state
        if(keyboard_leds & (1<<0)){
            set_numlock_image(modifierline_buf);
        }
        // capslock state
        if(keyboard_leds & (1<<1)){
            set_capslock_image(modifierline_buf);
        }

        if(is_shift_pressed()) {
            set_shift_image(modifierline_buf);
        }
        if(is_alt_pressed()) {
            set_alt_image(modifierline_buf);
        }
        if(is_ctrl_pressed()) {
            set_ctrl_image(modifierline_buf);
        }
        if(is_gui_pressed()) {
            set_gui_image(modifierline_buf);
        }
        
        prev_keyboard_leds = keyboard_leds;
        prev_keyboard_modifier_keys = keyboard_report->mods;
    }

    // if something is updated, rotate the wheel
    if (updated || keypressed) {
        if (display_on_state == false) {
            oled_display_on();
            display_on_state = true;
        }
        // rotate the wheel
        update_rotator(layerline_buf_1, rotator_index);
        rotator_index++;
        if(rotator_index>=16)
            rotator_index=0;

        // change display contents
        oled_updateDisplay(layerline_buf_1, layerline_buf_2, modifierline_buf);

        disp_off_timer = timer_read32();
    } else {
        if (TIMER_DIFF_32(timer_read32(), disp_off_timer) > DISP_SLEEP && display_on_state == true) {
            oled_display_off();
            display_on_state = false;
        }
    }  
    return 0;
}


#ifdef ONEHAND_ENABLE
__attribute__ ((weak))
// swap-hands action needs a matrix to define the swap
const keypos_t hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    /* Left hand, matrix positions */
    {{0,13}, {1,13}, {2,13}, {3,13}, {4,13}, {5,13}},
    {{0,12}, {1,12}, {2,12}, {3,12}, {4,12}, {5,12}},
    {{0,11}, {1,11}, {2,11}, {3,11}, {4,11}, {5,11}},
    {{0,10}, {1,10}, {2,10}, {3,10}, {4,10}, {5,10}},
    {{0,9}, {1,9}, {2,9}, {3,9}, {4,9}, {5,9}},
    {{0,8}, {1,8}, {2,8}, {3,8}, {4,8}, {5,8}},
    {{0,7}, {1,7}, {2,7}, {3,7}, {4,7}, {5,7}},
    /* Right hand, matrix positions */
    {{0,6}, {1,6}, {2,6}, {3,6}, {4,6}, {5,6}},
    {{0,5}, {1,5}, {2,5}, {3,5}, {4,5}, {5,5}},
    {{0,4}, {1,4}, {2,4}, {3,4}, {4,4}, {5,4}},
    {{0,3}, {1,3}, {2,3}, {3,3}, {4,3}, {5,3}},
    {{0,2}, {1,2}, {2,2}, {3,2}, {4,2}, {5,2}},
    {{0,1}, {1,1}, {2,1}, {3,1}, {4,1}, {5,1}},
    {{0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}},
};
#endif
