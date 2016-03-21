#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int transformation(int A /*This A=(L-1)/MN*/, int Array_Ocurrrencies[], int k){
    int sum;
    for(int i=0; i<=k; i++){
        sum = sum + Array_Ocurrrencies[i];
    }

    return sum;
}

void equalize_image (Mat image, int L){

    int R[64]={}, G[64]={}, B[64]={}; //Each array will have the index as the Intensity and the value as the occurency
    int height = image.size().height;
    int width = image.size().width;
    Vec3b intensity; // This vector will be used to  store intensity values of the three channels

    //First, let's build the histogram array

    for(int i=0; i<height;i++)
    {
        for(int j=0; j<width; j++)
        {
            intensity = image.at<Vec3b>(i,j);

            B[int((intensity.val[0]))/4]++;
            G[int((intensity.val[1]))/4]++;
            R[int((intensity.val[2]))/4]++;
        }
    }

    //Second, apply the transformation in each pixel

    float A = (float(L)-1.0)/(float(height*width));

    for(int i=0; i<height;i++)
    {
        for(int j=0; j<width; j++)
        {
            intensity = image.at<Vec3b>(i,j);

            intensity.val[0] = transformation(A, B, intensity.val[0]);
            intensity.val[1] = transformation(A, G, intensity.val[1]);
            intensity.val[2] = transformation(A, R, intensity.val[2]);

            image.at<Vec3b>(i,j) = intensity;
        }
    }
}


int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histG, histB;
  int nbins = 64; // This is the L of our equation
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;

    equalize_image(image, nbins);

    split (image, planes);

    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG, Point(i, histh),
           Point(i, cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB, Point(i, histh),
           Point(i, cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));
    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
