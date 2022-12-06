#ifndef FMPOOL_H
#define FMPOOL_H
#include <map>
#include "common.h"
#include <unordered_map>
#define MapType std::unordered_map<unsigned int,std::pair<void*,unsigned long>>
#define PairType std::pair<void*,unsigned long>



class FMpool{// objId,key索引
    private:
    MapType objs;//<objId,<value,valueSize>>//放在堆里，还是放在栈里，这是一个问题//放堆里比较麻烦，先放栈里吧
    unsigned int idTag;//1 base
    int FMsock;
    int ClientSock;
    
   

    public:

    FMpool(){//目前单个后端
        this->idTag = 0;
        //创建serverSock 并连接
       
#ifdef Debug
        printf("FMPool has been created!");
#endif   
    }

    ~FMpool(){
        
        // if(this->objs.size()!=0){
        //     for(auto it:(this->objs)){//这样可以吗？
        //         free(it.second.first);
        //     }
        // }
        
    }

    void clean(){
        if(this->objs.size()!=0){
            for(auto it:(this->objs)){//这样可以吗？
                free(it.second.first);
            }
        }
    }

    // void createPool(const char* serverIp,int serverPort){//在这个函数里面创建的话，是不是出了这个函数，sock就被析构了？//很有可能
    //     int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	   
    //     struct sockaddr_in serv_addr;
    //     memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    //     serv_addr.sin_family = AF_INET;  //使用IPv4地址
    //     serv_addr.sin_addr.s_addr = inet_addr(serverIp);  //具体的IP地址
    //     serv_addr.sin_port = htons(serverPort);  //端口
    //     bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //     this->FMsock = serv_sock;

    //     listen(serv_sock, 20);

    //     //接收客户端请求
    //     struct sockaddr_in clnt_addr;
    //     socklen_t clnt_addr_size = sizeof(clnt_addr);
    //     int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    //     this->ClientSock = clnt_sock;
    // }

    void run(const char* serverIp,int serverPort){
        //创建sock
        this->FMsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	   
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
        serv_addr.sin_family = AF_INET;  //使用IPv4地址
        serv_addr.sin_addr.s_addr = inet_addr(serverIp);  //具体的IP地址
        serv_addr.sin_port = htons(serverPort);  //端口
        bind(this->FMsock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        listen(this->FMsock, 20);

        //接收客户端请求
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        this->ClientSock = accept(this->FMsock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        //开始运行


        unsigned long ctlMsg = 0;
        unsigned long ctl;
        unsigned long valueSize;
        unsigned int  objId;
        void* value;
        bool sockClose = false;
        PairType value_and_size;

        
        while(!sockClose){
            recv(ClientSock,&ctlMsg,CtlMsgSize,0);
            ctl = ctlMsg-ctlMsg/10*10;
            switch (ctl)
            {
            case CtlSend:
                valueSize = ctlMsg/10;
                value = malloc(valueSize);
                objId = ++(this->idTag);
                this->objs.insert(std::make_pair(objId,std::make_pair(value,valueSize)));
                //send objId
                send(ClientSock,&objId,ObjIdSize,0);
                //recv value
                recv(ClientSock,value,valueSize,0);

                break;
            case CtlGet:
                objId = ctlMsg/10;
                value_and_size = this->objs.at(objId);
                value = value_and_size.first;
                valueSize = value_and_size.second;
                //send value
                send(ClientSock,value,valueSize,0);
                free(value);
                this->objs.erase(objId);


                break;
            
            case CtlClose:
                close(this->ClientSock);
                close(this->FMsock);
                sockClose = true;
                break;
            

            
            default:
                #ifdef Debug
                    printf("Undefined behavior!\n");
                #endif
                break;
            }


        }
        printf("SockClose\nFMClose\n");//之后支持多个client后，就需要改变了

    }

    

};

#endif