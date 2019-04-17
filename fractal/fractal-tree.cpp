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


class Image
{
    initialize()
    {
        start.x = w/2;                                         // Starting at x coordinate
        start.y = h/2;                                         // Starting at y coordinate

    }
    Image()
    {
        initialize();
    }

    char maggytone[] = "MaggyTone Project";             // Window header
    int bgr[3]={0,0,255};                               // Dot color
    int size=2;                                         // Circle default size
    Mat bckg(h, w, CV_8UC3, Scalar(255, 255, 255));     // Background colors
    Point start;                                           // Coordinates declaration                                        // Coordinates declaration

}image;

class Buffer
{
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
    void unmodify()
    {
        flag=false;
    }
    bool modified()
    {
        if (flag)
            return 1;
        return 0;
    }
    void modify()
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
        buffer.modify();              // enable modified flag
        #ifdef BUF_LOCK
        buffer.buf_mutex.unlock();
        #endif
        cout<<endl<<"buffer updated "<< i <<endl;
        i++;
    }
    pthread_exit(0);
}

// Drawing filled circles in specific location
void draw_line(int color[], int size, Point start, double angle)
{

    //calculate distance
    distance=sqrt((end.y-start.y)^2+(end.x-start.x)^2);

    //calculate slope function between two points
    m=(end.y-start.y)/(end.x-start.x);
    c=end.y-(m*end.x);
    cout<<"slope (y)="<<m<<"x+"<<c;

    // Draw between points according the slope function above
    while(zero<distance){
        circle( img,point,size,Scalar( color[0],color[1],color[2] ),FILLED,LINE_8 );
    }
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


bool stop_drawing(int location)
{
    if (buffer.data[location]<0)
        return 1;                                   // Stop Drawing
    return 0;
}

void *fractal(void *a)
{
    int button;
    if (buffer.modified())                                // If buffer is modified
    {
        for (int k = 0; k < PB; k++)
        {
            if (buffer.data[k]>0)
            {
                size=buffer.data[k];
                button=k;
                break;
            }
        }
        buffer.unmodify();                        // Disable flag
        draw_line( size, image.start, button);
        pthread_exit(0);
    }
    fractal();
}

int main( void )
{
    pthread_t   th_draw,            // Thread which draws out of matrix
                th_matrix_reader;   // Thread for simulating piano inputs out of matrix
    int th_submit;                  // Thread initiation report
    moveWindow( image.maggytone, 0, 0 );
    imshow( image.maggytone, bckg );

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
