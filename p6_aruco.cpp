#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

void drawCube(cv::Mat &image, const cv::Mat &camera_matrix, const cv::Mat &distortion_coefficients, const cv::Vec3d &rvec, const cv::Vec3d &tvec, double size)
{
    const std::vector<cv::Point3f> object_points = {
        cv::Point3f(-size/2, -size/2, 0),
        cv::Point3f(size/2, -size/2, 0),
        cv::Point3f(size/2, size/2, 0),
        cv::Point3f(-size/2, size/2, 0),
        cv::Point3f(-size/2, -size/2, size),
        cv::Point3f(size/2, -size/2, size),
        cv::Point3f(size/2, size/2, size),
        cv::Point3f(-size/2, size/2, size)
    };

    std::vector<cv::Point2f> image_points;
    cv::projectPoints(object_points, rvec, tvec, camera_matrix, distortion_coefficients, image_points);

    cv::line(image, image_points[0], image_points[1], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[1], image_points[2], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[2], image_points[3], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[3], image_points[0], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[4], image_points[5], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[5], image_points[6], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[6], image_points[7], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[7], image_points[4], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[0], image_points[4], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[1], image_points[5], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[2], image_points[6], cv::Scalar(0, 0, 255), 2);
    cv::line(image, image_points[3], image_points[7], cv::Scalar(0, 0, 255), 2);


}


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

    //cv::Mat camera_matrix = (cv::Mat1d(3,3) << 628.158, 0., 302.766, 0., 651.405, 238.713, 0., 0., 1.);
    //cv::Mat distortion_coefficients = (cv::Mat1d(5,1) << -0.418959, 0.170076, 7.49474e-05, -0.0010356, 0);

    cv::Mat camera_matrix = (cv::Mat1d(3,3) << 1.5116477324826665e+03, 0., 5.5324977732115144e+02, 0., 1.5044961231294651e+03, 2.2774035388304131e+02, 0., 0., 1.);
    cv::Mat distortion_coefficients = (cv::Mat1d(5,1) << 8.5849274523860070e-02, 1.0582173007708182e+00, -2.7890142610677137e-02, -2.3659683844196044e-02, -5.8190585616542938e+00);    

    const cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
    cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
    detectorParams.cornerRefinementMethod = cv::aruco::CORNER_REFINE_CONTOUR;
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);    

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

        // detect aruco markers
        std::vector<int> marker_ids;
        std::vector<std::vector<cv::Point2f>> marker_corners;
        detector.detectMarkers(image, marker_corners, marker_ids);

        cv::aruco::drawDetectedMarkers(image, marker_corners, marker_ids);

        // estimate pose of each marker
        std::vector<cv::Vec3d> rvecs, tvecs;
        cv::aruco::estimatePoseSingleMarkers(marker_corners, 0.05, camera_matrix, distortion_coefficients, rvecs, tvecs);
        
        // draw axis for each marker
        for(int i=0; i<marker_ids.size(); i++)
        {
            cv::drawFrameAxes(image, camera_matrix, distortion_coefficients, rvecs[i], tvecs[i], 0.03);
            drawCube(image, camera_matrix, distortion_coefficients, rvecs[i], tvecs[i], 0.05);
        }


        cv::imshow("Detected Markers", image);

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
