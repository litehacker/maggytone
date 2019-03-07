#include <pthread.h>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <iostream>

#define NUM_THREADS     5

using namespace cv;
using namespace std;

void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;

    waitKey(1000);
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for(t=0; t<NUM_THREADS; t++){
       printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          return -1;
       }

    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}
