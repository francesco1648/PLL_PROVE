
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "AbsoluteEncoder.h"
#include "Battery.h"
#include "DynamixelSerial.h"
#include "TractionEncoder.h"
#include "MovingAvgFilter.h"
#include "ExpSmoothingFilter.h"
#include "Debug.h"
#include "mcp2515.h"
#include "Display.h"
#include "SmartMotor.h"
#include "Motor.h"
#include "PID.h"
#include "can.h"

#include "include/definitions.h"
#include "include/mod_config.h"
#include "include/communication.h"

// #include "WebManagement.h"

int time_bat = 0;          // time of last battery check
int time_tel = 0;          // time of last telemetry send
int time_data = 0;         // time of last data received
int time_tel_avg = DT_TEL; // average time between telemetry sends

struct can_frame canMsg;
MCP2515 mcp2515(5, 10000000UL, &SPI); // passing all parameters avoids premature initialization of SPI, which should be done in setup()

SmartMotor motorTrLeft(DRV_TR_LEFT_PWM, DRV_TR_LEFT_DIR, ENC_TR_LEFT_A, ENC_TR_LEFT_B, false);
SmartMotor motorTrRight(DRV_TR_RIGHT_PWM, DRV_TR_RIGHT_DIR, ENC_TR_RIGHT_A, ENC_TR_RIGHT_B, true);

#ifdef MODC_YAW
AbsoluteEncoder encoderYaw(ABSOLUTE_ENCODER_ADDRESS);
#endif

#ifdef MODC_EE
DynamixelMotor motorEEPitch(SERVO_EE_PITCH_ID);
DynamixelMotor motorEEHeadPitch(SERVO_EE_HEAD_PITCH_ID);
DynamixelMotor motorEEHeadRoll(SERVO_EE_HEAD_ROLL_ID);
#endif

#ifdef MODC_PITCH
DynamixelMotor motorPitchA(SERVO_A_ID);
DynamixelMotor motorPitchB(SERVO_B_ID);
#endif

float oldAngle;

// WebManagement wm(CONF_PATH);

Display display;

void okInterrupt()
{
  display.okInterrupt();
}

void navInterrupt()
{
  display.navInterrupt();
}

void sendFeedback()
{
  canMsg.can_id = CAN_ID | CAN_EFF_FLAG; // source

  // send motor feedback as float
  float speedR = motorTrRight.getSpeed();
  float speedL = motorTrLeft.getSpeed();

  canMsg.can_dlc = 8;
  canMsg.can_id |= MOTOR_FEEDBACK << 16;
  canMsg.data[0] = ((uint8_t *)&speedL)[0];
  canMsg.data[1] = ((uint8_t *)&speedL)[1];
  canMsg.data[2] = ((uint8_t *)&speedL)[2];
  canMsg.data[3] = ((uint8_t *)&speedL)[3];
  canMsg.data[4] = ((uint8_t *)&speedR)[0];
  canMsg.data[5] = ((uint8_t *)&speedR)[1];
  canMsg.data[6] = ((uint8_t *)&speedR)[2];
  canMsg.data[7] = ((uint8_t *)&speedR)[3];
  mcp2515.sendMessage(&canMsg);

  // send yaw angle of the joint if this module has one
#ifdef MODC_YAW
  encoderYaw.update();
  float angle = encoderYaw.readAngle();

  canMsg.can_dlc = 4;
  canMsg.can_id |= JOINT_YAW_FEEDBACK << 16;
  canMsg.data[0] = ((uint8_t *)&angle)[0];
  canMsg.data[1] = ((uint8_t *)&angle)[1];
  canMsg.data[2] = ((uint8_t *)&angle)[2];
  canMsg.data[3] = ((uint8_t *)&angle)[3];
  mcp2515.sendMessage(&canMsg);
#endif

  // send end effector data (if module has it)
#ifdef MODC_EE
  int pitch = motorEEPitch.readPosition();
  canMsg.can_dlc = 4;
  canMsg.can_id |= DATA_EE_PITCH_FEEDBACK << 16;
  memcpy(canMsg.data, &pitch, 4);
  mcp2515.sendMessage(&canMsg);

  int headPitch = motorEEHeadPitch.readPosition();
  canMsg.can_dlc = 4;
  canMsg.can_id |= DATA_EE_HEAD_PITCH_FEEDBACK << 16;
  memcpy(canMsg.data, &headPitch, 4);
  mcp2515.sendMessage(&canMsg);

  int headRoll = motorEEHeadRoll.readPosition();
  canMsg.can_dlc = 4;
  canMsg.can_id |= DATA_EE_HEAD_ROLL_FEEDBACK << 16;
  memcpy(canMsg.data, &headRoll, 4);
  mcp2515.sendMessage(&canMsg);
#endif
}

