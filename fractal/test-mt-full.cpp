//COMPILE g++ `pkg-config --cflags opencv` -o test-mt-full test-mt-full.cpp `pkg-config --libs opencv` -pthread
//RUN ./test-mt-full
#include "../libs/maggytone.h"

using namespace MaggyTone;

Image image;
Buffer buffer;

typedef void * (*THREADFUNCPTR)(void *);

int main( void )
{
    pthread_t   th_draw,            // Thread which draws out of matrix
                th_matrix_reader;   // Thread for simulating piano inputs out of matrix
    int th_submit;                  // Thread initiation report


#ifdef ENABLE_DRAW

    th_submit = pthread_create(&th_draw, NULL, (THREADFUNCPTR) &fractal, NULL);
    if (th_submit){
       printf("ERROR; return code from pthread_create() is %d\n", th_submit);
       return -1;
    }
#endif

#ifdef ENABLE_BUFFER
    th_submit = pthread_create(&th_matrix_reader, NULL,(THREADFUNCPTR) &update_buffer, NULL);
    if (th_submit){
       printf("ERROR; return code from pthread_create() is %d\n", th_submit);
       return -1;
    }
#endif

#ifdef ENABLE_DRAW
    pthread_join(th_draw,NULL);
#endif

#ifdef ENABLE_BUFFER
    pthread_join(th_matrix_reader,NULL);
#endif

    waitKey( 0 );
    return(0);
}
