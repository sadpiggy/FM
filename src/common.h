//
// Created by 王崑运 on 2022/12/5.
//

#ifndef FM_COMMON_H
#define FM_COMMON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define Debug
#define Debug2
// #define ACK  1
// #define NAK  0
#define CtlSend 0
#define CtlGet 1
#define CtlClose 2
#define CtlMsgSize 8
#define ObjIdSize 4
//control信息，六个byte,第一个控制，后四个为大小，最后一个\0
//为了方便，换一种方式，传八个byte，也就是一个unsigned long int，十进制下，最后一位为control，

unsigned int sendObj(void* value,unsigned long valueSize,int farMem);//发送obj，得到在远端的索引//void* value,int valueSize,int farMem

void getObj(void* value,unsigned long valueSize,unsigned int objId,int farMem);//get之前，需要分配好内存的大小

int connectFM(const char* clientIp,int clientPort,const char* serverIp,int serverPort);

int closeFM(int farMem);

#endif //FM_COMMON_H
