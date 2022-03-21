#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
    //Mat Image = imread("D:/Marko_Files/CPE462/projects/broganek.png", IMREAD_UNCHANGED);
    Mat Image = imread("D:/Marko_Files/CPE462/projects/broganek.png");


    if (Image.empty())
    {
        cout << "Erro" << endl;

        return 0;
    }

    int down_width = 400;
    int down_height = 300;
    Mat resized_down;
   // resize(Image, resized_down, Size(down_width, down_height), INTER_LINEAR);

   // namedWindow("NewWindow");
   // imshow("NewWindow", Image);
    resize(Image, resized_down, Size(down_width, down_height), INTER_LINEAR);
    imshow("Resized Down by defining height and width", resized_down);
    waitKey(0);

   // destroyWindow("MyWindow");
    destroyAllWindows();

    return 0;
}

