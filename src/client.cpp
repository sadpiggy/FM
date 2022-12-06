#include "FMptr.h"


int main(int argc,const char* argv[]){
    const char* clientIp = argv[1];
    int clientPort = atoi(argv[2]);
    const char* serverIp = argv[3];
    int serverPort = atoi(argv[4]);

    int sock = connectFM(clientIp,clientPort,serverIp,serverPort);
    FMptr<int> ptr(new int,sock);
    *ptr = 10;
    printf("%d\n",*ptr);
}