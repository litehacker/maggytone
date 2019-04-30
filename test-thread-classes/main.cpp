//main.cpp
#include "lib.h"
using namespace TestNamespace;

TestClass test;

int main(){
    pthread_t testThread;
    int err = pthread_create(&testThread, NULL, threadFunction, NULL);
    if (err) {
        cout<<"error in thread create"<<endl;
    }
    pthread_join(testThread,NULL);
    return 0;
}
