#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void Label(Mat image, int hue, CvPoint* p){

    for(int i=0; i<image.size().height; i++){
      for(int j=0; j<image.size().width; j++){
        if(image.at<uchar>(i,j) == 255){
          //Found an object
          hue++;
          p->x=j;
          p->y=i;
          floodFill(image,*p,hue);
        }
      }
    }
}

void LOOK_4_HOLES(Mat image, int Objects, CvPoint* p){
    int j=p->x;
    cout << "Got here!" << endl;
    cout << j << endl;
    while(image.at<uchar>(0,j)!=1) //Keep looking for a hole until we reach the in this line background (pixel labled as 1)
    {
      cout << "FOUND A HOLE!" << endl;
      image.at<uchar>(0,j) = 255;
      j++;
    }
};

int main(){
  Mat image, mask;
  int width, height;
  int nobjects = 1, nobjets_hole = 0, current = -1;

  CvPoint p;
  image = imread("/home/msrj/Documents/PDI/Exercicios/labeling/bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  //MAKE THE EDGES WHITE
  {

  for(int i=0; i<height; i++) //traverse the left edge
  {
    for(int j=0; j<1; j++){
        image.at<uchar>(i,j) = 255;
    }
  }

  for(int i=0; i<height; i++) //traverse right edge
  {
    for(int j=width-1; j<width; j++)
    {
        image.at<uchar>(i,j) = 255;
    }
  }

  for(int i=height-1; i<height; i++) //traverse bottom edge
  {
    for(int j=0; j<width; j++)
    {
        image.at<uchar>(i,j) = 255;
    }
  }

  for(int i=0; i<1; i++) //traverse top edge
  {
    for(int j=0; j<width; j++)
    {
        image.at<uchar>(i,j) = 255;
    }
  }
  }

  floodFill(image,p,0); // Erase the objects that are touching the edges
  floodFill(image, p, 1); //Label the background with 1

  //LABEL THE OBJECTS. And count the total
  Label(image, nobjects, &p);

  //LOOK FOR HOLES IN THE OBJECTS
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j)<=nobjects || image.at<uchar>(i,j)>1){
          //Found a labled object!
          p.x = j;
          p.y = i;
          LOOK_4_HOLES(image, nobjects, &p);
      }
    }
  }

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
