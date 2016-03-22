#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int transformation(int k, float ArrayOfData[], int L)
{
    float sum = 0;
    k = k/4;
    for(int i=0; i<=k; i++){
        sum = sum + ArrayOfData[i];
    }
    return sum*(L-1);
}

void equalize_image (Mat image, int L){

    float R[64]={}, G[64]={}, B[64]={}; //Each array will have the index as the Intensity and the value as the occurency
    int height = image.size().height;
    int width = image.size().width;
    float sum=0.0;
    float A = width*height;
    Vec3b intensity;
    float sumR = 0, sumG=0, sumB=0;

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

    //Now we put the Histogram array in a form of a Probability Density Funcion
    for(int i=0; i<L; i++)
    {
        R[i] = R[i]/A;
        G[i] = G[i]/A;
        B[i] = B[i]/A;
    }


    int Blue, Green, Red;
    for(int i=0; i<height;i++)
    {
        for(int j=0; j<width; j++)
        {
            intensity = image.at<Vec3b>(i,j);

            intensity.val[0] = transformation(intensity.val[0], B, L)*4;
            intensity.val[1] = transformation(intensity.val[1], G, L)*4;
            intensity.val[2] = transformation(intensity.val[2], R, L)*4;

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
