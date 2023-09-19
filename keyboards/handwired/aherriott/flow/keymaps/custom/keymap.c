// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "pointing_device.h"

void pointing_device_init_kb(void) {
#ifdef PMW3360_CPI
    pointing_device_set_cpi(PMW3360_CPI);
#endif
}

// Layers
enum {
    DEFAULT,
    MOUSE,
    SYM,
    NAV,
    NUM,
    VSCODE
};

//***************************************
// Name Shorthand
//***************************************

#define XXX         KC_NO
#define ___         KC_TRNS

// Homerow mods:
#define HR_A LGUI_T(KC_A)
#define HR_R LALT_T(KC_R)
#define HR_S LSFT_T(KC_S)
#define HR_T LCTL_T(KC_T)
#define HR_N RCTL_T(KC_N)
#define HR_E RSFT_T(KC_E)
#define HR_I RALT_T(KC_I)
#define HR_O RGUI_T(KC_O)

// Layer taps:
#define TL1 LT(NAV, KC_BSPC)
#define TL2 LT(NUM, KC_ENT)
#define TR1 LT(SYM, KC_SPC)
#define TR2 LT(VSCODE, KC_TAB)

// Generic
#define SAVE LCTL(KC_S)
#define FIND LCTL(KC_F)
#define CMMNT LCTL(KC_SLSH)
#define PASTE LCTL(KC_V)
#define UNDO LCTL(KC_Z)
#define REDO LCTL(KC_Y)
#define WORD_LEFT   LCTL(KC_LEFT)
#define WORD_RIGHT  LCTL(KC_RIGHT)
#define SEL_L_LEFT LSFT(KC_HOME)
#define SEL_W_LEFT LSFT(LCTL(KC_LEFT))
#define SEL_W_RIGHT LSFT(LCTL(KC_RIGHT))
#define SEL_L_RIGHT LSFT(KC_END)

// App switching (use with jumpapp on ubuntu, etc)
#define HYP_0 HYPR(KC_0)
#define HYP_1 HYPR(KC_1)
#define HYP_2 HYPR(KC_2)
#define HYP_3 HYPR(KC_3)
#define HYP_4 HYPR(KC_4)
#define HYP_5 HYPR(KC_5)
#define HYP_6 HYPR(KC_6)

// VSCode
#define HOME_FILE   LCTL(KC_HOME)
#define END_FILE    LCTL(KC_END)
#define PALLATE     LCTL(LSFT(KC_P))
#define MV_L_DOWN   LALT(KC_DOWN)
#define MV_L_UP     LALT(KC_UP)
#define TOG_TERM    LCTL(KC_GRV)
#define NEW_TERM    LCTL(LSFT(KC_GRV))
#define FTREE_TOG   LCTL(KC_B)
#define FILE_LEFT   LCTL(KC_PGUP)
#define FILE_RIGHT  LCTL(KC_PGDN)
#define BOX_LEFT    LCTL(LSFT(LALT(KC_LEFT)))
#define BOX_UP      LCTL(LSFT(LALT(KC_UP)))
#define BOX_DOWN    LCTL(LSFT(LALT(KC_DOWN)))
#define BOX_RIGHT   LCTL(LSFT(LALT(KC_RIGHT)))
#define TOG_WRDW    LALT(KC_Z)
#define SEL_ALL_OC  LCTL(LSFT(KC_L))
#define CUR_DOWN    LCTL(LALT(KC_UP))
#define CUR_UP      LCTL(LALT(KC_DOWN))
#define CUR_UNDO    LCTL(KC_U)

//***************************************
// Tap Dance
//***************************************

enum tap_dance {
    CPY_CUT,
    BCK_FWD,
    FND_REP,
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [CPY_CUT] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_C), LCTL(KC_X)),
    [BCK_FWD] = ACTION_TAP_DANCE_DOUBLE(LALT(KC_LEFT), LALT(KC_RIGHT)),
    [FND_REP] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_F), LCTL(KC_H)),
};

//***************************************
// Custom Keys
//***************************************

