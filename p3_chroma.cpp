#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv) {

    if(argc<3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_video> <background_image>" << std::endl;
        return -1;
    }

    // Open the video
    cv::VideoCapture capturer(argv[1]);
    if (!capturer.isOpened())
    {
        std::cerr << "Error opening the video!" << std::endl;
        return -1;
    }

    // read background image
    cv::Mat background_image = cv::imread(argv[2]);
    if(background_image.empty())
    {
        std::cerr << "Could not open or find the background image!" << std::endl;
        return -1;
    }

    int wait_time = 15; // wait time in ms

    while( capturer.grab())
    {

        /// READ NEXT FRAME
        cv::Mat image;
        if(!capturer.retrieve(image))
        {
            std::cerr << "Error reading the frame!" << std::endl;
            break;
        }
        // resize to half
        //cv::resize(image, image, image.size()/2);
        cv::imshow("Original Video", image);

        // bilateral filter
        cv::Mat bilateral_image;
        cv::bilateralFilter(image, bilateral_image, 9, 75, 75);

        // hsv image
        cv::Mat hsv_image;
        cv::cvtColor(bilateral_image, hsv_image, cv::COLOR_BGR2HSV);

        cv::Mat thresh_background;
        static int h0=45, h1=70, s0=80, s1=255, v0=80, v1=255;
        cv::inRange(hsv_image, cv::Scalar(h0,s0,v0), cv::Scalar(h1,s1,v1), thresh_background);
        //cv::dilate(thresh_background, thresh_background, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)), cv::Point(-1,-1), 1);
        //cv::erode(thresh_background, thresh_background, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)), cv::Point(-1,-1), 1);
        
        cv::imshow("Ball Segmentation", thresh_background);
        cv::createTrackbar("Hue low", "Ball Segmentation", nullptr, 180, [](int pos, void* data){h0 = pos;});
        cv::createTrackbar("Hue high", "Ball Segmentation", nullptr, 180, [](int pos, void* data){h1 = pos;});
        cv::createTrackbar("Saturation low", "Ball Segmentation", nullptr, 255, [](int pos, void* data){s0 = pos;});
        cv::createTrackbar("Saturation high", "Ball Segmentation", nullptr, 255, [](int pos, void* data){s1 = pos;});
        cv::createTrackbar("Value low", "Ball Segmentation", nullptr, 255, [](int pos, void* data){v0 = pos;});
        cv::createTrackbar("Value high", "Ball Segmentation", nullptr, 255, [](int pos, void* data){v1 = pos;});

        cv::Mat final_image = image.clone();
        background_image.copyTo(final_image, thresh_background);
        cv::imshow("Image Background", final_image);

        char key = cv::waitKey(wait_time);
        if(key==27) // ESC
            break;
        else if(key=='p') // SPACE
        {
            wait_time = wait_time==0?15:0;
        }
    }

    return 0;
}
