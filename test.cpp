#include "stdio.h"
#include "iostream"
#include "serial_port.h"
int main(void){
  SerialPort serial_port;
  int status = serial_port.Open("/dev/cu.usbserial-AQ01C796", 9600, 8, SerialPort::PARITY_NONE, 1);
  if(status != SerialPort::OK){
    std::cout << "Open port failed, status = " << status <<"\r\n";
    return -1;
  }
  char str[] = "This is a test\r\n";
  serial_port.Write(str, strlen(str));
  char buf[128];
  serial_port.Read(buf, strlen(str), 100);
  std::cout << buf << std::endl;
  return  0;
}