enum custom_keycodes {
    // Macros
    SCRL_UP = SAFE_RANGE,
    SCRL_DOWN,
    DBL_PIPE,
    DBL_EQL,
    DBL_COLN,
    SN_TOGG,
    DDOT_SLSH,
    SEL_LINE,
    // VSCode Macros
    FOLD_REG,
    UNFOLD_REG,
    ZEN_MODE,
    RVL_EXP,
};

static bool snake_case = false;

// Macro definitions
#define MACRO(name, string)             \
    case name:                          \
        if(record->event.pressed) {     \
            SEND_STRING(string);        \
        }                               \
        break;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode)
    {
        // Generic Macros
        MACRO(SCRL_UP, SS_TAP(X_UP)
            SS_TAP(X_UP)
            SS_TAP(X_UP)
            SS_TAP(X_UP)
            SS_TAP(X_UP))
        MACRO(SCRL_DOWN, SS_TAP(X_DOWN)
            SS_TAP(X_DOWN)
            SS_TAP(X_DOWN)
            SS_TAP(X_DOWN)
            SS_TAP(X_DOWN))
        MACRO(DBL_PIPE, "||")
        MACRO(DBL_EQL, "==")
        MACRO(DBL_COLN, "::")
        MACRO(DDOT_SLSH, "../")
        MACRO(SEL_LINE, SS_TAP(X_HOME)SS_LSFT(SS_TAP(X_END)))
        // VSCode Macros
        MACRO(FOLD_REG, SS_LCTL("k0"))
        MACRO(UNFOLD_REG, SS_LCTL("kj"))
        MACRO(ZEN_MODE, SS_LCTL("k")"z")
        MACRO(RVL_EXP, SS_LCTL("k")"r")
        // Snake Case
        case SN_TOGG:
            if(record->event.pressed) {
                snake_case = true;
            }
            return true;
        case KC_A ... KC_Z:
            if(snake_case){
                if(record->event.pressed) {
                    register_mods(MOD_LSFT);
                } else {
                    unregister_mods(MOD_LSFT);
                }
                return true;
            }
            break;
        case KC_SPC:
            if(snake_case){
                if(record->event.pressed) {
                    register_mods(MOD_LSFT);
                    tap_code(KC_MINUS);
                    unregister_mods(MOD_LSFT);
                }
                return false;
            }
            break;
        default:
            snake_case = false;
            break;
    }
    return true;
}

//***************************************
// Combos
//***************************************

// To mouse layer & back
const uint16_t PROGMEM to_mouse[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM no_mouse[] = {KC_WH_D, KC_WH_R, COMBO_END};
const uint16_t PROGMEM to_default[] = {KC_MUTE, KC_VOLU, COMBO_END};
const uint16_t PROGMEM no_default[] = {KC_L, KC_U, COMBO_END};

combo_t key_combos[] = {
    COMBO(to_mouse, TO(MOUSE)),
    COMBO(to_default, TO(DEFAULT)),
    COMBO(no_mouse, KC_NO),
    COMBO(no_default, KC_NO),
};

//***************************************
// Custom Shift
//***************************************

const key_override_t comma_qmark = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_QUES);
const key_override_t dot_excl = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_EXLM);
const key_override_t fslh_bslh = ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, KC_BSLS);
const key_override_t **key_overrides = (const key_override_t *[]){
    &comma_qmark,
    &dot_excl,
    &fslh_bslh,
    NULL // Null terminate the array of overrides!
};

//**************************************************
// Timing
//**************************************************

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HR_R:
        case HR_S:
        case HR_T:
        case HR_N:
        case HR_E:
        case HR_I:
            return 130;
        case HR_O:
        case HR_A:
            return 160;
        case TL1:
        case TL2:
        case TR1:
        case TR2:
            return 160;
        default:
            return 180;
    }
}

