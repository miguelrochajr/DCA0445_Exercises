#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>



using namespace cv;
using namespace std;

struct Object {
    int value;
    bool flag_hole =0; // zero means there is NO hole. One means that HAS a hole.
};

vector<Object> objects; //this vector will store each object with its specific grayscale value and say if it has a hole or not.

void Label(Mat image, int& labels, CvPoint* p) //Label the regions of the image
{
    Object temp;

    for(int i=0; i<image.size().height; i++)
    {
      for(int j=0; j<image.size().width; j++)
      {
        if(image.at<uchar>(i,j) == 255)
        {
          //Found an object
          labels++;
          p->x=j;
          p->y=i;
          floodFill(image,*p,labels);
          temp.value = labels;
          objects.push_back(temp);
        }
      }
    }
}

void white_edges(Mat image, int width, int height) //Makes the edges of the picture white
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

void look4holes (Mat image, CvPoint& p, int object_label, int& column)
{
    int current_value = -1;
    int i = p.y;

    do                               //Now, look for holes on this line until we reach the other side of the object, which is the background (label 1).
    {                                //Note that we will only increase the "j". So is a horizontal traverse.
        current_value = image.at<uchar>(i,column);
        //cout << "Current value: " << current_value << endl;
        if(current_value == 0) //Found a hole in the object!
        {
            objects[object_label-2].flag_hole = 1; //The index for the object is indicated by its label-2.
        }
        column++;
    }while(current_value != 1);
}

void look4objects (Mat image, int& labels)
{
    CvPoint p;
    uchar label;

    for(int i=0; i<image.size().height; i++)
    {
      for(int j=0; j<image.size().width; j++)
      {
        label = image.at<uchar>(i,j);
        cout << label << " ";
        //label = image.at<uchar>(i,j);
        //cout << label << endl;
        /*
        if(label>1 && label<= labels)  //Found a labled object!
        {                               //Note that the Background is labled as 1. So we can't consider it as an object
            //cout << "Calling look4holes with a pixel value of " << label << endl;
            look4holes(image, p, label, j);
        }*/
      }
      cout << endl;
    }
}

void countObjects (int& with_holes, int& no_holes)
{
    for(unsigned int i=0; i<objects.size(); i++)
    {
        if(objects[i].flag_hole == 1)
            with_holes++;
        else
            no_holes++;
    }
}

int main()
{
  Mat image, mask;
  int width, height;
  int nobjects = 0, nobjects_hole = 0, nobjects_WithoutHole=0, labels=1;

  CvPoint p;
  image = imread("/home/msrj/Documents/PDI/Exercicios/labeling/bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    cout << "Sorry! The image could not be loaded. \n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  white_edges(image, width, height); //Make the edges white

  floodFill(image,p,0); // Erase the objects that are touching the edges
  floodFill(image, p, 1); //Label the background with 1

  Label(image, labels, &p); //Label the objects

  //look4objects(image, labels);

  int label = -1;
  for(int i=0; i<image.size().height; i++)
  {
    for(int j=0; j<image.size().width; j++)
    {
      if(label != 1)
          cout << label << endl;
      //label = image.at<uchar>(i,j);
      //cout << label << endl;
      /*
      if(label>1 && label<= labels)  //Found a labled object!
      {                               //Note that the Background is labled as 1. So we can't consider it as an object
          //cout << "Calling look4holes with a pixel value of " << label << endl;
          look4holes(image, p, label, j);
      }*/
    }
  }
  //countObjects(nobjects_hole, nobjects_WithoutHole);
  nobjects = labels-1;


  cout << "We have " << nobjects << " objects \n";
  cout << "We have " << nobjects_hole << " objects with a hole \n";
  cout << "We have " << nobjects_WithoutHole << " objects without a hole \n";

  for (unsigned int i=0; i<objects.size(); i++){
      cout << "Object number "<< i+1 << ": " << objects[i].value << " " << objects[i].flag_hole << endl;
  }

  imshow("image", image);
  //imwrite("labeling.png", image);
  waitKey();
  return 0;
}
