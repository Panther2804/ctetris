#include <string>
#include <iostream>
#include <thread_db.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "main.h"


using namespace std;


int fun() {
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}

void sprint(char a) {
    std::cout << a;
}

void sprintln(char a) {
    std::cout << a << std::endl;
}

void sbegin(int a) {

}

int delay(int milisec)
{
    struct timespec req={0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1)
        continue;
    return 1;
}

void memcpy(int a[9][14], int b[9][14], int c) {
    a = b;
}

