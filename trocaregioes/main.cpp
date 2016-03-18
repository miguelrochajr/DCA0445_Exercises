#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <highgui.h>
#include <time.h>

using namespace cv;
using namespace std;

bool is_in (int c[4], int a){
    bool flag=0;
    for (int i=0; i<4; i++){
        if(a == c[i]){
            flag = 1;
            break;
        }
    }
    return flag;
}

int main(){

  Mat image, fstQ, sndQ, trdQ, fthQ; // fstQ = First Quarter, sndQ = Second Quarter and so on
  Vec3b val;
  int chosen[4] = {-1,-1,-1,-1};
  bool flag = 1;
  int iteration = 0;

  srand (time(NULL));


  image= imread("/home/msrj/Downloads/Teste.jpg",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "Could not open the file" << endl;

  fstQ = image(Rect(0, 0, image.cols/2, image.rows/2)); // Rect (x, y, width, height)
  sndQ = image(Rect( image.cols/2, 0, image.cols/2, image.rows/2));
  trdQ = image(Rect( 0, image.rows/2, image.cols/2, image.rows/2));
  fthQ = image(Rect( image.cols/2, image.rows/2, image.cols/2, image.rows/2));

  while (flag) //this while loop fills the choosen vector with four random numbers from 0 to 3 and makes sure that all numbers are diferent.
  {
      int num = rand() % 4;
      if(!is_in(chosen,num)){
          chosen[iteration] = num;
          iteration++;
          if(iteration==4){
              flag = 0;
          }
      }
  }

  Mat final = image.clone();

  for (int i=0; i<4; i++){
      int choose;
      Mat temp;
      choose = chosen[i];
      cout << choose << endl;


      switch (choose) //Which quarter of the image is going to be placed
      {
            case 0:
                cout << "First quarter" << endl;
                temp = fstQ.clone();
                break;
            case 1:
                cout << "Second quarter" << endl;
                temp = sndQ.clone();
                break;
            case 2:
                cout << "Third Quarter" << endl;
                temp = trdQ.clone();
                break;
            case 3:
                cout << "Fourth Quarter" << endl;
                temp = fthQ.clone();
                break;
      }


      switch (i) //The first selected quarter will be placed into the first quarter. The second selected
                 //quarter will be placed into the second quarter and so on
      {
            case 0:
                cout << "placed on first" << endl;
                temp.copyTo(final(Rect(0, 0, image.cols/2, image.rows/2)));
                break;
            case 1:
                cout << "placed on 2nd" << endl;
                temp.copyTo(final(Rect( image.cols/2, 0, image.cols/2, image.rows/2)));
                break;
            case 2:
                cout << "placed on 3rd" << endl;
                temp.copyTo(final(Rect( 0, image.rows/2, image.cols/2, image.rows/2)));
                break;
            case 3:
                cout << "placed on 4th" << endl;
                temp.copyTo(final(Rect( image.cols/2, image.rows/2, image.cols/2, image.rows/2)));
                break;
      }
  }

  namedWindow("janela", WINDOW_AUTOSIZE);
  imshow("janela", final);
  waitKey();

  return 0;
}
