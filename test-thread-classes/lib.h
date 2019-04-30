//lib.h
#include <iostream>
#include <pthread.h>
using namespace std;

namespace TestNamespace{
    class TestClass{
    public:
        TestClass();
    };
    void *threadFunction(void *);
}
