//
// Created by kaylor on 2022/4/6.
//

#ifndef SERIAL_PORT__SERIAL_PORT_STREAM_H_
#define SERIAL_PORT__SERIAL_PORT_STREAM_H_

class SerialPortStream {
 private:
  //缓冲区
  char *buffer_;
  //缓冲区容量
  int capacity_;
  //流的开头
  int start_;
  //流的长度
  int length_;

 public:
  //初始化一个流，参数为：初始容量
  SerialPortStream(int initCapacity = 16);
  //清理
  ~SerialPortStream();
  //获取流的长度
  int GetLength();
  //向流的末尾增加一字符
  void Append(char aChar);
  //向流的末尾增加多个字符
  void Append(const char *buf, int len);
  //查看流的第一个字符，如果长度为0则返回0
  char Peek();
  //查看流开头的多个字符，返回实际查看到的长度
  int Peek(char *buf, int len);
  //取出流的第一个字符，如果长度为0则返回0
  char Take();
  //取出流开头的多个字符，返回实际取出的长度
  int Take(char *buf, int len);

 private:
  //扩大容量至两倍
  void Expand();
};

#endif //SERIAL_PORT__SERIAL_PORT_STREAM_H_
