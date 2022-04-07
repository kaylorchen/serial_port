//
// Created by kaylor on 2022/4/6.
//

#ifndef SERIAL_PORT__TIME_STAMP_H_
#define SERIAL_PORT__TIME_STAMP_H_
typedef long long timestamp_t;
class TimeStamp {
 public:
  static timestamp_t now();
};

#endif //SERIAL_PORT__TIME_STAMP_H_
