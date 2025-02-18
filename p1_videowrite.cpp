#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

int main(int argc, char** argv) {

    if(argc<2)
    {
        std::cerr << "Usage: " << argv[0] << " <video_path>" << std::endl;
        return -1;
    }

    // Open the video
    cv::VideoCapture capturer(argv[1]);
    if (!capturer.isOpened())
    {
        std::cerr << "Error opening the video!" << std::endl;
        return -1;
    }

    // VideoWriter
    bool record_video = false;
    const cv::Size output_size = cv::Size(capturer.get(cv::CAP_PROP_FRAME_WIDTH), capturer.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter writer("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, output_size); 
    
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


        /// PROCESS IMAGE HERE
        // ...


        /// SHOW RESULT
        cv::imshow("Video", image);

        /// WRITE TO FILE
        if(record_video)
        {
            writer.write(image);
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
