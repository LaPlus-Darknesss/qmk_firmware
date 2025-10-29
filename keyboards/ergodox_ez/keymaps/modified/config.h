#pragma once

// Tap-dance / combo timing — your requested starting point
#define TAPPING_TERM 200
#define COMBO_TERM 35

// Keep combos very deliberate
#define COMBO_ONLY_FROM_LAYER 0   // allow from base
#define COMBO_MUST_TAP_PER_COMBO  // avoid rolls-from-hold triggering

// Mouse comfort
#define MOUSEKEY_DELAY       0
#define MOUSEKEY_INTERVAL    16
#define MOUSEKEY_MAX_SPEED   7
#define MOUSEKEY_TIME_TO_MAX 100

// We’ll use right LED #1 as mouse-layer indicator on the ErgoDox EZ
