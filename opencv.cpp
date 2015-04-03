#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{

  /// Compute the histograms:
 
  // Draw the histograms for B, G and R
  int hist_w = 500; int hist_h = 500;

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
  
	for( int i=0; i < 500 ; i+=10)
	{
		line( histImage, Point(i, 500) , Point(i, 200), Scalar( 255, 0, 0), 2, 8, 0  );
//		line( histImage, Point(300, 500) , Point(300, 300), Scalar( 255, 0, 0), 2, 8, 0  );

		namedWindow("Visibility histogram", CV_WINDOW_AUTOSIZE );
		imshow("Visibility histogram", histImage );
	
//		cv::waitKey(10);
		histImage.setTo(Scalar(0,0,0));
	}
  
  cv::waitKey(0);

  return 0;
}

