// This is the canonical layout file for the Quantum project. If you want to add another keyboard,
// this is the style you want to emulate.

#include "planck.h"
#include "action_layer.h"
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#include "eeconfig.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _DVORAK 1
#define _LOWER 2
#define _RAISE 3
#define _ADJUST 16

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  DVORAK,
  LOWER,
  RAISE,
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
#define __XXX__ KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = {
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC},
  {KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },
  {KC_GRV, KC_LCTL, KC_LALT, KC_LGUI,  KC_LCTL, RAISE, LOWER, KC_RALT,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * |   "  |   ,  |   .  |   P  |   Y  |  DEL | Bksp |   F  |   G  |   C  |   R  |   L  | 
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |   A  |   O  |   E  |   U  |   I  |  TAB | Enter|   D  |   H  |   T  |   N  |   S  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |   ;  |   Q  |   J  |   K  |   X  |   =  |   -  |   B  |   M  |   W  |   V  |   Z  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   `  |  GUI | Alt  |  S&S | Ctrl | Raise| Lower|  Alt |  S&S |  Ctrl|   \  |   /  |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK] = {
    {KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_DEL,  KC_BSPC, KC_F,    KC_G,    KC_C,    KC_R,    KC_L},
    {KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_TAB,  KC_ENT,  KC_D,    KC_H,    KC_T,    KC_N,    KC_S},
    {KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    SFT_T(KC_EQL),  SFT_T(KC_MINS), KC_B,    KC_M,    KC_W,    KC_V,    KC_Z},
    {LT(_LOWER,KC_GRV), KC_LGUI, RAISE, LSFT_T(KC_SPC), KC_LCTL, RAISE, LOWER, KC_RALT, RSFT_T(KC_SPC), LOWER, KC_BSLS, KC_SLSH}
},

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   !  |   @  |   #  |   $  |   %  |  INS |  Bksp|   ^  |   &  |   *  |   (  |   )  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |  ESC |      |   [  |   ]  |      |      | Enter|      |  Lft |   Up | Rght |   _  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |   {  |   }  |      |   =  |    - |      |  Lft |  Dwn | Rght |   ?  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   `  | LSft | Alt  |  S&S | Ctrl | Raise| Lower|  Alt |  S&S |  Ctrl| RSft |   /  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = {
    {KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_INS,  KC_BSPC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN,  KC_RPRN},
    {KC_ESC,  KC_LBRC, KC_RBRC, KC_LPRN,  KC_RPRN, __XXX__, KC_ENT,  __XXX__, KC_LEFT, KC_UP,   KC_RGHT, KC_UNDS},
    {__XXX__, __XXX__, __XXX__, KC_LCBR, KC_RCBR, KC_EQL,  KC_MINS, __XXX__, KC_LEFT, KC_DOWN, KC_RGHT,  KC_QUES},
    {_______,  KC_LGUI, _______, LSFT_T(KC_SPC), KC_LCTL, RAISE, LOWER, KC_RALT, RSFT_T(KC_SPC), _______, KC_RSFT, _______}    
},

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   1  |   2  |   3  |   4  |   5  |      |      |   6  |   7  |   8  |   9  |   0  | 
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |      |      |      |   4  |   5  |   6  |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      |   1  |   2  |   3  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   `  |  GUI | Alt  |  S&S | Ctrl | Raise| Lower|      |   0  |   ,  |   .  |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = {
  {KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    __XXX__,  __XXX__,     KC_6,    KC_7,    KC_8,    KC_9,     KC_0},
  {KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   __XXX__,  __XXX__,  __XXX__,    KC_4,    KC_5,    KC_6,    __XXX__},
  {KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,  __XXX__,    KC_1,    KC_2,    KC_3,    __XXX__},
  {_______, KC_LGUI, _______, KC_LCTL, SFT_T(KC_SPC), RAISE, LOWER, __XXX__,    KC_0, KC_COMM,  _______, _______}
},

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = {
  {__XXX__, RESET,   __XXX__, __XXX__, __XXX__, __XXX__, __XXX__, __XXX__, __XXX__, __XXX__, __XXX__, KC_DEL},
  {__XXX__, __XXX__, __XXX__, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  __XXX__, DVORAK,  __XXX__, __XXX__},
  {__XXX__, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  __XXX__, __XXX__, __XXX__, __XXX__, __XXX__},
  {_______, __XXX__, __XXX__, __XXX__, _______, _______, _______, _______, __XXX__, __XXX__, __XXX__, _______}
}
};

#ifdef AUDIO_ENABLE

float tone_startup[][2]    = SONG(STARTUP_SOUND);
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);

float tone_goodbye[][2] = SONG(GOODBYE_SOUND);
#endif


void persistant_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_qwerty);
        #endif
        persistant_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case DVORAK:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_dvorak);
        #endif
        persistant_default_layer_set(1UL<<_DVORAK);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
}

#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
    PLAY_SONG(tone_startup);
}

void shutdown_user()
{
    PLAY_SONG(tone_goodbye);
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_SONG(music_scale);
}

#endif
