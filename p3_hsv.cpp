#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv) {

    if(argc<2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image>" << std::endl;
        return -1;
    }

    // read image
    cv::Mat image = cv::imread(argv[1]);

    // check image
    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    cv::imshow("Image", image);

    // convert to hsv
    cv::Mat hsv_image;
    cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

    // split and show hsv channels
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv_image, hsv_channels);
    cv::imshow("Hue", hsv_channels[0]);
    cv::imshow("Saturation", hsv_channels[1]);
    cv::imshow("Value", hsv_channels[2]);
    
    do
    {

        // range of colors HSV
        cv::Mat thresh_color;
        static int h0=0, h1=255, s0=15, s1=255, v0=0, v1=255;
        cv::inRange(hsv_image, cv::Vec3b(h0,s0,v0), cv::Vec3b(h1,s1,v1), thresh_color);
    
        cv::imshow("Color segmentation", thresh_color);

        cv::createTrackbar("Hue low", "Color segmentation", nullptr, 180, [](int pos, void* data){h0 = pos;});
        cv::createTrackbar("Hue high", "Color segmentation", nullptr, 180, [](int pos, void* data){h1 = pos;});
        cv::createTrackbar("Saturation low", "Color segmentation", nullptr, 255, [](int pos, void* data){s0 = pos;});
        cv::createTrackbar("Saturation high", "Color segmentation", nullptr, 255, [](int pos, void* data){s1 = pos;});
        cv::createTrackbar("Value low", "Color segmentation", nullptr, 255, [](int pos, void* data){v0 = pos;});
        cv::createTrackbar("Value high", "Color segmentation", nullptr, 255, [](int pos, void* data){v1 = pos;});

    }while(cv::waitKey(10)!=27);
    

    return 0;
}
