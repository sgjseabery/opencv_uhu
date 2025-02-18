#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

//using namespace cv;

int main(int argc, char** argv) {

    //////////////////////////// Crear y mostrar imagenes
    cv::Mat image_gray = cv::Mat(100, 200, CV_8UC1, 120 );
    cv::Mat image_blue = cv::Mat(100, 200, CV_8UC3, cv::Vec3b( 255, 0, 0 ) );

    cv::imshow("image_gray", image_gray);
    cv::imshow("image_blue", image_blue);

    image_gray.setTo(255);
    image_blue.setTo(cv::Vec3b(0, 255, 0));

    std::cout << "Size: " << image_blue.size() << std::endl;
    std::cout << "Rows: " << image_blue.rows << std::endl;
    std::cout << "Columns: " << image_blue.cols << std::endl;
    std::cout << "Canales: " << image_blue.channels() << std::endl;
    std::cout << "Depth: " << image_blue.depth() << std::endl;   

    cv::imshow("image_gray", image_gray);
    cv::imshow("image_green", image_blue);
    cv::waitKey(0);

    cv::destroyAllWindows();
    //////////////////////////// Leer de fichero
    cv::Mat turtle = cv::imread("clean/images/turtle.jpg");
    cv::imshow("turtle", turtle);
    cv::waitKey(0);

    //////////////////////////// Multiplicar por escalar
    cv::Mat turtle_darker = turtle * 0.5;
    cv::imshow("turtle_darker", turtle_darker);
    cv::waitKey(0);

    //////////////////////////// Multiplicar por escalar
    cv::Mat turtle_negative = 255 - turtle;
    cv::imshow("turtle_negative", turtle_negative);
    cv::waitKey(0);

    //////////////////////////// Sumar y combinar
    cv::Mat penguin = cv::imread("clean/images/penguin.jpg");
    cv::Mat fusion = turtle*0.5 + penguin*0.5;
    cv::imshow("fusion", fusion);
    cv::waitKey(0);

    cv::destroyAllWindows();
    //////////////////////////// Lectura/escritura de pixeles
    cv::Vec3b color1 = turtle.at<cv::Vec3b>(20, 20);
    cv::Vec3b color2 = turtle.ptr<cv::Vec3b>(20)[20];
    std::cout << (int)color1[0] << std::endl;
    std::cout << (int)color1[1] << std::endl;
    std::cout << (int)color1[2] << std::endl;
    std::cout << color1 << std::endl;

    turtle.at<cv::Vec3b>(20, 20) = cv::Vec3b(0,0,255);

    // for(int r=20; r<40; r++)
    // {
    //     cv::Vec3b* r_ptr = man.ptr<cv::Vec3b>(r);
    //     for(int c=30; c<50; c++)
    //     {
    //         r_ptr[c] = cv::Vec3b(0,0,255);
    //     }
    // }

    for(int r=20; r<40; r++)
    {
        for(int c=30; c<50; c++)
        {
            turtle.at<cv::Vec3b>(r,c) = cv::Vec3b(0,0,255);
        }
    }

    cv::imshow("write pixels", turtle);
    cv::waitKey(0);

    cv::destroyAllWindows();
    //////////////////////////// Copia y clonado

    cv::Mat penguin_clone = penguin;
    penguin_clone.setTo(cv::Vec3b(0,255,0));
    cv::imshow("penguin copy", penguin_clone);
    cv::imshow("penguin", penguin);
    cv::waitKey(0);

    cv::Mat turtle_copy = turtle.clone();
    turtle_copy.setTo(cv::Vec3b(0,255,0));
    cv::imshow("turtle clone", turtle_copy);
    cv::imshow("turtle", turtle);
    cv::waitKey(0);


    cv::destroyAllWindows();
    //////////////////////////// Crop
    cv::Rect rectangle(220, 340, 150, 150);
    cv::Mat turtle_face = turtle(rectangle).clone();
    cv::imshow("turtle", turtle);
    cv::imshow("turtle_face", turtle_face);
    cv::waitKey(0);


    //////////////////////////// Resize
    cv::Mat turtle_face_big;
    cv::resize(turtle_face, turtle_face_big, cv::Size(300, 300));
    cv::imshow("turtle_face_big", turtle_face_big);
    cv::waitKey(0);


    //////////////////////////// Save to file
    cv::imwrite("turtle_face_big.jpg", turtle_face_big);

    return 0;

}
