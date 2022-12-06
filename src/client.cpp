#include "FMptr.h"
#include <time.h>

#define msgSize 1024
#define sendNum 1024*100
int sock;

double testIntArray(){
     FMptr<int> ptr0(new int[msgSize],sock);
    for(int i=0;i<sendNum;i++){
        FMptr<int> ptr(new int[msgSize],sock);
        ptr.offload();
        ptr.inload();
    }
    #ifdef Debug2
    printf("deleteNum==%d\n",ptr0.getDeleteNum());
    #endif
}

int main(int argc,const char* argv[]){
    const char* clientIp = argv[1];
    int clientPort = atoi(argv[2]);
    const char* serverIp = argv[3];
    int serverPort = atoi(argv[4]);

    sock = connectFM(clientIp,clientPort,serverIp,serverPort);

    //clock_t start_t,finish_t;
    time_t start_t,finish_t;;
 
    start_t = time(NULL);
    //start_t = clock();
    testIntArray();
    finish_t = time(NULL);
    // double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    long int total_t = finish_t - start_t;
    printf("time==%ld\n",total_t);

    // FMptr<int> ptr(new int,sock);
    // *ptr = 10;
    // printf("%d\n",*ptr);
    // ptr.offload();
    // printf("%d\n",*ptr);
    // ptr.inload();
    // printf("%d\n",*ptr);

    closeFM(sock);
}