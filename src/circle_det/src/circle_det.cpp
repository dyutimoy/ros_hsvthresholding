#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Image window";
using namespace cv;
using namespace std;
int low_r=30, low_g=30, low_b=30;
int high_r=100, high_g=100, high_b=100;
int cannyThreshold = 200;
int accumulatorThreshold = 50;
//
Mat circle_det(Mat);
void threshold();

void HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold)
{
        // will hold the results of the detection
        std::vector<Vec3f> circles;
        // runs the actual detection
        HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/8, cannyThreshold, accumulatorThreshold, 0, 0 );

        // clone the colour, input image for displaying purposes
        Mat display = src_display.clone();
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( display, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle( display, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

        // shows the results
        imshow( "output", display);
}

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/kraken/front_camera", 1, 
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr,cv_final_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    cv_final_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }


    // Draw an example circle on the video stream
   // if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
    //  cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));


    

    threshold();//intialize the trackbars 
    // Update GUI Window   while (1)
  
    //cv_final_ptr=cv_ptr;
    cv_final_ptr->image = circle_det(cv_ptr->image);

        //runs the detection, and update the display
    //HoughDetection(cv_final_ptr->image,cv_ptr->image, cannyThreshold, accumulatorThreshold);
    imshow("output",cv_ptr->image);
    imshow(OPENCV_WINDOW, cv_final_ptr->image);
    waitKey(30);
  
    
     
    // Output modified video stream
    image_pub_.publish(cv_final_ptr->toImageMsg());
  }
};



int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}

void threshold()
{
  
  namedWindow(OPENCV_WINDOW, CV_WINDOW_AUTOSIZE);
  createTrackbar("Low R",OPENCV_WINDOW, &low_r, 255);
  createTrackbar("High R",OPENCV_WINDOW, &high_r, 255);
  createTrackbar("Low G",OPENCV_WINDOW, &low_g, 255);
  createTrackbar("High G",OPENCV_WINDOW, &high_g, 255);
  createTrackbar("Low B",OPENCV_WINDOW, &low_b, 255);
  createTrackbar("High B",OPENCV_WINDOW, &high_b, 255);
  
}

Mat circle_det(Mat img)
{
    Mat img_det;
   inRange(img,Scalar(low_b,low_g,low_r), Scalar(high_b,high_g,high_r),img_det);
   
  Mat src_gray = img_det;
  
    /// Convert it to gray
  //cvtColor( img_det, src_gray, CV_BGR2GRAY );

  /// Reduce the noise so we avoid false circle detection
   //part 1
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
   }


   //////////////////////////////////////////////////////////////////////////////////////////////////
   //part 2

   //declare and initialize both parameters that are subjects to change
    

    // create the main window, and attach the trackbars
   
  return img_det;
}

