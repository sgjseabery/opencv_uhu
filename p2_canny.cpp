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

        /// grayscale
        cv::Mat grayscale_image;
        cv::cvtColor(image, grayscale_image, cv::COLOR_BGR2GRAY);
        cv::imshow("Grayscale", grayscale_image);

        /// canny
        cv::Mat canny_image;
        cv::Canny(grayscale_image, canny_image, 100, 200);
        cv::imshow("Canny", canny_image);
        

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
