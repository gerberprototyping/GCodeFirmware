#ifndef __CONFIG_H
#define __CONFIG_H


#define SUB_MM_RESOLUTION       1000

#define STEPPING                8
#define X_STEPPING              STEPPING
#define Y_STEPPING              STEPPING
#define Z_STEPPING              STEPPING

#define STEPS_PER_MM            100
#define X_STEPS_PER_MM          STEPS_PER_MM
#define Y_STEPS_PER_MM          STEPS_PER_MM
#define Z_STEPS_PER_MM          STEPS_PER_MM

#define ENABLE_PIN              ARDUINO_D8          // Active Low
#define COOLANT_EN_PIN          ARDUINO_A3
#define RESET_PIN               ARDUINO_A2          // Active Low
#define FEED_HOLD_PIN           ARDUINO_A1          // Active Low?
#define ABORT_PIN               ARDUINO_A0
#define A_DIR_PIN               ARDUINO_D13
#define X_DIR_PIN               ARDUINO_D5
#define X2_DIR_PIN
#define Y_DIR_PIN               ARDUINO_D6
#define Y2_DIR_PIN              A_DIR_PIN
#define Z_DIR_PIN               ARDUINO_D7
#define Z2_DIR_PIN
#define A_STEP_PIN              ARDUINO_D12
#define X_STEP_PIN              ARDUINO_D2
#define X2_STEP_PIN
#define Y_STEP_PIN              ARDUINO_D3
#define Y2_STEP_PIN             A_STEP_PIN
#define Z_STEP_PIN              ARDUINO_D4
#define Z2_STEP_PIN

#define DUAL_DRIVE_X            false
#define DUAL_DRIVE_Y            true
#define DUAL_DRIVE_Z            false


#define X_LIMIT_PIN             ARDUINO_D9
#define X2_LIMIT_PIN
#define Y_LIMIT_PIN             ARDUINO_D10
#define Y2_LIMIT_PIN            ARDUINO_D11
#define Z_LIMIT_PIN             ARDUINO_D11
#define Z2_LIMIT_PIN
#define X_NORMALLY_OPEN         false
#define Y_NORMALLY_OPEN         false
#define Z_NORMALLY_OPEN         false


#endif