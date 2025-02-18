#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>
#include <iostream>


int main(int argc, char** argv) {

    if(argc<3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image> <model_file>" << std::endl;
        return -1;
    }

    // Read image
    cv::Mat image = cv::imread(argv[1]);
    if(image.empty())
    {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    cv::Ptr<cv::FaceDetectorYN> detector = cv::FaceDetectorYN::create(argv[2], "", cv::Size(320, 320));
    detector->setInputSize(image.size());
    cv::Mat faces;
    detector->detect(image, faces);
    if (faces.empty())
    {
        std::cerr << "Cannot find a face in image" << std::endl;
        return 1;
    }    
    for (int i = 0; i < faces.rows; i++)
    { 
        // Draw bounding box
        cv::rectangle(image, cv::Rect2i(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1)), 
                                int(faces.at<float>(i, 2)), int(faces.at<float>(i, 3))), 
                                cv::Vec3b(0, 255, 0), 2);
    }

    cv::imshow("Faces", image);
    cv::waitKey(0);

    return 0;

}
