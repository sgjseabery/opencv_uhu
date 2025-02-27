#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>

int main(int argc, char** argv) {

    if(argc<2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image>" << std::endl;
        return -1;
    }

    // Read image
    cv::Mat image = cv::imread(argv[1]);
    if(image.empty())
    {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    
    cv::QRCodeDetector qrDecoder;
    std::string decodedText = qrDecoder.detectAndDecode(image);

    if (!decodedText.empty()) {
        std::cout << "QR Text: " << decodedText << std::endl;
    }

    cv::imshow("QR Code", image);
    cv::waitKey(0);

    return 0;
}
