#include "lsyt201b.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "usart.h"

//定义数据数组
uint8_t Host_RXbuffur[Host_DataSize]={0};
uint8_t LSYT_RXbuffur[LSYT_DataSize]={0};

//定义解析标志位
int A_flag = 0;

int DataSize = LSYT_DataSize;

//串口的空闲接收中断
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size){
  __HAL_UNLOCK(huart);      //解锁串口状态
  
  if(huart == &huart1){           //如果是主机发送的数据
    printf("%s",Host_RXbuffur);   //数据回显
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,Host_RXbuffur,Host_DataSize-1); //再次开启空闲中断
    
  }else if(huart == & huart2){    //如果是模块发来的数据
    printf("LSYT_Data:");
    printf("%X %X %X %X %X %X\r\n",
      LSYT_RXbuffur[0],LSYT_RXbuffur[1],LSYT_RXbuffur[2],LSYT_RXbuffur[3],LSYT_RXbuffur[4],LSYT_RXbuffur[5]);
    //转发给电脑
    
    A_flag = 1;
    HAL_UARTEx_ReceiveToIdle_IT(&huart2,LSYT_RXbuffur,LSYT_DataSize-1); //再次开启空闲中断
    
  }
}

uint8_t LSYT_CMD[][6]={
  {0x0A,0x02,0x00,0x00,0x00,0xED},    //语音：已执行
  {0x0A,0x02,0x00,0x00,0x01,0xED},    //语音：执行失败
  {0x0A,0x02,0x00,0x00,0x02,0xED},    //语音：已完成
};

//向模块发送命令
int LSYT201B_SendCMD(uint8_t Data){
  uint8_t* tmp0 = malloc(sizeof(int)*6);
  
  switch(Data){
    case Executed:
      memcpy(tmp0,LSYT_CMD[0],6);
      break;
    case Failed:
      memcpy(tmp0,LSYT_CMD[1],6);
      break;
    case Completed:
      memcpy(tmp0,LSYT_CMD[2],6);
      break;
    default:
      return ERROR;
  }
  
  int tmp1 = HAL_UART_Transmit(&huart2,tmp0,6,10);
  free(tmp0);
  
  return tmp1;
}

//解析模块数据
int LSYT201B_Analysis(uint8_t* result){
  if(A_flag == 0){
    return ERROR;
  }
  A_flag = 0;
  
  if(LSYT_RXbuffur[0] != 0x0A){     //判断数据是否有效
    return ERROR;
  }
    
  switch(LSYT_RXbuffur[1]){       //依据数据格式处理数据
/*System:*/
    case 0x43:
      result[0] = SystemCMD;
      result[1] = Close;
      break;
    case 0x04:
      result[0] = SystemCMD;
      result[1] = Start;
      break;
    
/*CMD:*/
    case 0x0F:
      result[0] = NormalCMD;
      result[1] = LSYT_RXbuffur[4];
      break;
    case 0x0B:
      result[0] = ExtendCMD;
      result[1] = LSYT_RXbuffur[4];
      break;
    default:
      return ERROR;
  }
  
  return SUCCESS;
}

