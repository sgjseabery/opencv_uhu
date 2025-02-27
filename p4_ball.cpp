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
        cv::resize(image, image, image.size()/2);
        cv::imshow("Original Video", image);

        // bilateral filter
        cv::Mat bilateral_image;
        cv::bilateralFilter(image, bilateral_image, 9, 75, 75);
        cv::imshow("Bilateral filter", bilateral_image);

        // hsv image
        cv::Mat hsv_image;
        cv::cvtColor(bilateral_image, hsv_image, cv::COLOR_BGR2HSV);

        // show 3 channels
        std::vector<cv::Mat> hsv_channels;
        cv::split(hsv_image, hsv_channels);
        cv::imshow("Hue Channel", hsv_channels[0]);
        cv::imshow("Saturation Channel", hsv_channels[1]);
        cv::imshow("Value Channel", hsv_channels[2]);

        cv::Mat thresh_ball;
        static int h0=30, h1=85, s0=65, s1=255, v0=0, v1=255;
        cv::inRange(hsv_image, cv::Scalar(h0,s0,v0), cv::Scalar(h1,s1,v1), thresh_ball);
        //cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)), cv::Point(-1,-1), 1);
        //cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)), cv::Point(-1,-1), 1);

        cv::imshow("Ball Segmentation", thresh_ball);
        cv::createTrackbar("Hue low", "Ball Segmentation", nullptr , 180, [](int pos, void* data){h0 = pos;});
        cv::createTrackbar("Hue high", "Ball Segmentation", nullptr, 180, [](int pos, void* data){h1 = pos;});
        cv::createTrackbar("Saturation low", "Ball Segmentation", nullptr, 255, [](int pos, void* data){s0 = pos;});
        cv::createTrackbar("Saturation high", "Ball Segmentation", nullptr, 255, [](int pos, void* data){s1 = pos;});
        cv::createTrackbar("Value low", "Ball Segmentation", nullptr, 255, [](int pos, void* data){v0 = pos;});
        cv::createTrackbar("Value high", "Ball Segmentation", nullptr, 255, [](int pos, void* data){v1 = pos;});

        // find contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresh_ball, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        

        bool ball_found = false;
        cv::Point2f ball_center;
        float ball_radious = 0;        

        for(const auto &contour : contours)
        {
            if(contour.size() < 25)
                continue;

            // check if contour close to circle
            cv::Rect bounding_box = cv::boundingRect(contour);
            if(bounding_box.width < bounding_box.height*0.8 || bounding_box.width > bounding_box.height*1.2)
                continue;

            cv::Point2f current_ball_center;
            float current_ball_radious;        
            cv::minEnclosingCircle(contour, current_ball_center, current_ball_radious);

            if(current_ball_radious > ball_radious)
            {
                ball_center = current_ball_center;
                ball_radious = current_ball_radious;
                ball_found = true;
            }
        }

        cv::Mat image_ball = image.clone();
        if(ball_found)
        {
            cv::circle(image_ball, ball_center, ball_radious, cv::Scalar(255,0,0), 2);
            cv::circle(image_ball, ball_center, 5, cv::Scalar(0,0,255), -1);
        }
        cv::imshow("Ball", image_ball);

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
