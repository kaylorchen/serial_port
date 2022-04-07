//
// Created by kaylor on 2022/4/6.
//

#ifndef SERIAL_PORT__SERIAL_PORT_H_
#define SERIAL_PORT__SERIAL_PORT_H_

#include <mutex>
#include <thread>
#include "serial_port_stream.h"
#include "time_stamp.h"
class SerialPort {
 public:
  //无校验
  static const int PARITY_NONE = 0;
  //奇校验
  static const int PARITY_ODD = 1;
  //偶校验
  static const int PARITY_EVEN = 2;
  //函数成功
  static const int OK = 1;
  //设备未找到
  static const int DEV_NOT_FOUND = -1;
  //不支持该波特率
  static const int BAUD_NOT_SUPPORTED = -2;
  //不支持该数据位数
  static const int DATABITS_NOT_SUPPORTED = -3;
  //不支持该校验模式
  static const int PARITYMODE_NOT_SUPPORTED = -4;
  //不支持该停止位数
  static const int STOPBITS_NOT_SUPPORTED = -5;
  //未知配置错误
  static const int CONFIG_FAIL = -6;
  //创建线程出错
  static const int NEW_THREAD_FAIL = -7;
  //成功读到结尾符
  static const int READ_END = 1;
  //读取超时
  static const int READ_TIMEOUT = -1;
  //读取时缓冲区满
  static const int READ_BUFFER_FULL = -2;

 private:
  //串口设备文件描述符
  int fd_;
  //字符流
  SerialPortStream stream_;

  std::mutex mutex_;

  std::thread *pthread_;

  bool running_flag_;

  int RTS_flag_;

  int DTR_flag_;

 public:
  SerialPort();
  ~SerialPort();
  //开启串口，参数为：设备名、波特率、数据位数、校验模式、停止位数，返回函数执行结果
  int Open(const char *dev, int baud, int dataBits, int parityMode, int stopBits);
  //关闭串口
  int Close();
  //写串口，参数为：数据、长度，返回实际写入长度
  int Write(const char *data, int len);
  //获取可读长度
  int Available();
  //读串口，但不移除数据，返回实际读取长度
  int Peek(char *buf, int len);
  //读串口，直到收到预期长度的数据或超时，参数为：接收缓冲区、预期接收长度、超时（毫秒）,返回实际读取长度
  int Read(char *buf, int len, int timeout);
  //读串口，直到读到预期的结尾符或缓冲区满或超时，参数为：接收缓冲区、最大长度、预期结尾符、超时（毫秒）、实际接收长度，返回READ_END、READ_TIMEOUT或READ_BUFFER_FULL
  int Read(char *buf, int maxLen, const char *end, int timeout, int *recvLen);

 private:
  //将数字型波特率转化为系统调用参数
  int TransformBaud(int baud);
  //将数字型数据位数转化为系统调用参数
  int TransformDataBits(int dataBits);
  long long getTimestamp();
  //判断字符串str是否以字符串end结尾
  bool EndsWith(const char *str, int strLen, const char *end, int endLen);

  void SetRxMode(void);

  void SetTxMode(void);

  friend void* ReceiveThread(void* arg);
};

#endif //SERIAL_PORT__SERIAL_PORT_H_
