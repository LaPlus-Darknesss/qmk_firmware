#include QMK_KEYBOARD_H
#include "version.h"

// ─────────────────────────────────────────────────────────────────────────────
// Layers
// ─────────────────────────────────────────────────────────────────────────────
enum layer_names {
    _BASE,
    _MOUSE,
};

// shorthand
#define _____ KC_TRNS
#define XXXXX KC_NO

// ─────────────────────────────────────────────────────────────────────────────
// Custom keycodes
// ─────────────────────────────────────────────────────────────────────────────
enum custom_keycodes {
    TD_TAB = SAFE_RANGE,   // tap-dance Tab
    MS_TOG,                // mouse layer toggle (for combo output)
    C_COPY,                // Ctrl+C macro
    C_PASTE,               // Ctrl+V macro
    C_UNDO,                // Ctrl+Z macro
    C_REDO,                // Ctrl+Y macro
    CTRL_PLUS,             // Ctrl + '+' (zoom in)
    WIN_KEY,               // Win key (for Alt+C combo if enabled)
};

// ─────────────────────────────────────────────────────────────────────────────
// Tap Dance: Tab → single tap = Tab; double-tap & hold = hold Alt + send Tab
// ─────────────────────────────────────────────────────────────────────────────
static bool td_tab_alt_active = false;

