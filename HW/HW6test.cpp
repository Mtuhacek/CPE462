// imgproc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Usage: imaproc in_file_name.jpg out_file_name.jpg
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;


int main(int argc, char* argv[])
//int main(int argc)
{
	FILE* in, * out;
	int   j, k, width, height;
	int** image_in, ** image_out;
	float sum1, sum2;
	float new_T, old_T, delta_T;
	long count1, count2;
	errno_t err;

	/****************************/
	/* Some OpenCV here */
	/********************/

	Mat M_in = imread(argv[1]);
	//Mat M_in = imread("C:\\Users\\marco\\source\\repos\\DisplayImage1\\goldhill.jpg");
	Mat_<uchar> M_in_g(M_in.rows, M_in.cols);
	cvtColor(M_in, M_in_g, COLOR_BGR2GRAY);
	//cout << "height" << M_in_g.rows << endl;

	String windowNameOriginal = "Input Image Original"; //Name of the window
	namedWindow(windowNameOriginal); // Create a window
	imshow(windowNameOriginal, M_in); // Show our image inside the created window.
	waitKey(0); // Wait for any keystroke in the window
	//destroyWindow(windowName0); //destroy the created window

	String windowNameGrayscale = "Image Grayscale"; //Name of the window
	namedWindow(windowNameGrayscale); // Create a window
	imshow(windowNameGrayscale, M_in_g); // Show our image inside the created window.
	waitKey(0); // Wait for any keystroke in the window
	//destroyWindow(windowNameGrayscale); //destroy the created window

	height = M_in_g.rows;
	width = M_in_g.cols;

	/********************************************************************/

	image_in = (int**)calloc(height, sizeof(int*));
	if (!image_in)
	{
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	image_out = (int**)calloc(height, sizeof(int*));
	if (!image_out)
	{
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	for (j = 0; j < height; j++)
	{
		image_in[j] = (int*)calloc(width, sizeof(int));
		if (!image_in[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

		image_out[j] = (int*)calloc(width, sizeof(int));
		if (!image_out[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

	}

	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
			image_in[j][k] = M_in_g(j, k);


	/********************************************************************/
	/* Image Processing begins                                          */
	/* Histogram Equalization                                           */
	/********************************************************************/

	//create intensity histogram
	float histogram[256] = { 0.0 };
	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
			histogram[image_in[j][k]]++;

	//create cumulative frequency histogram
	float equalized_lookup[256] = { 0.0 };
	int curr = 0;
	int total = width * height;
	for (j = 0; j < 256; j++) {
		curr += histogram[j];
		equalized_lookup[j] = round((((float)curr) * 255) / total);
	}

	//equalize the image
	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
			image_out[j][k] = (int)equalized_lookup[image_in[j][k]];

	//display equalized image
	Mat_<uchar> M_out(height, width);
	for (int ii = 0; ii < height; ii++)
		for (int jj = 0; jj < width; jj++)
			M_out(ii, jj) = image_out[ii][jj];
	String windowNameEqualized = "Image Equalized";
	namedWindow(windowNameEqualized);
	imshow(windowNameEqualized, M_out);
	waitKey(0);
	//destroyWindow(windowNameEqualized);

	/********************************************************************/
	/* Image Processing ends                                            */
	/* Histogram Equalization                                           */
	/********************************************************************/

	/********************************************************************/
	/* Image Processing begins                                          */
	/* Masking                                                          */
	/********************************************************************/
	float mask[3][3] = { {-1.0, -1.0, -1.0},  {-1.0, 9.0, -1.0},  {-1.0, -1.0, -1.0} };
	float value_out = 0.0;
	int m, n;
	float sum;

	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			if (j == 0 || k == 0 || j == height - 1 || k == width - 1)
				image_out[j][k] = image_in[j][k];
			else
			{
				sum = 0.0;
				for (m = 0; m < 3; m++)
					for (n = 0; n < 3; n++)
					{
						sum += ((float)image_in[j + m - 1][k + n - 1]) * mask[m][n];
					}
				value_out = sum;
				value_out = (value_out > 255 ? 255 : value_out);
				value_out = (value_out < 0 ? 0 : value_out);
				image_out[j][k] = (int)value_out;
			}
		}

	//display masked image
	Mat_<uchar> M_out2(height, width);
	for (int ii = 0; ii < height; ii++)
		for (int jj = 0; jj < width; jj++)
			M_out2(ii, jj) = image_out[ii][jj];
	String windowNameMasked = "Image Masked";
	namedWindow(windowNameMasked);
	imshow(windowNameMasked, M_out2);
	waitKey(0);

	/********************************************************************/
	/* Image Processing ends                                            */
	/* Masking                                                          */
	/********************************************************************/

	//destroy all windows

	//destroyWindow(windowNameMasked);



	bool isSuccess = imwrite(argv[2], M_out); //write the image to a file as JPEG
	//bool isSuccess = imwrite("outfile.jpg", M_out); //write the image to a file as JPEG
	//bool isSuccess = imwrite("MyOutputImage.png", M_out); //write the image to a file as PNG
	if (isSuccess == false)
	{
		cout << "Failed to save the image" << endl;
		//cin.get(); //wait for a key press
		return -1;
	}


	/***********************/

	for (j = 0; j < height; j++)
	{
		free(image_in[j]);
		free(image_out[j]);
	}
	free(image_in);
	free(image_out);

	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file