//**************************************************
// Keymap!
//**************************************************

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEFAULT] = LAYOUT(
        KC_Q,    KC_W,  KC_F,    KC_P,  KC_B,           KC_J,   KC_L,   KC_U,       KC_Y,    KC_SCLN,
        HR_A,    HR_R,  HR_S,    HR_T,  KC_G,           KC_M,   HR_N,   HR_E,       HR_I,    HR_O,
        KC_Z,    KC_X,  KC_C,    KC_D,  KC_V,           KC_K,   KC_H,   KC_COMM,    KC_DOT,  KC_SLSH,
                        KC_DEL,  TL1,   TL2,            TR2,    TR1,    KC_ESC
    ),

    [MOUSE] = LAYOUT(
        KC_WH_L,    KC_WH_U,        KC_WH_D,    KC_WH_R,    XXX,            XXX,    KC_MUTE,    KC_VOLU,    KC_VOLD,    KC_BRIU,
        KC_LCTL,    KC_LSFT,        KC_BTN2,    KC_BTN1,    KC_BTN3,        XXX,    XXX,        XXX,        XXX,        XXX,
        KC_LALT,    LCTL(KC_LSFT),  XXX,        XXX,        XXX,            XXX,    KC_MRWD,    KC_MPLY,    KC_MFFD,    KC_BRID,
                                    ___,        ___,        ___,            ___,    ___,        ___
    ),

    [SYM] = LAYOUT(
        KC_PIPE,    KC_PLUS,    KC_AT,      KC_LCBR,    KC_RCBR,        KC_CIRC,    KC_PERC,    KC_DLR,     CW_TOGG,    DBL_COLN,
        KC_AMPR,    KC_MINUS,   KC_EQL,     KC_LPRN,    KC_RPRN,        KC_TILD,    KC_QUOT,    KC_DQT,     KC_UNDS,    KC_HASH,
        DBL_PIPE,   KC_ASTR,    DBL_EQL,    KC_LBRC,    KC_RBRC,        KC_GRV,     KC_LT,      KC_GT,      SN_TOGG,    DDOT_SLSH,
                                ___,        ___,        ___,            XXX,        ___,        QK_BOOT
    ),

    [NAV] = LAYOUT(
        KC_HOME,    TD(BCK_FWD),    TD(FND_REP),    KC_END,     KC_PSCR,        KC_F10,    KC_F7,  KC_F8,  KC_F9,  KC_F13,
        KC_LEFT,    KC_UP,          KC_DOWN,        KC_RIGHT,   SAVE,           KC_F11,    KC_F4,  KC_F5,  KC_F6,  KC_F14,
        UNDO,       REDO,           TD(CPY_CUT),    PASTE,      CMMNT,          KC_F12,    KC_F1,  KC_F2,  KC_F3,  KC_F15,
                                    QK_BOOT,        ___,        XXX,            ___,    ___,    ___
    ),

    [NUM] = LAYOUT(
        SEL_L_LEFT,     SEL_W_LEFT,     SEL_W_RIGHT,    SEL_L_RIGHT,    SEL_LINE,           KC_PERC,    KC_7,   KC_8,   KC_9,   KC_COLN,
        OSM(MOD_LGUI),  OSM(MOD_LALT),  OSM(MOD_LSFT),  OSM(MOD_LCTL),  QK_REP,             KC_DOT,     KC_4,   KC_5,   KC_6,   KC_0,
        HYP_4,          HYP_3,          HYP_2,          HYP_1,          HYP_0,              KC_COMM,    KC_1,   KC_2,   KC_3,   KC_SLSH,
                                        QK_RBT,         XXX,            ___,                ___,        ___,    ___
    ),

    [VSCODE] = LAYOUT(
        HOME_FILE,      SCRL_UP,        SCRL_DOWN,      END_FILE,       FOLD_REG,           PALLATE,    WORD_LEFT,  MV_L_DOWN,  MV_L_UP,    WORD_RIGHT,
        OSM(MOD_LGUI),   OSM(MOD_LALT), OSM(MOD_LSFT),  OSM(MOD_LCTL),  QK_REP,             ZEN_MODE,   FILE_LEFT,  TOG_TERM,   FTREE_TOG,  FILE_RIGHT,
        BOX_LEFT,       BOX_UP,         BOX_DOWN,       BOX_RIGHT,      UNFOLD_REG,         TOG_WRDW,   RVL_EXP,    CUR_DOWN,   CUR_UP,     CUR_UNDO,
                                        ___,            ___,            ___,                ___,        XXX,        QK_RBT
    ),
};
