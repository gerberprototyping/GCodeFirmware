#ifndef __CONFIG_H
#define __CONFIG_H




#define POSITIVE                            true
#define NEGATIVE                            false




#define UART_ECHO                           true




#define GCODE_ACK_MSG                       "ACK"
#define GCODE_NACK_SYNTAX_MSG               "NACK syntax error"
#define GCODE_JOB_START_MSG                 "Ready"
#define GCODE_JOB_DONE_MSG                  "Job complete"

#define GCODE_RX_BUFF_SIZE                  256
#define GCODE_MAX_WORDS_PER_LINE            20




#define STEP_INTERRUPT_PERIOD               1000
#define STEP_INSTRUCTION_BUFFER_SIZE        8



#define STEPPING                            8
#define X_STEPPING                          STEPPING
#define Y_STEPPING                          STEPPING
#define Z_STEPPING                          STEPPING

#define FULL_STEPS_PER_MM                   100
#define STEPS_PER_MM                        FULL_STEPS_PER_MM*STEPPING
#define X_STEPS_PER_MM                      STEPS_PER_MM
#define Y_STEPS_PER_MM                      STEPS_PER_MM
#define Z_STEPS_PER_MM                      STEPS_PER_MM

#define MAX_SPEED                           600                                                // Max speed in mm/min (double)
#define MAX_PACE                            ((uint32_t)(60000000/(MAX_SPEED*STEPS_PER_MM)))    // Max (or rather min) pace in microseconds/step


#define X_HOME_POSITION                     NEGATIVE
#define Y_HOME_POSITION                     NEGATIVE
#define Z_HOME_POSITION                     NEGATIVE

#define X_MAX_MM                            180
#define Y_MAX_MM                            90
#define Z_MAX_MM                            999 // dual limit switches used
#define X_MIN_MM                            -1
#define Y_MIN_MM                            -1
#define Z_MIN_MM                            -1

#define X_MAX_STEPS                         (X_MAX_MM * X_STEPS_PER_MM)
#define Y_MAX_STEPS                         (Y_MAX_MM * Y_STEPS_PER_MM)
#define Z_MAX_STEPS                         (Z_MAX_MM * Z_STEPS_PER_MM)
#define X_MIN_STEPS                         (X_MIN_MM * X_STEPS_PER_MM)
#define Y_MIN_STEPS                         (Y_MIN_MM * Y_STEPS_PER_MM)
#define Z_MIN_STEPS                         (Z_MIN_MM * Z_STEPS_PER_MM)

#define LIMIT_NORMALLY_OPEN                 false




#define ENABLE_PIN                          ARDUINO_D8            // Active Low
//#define COOLANT_EN_PIN                      ARDUINO_A3
//#define RESET_PIN                           ARDUINO_A2          // Active Low
//#define FEED_HOLD_PIN                       ARDUINO_A1          // Active Low?
//#define ABORT_PIN                           ARDUINO_A0
#define A_DIR_PIN                           ARDUINO_D13
#define X_DIR_PIN                           ARDUINO_D5
#define Y_DIR_PIN                           ARDUINO_D6
#define Z_DIR_PIN                           ARDUINO_D7
#define A_STEP_PIN                          ARDUINO_D12
#define X_STEP_PIN                          ARDUINO_D2
#define Y_STEP_PIN                          ARDUINO_D3
#define Z_STEP_PIN                          ARDUINO_D4

#define X_LIMIT_NEG_PIN                     ARDUINO_D9
#define Y_LIMIT_NEG_PIN                     ARDUINO_D10
#define Z_LIMIT_NEG_PIN                     ARDUINO_D11
//#define X_LIMIT_POS_PIN
//#define Y_LIMIT_POS_PIN
#define Z_LIMIT_POS_PIN                     ARDUINO_A3          // CoolEn on CNC Shield v3


#define DUAL_DRIVE_X                        false
#define X2_DIR_PIN
#define X2_STEP_PIN
#define X2_LIMIT_POS_PIN
#define X2_LIMIT_NEG_PIN

#define DUAL_DRIVE_Y                        true
#define Y2_DIR_PIN                          A_DIR_PIN
#define Y2_STEP_PIN                         A_STEP_PIN
//#define Y2_LIMIT_POS_PIN
#define Y2_LIMIT_NEG_PIN                    ARDUINO_A1          // Hold on CNC Shield v3

#define DUAL_DRIVE_Z                        false
#define Z2_DIR_PIN
#define Z2_STEP_PIN
#define Z2_LIMIT_POS_PIN
#define Z2_LIMIT_NEG_PIN




#endif