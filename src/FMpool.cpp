#include "FMpool.h"



int main(int argc,const char* argv[]){
    const char* serverIp = argv[1];
    int serverPort = atoi(argv[2]);
    printf("-----------This is server------------\n");

    FMpool fmPool;

    fmPool.run(serverIp,serverPort);

    printf("fmPool id==%d\n",fmPool.getIdTag());

     printf("fmPool size==%d\n",fmPool.getObjsNum());

    printf("fmPool will be closed\n");

    fmPool.clean();

    printf("fmPool has been cleaned\n");


}