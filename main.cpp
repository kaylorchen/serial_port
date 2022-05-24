#include <iostream>
#include <cstring>
#include <unistd.h>
#include "ptz_control.h"

int main() {
  std::cout << "time = " << TimeStamp::now() << std::endl;
  PtzControl ptz_control;
  if (ptz_control.Open("/dev/ttyUSB0", 9600, 8, SerialPort::PARITY_NONE, 1) != SerialPort::OK){
    std::cout << "Can not open serial port" << std::endl;
    return -1;
  }
  ptz_control.SetTiltSpeed(0x24);
  ptz_control.SetPanSpeed(0x24);
  
  ptz_control.Action(1, PtzControl::kRightUp, PtzControl::kFocusNothing, PtzControl::kIrisClose);
  usleep(1000000*5);
  ptz_control.Stop();
  std::cout << "exit" << std::endl;
  return 0;
}
