#include <iostream>
#include <cstring>
#include "serial_port.h"

int main() {
  std::cout << "time = " << TimeStamp::now() << std::endl;
  SerialPort serial_port;
  if (serial_port.Open("/dev/ttyUSB0", 115200, 8, SerialPort::PARITY_NONE, 1) != SerialPort::OK){
    std::cout << "Can not open serial port" << std::endl;
    return -1;
  }
  const char *str = "This is a test.\n";
  serial_port.Write(str, strlen(str));
  char buf[1024];
  int len = serial_port.Read(buf, sizeof (buf), 100);
  buf[len]=0;
  printf("recv(len=%d): %s\n",len,buf);
  //测试write和int read(char* buf,int maxLen,const char* end,int timeout,int* recvLen);
  const char *output="hello\r\nMy name is kaylor\r\nI like programming~\r\n";
  serial_port.Write(output,strlen(output));
  while(serial_port.Read(buf,sizeof(buf),"\r\n",100,&len)==SerialPort::READ_END)
  {
    buf[len]=0;
    printf("recv line(len=%d): %s\n",len,buf);
  }
  std::cout << "exit" << std::endl;
  return 0;
}
