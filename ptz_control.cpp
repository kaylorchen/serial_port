//
// Created by kaylor on 2022/4/7.
//

#include "ptz_control.h"
uint8_t PtzControl::CalcCheckSum(uint8_t address) {
  return address + command1_.value + command2_.value + pan_speed_ + tilt_speed_;
}

void PtzControl::SendCommand(uint8_t address) {
  uint8_t buf[7];
  buf[0] = start_flag_;
  buf[1] = address;
  buf[2] = command1_.value;
  buf[3] = command2_.value;
  buf[4] = pan_speed_;
  buf[5] = tilt_speed_;
  buf[6] = CalcCheckSum(address);
  Write((char *) buf, 7);
}

void PtzControl::SetPanSpeed(uint8_t speed) {
  if (speed > 0x3F) {
    perror("Error: speed should be lower than 0x3F\n");
    return;
  }
  pan_speed_ = speed;
}

void PtzControl::SetTiltSpeed(uint8_t speed) {
  if (speed > 0x3F) {
    perror("Error: speed should be lower than 0x3F\n");
    return;
  }
  tilt_speed_ = speed;
}

void PtzControl::Stop(uint8_t address) {
  command1_.value = 0;
  command2_.value = 0;
  pan_speed_ = 0;
  tilt_speed_ = 0;
  SendCommand(address);
}

void PtzControl::Action(uint8_t address, enum Motion motion, enum Focus focus, enum Iris iris) {
  command1_.value = 0;
  command2_.value = 0;
  switch (motion) {
    case kUP: command2_.data.index3 = 1;
      break;
    case kDown: command2_.data.index4 = 1;
      break;
    case kLeft: command2_.data.index2 = 1;
      break;
    case kRight:command2_.data.index1 = 1;
      break;
    case kLeftDown: command2_.data.index2 = 1;
      command2_.data.index4 = 1;
      break;
    case kLeftUp:command2_.data.index2 = 1;
      command2_.data.index3 = 1;
      break;
    case kRightDown: command2_.data.index1 = 1;
      command2_.data.index4 = 1;
      break;
    case kRightUp:command2_.data.index1 = 1;
      command2_.data.index3 = 1;
      break;
  }
  switch (focus) {
    case kFocusNear:command1_.data.index0 = 1;
      break;
    case kFocusFar:command2_.data.index7 = 1;
      break;
  }
  switch (iris) {
    case kIrisClose:command1_.data.index2 = 1;
      break;
    case kIrisOpen:command1_.data.index1 = 1;
      break;
  }
  SendCommand(address);
}