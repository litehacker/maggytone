//COMPILE g++ `pkg-config --cflags opencv` -o maggytone-tree maggytone-tree.cpp `pkg-config --libs opencv` -pthread
//RUN ./fractal-tree
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <mutex>
#include <pthread.h>
#include <math.h>
#include <iostream>

#define w 1200      // screen width
#define h 900       // screen height
#define PB 61       // buttons in piano
#define LIMIT       // Limit of fractal iterations
#define pi 3.14159265359
#define ENABLE_DRAW
#define ENABLE_BUFFER
#define DEGREES 360
//#define BUF_LOCK

using namespace cv;
using namespace std;

Mat bckg(h, w, CV_8UC3, Scalar(255, 255, 255));     // Background colors

class Image
{
    void initialize()
    {
        start.x = w/2;                              // Starting at x coordinate
        start.y = h/2;                              // Starting at y coordinate
        maggytone = "MaggyTone Project";
    }
public:
    Image()
    {
        initialize();
    }
    string maggytone;                                   // Window header
    int bgr[3]={0,0,255};                               // Dot color
    int size=2;                                         // Circle default size
    Point start;                                        // Coordinates declaration
}image;

class Buffer
{
private:
    mutex buf_mutex;
    bool mut_lock;
public:
    bool flag;
    int data[PB];       //pressed buttons data
    bool press[PB];     //storing pressed buttons

    Buffer()
    {
        flag=false;
        data[PB]={1};
        press[PB]={0};
        mut_lock=0;
    }
    void unmodify()
    {
        flag=false;
        data[PB]={0};           // delete all old input data
        for (int i = 0; i<PB; i++)
        data[i]=0;
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
    void lock(){
        mut_lock=1;
        buf_mutex.lock();
    }
    void unlock(){
        buf_mutex.unlock();
        mut_lock=0;
    }
    bool locked(){
        if (mut_lock==1)
            return 1;
        return 0;
    }
}buffer;

// Continue drawing if the buffer has any of the input
bool continue_drawing()
{

    if (!buffer.modified())
        return 1;
    else
        return 0;
}

// Drawing filled circles in specific location
void draw_line(double size, double angle)
{
    int color[3]={0};
    double x_coord = image.start.x, y_coord = image.start.y;
    //buffer.unlock();

    angle=(angle*DEGREES/PB);

    while(continue_drawing()){
        circle(bckg, image.start, 1, Scalar( color[0],color[1],color[2] ), FILLED,LINE_8 );

        moveWindow( image.maggytone, 0, 0 );
        imshow( image.maggytone, bckg );
        waitKey(100);

        //when to update size
        if(size>1)
            size-=0.1;

        //update coordinates
        x_coord += sin(angle*pi/180);
        y_coord += -cos(angle*pi/180);

        image.start.x = x_coord;
        image.start.y = y_coord;
    }
    std::cout << "---------------------------: buffer.modified()=" <<buffer.modified()<< '\n';

}

void print_buf()
{
    for (int i = 0; i<PB; i++)
    cout << buffer.data[i]<<" ";
    cout<<endl;
}

void *fractal(void *a)
{
    int angle, button, size;
    while(1==1)
    {
        if (buffer.modified())        // If buffer is modified
        {
            //buffer.lock();
            for (int k = 0; k < PB; k++)
            {
                if (buffer.data[k]>0)
                {
                    size=buffer.data[k];
                    angle=k;
                    break;
                }
            }
            buffer.unmodify();                  // Disable flag and reset buffer
            print_buf();
            draw_line( size, angle);
        }
    }
    //fractal(NULL);
    pthread_exit(0);
}



void *update_buffer(void*a)
{
    long i=0;
    int button;
    while (1==1)
    {
        // some action to tigger to update the buffer
        while(buffer.modified()){}
        cin>>button;
        // actual update starts here. lock and unlock for safety
        #ifdef BUF_LOCK
        //lock_guard <mutex> lockGuard(buffer.buf_mutex); //Do this for each buffer you want to access

        buffer.lock();
        #endif

        buffer.data[button]=button;    // mark pressed button
        buffer.modify();               // enable modified flag

        #ifdef BUF_LOCK

        buffer.unlock();

        #endif
        if (buffer.modified())
            cout<<"buffer updated successfully"<< i <<":"<<buffer.modified()<<endl;
        i++;
    }
    pthread_exit(0);
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
