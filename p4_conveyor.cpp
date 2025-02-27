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
        cv::imshow("Bilateral filter", bilateral_image);

        // treshold
        cv::Mat threshold_image;
        static int thresh = 128;
        cv::threshold(bilateral_image, threshold_image, thresh, 255, cv::THRESH_BINARY);
        cv::imshow("Threshold", threshold_image);
        cv::createTrackbar("Threshold", "Threshold", nullptr, 255, [](int pos, void* data){thresh = pos;});

        // find contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(threshold_image, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
        cv::Mat contours_image = image.clone();
        

        // for each contour, detect triangles or squares
        for(size_t i=0; i<contours.size(); i++)
        {
            const auto &contour = contours[i];

            if(contour.size()<20) continue;
            
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contour, approx, 0.2*contour.size(), true);

            // if has a point in image border, skip
            if(std::any_of(approx.begin(), approx.end(), [&threshold_image](const cv::Point &p){return p.x==0 || p.y==0 || p.x==threshold_image.cols-1 || p.y==threshold_image.rows-1;}))
                continue;

            if(approx.size()==3)
            {
                std::vector<std::vector<cv::Point>> contours_poly = {approx};
                cv::drawContours(contours_image, contours_poly, 0, cv::Scalar(0,0,255), 3);
            }
            else if(approx.size()==4)
            {
                std::vector<std::vector<cv::Point>> contours_poly = {approx};
                cv::drawContours(contours_image, contours_poly, 0, cv::Scalar(255,0,0), 3);
            }
        }
        
        cv::imshow("Contours", contours_image);


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
