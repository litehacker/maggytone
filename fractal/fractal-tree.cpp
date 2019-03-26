//COMPILE g++ `pkg-config --cflags opencv` -o pole pole.cpp `pkg-config --libs opencv` -pthread

//RUN ./a.out
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <mutex>
#include <pthread.h>
#include <cmath>
#include <iostream>

#define w 1200      // screen width
#define h 900       // screen height
#define PB 61       // buttons in piano
#define LIMIT       // Limit of fractal iterations

#define ENABLE_DRAW
#define ENABLE_BUFFER
#define BUF_LOCK

using namespace cv;
using namespace std;

class Buffer{

    bool flag;
    int data[PB];       //pressed buttons data
    bool press[PB];     //storing pressed buttons
    mutex buf_mutex;
    Buffer()
    {
        flag=false;
        data[PB]={1};
        press[PB]={0};
    }
    void unmodified()
    {
        flag=false;
    }
    void modified()
    {
        flag=true;
    }
    void clear_buffer()
    {
        data[PB]={0};
    }
    bool pressed(int location)
    {
        if (press[location])
        {
            return 1;
        }
        return 0;
    }
    void unpress(int location)
    {
        press[location]=0;
    }

}buffer;

void *update_buffer(void*a)
{
    long i=0;
    int button;
    while (1==1)
    {
        // some action to tigger to update the buffer
        cin>>button;
        // actual update starts here. lock and unlock for safety
        #ifdef BUF_LOCK
        //lock_guard <mutex> lockGuard(buffer.buf_mutex); //Do this for each buffer you want to access
        buffer.buf_mutex.lock();
        #endif
        for (size_t t = 0; t < PB; t++)
        {
            buffer.data[t]=0;           // delete all old input data
        }

        buffer.data[button]=button;     // mark pressed button
        buffer.modified();              // enable modified flag
        #ifdef BUF_LOCK
        buffer.buf_mutex.unlock();
        #endif
        cout<<endl<<"buffer updated "<< i <<endl;
        i++;
    }
    pthread_exit(0);
}

// Drawing filled circles in specific location
void MyFilledCircle( Mat img, Point center, int bgr[], int size )
{
    circle( img,center,size,Scalar( bgr[0],bgr[1],bgr[2] ),FILLED,LINE_8 );
}

// Continue drawing if the buffer has any of the input
bool continue_drawing()
{
    for (size_t k = 0; k < PB; k++)
    {
        if (buffer.data[k]>0)
        {
            return 1;
        }
    }
    return 0;
}

char maggytone[] = "MaggyTone Project"; // window header
int bgr[3]={0,0,255};                   // Background color
int size=2;                             // Circle default size
Mat bckg(h, w, CV_8UC3, Scalar(255, 255, 255)); // background colors
Point pt;                                       // coordinates declaration
pt.x = w/2; // Starting at x coordinate
pt.y = h/2; // Starting at y coordinate

bool stop_drawing(int location)
{
    if (buffer.data[location]<0)
        return 1;                   // Stop Drawing
    return 0;
}

void *fractal(void *a)
{
    cout<<"i start drawing!"<<endl;
    int button;
    if (buffer.flag)                            // if buffer is modified
    {
        for (size_t k = 0; k < PB; k++)
        {
            if (buffer.data[k]>0)
            {
                size=buffer.data[k];
                button=k;
                by=k%PB;
            }
        }
        buffer.unmodified();                    // disable flag
    }

    //pt.x=;
    //pt.y=;

    MyFilledCircle(bckg,pt,bgr,size);
    moveWindow( maggytone, 0, 0 );
    imshow( maggytone, bckg );
    waitKey(1);

    if (stop_drawing(button))
    {
        pthread_exit(0);
    }
    else
    fractal();

}

int main( void )
{
    pthread_t   th_draw, // thread which draws out of matrix
                th_matrix_reader; // thread for simulating piano inputs out of matrix
    int th_submit; // thread initiation report


#ifdef ENABLE_DRAW

    th_submit = pthread_create(&th_draw, NULL, fractal, NULL);
    if (th_submit){
       printf("ERROR; return code from pthread_create() is %d\n", th_submit);
       return -1;
    }
#endif

#ifdef ENABLE_BUFFER
    th_submit = pthread_create(&th_matrix_reader, NULL, update_buffer, NULL);
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
