#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv) {

    if(argc<2)
    {
        std::cerr << "Usage: " << argv[0] << " <video1_path> <video2_path> <video3_path> <video4_path>" << std::endl;
        return -1;
    }

    // Open the video
    cv::VideoCapture capturer1(argv[1]);
    cv::VideoCapture capturer2(argv[2]);
    cv::VideoCapture capturer3(argv[3]);
    cv::VideoCapture capturer4(argv[4]);
    if (!capturer1.isOpened() || !capturer2.isOpened() || !capturer3.isOpened() || !capturer4.isOpened())
    {
        std::cerr << "Error opening one of the videos" << std::endl;
        return -1;
    }

    // check all videos have the same size
    cv::Size size1 = cv::Size(capturer1.get(cv::CAP_PROP_FRAME_WIDTH), capturer1.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::Size size2 = cv::Size(capturer2.get(cv::CAP_PROP_FRAME_WIDTH), capturer2.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::Size size3 = cv::Size(capturer3.get(cv::CAP_PROP_FRAME_WIDTH), capturer3.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::Size size4 = cv::Size(capturer4.get(cv::CAP_PROP_FRAME_WIDTH), capturer4.get(cv::CAP_PROP_FRAME_HEIGHT));
    if(size1!=size2 || size1!=size3 || size1!=size4)
    {
        std::cerr << "All videos must have the same size" << std::endl;
        return -1;
    }

    // VideoWriter
    bool record_video = true;
    const cv::Size output_size = size1;
    cv::VideoWriter writer("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, output_size); 
    
    int wait_time = 30; // wait time in ms

    while( capturer1.grab() && capturer2.grab() && capturer3.grab() && capturer4.grab())
    {

        /// READ NEXT FRAME
        cv::Mat image1, image2, image3, image4;
        if(!capturer1.retrieve(image1) || !capturer2.retrieve(image2) || !capturer3.retrieve(image3) || !capturer4.retrieve(image4))
        {
            std::cerr << "Error reading a frame!" << std::endl;
            break;
        }

        cv::Mat image1_half, image2_half, image3_half, image4_half;
        cv::resize(image1, image1_half, cv::Size(image1.cols/2, image1.rows/2));
        cv::resize(image2, image2_half, cv::Size(image2.cols/2, image2.rows/2));
        cv::resize(image3, image3_half, cv::Size(image3.cols/2, image3.rows/2));
        cv::resize(image4, image4_half, cv::Size(image4.cols/2, image4.rows/2));

        cv::Mat collage = cv::Mat(output_size.height, output_size.width, CV_8UC3);

        cv::Rect top_left = cv::Rect(0, 0, collage.cols/2, collage.rows/2);
        cv::Rect top_right = cv::Rect(collage.cols/2, 0, collage.cols/2, collage.rows/2);
        cv::Rect bottom_left = cv::Rect(0, collage.rows/2, collage.cols/2, collage.rows/2);
        cv::Rect bottom_right = cv::Rect(collage.cols/2, collage.rows/2, collage.cols/2, image1.rows/2);

        image1_half.copyTo(collage(top_left));
        image2_half.copyTo(collage(top_right));
        image3_half.copyTo(collage(bottom_left));
        image4_half.copyTo(collage(bottom_right));

        /// SHOW RESULT
        cv::imshow("Collage", collage);

        /// WRITE TO FILE
        if(record_video)
        {
            writer.write(collage);
        }

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
