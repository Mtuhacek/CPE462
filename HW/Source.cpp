#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
    Mat Image = imread("D:/Marko_Files/CPE462/projects/broganek.jpg", IMREAD_UNCHANGED);
    //Mat Image = imread(".JPG", CV_LOAD_IMAGE_UNCHANGED);

    if (Image.empty())
    {
        cout << "Erro" << endl;

        return 0;
    }

    namedWindow("NewWindow");
    imshow("NewWindow", Image);

    waitKey(0);

    destroyWindow("MyWindow");

    return 0;
}