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

        /// blur
        static int blur_ksize = 5;
        if(blur_ksize==0) blur_ksize = 1;
        cv::Mat blur_image;
        cv::blur(image, blur_image, cv::Size(blur_ksize, blur_ksize));
        cv::imshow("Blur", blur_image);        
        //cv::createTrackbar("Win size", "Blur", &blur_ksize, 100);
        cv::createTrackbar("Win size", "Blur", nullptr, 100, [](int pos, void* data){blur_ksize = pos;});

        // gaussian blur
        static int gaussian_blur_ksize = 5;
        if(gaussian_blur_ksize%2==0) gaussian_blur_ksize++;
        cv::Mat gaussian_blur_image;
        cv::GaussianBlur(image, gaussian_blur_image, cv::Size(gaussian_blur_ksize,gaussian_blur_ksize), 0);
        cv::imshow("Gaussian Blur", gaussian_blur_image);
        cv::createTrackbar("Win size", "Gaussian Blur", nullptr, 100, [](int pos, void* data){gaussian_blur_ksize = pos;});

        // median blur
        static int median_blur_ksize = 5;
        if(median_blur_ksize%2==0) median_blur_ksize++;
        cv::Mat median_blur_image;
        cv::medianBlur(image, median_blur_image, median_blur_ksize);
        cv::imshow("Median Blur", median_blur_image);
        cv::createTrackbar("Win size", "Median Blur", nullptr, 100, [](int pos, void* data){median_blur_ksize = pos;});


        // bilateral filter
        static int bilateral_filter_ksize = 5;
        if(bilateral_filter_ksize==0) bilateral_filter_ksize = 1;
        cv::Mat bilateral_filter_image;
        cv::bilateralFilter(image, bilateral_filter_image, bilateral_filter_ksize, 75, 75);
        cv::imshow("Bilateral Filter", bilateral_filter_image);
        cv::createTrackbar("Win size", "Bilateral Filter", nullptr, 100, [](int pos, void* data){bilateral_filter_ksize = pos;});

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