void td_tab_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code(KC_TAB);
    } else if (state->count >= 2) {
        register_code(KC_LALT);
        td_tab_alt_active = true;
        tap_code(KC_TAB);
    }
}
void td_tab_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (td_tab_alt_active) {
        unregister_code(KC_LALT);
        td_tab_alt_active = false;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Combos
//  • You approved F11+F12 toggling mouse layer
//  • Your full set below. For combos that should output shifted punctuation,
//    we detect the *current* Shift state and emit the right symbol.
// ─────────────────────────────────────────────────────────────────────────────
enum combo_events {
    CB_F11F12_MTOG = 0,

    CB_ZX_UNDO,
    CB_XC_CUT,
    CB_CV_PASTE,
    CB_VB_REDO,

    CB_NM_COMMA,      // ',' or '<' if Shift held
    CB_MCOMM_DOT,     // '.' or '>' if Shift held
    CB_COMM_DOT_SLASH, // '/' or '?' if Shift held
    CB_DOT_SLSH_EQ,   // '=' or '+' if Shift held
    CB_L_SCLN_QUOTE,  // ''' or '"' if Shift held
    CB_90_MINUS,      // '-' or '_' if Shift held

    CB_SD_BSPC,
    CB_DF_TAB,
    CB_JK_ESC,
    CB_EB_ENT,

    // Arrows with your remapped left-side keys
    CB_E_BSPC_LEFT,   // E + (new Backspace key) = Left Arrow
    CB_E_SPC_RIGHT,   // E + (new Space key)     = Right Arrow
    CB_UN_UP,         // U + N = Up Arrow
    CB_UM_DOWN,       // U + M = Down Arrow

    CB_AS_SELALL,     // Ctrl+A
    CB_HJ_CTRL_PLUS,  // Ctrl + '+'

    // Optional dangerous ones are excluded by request (Alt+F4, etc.)
    // Optional Alt+C -> WinKey is defined but DISABLED by default (see bottom)
    CB_ALTC_WINKEY,
};

// Define combo keys by keycodes (works across base/mouse layers)
const uint16_t PROGMEM f11f12_combo[]   = {KC_F11, KC_F12, COMBO_END};

const uint16_t PROGMEM zx_combo[]       = {KC_Z, KC_X, COMBO_END};
const uint16_t PROGMEM xc_combo[]       = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM cv_combo[]       = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM vb_combo[]       = {KC_V, KC_B, COMBO_END};

const uint16_t PROGMEM nm_combo[]       = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM mcomm_combo[]    = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM commdot_combo[]  = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM dotslsh_combo[]  = {KC_DOT, KC_SLSH, COMBO_END};
const uint16_t PROGMEM l_scln_combo[]   = {KC_L, KC_SCLN, COMBO_END};
const uint16_t PROGMEM nine_zero_combo[]= {KC_9, KC_0, COMBO_END};

const uint16_t PROGMEM sd_combo[]       = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM df_combo[]       = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM jk_combo[]       = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM eb_combo[]       = {KC_E, KC_B, COMBO_END};

// The two “new” physical buttons you asked to remap on the left side:
//  • “new Backspace button” = the key that *was* Left Arrow on your left half
//  • “new Space button”     = the key that *was* Right Arrow on your left half
// We’ll reference them by their *new* base-layer keycodes (KC_BSPC / KC_SPC)
const uint16_t PROGMEM e_bspc_combo[]   = {KC_E, KC_BSPC, COMBO_END};
const uint16_t PROGMEM e_spc_combo[]    = {KC_E, KC_SPC,  COMBO_END};

const uint16_t PROGMEM un_combo[]       = {KC_U, KC_N, COMBO_END};
const uint16_t PROGMEM um_combo[]       = {KC_U, KC_M, COMBO_END};

const uint16_t PROGMEM as_combo[]       = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM hj_combo[]       = {KC_H, KC_J, COMBO_END};

// Optional Alt+C → WinKey (DISABLED by default; see combos[] below)
// const uint16_t PROGMEM altc_combo[]     = {KC_LALT, KC_C, COMBO_END};

combo_t key_combos[] = {
    [CB_F11F12_MTOG]  = COMBO_ACTION(f11f12_combo),

    [CB_ZX_UNDO]      = COMBO_ACTION(zx_combo),
    [CB_XC_CUT]       = COMBO_ACTION(xc_combo),
    [CB_CV_PASTE]     = COMBO_ACTION(cv_combo),
    [CB_VB_REDO]      = COMBO_ACTION(vb_combo),

    [CB_NM_COMMA]     = COMBO_ACTION(nm_combo),
    [CB_MCOMM_DOT]    = COMBO_ACTION(mcomm_combo),
    [CB_COMM_DOT_SLASH] = COMBO_ACTION(commdot_combo),
    [CB_DOT_SLSH_EQ]  = COMBO_ACTION(dotslsh_combo),
    [CB_L_SCLN_QUOTE] = COMBO_ACTION(l_scln_combo),
    [CB_90_MINUS]     = COMBO_ACTION(nine_zero_combo),

    [CB_SD_BSPC]      = COMBO(sd_combo, KC_BSPC),
    [CB_DF_TAB]       = COMBO(df_combo, KC_TAB),
    [CB_JK_ESC]       = COMBO(jk_combo, KC_ESC),
    [CB_EB_ENT]       = COMBO(eb_combo, KC_ENT),

    [CB_E_BSPC_LEFT]  = COMBO(e_bspc_combo, KC_LEFT),
    [CB_E_SPC_RIGHT]  = COMBO(e_spc_combo,  KC_RGHT),
    [CB_UN_UP]        = COMBO(un_combo, KC_UP),
    [CB_UM_DOWN]      = COMBO(um_combo, KC_DOWN),

    [CB_AS_SELALL]    = COMBO_ACTION(as_combo),
    [CB_HJ_CTRL_PLUS] = COMBO_ACTION(hj_combo),

    // Optional: enable at your own risk (can conflict with regular Alt+C usage)
    // [CB_ALTC_WINKEY]  = COMBO_ACTION(altc_combo),
};

// What each COMBO_ACTION does
void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) return;

    uint8_t mods = get_mods();

    switch (combo_index) {
        case CB_F11F12_MTOG:
            // Toggle mouse layer + LED blink
            if (layer_state_is(_MOUSE)) {
                layer_off(_MOUSE);
            } else {
                layer_on(_MOUSE);
            }
            // Quick visual blink on LED 1
            ergodox_right_led_1_on();
            wait_ms(120);
            if (!layer_state_is(_MOUSE)) {
                ergodox_right_led_1_off();
            }
            break;

        // Edit/system
        case CB_ZX_UNDO:   tap_code16(C(KC_Z)); break;
        case CB_XC_CUT:    tap_code16(C(KC_X)); break;
        case CB_CV_PASTE:  tap_code16(C(KC_V)); break;
        case CB_VB_REDO:   tap_code16(C(KC_Y)); break;

        // Punctuation that depends on Shift being held
        case CB_NM_COMMA:
            if (mods & (MOD_MASK_SHIFT)) tap_code16(KC_LT);  // '<'
            else                         tap_code(KC_COMM);  // ','
            break;
        case CB_MCOMM_DOT:
            if (mods & (MOD_MASK_SHIFT)) tap_code16(KC_GT);  // '>'
            else                         tap_code(KC_DOT);   // '.'
            break;
        case CB_COMM_DOT_SLASH:
            if (mods & (MOD_MASK_SHIFT)) tap_code16(KC_QUES); // '?'
            else                         tap_code(KC_SLSH);   // '/'
            break;
        case CB_DOT_SLSH_EQ:
            if (mods & (MOD_MASK_SHIFT)) tap_code16(KC_PLUS); // '+'
            else                         tap_code(KC_EQL);    // '='
            break;
        case CB_L_SCLN_QUOTE:
            if (mods & (MOD_MASK_SHIFT)) tap_code16(KC_DQUO); // '"'
            else                         tap_code(KC_QUOT);   // '''
            break;
        case CB_90_MINUS:
            if (mods & (MOD_MASK_SHIFT)) tap_code(KC_UNDS);   // '_'
            else                         tap_code(KC_MINS);   // '-'
            break;

        case CB_AS_SELALL:     tap_code16(C(KC_A)); break;
        case CB_HJ_CTRL_PLUS:  tap_code16(C(S(KC_EQL))); break; // Ctrl + '+'

        default: break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Process custom keycodes (macros & mouse toggle from places other than combo)
// ─────────────────────────────────────────────────────────────────────────────
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD_TAB:
            if (record->event.pressed) {
                // handled by tap-dance hooks above
            }
            return false;

        case MS_TOG:
            if (record->event.pressed) {
                if (layer_state_is(_MOUSE)) layer_off(_MOUSE);
                else                        layer_on(_MOUSE);
                // LED blink
                ergodox_right_led_1_on();
                wait_ms(120);
                if (!layer_state_is(_MOUSE)) ergodox_right_led_1_off();
            }
            return false;

        case C_COPY:  if (record->event.pressed) tap_code16(C(KC_C));  return false;
        case C_PASTE: if (record->event.pressed) tap_code16(C(KC_V));  return false;
        case C_UNDO:  if (record->event.pressed) tap_code16(C(KC_Z));  return false;
        case C_REDO:  if (record->event.pressed) tap_code16(C(KC_Y));  return false;

        case CTRL_PLUS: if (record->event.pressed) tap_code16(C(S(KC_EQL))); return false;
        case WIN_KEY:   if (record->event.pressed) tap_code(KC_LGUI);        return false;

        default: break;
    }
    return true;
}

