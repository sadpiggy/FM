#include "common.h"
unsigned int sendObj(void* value,unsigned long valueSize,int farMem){//farMem is a sock //发送
    //发送信号，告知内存池分配相应大小的内存
    unsigned long ctlMsg;
    ctlMsg = valueSize*10+CtlSend;

    int ack = 0;
    send(farMem,&ctlMsg,CtlMsgSize,0);
    //需要recv，之后可以换成sendTo，可能会更好
    //recv(farMem,&ack,sizeof(int),0);
    unsigned int objId = 0;
    recv(farMem,&objId,sizeof(objId),0);
// #ifdef Debug
//     if(ack!=ACK){
//         printf("sendObj has fault,ack!=ACK\n");
//         return;
//     }
// #endif

    send(farMem,value,valueSize,0);
    // unsigned int objId = 0;
    // recv(farMem,&objId,sizeof(int),0);
    return objId;
    //send(clnt_sock,str,msgSize,0);
}


void getObj(void* value,unsigned long valueSize,unsigned int objId,int farMem){//
//发送信号，告知内存池发送回obj
    unsigned long ctlMsg;
    ctlMsg = objId*10+CtlGet;
    send(farMem,&ctlMsg,CtlMsgSize,0);
    // send(farMem,&objId,sizeof(int),0);
    recv(farMem,&value,valueSize,0);

}

int connectFM(const char* clientIp,int clientPort,const char* serverIp,int serverPort){
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));  //每个字节都用0填充
    client_addr.sin_family = AF_INET;  //使用IPv4地址
    client_addr.sin_addr.s_addr = inet_addr(clientIp);  //具体的IP地址
    client_addr.sin_port = htons(clientPort);  //端口
    bind(sock, (struct sockaddr*)&client_addr, sizeof(client_addr));

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(serverIp);  //具体的IP地址
    serv_addr.sin_port = htons(serverPort);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    return sock;
}

int closeFM(int farMem){
    unsigned long ctlMsg = CtlClose;
    send(farMem,&ctlMsg,CtlMsgSize,0);
    close(farMem);
}