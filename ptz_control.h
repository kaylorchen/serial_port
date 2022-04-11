//
// Created by kaylor on 2022/4/7.
//

#ifndef SERIAL_PORT__PTZ_CONTROL_H_
#define SERIAL_PORT__PTZ_CONTROL_H_
#include "serial_port.h"
typedef union {
  uint8_t value;
  struct {
    uint8_t index0: 1;
    uint8_t index1: 1;
    uint8_t index2: 1;
    uint8_t index3: 1;
    uint8_t index4: 1;
    uint8_t index5: 1;
    uint8_t index6: 1;
    uint8_t index7: 1;
  } data;
} Command;
class PtzControl : public SerialPort {
 public:
  enum Motion {
    kMotionNothing = 0,
    kUP,
    kDown,
    kLeft,
    kRight,
    kLeftUp,
    kRightUp,
    kLeftDown,
    kRightDown,
  };
  enum Mode {
    kNothing1 = 0,
    kCameraOff,
    kManualScanOn,
    kManualScanOnAndCameraOff,
    kNothing2,
    kCameraOn,
    kAutoScanOn,
    kAutoScanOnAndCameraOn,
  };
  enum Focus {
    kFocusNothing = 0,
    kFocusNear,
    kFocusFar,
  };
  enum Iris {
    kIrisNothing = 0,
    kIrisOpen,
    kIrisClose,
  };
 private:
  const uint8_t start_flag_ = 0xFF;
  Command command1_;
  Command command2_;
  uint8_t pan_speed_;
  uint8_t tilt_speed_;
 public:
  void SetPanSpeed(uint8_t speed = 0x00);
  void SetTiltSpeed(uint8_t speed = 0x00);
  void Stop(uint8_t address = 0x01);
  void Action(uint8_t address, enum Motion motion, enum Focus focus, enum Iris iris);
 private:
  void SendCommand(uint8_t address = 0x01);
  uint8_t CalcCheckSum(uint8_t address);

};

#endif //SERIAL_PORT__PTZ_CONTROL_H_