// Esc exits mouse layer as a convenience
layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, _MOUSE)) {
        ergodox_right_led_1_on();
    } else {
        ergodox_right_led_1_off();
    }
    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) { return false; }

// ─────────────────────────────────────────────────────────────────────────────
// Keymaps
//  • Base layer is essentially your Oryx layout, with two requested swaps on
//    the left side: the keys that *were* ← and → now become Backspace / Space.
//  • Right thumb (base): Space → Ctrl+V, Enter → Ctrl+C (as you asked).
//  • Mouse layer remaps those same right-thumb keys to Left/Right click.
// ─────────────────────────────────────────────────────────────────────────────
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// ========== BASE ==========
[_BASE] = LAYOUT_ergodox_pretty(
  // Left hand
  KC_GRV,  KC_1,   KC_2,    KC_3,    KC_4,     KC_5,   KC_LBRC,
  KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,     KC_T,   KC_RBRC,
  KC_ESC,  KC_A,   KC_S,    KC_D,    KC_F,     KC_G,
  KC_LSFT, KC_Z,   KC_X,    KC_C,    KC_V,     KC_B,   KC_MINS,
  KC_LCTL, KC_LALT,KC_LGUI, XXXXX,   XXXXX,
                                        KC_BSPC, TD_TAB,       // (Left thumb) Backspace, TapDance Tab
                                                 KC_HOME,
                            KC_SPC,    KC_DEL,  KC_ENT,        // (Left thumb) Space, Delete, Enter

  // Right hand
  KC_EQL,  KC_6,   KC_7,    KC_8,    KC_9,     KC_0,   KC_BSLS,
  KC_QUOT, KC_Y,   KC_U,    KC_I,    KC_O,     KC_P,   KC_SLSH,
           KC_J,   KC_K,    KC_L,    KC_SCLN,  KC_H,
  KC_UNDS, KC_N,   KC_M,    KC_COMM, KC_DOT,   KC_SLSH,KC_RSFT,
                   XXXXX,   XXXXX,   KC_RGUI,  KC_RALT,KC_RCTL,
  TD_TAB,  MO(_MOUSE),  // (Right thumb outer-left) TapDance Tab; (outer-right) momentary mouse (backup)
  KC_END,
  C_PASTE, KC_DEL, C_COPY  // (Right thumb)  Space → Ctrl+V, Delete, Enter → Ctrl+C  (per your request)
),

// ========== MOUSE ==========
[_MOUSE] = LAYOUT_ergodox_pretty(
  // Left
  _____, _____, _____, _____, _____, _____, _____,
  _____, _____, _____, KC_MS_U,_____, _____, _____,
  _____, _____, KC_MS_L,KC_MS_D,KC_MS_R, _____,
  _____, _____, KC_WH_U,KC_BTN1,KC_BTN2,KC_WH_D, _____,
  _____, _____, _____, _____, _____,
                                   _____, MS_TOG,  // Left thumb: keep Backspace; TD_Tab as-is
                                            _____,
                       _____,       _____,  _____,

  // Right
  _____, _____, _____, _____, _____, _____, _____,
  _____, _____, _____, _____, _____, _____, _____,
         KC_H,  KC_J,  KC_K,  KC_L,  _____,
  _____, _____, KC_M,  _____, _____, _____, _____,
                 _____, _____, _____, _____, _____,
  MS_TOG, _____,
  _____,
  KC_BTN1, _____, KC_BTN2   // Right thumb: Space→LeftClick, Enter→RightClick (while in mouse layer)
)
};

// Esc exits mouse layer as well
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (layer_state_is(_MOUSE) && keycode == KC_ESC && record->event.pressed) {
        layer_off(_MOUSE);
        ergodox_right_led_1_off();
        return false;
    }
    return true;
}

// Register tap dance
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_TAB - SAFE_RANGE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_tab_finished, td_tab_reset),
};
