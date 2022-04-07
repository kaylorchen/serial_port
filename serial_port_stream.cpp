//
// Created by kaylor on 2022/4/6.
//

#include "serial_port_stream.h"
SerialPortStream::SerialPortStream(int initCapacity)
{
  buffer_=new char[initCapacity];
  capacity_=initCapacity;
  start_=0;
  length_=0;
}

SerialPortStream::~SerialPortStream()
{
  delete[] buffer_;
}

int SerialPortStream::GetLength()
{
  return length_;
}

void SerialPortStream::Append(char aChar)
{
  if(length_>=capacity_)
    Expand();
  int pos=start_+length_;
  if(pos>=capacity_)
    pos-=capacity_;
  buffer_[pos]=aChar;
  length_++;
}

void SerialPortStream::Append(const char* buf,int len)
{
  for(int i=0;i<len;i++)
    Append(buf[i]);
}

char SerialPortStream::Peek()
{
  if(length_==0)
    return 0;
  return buffer_[start_];
}

int SerialPortStream::Peek(char* buf,int len)
{
  if(len>length_)
    len=length_;
  for(int i=0;i<len;i++)
  {
    int pos=start_+i;
    if(pos>=capacity_)
      pos-=capacity_;
    buf[i]=buffer_[pos];
  }
  return len;
}

char SerialPortStream::Take()
{
  if(length_==0)
    return 0;
  char aChar=buffer_[start_];
  start_++;
  length_--;
  if(start_>=capacity_)
    start_-=capacity_;
  return aChar;
}

int SerialPortStream::Take(char* buf,int len)
{
  if(len>length_)
    len=length_;
  for(int i=0;i<len;i++)
    buf[i]=Take();
  return len;
}

void SerialPortStream::Expand()
{
  int newCapacity=capacity_*2;
  char* newBuf=new char[newCapacity];
  int newLength=length_;
  Take(newBuf,newLength);
  delete[] buffer_;
  buffer_=newBuf;
  capacity_=newCapacity;
  start_=0;
  length_=newLength;
}