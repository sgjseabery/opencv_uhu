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

    do
    {

        // range of colors HSV
        cv::Mat thresh_color;
        static int h0=0, h1=255, s0=15, s1=255, v0=0, v1=255;
        cv::inRange(hsv_image, cv::Scalar(h0,s0,v0), cv::Scalar(h1,s1,v1), thresh_color);
    
        cv::imshow("Range HSV", thresh_color);

        cv::createTrackbar("Hue low", "Range HSV", nullptr, 180, [](int pos, void* data){h0 = pos;});
        cv::createTrackbar("Hue high", "Range HSV", nullptr, 180, [](int pos, void* data){h1 = pos;});
        cv::createTrackbar("Saturation low", "Range HSV", nullptr, 255, [](int pos, void* data){s0 = pos;});
        cv::createTrackbar("Saturation high", "Range HSV", nullptr, 255, [](int pos, void* data){s1 = pos;});
        cv::createTrackbar("Value low", "Range HSV", nullptr, 255, [](int pos, void* data){v0 = pos;});
        cv::createTrackbar("Value high", "Range HSV", nullptr, 255, [](int pos, void* data){v1 = pos;});

        // detect contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresh_color, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

        cv::Mat contours_image = image.clone();
        cv::drawContours(contours_image, contours, -1, cv::Scalar(0,0,0), 2);
        cv::imshow("All contours", contours_image);

        // look for squares
        cv::Mat contours_squares = image.clone();
        const int n_sides = 4;
        for(size_t i=0; i<contours.size(); i++)
        {
            const auto &contour = contours[i];

            if(contour.size()<20) continue;

            std::vector<cv::Point> approx;
            cv::approxPolyDP(contour, approx, contour.size()*0.04, true);
            if(approx.size()==n_sides)
            {
                //cv::drawContours(contours_squares, contours, i, cv::Scalar(0,255,0), 2);
                
                std::vector<std::vector<cv::Point>> contours_poly = {approx};
                cv::drawContours(contours_squares, contours_poly, 0, cv::Scalar(0,0,0), 2);
            }
        }
        cv::imshow("Contours Squares", contours_squares);

    }while(cv::waitKey(10)!=27);
    

    return 0;
}
