#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;


void puzzle(Mat image, int rows, int cols){

}



int main()
{
    Mat image;
    Vec3b val;
    int x0=200, x1=300,y0=300, y1=700;
    int time = rand();


    val[0] = 0; //B
    val[1] = 0; //G
    val[2] = 0; //R

    image = imread ("/home/msrj/Downloads/Teste.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!image.data)
        cout << "Could not open the image. \n";

    for (int i=x0; i<x1; i++){
        for(int j=y0; j<y1; j++){
            image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
        }
    }

    namedWindow ("Window", WINDOW_AUTOSIZE);
    imshow("Window", image);
    waitKey();

}

