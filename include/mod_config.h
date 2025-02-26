#ifndef module_configuration_h
#define module_configuration_h

// MOD_HEAD, MOD_MIDDLE, MOD_TAIL are defined in the makefile
#define MOD_HEAD

#if defined(MOD_HEAD)
#define CAN_ID    0x11  // HEAD
#define MODC_EE
#define SERVO_EE_PITCH_ID 6
#define SERVO_EE_HEAD_ROLL_ID 4
#define SERVO_EE_HEAD_PITCH_ID 2
#define SERVO_SPEED 200

#elif defined(MOD_MIDDLE)
#define CAN_ID    0x12  // MIDDLE
#define MODC_YAW

#elif defined(MOD_TAIL)
#define CAN_ID    0x13  // TAIL
#define MODC_YAW
#define MODC_PITCH
#define SERVO_A_ID 5
#define SERVO_B_ID 1
#define SERVO_MIN 200
#define SERVO_MAX 800
#define SERVO_SPEED 200
#endif

#endif
