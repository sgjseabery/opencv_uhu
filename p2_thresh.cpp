#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv) {

    if(argc<2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_video>" << std::endl;
        return -1;
    }

    // Open the video
    cv::VideoCapture capturer(argv[1]);
    if (!capturer.isOpened())
    {
        std::cerr << "Error opening the video!" << std::endl;
        return -1;
    }

    int wait_time = 30; // wait time in ms


    while( capturer.grab())
    {

        /// READ NEXT FRAME
        cv::Mat image;
        if(!capturer.retrieve(image))
        {
            std::cerr << "Error reading the frame!" << std::endl;
            break;
        }
        cv::imshow("Original Video", image);

        // grayscale image
        cv::Mat grayscale_image;
        cv::cvtColor(image, grayscale_image, cv::COLOR_BGR2GRAY);
        cv::imshow("Grayscale", grayscale_image);

        // bilateral filter
        cv::Mat bilateral_image;
        cv::bilateralFilter(grayscale_image, bilateral_image, 9, 75, 75);

        // treshold
        cv::Mat threshold_image;
        static int thresh = 128;

        cv::threshold(bilateral_image, threshold_image, thresh, 255, cv::THRESH_BINARY);
        
        cv::imshow("Threshold", threshold_image);
        cv::createTrackbar("Threshold", "Threshold", nullptr, 255, [](int pos, void* data){thresh = pos;});

        // adaptive threshold
        cv::Mat adaptive_threshold_image;
        static int win_size = 35;
        static int constant = 7;
        if(win_size%2==0) win_size++;
        if(win_size<3) win_size=3;
        cv::adaptiveThreshold(bilateral_image, adaptive_threshold_image, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, win_size, constant);
        cv::imshow("Adaptive Threshold", adaptive_threshold_image);
        cv::createTrackbar("Win Size", "Adaptive Threshold", nullptr, 100, [](int pos, void* data){win_size = pos;});
        cv::createTrackbar("Constant", "Adaptive Threshold", nullptr, 100, [](int pos, void* data){constant = pos;});


        char key = cv::waitKey(wait_time);
        if(key==27) // ESC
            break;
        else if(key=='p') // SPACE
        {
            wait_time = wait_time==0?30:0;
        }
    }

    return 0;
}