void setup()
{
  Serial.begin(115200);
  Debug.setLevel(Levels::INFO); // comment to set debug verbosity to debug
  Wire1.setSDA(I2C_SENS_SDA);
  Wire1.setSCL(I2C_SENS_SCL);
  Wire1.begin();

  SPI.setRX(4);
  SPI.setCS(5);
  SPI.setSCK(6);
  SPI.setTX(7);
  SPI.begin();

  // LittleFS.begin();

  // String hostname = WIFI_HOSTBASE+String(CAN_ID);
  // wm.begin(WIFI_SSID, WIFI_PWD, hostname.c_str());

  void initCAN(); // CAN initialization
  // initializing PWM
  analogWriteFreq(PWM_FREQUENCY);  // switching frequency to 15kHz
  analogWriteRange(PWM_MAX_VALUE); // analogWrite range from 0 to 512, default is 255

  // initializing ADC
  analogReadResolution(12); // set precision to 12 bits, 0-4095 input

  // motor initialization
  motorTrLeft.begin();
  motorTrRight.begin();
  if (motorTrLeft.calibrate() != 0)
  {
    Debug.println("Motor calibration failed: could not reach target speed", Levels::ERROR);
  }
  if (motorTrRight.calibrate() != 0)
  {
    Debug.println("Motor calibration failed: could not reach target speed", Levels::ERROR);
  }

#if defined MODC_PITCH || defined MODC_EE
  Serial1.setRX(1);
  Serial1.setTX(0);
  Dynamixel.setSerial(&Serial1);
  Dynamixel.begin(19200);
#endif

  Debug.println("BEGIN", Levels::INFO);

#ifdef MODC_YAW
  encoderYaw.update();
  oldAngle = encoderYaw.readAngle();
  encoderYaw.setZero();
#endif

  // Display initialization
  display.begin();

  // Buttons initialization
  pinMode(BTNOK, INPUT_PULLUP);
  pinMode(BTNNAV, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTNOK), okInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTNNAV), navInterrupt, FALLING);
};

void loop()
{
  int time_cur = millis(); // time of  loop start

  // update motors
  motorTrLeft.update();
  motorTrRight.update();

  // check battery status
  if (time_cur - time_bat >= DT_BAT)
  {
    time_bat = time_cur;

    if (time_tel_avg > DT_TEL)
      Debug.println("Telemetry frequency below required: " + String(1000 / time_tel_avg) + " Hz", Levels::WARN);

    if (!battery.charged())
      Debug.println("Battery voltage low! " + String(battery.readVoltage()) + "v", Levels::WARN);
  }

  // send telemetry
  if (time_cur - time_tel >= DT_TEL)
  {
    time_tel_avg = (time_tel_avg + (time_cur - time_tel)) / 2;
    time_tel = time_cur;

    sendFeedback();
  }

  // receive CAN data
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    time_data = time_cur;

    Debug.println("RECEIVED CANBUS DATA");
    int16_t data;
    byte buf[4];

    byte type = canMsg.can_id >> 16;

    switch (type)
    {
    case MOTOR_SETPOINT:
      // take the first four bytes from the array and and put them in a float
      float leftSpeed, rightSpeed;
      memcpy(&leftSpeed, canMsg.data, 4);
      memcpy(&rightSpeed, canMsg.data + 4, 4);
      motorTrLeft.setSpeed(leftSpeed);
      motorTrRight.setSpeed(rightSpeed);

      Debug.print("TRACTION DATA :\tleft: \t");
      Debug.print(leftSpeed);
      Debug.print("\tright: \t");
      Debug.println(rightSpeed);
      break;
    case DATA_PITCH:
      data = canMsg.data[1] | canMsg.data[2] << 8;
#ifdef MODC_PITCH
      data = map(data, 0, 1023, SERVO_MIN, SERVO_MAX);
      motorPitchA.moveSpeed(data, SERVO_SPEED);
      motorPitchB.moveSpeed(motorPitchB.readPosition() + motorPitchA.readPosition() - data, SERVO_SPEED);
#endif
      Debug.print("PITCH MOTOR DATA : \t");
      Debug.println(data);
      break;

    case DATA_EE_PITCH_SETPOINT:
      memcpy(&data, canMsg.data, 2);
#ifdef MODC_EE
      motorEEPitch.moveSpeed(data, SERVO_SPEED);
#endif
      Debug.print("PITCH END EFFECTOR MOTOR DATA : \t");
      Debug.println(data);
      break;

    case DATA_EE_HEAD_PITCH_SETPOINT:
      memcpy(&data, canMsg.data, 2);
#ifdef MODC_EE
      motorEEHeadPitch.moveSpeed(data, SERVO_SPEED);
#endif
      Debug.print("HEAD PITCH END EFFECTOR MOTOR DATA : \t");
      Debug.println(data);
      break;

    case DATA_EE_HEAD_ROLL_SETPOINT:
      memcpy(&data, canMsg.data, 2);
#ifdef MODC_EE
      motorEEHeadRoll.moveSpeed(data, SERVO_SPEED);
#endif
      Debug.print("HEAD ROLL END EFFECTOR MOTOR DATA : \t");
      Debug.println(data);
      break;
    }
  }
  else if (time_cur - time_data > 1000 && time_data != -1)
  { // if we do not receive data for more than a second stop motors
    time_data = -1;
    Debug.println("Stopping motors after timeout.", Levels::INFO);
    motorTrLeft.stop();
    motorTrRight.stop();
  }

  // wm.handle();
  // display.handleGUI();

  display.showLogo();
  Debug.println("showLogo \n", Levels::INFO);
  delay(10000);
  display.showBattery();
  delay(10000);
};
