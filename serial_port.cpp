//
// Created by kaylor on 2022/4/6.
//

#include "serial_port.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include "thread"
#include <sys/ioctl.h>

SerialPort::SerialPort() : stream_(), pthread_(NULL), RTS_flag_(TIOCM_RTS), DTR_flag_(TIOCM_DTR), running_flag_(true){
}

SerialPort::~SerialPort() {
  running_flag_ = false;
  if (pthread_ != NULL) {
    pthread_->join();
    delete pthread_;
  }
  Close();
}

void *ReceiveThread(void *arg) {
  SerialPort *serial_port = (SerialPort *) (arg);
  char buf[1024];
  while (true) {
    int len = read(serial_port->fd_, buf, sizeof(buf));
    if (len > 0) {
      serial_port->mutex_.lock();
      serial_port->stream_.Append(buf, len);
      serial_port->mutex_.unlock();
    }
    usleep(100);
    if (serial_port->running_flag_ == false){
      break;
    }
  }
  printf("exit receive thread\n");
  return NULL;
}

int SerialPort::Open(const char *dev, int baud, int dataBits, int parityMode, int stopBits) {
  struct termios options;
  bzero(&options, sizeof(options));
  int baudT = TransformBaud(baud);
  if (baudT < 0)
    return BAUD_NOT_SUPPORTED;
  //  Configure the input baud rate
  cfsetispeed(&options, baudT);
  //  Configure the output baud rate
  cfsetospeed(&options, baudT);
  int dataBitsT = TransformDataBits(dataBits);
  if (dataBitsT < 0)
    return DATABITS_NOT_SUPPORTED;
  //  Set the data bits = dataBitsT
  options.c_cflag |= dataBitsT;
  //  Disable/Enable the Parity Enable bit(PARENB)
  if (parityMode == PARITY_ODD) {
    options.c_cflag |= PARENB;
    options.c_cflag |= PARODD;
  } else if (parityMode == PARITY_EVEN) {
    options.c_cflag |= PARENB;
  } else if (parityMode != PARITY_NONE) {
    return PARITYMODE_NOT_SUPPORTED;
  }
  if (stopBits == 2) {
    options.c_cflag |= CSTOPB;
  } else if (stopBits != 1) {
    return STOPBITS_NOT_SUPPORTED;
  }
  // Read at least 1 character
  options.c_cc[VMIN] = 1;
  // Set timeout = 1*100 ms from the 1st character is received
  options.c_cc[VTIME] = 1;

//  fd_ = ::open(dev, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  fd_ = ::open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd_ < 0)
    return DEV_NOT_FOUND;
  if (tcsetattr(fd_, TCSANOW, &options))
    return CONFIG_FAIL;
  /*------------------------- Putting MAX485 chip in USB2SERIAL in Receive Mode ----------------------------*/
  //                                                                                                        //
  //	----+			+-----------+              L  +-----------+                                           //
  //		|			| 	    ~RTS| --------------> |~RE        |                                           //
  //	 PC |<==========| FT232     |                 |   MAX485  +(A,B)<~~~~~~~~~~~~~~~Data in(RS485)        //
  //	    |    USB    |       ~DTR| --------------> | DE        |        Twisted Pair                       //
  //  ----+			+-----------+              L  +-----------+                                           //
  //                                                                                                        //
  //--------------------------------------------------------------------------------------------------------//
  // Set RX mode
  SetRxMode();
  if (tcflush(fd_, TCIOFLUSH))
    return CONFIG_FAIL;
  pthread_ = new std::thread(ReceiveThread, this);
  return OK;
}

int SerialPort::Close() {
  if (fd_ >= 0) {
    int status = close(fd_);
    fd_ = -1;
    return status;
  }
  return 0;
}

int SerialPort::Write(const char *data, int len) {
  SetTxMode();
  //because RS485 hardware issue, receiver's the first bytes is wrong 
  int ret;
  ret = ::write(fd_, data, len);
  SetRxMode();
  return ret;
}

int SerialPort::Available() {
  int len = stream_.GetLength();
  return len;
}

int SerialPort::Peek(char *buf, int len) {
  len = stream_.Peek(buf, len);
  return len;
}

int SerialPort::Read(char *buf, int len, int timeout) {
  timestamp_t start = TimeStamp::now();
  int total = 0;
  while (total < len) {
    mutex_.lock();
    int readLen = stream_.Take(buf + total, len - total);
    mutex_.unlock();
    if (readLen > 0)
      total += readLen;
    timestamp_t now = TimeStamp::now();
    if (now >= start + timeout)
      break;
//    usleep(1000);
  }
  return total;
}

int SerialPort::Read(char *buf, int maxLen, const char *end, int timeout, int *recvLen) {
  int endLen = strlen(end);
  timestamp_t start = TimeStamp::now();
  int total = 0;
  while (total < maxLen) {
    mutex_.lock();
    int readLen = stream_.Take(buf + total, 1);
    mutex_.unlock();
    if (readLen > 0) {
      total += readLen;
      if (EndsWith(buf, total, end, endLen)) {
        if (recvLen != 0)
          *recvLen = total;
        return READ_END;
      }
    }
    timestamp_t now = TimeStamp::now();
    if (now >= start + timeout)
      return READ_TIMEOUT;
//    usleep(1000);
  }
  return READ_BUFFER_FULL;
}

int SerialPort::TransformBaud(int baud) {
  int map[][2] = {{2400, B2400}, {4800, B4800}, {9600, B9600},
                  {19200, B19200}, {38400, B38400}, {57600, B57600},
                  {115200, B115200}, {230400, B230400}, {460800, B460800},
                  {500000, B500000}, {576000, B576000}, {921600, B921600},
                  {1000000, B1000000}, {1152000, B1152000}, {1500000, B1500000},
                  {2000000, B2000000}, {2500000, B2500000}, {3000000, B3000000},
                  {3500000, B3500000}, {4000000, B4000000}};
  for (int i = 0; i < sizeof(map) / sizeof(int) / 2; i++)
    if (map[i][0] == baud)
      return map[i][1];
  return -1;
}

int SerialPort::TransformDataBits(int dataBits) {
  int map[][2] = {{5, CS5}, {6, CS6}, {7, CS7}, {8, CS8}};
  for (int i = 0; i < sizeof(map) / sizeof(int) / 2; i++)
    if (map[i][0] == dataBits)
      return map[i][1];
  return -1;
}

bool SerialPort::EndsWith(const char *str, int strLen, const char *end, int endLen) {
  if (strLen < endLen)
    return false;
  for (int i = endLen - 1; i >= 0; i--)
    if (end[i] != str[strLen - endLen + i])
      return false;
  return true;
}

void SerialPort::SetRxMode(void){
  usleep(1000*10);
  ioctl(fd_, TIOCMBIS, &RTS_flag_);
  ioctl(fd_, TIOCMBIS, &DTR_flag_);
}
void SerialPort::SetTxMode(void){
  ioctl(fd_, TIOCMBIC, &RTS_flag_);
  ioctl(fd_, TIOCMBIC, &DTR_flag_);
  usleep(1000*10);
}