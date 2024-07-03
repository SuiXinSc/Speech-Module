#ifndef __LSYT201B_H_
#define __LSYT201B_H_

#include "main.h"

//宏定义
#define Host_DataSize     512
#define LSYT_DataSize     8

#define Start             0x0F
#define Close             0xF0

#define Executed          0xA0
#define Failed            0xA1
#define Completed         0xA2

#define SystemCMD         0x00
#define NormalCMD         0x01
#define ExtendCMD         0x02

//枚举定义命令
enum Normal_CMD{
  Crotate = 0x00,
  Urotate,
  Forward,
  FallBack,
  Lshift,
  Rshift,
  Rise,
  Down,
  Add,
  Reduce,
  Origin = 0x10,
  Reset
};

enum Extend_CMD{
  CMD0 = 0x00,
  CMD1,
  CMD2,
  CMD3,
  CMD4,
  CMD5,
};

extern uint8_t Host_RXbuffur[Host_DataSize];
extern uint8_t LSYT_RXbuffur[LSYT_DataSize];

int LSYT201B_SendCMD(uint8_t Data);
int LSYT201B_Analysis(uint8_t* result);

#endif
