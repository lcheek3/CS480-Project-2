
//CS480
//Project 1
//Motion Tracking with OpenCV
//Lucas, Laurin, and Tom
//10/21/15

//TUTORIALS
/*
http://www.pyimagesearch.com/2015/05/25/basic-motion-detection-and-tracking-with-python-and-opencv/

http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html

https://dl.dropboxusercontent.com/u/28096936/tuts/motionTrackingTut/finalCode/motionTracking.cpp
*/




#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include "trackerFunctions.h"
#include "SerialClass.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

int main() {
	//object xy
	cv::Point targ;

	//variables for target path smoothing and rate governor
	cv::Point current;
	cv::Point destination;
	std::vector<cv::Point> samplePoints;

	//variable for trail data
	std::vector<cv::Point> trailPoints;

	std::vector<cv::Point> targetPoints;

	std::map<std::string, int> configuration = getConfiguration();
	if (!configurationAutoReplace(configuration)) {
		configuration = getConfiguration();
	}
	for (auto it = configuration.cbegin(); it != configuration.end(); ++it) {
		std::cout << it->first << " : " << it->second << std::endl;
	}
	//Lucas' Target code

	//some boolean variables for added functionality
	bool objectDetected = false;
	//these two can be toggled by pressing 'd' or 't'
	bool debugMode = false;
	bool trackingEnabled = false;
	//pause and resume code
	bool pause = false;
	//set up the matrices that we will need
	//the two frames we will be comparing
	cv::Mat frame1, frame2;
	//their grayscale images (needed for absdiff() function)
	cv::Mat grayImage1, grayImage2;
	//resulting difference image
	cv::Mat differenceImage;
	//thresholded difference image (for use in findContours() function)
	cv::Mat thresholdImage;
	//video capture object.
	//VideoCapture capture;
	
	cv::VideoCapture stream1(configuration["CAMERA"]);
	if (!stream1.isOpened()) {
		std::cout << "Cannot open camera." << std::endl;
	}
	std::stringstream comss;
	comss << "\\\\.\\COM" << configuration["COM"];
	char* comNum = _strdup((comss.str()).c_str());
	Serial* turret = new Serial(comNum);    // adjust as needed

	if (turret->IsConnected())
		printf("Connection Success, ready for tracking.\n");

	while (1) {
		char incomingData[256] = "";
		int dataLength = 255;
		int readResult = 0;
		readResult = turret->ReadData(incomingData, dataLength);
		//printf("Bytes read: (0 means no data available) %i\n", readResult);
		incomingData[readResult] = 0;
		//read first frame
		stream1.read(frame1);
		//convert frame1 to gray scale for frame differencing
		cvtColor(frame1, grayImage1, cv::COLOR_BGR2GRAY);
		//copy second frame
		stream1.read(frame2);
		//convert frame2 to gray scale for frame differencing
		cvtColor(frame2, grayImage2, cv::COLOR_BGR2GRAY);
		//perform frame differencing with the sequential images. This will output an "intensity image"
		//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
		absdiff(grayImage1, grayImage2, differenceImage);
		//threshold intensity image at a given sensitivity value
		threshold(differenceImage, thresholdImage, configuration["SENSITIVITY_VALUE"], 255, cv::THRESH_BINARY);
		if (debugMode == true) {
			//show the difference image and threshold image
			imshow("Difference Image", differenceImage);
			imshow("Threshold Image", thresholdImage);
		}
		//blacking out goes here
		else {
			//if not in debug mode, destroy the windows
			cv::destroyWindow("Difference Image");
			cv::destroyWindow("Threshold Image");
		}
		//blur() to smooth the image, remove noise
		blur(thresholdImage, thresholdImage, cv::Size(configuration["BLUR_SIZE"], configuration["BLUR_SIZE"]));
		//threshold again to obtain binary image from blur output
		threshold(thresholdImage, thresholdImage, configuration["SENSITIVITY_VALUE"], 255, cv::THRESH_BINARY);
		//verifies that image is 8 bit for findcontours()
		thresholdImage.convertTo(thresholdImage, CV_8U);
		if (debugMode == true) {
			//show the threshold image after it's been "blurred"
			imshow("Final Threshold Image", thresholdImage);
		}
		else {
			cv::destroyWindow("Final Threshold Image");
		}
		//if tracking enabled, search for contours in our thresholded image
		if (trackingEnabled) {
			//Collects a number of sample averages specified by SMOOTHING_SAMPLE and sticks them in the samplePoints global vector
			collectSamples(thresholdImage, frame1, samplePoints, targ, configuration);
		}
		//when samplePoints vector reaches the size specified by SMOOTHING_SAMPLE, updates the current point and clears samplePoints
		if (samplePoints.size() == configuration["SMOOTHING_SAMPLE"]) {
			destination = pathSmoothing(frame1, samplePoints, configuration);
			samplePoints.clear();
		}
		
		if (trackingEnabled) {
			//drawTrails(frame1, current, targetPoints, configuration);
			std::vector<int> coordinates = getCoordinates(destination, configuration);
			std::stringstream coord_stream; 
			coord_stream <<'A' << coordinates[0] << ' ' << coordinates[1] << '\n';
			std::string turret_coordstr = coord_stream.str();
			std::cout << turret_coordstr;
			char* turret_coords = _strdup(turret_coordstr.c_str());
			turret->WriteData(turret_coords, turret_coordstr.size());
			free(turret_coords);
		}


		//limits the speed of movement of the target crosshair. We can tweak SPEED_OF_MOVEMENT to accurately reflect the actual position of the laser in project 2
		//so that we can draw an obscuring circle over the laser
		current = destination;
		//cv::Point predictLine = speedGovernor(current, destination, 100);

		//draw the target
		drawTarget(current, frame1);
		//line(frame1, current, predictLine, cv::Scalar(0, 255, 0), 2);
		//line(frame1, cv::Point(configuration["CAM_RES_X"]/2 - 5, configuration["CAM_RES_Y"] / 2), cv::Point(configuration["CAM_RES_X"]/2 + 5, configuration["CAM_RES_Y"] / 2), cv::Scalar(0, 255, 0), 2);
		//line(frame1, cv::Point(configuration["CAM_RES_X"] / 2, configuration["CAM_RES_Y"] / 2 - 5), cv::Point(configuration["CAM_RES_X"] / 2, configuration["CAM_RES_Y"] / 2 + 5), cv::Scalar(0, 255, 0), 2);
		imshow("Frame1", frame1);

		//hotkey code from tutorial at (https://dl.dropboxusercontent.com/u/28096936/tuts/motionTrackingTut/finalCode/motionTracking.cpp)
		switch (cv::waitKey(10)) {
		case 27: //'esc' key has been pressed, exit program.
			return 0;
		case 116: //'t' has been pressed. this will toggle tracking
			trackingEnabled = !trackingEnabled;
			if (trackingEnabled == false) std::cout << "Tracking disabled." << std::endl;
			else std::cout << "Tracking enabled." << std::endl;
			break;
		case 100: //'d' has been pressed. this will debug mode
			debugMode = !debugMode;
			if (debugMode == false) std::cout << "Debug mode disabled." << std::endl;
			else std::cout << "Debug mode enabled." << std::endl;
			break;
		case 112: //'p' has been pressed. this will pause/resume the code.
			pause = !pause;
			if (pause == true) {
				std::cout << "Code paused, press 'p' again to resume" << std::endl;
				while (pause == true) {
					//stay in this loop until 
					switch (cv::waitKey()) {
					case 112:
						//change pause back to false
						pause = false;
						std::cout << "Code resumed." << std::endl;
						break;
					}
				}
			}

		}
	}
	return 0;
}