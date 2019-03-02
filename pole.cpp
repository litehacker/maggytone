//COMPILE g++ `pkg-config --cflags opencv` pole.cpp `pkg-config --libs opencv`
//RUN ./a.out
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#define w 1000
#define h 750
using namespace cv;

void MyFilledCircle( Mat img, Point center, int bgr[], int size )
{
    circle( img,
        center,
        size,
        Scalar( bgr[0],bgr[1],bgr[2] ),
        FILLED,
        LINE_8 );
}

int main( void )
{
    char maggytone[] = "MaggyTone Project";
    int bgr[3]={0,0,255};
    int size=10;

    Point pt;
    Mat bckg(h, w, CV_8UC3, Scalar(255, 255, 255));

    pt.x = 0;
    pt.y = 100;

    for (size_t i = 0; i < 100; i++) {
        MyFilledCircle(bckg,pt,bgr,size);
        pt.x=i;
    }


  imshow( maggytone, bckg );
  moveWindow( maggytone, 0, 0 );
  waitKey( 0 );
  return(0);
}
