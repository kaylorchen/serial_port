//
// Created by kaylor on 2022/4/7.
//

#ifndef SERIAL_PORT__PTZ_CONTROL_H_
#define SERIAL_PORT__PTZ_CONTROL_H_
#include "serial_port.h"
class PtzControl : SerialPort {
 private:
  static const unsigned char  common_feedback_[];
  static const unsigned char  zoom_in_[];
  static const unsigned char  zoom_out_[];
  static const unsigned char  zoom_stop_[];
  static const unsigned char  zoom_in_speed_[];
  static const unsigned char  zoom_out_speed_[];
  static const unsigned char  power_on_[];
  static const unsigned char  power_down_[];
  static const unsigned char  inquire_zoom_[];
  static const unsigned char  zoom_in_focus_[];
  static const unsigned char  zoom_out_focus_[];
 public:
  bool ZoomIn();
  bool ZoomOut();
  bool SetZoomInSpeed(int speed);
  bool SetZoomOutSpeed(int speed);
  bool StopZoom();
  bool PowerOn();
  bool PowerDown();
  int InquireZoom();
  bool ZoomInFocus();
  bool ZoomOutFocus();
  bool Feedback();
};

#endif //SERIAL_PORT__PTZ_CONTROL_H_
