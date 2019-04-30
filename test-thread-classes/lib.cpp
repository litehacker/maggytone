//lib.cpp
#include "lib.h"

namespace TestNamespace{
    TestClass::TestClass(){
        cout<<"constructor started"<<endl;
    }
    void *threadFunction(void*){
        cout<<"tread function initiated"<<endl;
    }
}
