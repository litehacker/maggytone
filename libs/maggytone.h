
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <mutex>
#include <pthread.h>
#include <math.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <stdio.h>

#define w 1200      // screen width
#define h 900       // screen height
#define MAX_NOTE_NUMBER 61       // buttons in piano
#define LIMIT       // Limit of fractal iterations
#define pi 3.14159265359
#define ENABLE_DRAW
#define ENABLE_BUFFER
#define DEGREES 360
#define LENGTH 16
//#define BUF_LOCK

using namespace std;
using namespace cv;

namespace MaggyTone
{

    class Image
    {
        void initialize();
    public:
        Image();
        string maggytone;                                   // Window header
        int bgr[3]={0,0,255};                               // Dot color
        int size=2;                                         // Circle default size
        Point start;                                        // Coordinates declaration
    };

    class Buffer
    {
    private:
        mutex buf_mutex;
        bool mut_lock;
    public:
        bool flag;
        int data[MAX_NOTE_NUMBER];       //pressed buttons data
        bool press[MAX_NOTE_NUMBER];     //storing pressed buttons

        Buffer();
        void unmodify();
        bool modified();
        void modify();
        void clear_buffer();
        bool pressed(int location);
        void unpress(int location);
        void lock();
        void unlock();
        bool locked();
    };

    bool busy();
    void add_value_to_matrix(int matrix[][MAX_NOTE_NUMBER],vector <int> starts,int st_n,vector <int> ends, int ed_n,int counter,int force);
    void print_matrix(int matrix[LENGTH][MAX_NOTE_NUMBER]);
    void read_input();
    bool continue_drawing();
    void draw_line(double size, double angle);
    void print_buf();
    void *fractal(void *);
    void *update_buffer(void*);

}
