#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>
#include <iostream>

int main(int argc, char** argv) {

    if(argc<4)
    {
        std::cerr << "Usage: " << argv[0] << " <input_video> <model> <face_image> <face_alpha>" << std::endl;
        return -1;
    }

    // Open the video
    cv::VideoCapture capturer(argv[1]);
    if (!capturer.isOpened())
    {
        std::cerr << "Error opening the video!" << std::endl;
        return -1;
    }

    cv::Ptr<cv::FaceDetectorYN> detector = cv::FaceDetectorYN::create(argv[2], "", cv::Size(320, 320));

    // load face image
    cv::Mat dog_face_raw = cv::imread(argv[3]);
    if(dog_face_raw.empty())
    {
        std::cerr << "Could not open or find the face image!" << std::endl;
        return -1;
    }

    // load alpha image
    cv::Mat dog_face_alpha = cv::imread(argv[4]);
    if(dog_face_alpha.empty())
    {
        std::cerr << "Could not open or find the alpha image!" << std::endl;
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

        cv::imshow("Original", image);

        // detect faces
        detector->setInputSize(image.size());
        cv::Mat faces;
        detector->detect(image, faces);

        // draw faces
        for (int i = 0; i < faces.rows; i++)
        { 
            // Draw bounding box
            cv::Rect face_box = cv::Rect( int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1)), 
                                int(faces.at<float>(i, 2)), int(faces.at<float>(i, 3)));

            //cv::rectangle(image, face_box, cv::Vec3b(0, 255, 0), 2);

            // blur rectangle
            //cv::Mat roi = image(face_box);
            //cv::blur(roi, roi, cv::Size(35,35));

            // put face
            cv::Mat roi = image(face_box);
            cv::Mat dog_face_tmp, dog_face_alpha_tmp;
            cv::resize(dog_face_raw, dog_face_tmp, face_box.size());
            cv::resize(dog_face_alpha, dog_face_alpha_tmp, face_box.size());
            dog_face_tmp.copyTo(roi, dog_face_alpha_tmp);
        }

        cv::imshow("Blurred faces", image);
        

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
