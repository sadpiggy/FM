//
// Created by 王崑运 on 2022/12/5.
//

#ifndef FM_FMPTR_H
#define FM_FMPTR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common.h"

template <class T> //用来模板，是不是就不能统一管理了？ //也可以，不过需要一个ptrId注册在manager，然后在解引用（调用到指针的时候）的时候，判断是否回调
class FMptr{ // isShare之后考虑
private:
    T *value;
    unsigned long valueSize; // unsigned int 32 bit,4GB
    bool isFar;
    bool isDeleted;
    unsigned int farId; // 1 base
    int farMem;         // sock
public:
    FMptr(T *value_, int farMem_){
        this->value = value_;
        this->valueSize = sizeof(T);
        this->farMem = farMem_;
        this->isFar = false;
        this->farId = 0; //
        this->isDeleted = false;
    }; // value,farMem

    FMptr(const FMptr<T> &obj){
        printf("unique ptr cann't share the same value\nWe will return a empty FMptr\n");
        this->value = nullptr;
        this->valueSize = 0;
        this->farMem = 0;
        this->isFar = false;
        this->farId = 0; //
        this->isDeleted = false;
    }; //拷贝构造函数，可能是需要的。但是如果两个的value指向同一个对象，则会导致double delete，所以应该需要禁用

    ~FMptr(){
        if (this->isFar || this->isDeleted)
            return;
        // delete(this->value);
        free(this->value);
        //使用delete，会调用T的析构函数//有一个问题，一个对象的数据在远端的时候，我需要将它召回再delete吗？如果不召回的话，就不能delete，而如果不delete，则会导致析构函数不会被执行，这可能会导致错误
        //或许可以加一个tag，默认召回，如果标记，则不召回
        //不应该delete，因为如果delete，送回远端的时候delete一次，在本地被析构的时候又delete一次，就调用了两次析构函数。应该free
    }; //如果在远端，就不析构，可能会带来bug，但是先不管

    void offload(){
#ifdef Debug
        if (this->isFar)
        {
            printf("This obj is far already\n");
            return;
        }
#endif
        // todo 现在还没有error处理
        this->farId = sendObj(this->value, this->valueSize, this->farMem);
        this->isFar = true;
        free(this->value);
    }; //放入远内存

    void inload(){
#ifdef Debug
        if (!this->isFar)
        {
            printf("This obj is local already\n");
            return;
        }
#endif
        this->value = (T*)malloc(this->valueSize);
        getObj(this->value, this->valueSize, this->farId, this->farMem);
        this->isFar = false;
    }; //放入本地

    T &operator*(){
#ifdef Debug
        if (this->value == nullptr)
        {
            printf("Panic!you are attempt to dereference the nullptr\n");
            closeFM(this->farMem);
            exit(1);
        }
#endif

        return *(this->value);
    };

    T *operator->(){
#ifdef Debug
        if (this->value == nullptr)
        {
            printf("Panic!you are attempt to dereference the nullptr\n");
            closeFM(this->farMem);
            exit(1);
        }
#endif

        return (this->value);
    };

    void deleteValue(){
        delete this->value;
        this->isDeleted = true;
    }
}; //如果需要被销毁时使用析构函数，使用这个函数、1.为了不delete不需要delete的对象；2.为了避免重复delete带来的bug

;

#endif // FM_FMPTR_H
