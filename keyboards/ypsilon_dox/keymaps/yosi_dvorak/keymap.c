#include QMK_KEYBOARD_H
#include "version.h"
// #include "ergodox.h"
// #include "debug.h"
// #include "action_layer.h"

// A 'transparent' key code (that falls back to the layers below it).
#define ___ KC_TRANSPARENT

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define MDIA 2 // media keys

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 *    ,--------------------------------------------------.           ,--------------------------------------------------.
 *    |   =    |   1  |   2  |   3  |   4  |   5  | NumLk|           | Del  |   6  |   7  |   8  |   9  |   0  |   \    | 
 *    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------| 
 *    | Tab    |   '  |   ,  |   .  |   P  |   Y  |  L1  |           |  L1  |   F  |   G  |   C  |   R  |   L  |   /    | 
 *    |--------+------+------+------+------+------| (mo) |           | (mo) |------+------+------+------+------+--------|
 *    | Esc    |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |   S  |   -    |
 *    |--------+------+------+------+------+------|  L2  |           |  L2  |------+------+------+------+------+--------|
 *    | LShift |   ;  |   Q  |   J  |   K  |   X  | (mo) |           | (mo) |   B  |   M  |   W  |   V  |   Z  | RShift |
 *    `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *          |  `    |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  |
 *           ----------------------------'                                       `---------------------------- 
 *                                        ,-------------.       ,-------------.
 *                                        | Ctl  | Alt  |       | Ctl  |  Alt   |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | GUI  |       | GUI  |        |      |
 *                                 | Space| Bksp |------|       |------| Enter  | Spc  |
 *                                 |      |      | Del  |       | Ins  |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
    [BASE] = LAYOUT_ydox(  // layer 0 : default
        // left hand
	KC_NO,     KC_EQL,     KC_1,     KC_2,     KC_3,     KC_4,    KC_5,      KC_NLCK,
        KC_NO,     KC_TAB,     KC_QUOT,  KC_COMM,  KC_DOT,   KC_P,    KC_Y,      MO(SYMB),
        KC_NO,     KC_ESC,     KC_A,     KC_O,     KC_E,     KC_U,    KC_I,
                   KC_LSFT,    KC_SCLN,  KC_Q,     KC_J,     KC_K,    KC_X,      MO(MDIA),
                               KC_GRV,   KC_LANG1, KC_LEFT,  KC_RGHT,
                   KC_LCTL,    KC_LALT,
                               KC_LGUI,
        SFT_T(KC_SPC),KC_BSPC, KC_DELT,
        
        // right hand
        KC_DELT,     KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_BSLS,  KC_NO,
        MO(SYMB),    KC_F,   KC_G,   KC_C,   KC_R,   KC_L,   KC_SLSH,  KC_NO,
                     KC_D,   KC_H,   KC_T,   KC_N,   KC_S,   KC_MINS,  KC_NO,
        MO(MDIA),    KC_B,   KC_M,   KC_W,   KC_V,   KC_Z,   KC_RSFT,
                             KC_UP,  KC_DOWN,KC_LBRC,KC_RBRC,
        KC_RCTL, KC_RALT,
        KC_RGUI,
        KC_INS,  KC_ENT, SFT_T(KC_SPC)
        ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |   0  |    . |   0  |   =  |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |  0   |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
    [SYMB] = LAYOUT_ydox(
        // left hand
        ___    , ___    , KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   ___    ,
        ___    , ___    , KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE, ___    ,
        ___    , ___    , KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,
                 ___    , KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, ___    ,
        ___    , ___    , ___    , ___    ,
        ___    , ___    ,
        ___    ,
        ___    ,___    ,___    ,
        // right hand
        ___    , KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  ___    ,
        ___    , KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_F12,  ___    ,
                 KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, ___    , ___    ,
        ___    , KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS, ___    ,
        KC_0,    KC_DOT,  KC_0,   KC_EQL,
        ___    , ___    ,
        ___    ,
        ___    , ___    , KC_0
        ),
/* Keymap 2: Media and mouse keys  456123789
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |      |      |      |      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
    LAYOUT_ydox( 
        ___    , ___    , ___    , ___    , ___    , ___    , ___    , ___    ,
        ___    , ___    , ___    , ___    , KC_MS_U, ___    , ___    , ___    ,
        ___    , ___    , ___    , KC_MS_L, KC_MS_D, KC_MS_R, ___    ,
                 ___    , ___    , ___    , ___    , ___    , ___    , ___    ,
        ___    , ___    , KC_BTN1, KC_BTN2,
        ___    , ___    ,
        ___    ,
        ___    , ___    , ___    ,
        // right hand
        ___    ,  ___    , ___    , ___    , ___    , ___    , ___    , ___    ,
        ___    ,  ___    , ___    , ___    , ___    , ___    , ___    , ___    ,
        ___    , ___    , ___    , ___    , ___    , KC_MPLY, ___    ,
        ___    ,  ___    , ___    , KC_MPRV, KC_MNXT, ___    , ___    ,
        KC_VOLU, KC_VOLD, KC_MUTE, ___    ,
        ___    , ___    ,
        ___    ,
        ___    , ___    , KC_WBAK
        ),
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    // MACRODOWN only works in this function
    switch(id) {
    case 0:
        if (record->event.pressed) {
            register_code(KC_RSFT);
        } else {
            unregister_code(KC_RSFT);
        }
        break;
    }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
        // TODO: Make this relevant to the ErgoDox EZ.
    case 1:
        ergodox_right_led_1_on();
        break;
    case 2:
        ergodox_right_led_2_on();
        break;
    default:
        // none
        break;
    }
};

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
    case BASE:
        oled_write_P(PSTR("Base\n"), false);
        break;
    case SYMB:
        oled_write_P(PSTR("Symbol\n"), false);
        break;
    case MDIA:
        oled_write_P(PSTR("Media\n"), false);
        break;
    default:
        // Or use the write_ln shortcut over adding '\n' to the end of your string
        oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}
#endif
