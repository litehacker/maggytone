//COMPILE g++ `pkg-config --cflags opencv` pole.cpp `pkg-config --libs opencv`
//RUN ./a.out
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <iostream>

#define w 1200
#define h 900
using namespace cv;
using namespace std;


void MyFilledCircle( Mat img, Point center, int bgr[], int size )
{
    circle( img,
        center,
        size,
        Scalar( bgr[0],bgr[1],bgr[2] ),
        FILLED,
        LINE_8 );
}

void draw(Mat bckg, char maggytone[])
{
    int bgr[3]={0,0,255};
    int size=2,r=22;
    int i;
    Point pt;
    pt.x = w/2;
    pt.y = h/2;


    for (double i = 0; i < 6.3; i=i+0.001)
    {
        //size=rand()%2+2;

        pt.x=(16*pow(sin(i),3))*r+w/2;
        pt.y=-(13*cos(i)-5*cos(2*i)-2*cos(3*i)-cos(4*i))*r+h/2;
        MyFilledCircle(bckg,pt,bgr,size);
        moveWindow( maggytone, 0, 0 );
        imshow( maggytone, bckg );
        waitKey(1);

        cout<<i<<endl;
    }
}

int main( void )
{
    char maggytone[] = "MaggyTone Project";

    Mat bckg(h, w, CV_8UC3, Scalar(255, 255, 255));
    draw(bckg,maggytone);





    waitKey( 0 );
    return(0);
}
