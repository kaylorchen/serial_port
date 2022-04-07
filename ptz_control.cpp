//
// Created by kaylor on 2022/4/7.
//

#include "ptz_control.h"
const unsigned char  PtzControl::common_feedback_[] = {0x90, 0x41, 0xFF, 0x90, 0x51, 0xFF};
const unsigned char  PtzControl::zoom_in_[] = {0x81, 0x01, 0x04, 0x07, 0x02, 0xFF};
const unsigned char  PtzControl::zoom_out_[] = {0x81, 0x01, 0x04, 0x07, 0x03, 0xFF};
const unsigned char  PtzControl::zoom_stop_[] = {0x81, 0x01, 0x04, 0x07, 0x00, 0xFF};
const unsigned char  PtzControl::zoom_in_speed_[] = {0x81, 0x01, 0x04, 0x07, 0x20, 0xFF};
const unsigned char  PtzControl::zoom_out_speed_[] = {0x81, 0x01, 0x04, 0x07, 0x30, 0xFF};
const unsigned char  PtzControl::power_on_[] = {0x81, 0x01, 0x04, 0x00, 0x02, 0xFF};
const unsigned char  PtzControl::power_down_[] = {0x81, 0x01, 0x04, 0x00, 0x03, 0xFF};
const unsigned char  PtzControl::inquire_zoom_[] = {0x81, 0x09, 0x04, 0x47, 0xFF};
const unsigned char  PtzControl::zoom_in_focus_[] = {0x81, 0x01, 0x04, 0x08, 0x02, 0xFF};
const unsigned char  PtzControl::zoom_out_focus_[]{0x81, 0x01, 0x04, 0x08, 0x03, 0xFF};

bool compare(const unsigned char *str1, const unsigned char *str2, const int size) {
  for (int i = 0; i < size; ++i) {
    if (*(str1 + i) != *(str2 + i)) {
      return false;
    }
  }
  return true;
}

bool PtzControl::Feedback() {
  char buf[6];
  int len;
  do {
    len = 0;
    len = Read(buf, 1, 10);
  } while (len == 0 || buf[0] != 0x90);
  len = Read(&buf[1], sizeof(buf) - 1, 100);
  if (len == sizeof(buf) - 1) {
    if (compare((unsigned char *) buf, common_feedback_, sizeof(common_feedback_))) {
      printf("Command has been received\n");
      return true;
    }
  }
  return false;
}

bool PtzControl::PowerOn() {
  char *data = (char *) power_on_;
  Write(data, sizeof(power_on_));
  return Feedback();
}

bool PtzControl::PowerDown() {
  char *data = (char *) power_down_;
  Write(data, sizeof(power_down_));
  return Feedback();
}

bool PtzControl::ZoomInFocus() {
  char *data = (char *) zoom_in_focus_;
  Write(data, sizeof(zoom_in_focus_));
  return Feedback();
}

bool PtzControl::ZoomOutFocus() {
  char *data = (char *) zoom_out_focus_;
  Write(data, sizeof(zoom_out_focus_));
  return Feedback();
}