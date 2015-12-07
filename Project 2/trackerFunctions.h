#ifndef TRACKERFUNCTIONS_H_INCLUDED
#define TRACKERFUNCTIONS_H_INCLUDED

//Helper functions for motion tracker

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//42mm 90mm
//50 degrees cone of vision (25 degrees for half)
std::vector<int> getCoordinates(cv::Point destination, std::map<std::string, int> configuration) {
	double parallax_unit = (tan(0.43)*configuration["PARALLAX"]) / (configuration["CAM_RES_X"] / 2);
	int dist_cam_turr = 65; //in mm
	double vertical_offset = (atan2(dist_cam_turr,configuration["PARALLAX"]))*180/CV_PI;
	std::cout << vertical_offset << std::endl;
	int x_command = -atan2((destination.x - configuration["CAM_RES_X"] / 2)*parallax_unit , configuration["PARALLAX"])*180/CV_PI+90;
	int y_command = atan2((destination.y - configuration["CAM_RES_Y"] / 2)*parallax_unit , configuration["PARALLAX"])*180/CV_PI+90+vertical_offset;
	std::vector<int> turr_coords;
	turr_coords.push_back(x_command);
	turr_coords.push_back(y_command);
	return turr_coords;
}

//Replaces any missing configuration settings with default values
bool configurationAutoReplace(std::map<std::string, int> configuration) {
	bool unchanged = true;
	std::fstream openFile("settings.conf", std::ios::out | std::ios::app);
	if (!openFile.is_open()) {
		std::cout << "could not find settings.conf" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		exit(1);
	}
	if (!configuration["SENSITIVITY_VALUE"]) {
		openFile << "SENSITIVITY_VALUE = 30\n";
		unchanged = false;
	}
	if (!configuration["BLUR_SIZE"]) {
		openFile << "BLUR_SIZE = 30;\n";
		unchanged = false;
	}
	if (!configuration["SMOOTHING_SAMPLE"]) {
		openFile << "SMOOTHING_SAMPLE = 3\n";
		unchanged = false;
	}
	if (!configuration["SPEED_OF_MOVEMENT"]) {
		openFile << "SPEED_OF_MOVEMENT = 20\n";
		unchanged = false;
	}
	if (!configuration["TRAILS"]) {
		openFile << "TRAILS = 10\n";
		unchanged = false;
	}
	if (!configuration["CAM_RES_X"]) {
		openFile << "CAM_RES_X = 640\n";
		unchanged = false;
	}
	if (!configuration["CAM_RES_Y"]) {
		openFile << "CAM_RES_Y = 480\n";
		unchanged = false;
	}
	if (!configuration["PARALLAX"]) {
		openFile << "PARALLAX = 914\n"; //mm
		unchanged = false;
	}
	if (!configuration["COM"]) {
		openFile << "COM = 5\n";
		unchanged = false;
	}
	if (!configuration["CAMERA"]) {
		openFile << "CAMERA = 0\n";
		unchanged = false;
	}
	return unchanged;

}

//reads the configuration file and creates a name -> value mapping
std::map<std::string, int> getConfiguration() {
	std::map<std::string, int> configuration;
	std::fstream openFile("settings.conf", std::ios::in);
	if (!openFile.is_open()) {
		std::cout << "could not find settings.conf" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		exit(1);
	}
	std::string line;
	bool error = false;
	for (std::string line; std::getline(openFile, line); )
	{
		std::istringstream iss(line);
		std::string id, eq, val;
		bool error = false;
		if (!(iss >> id))
		{
			error = true;
		}
		else if (id[0] == '#')
		{
			continue;
		}
		else if (!(iss >> eq >> val >> std::ws) || eq != "=" || iss.get() != EOF)
		{
			error = true;
		}
		if (error)
		{
			std::cout << "settings.conf parse error! Please follow directions in file" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			exit(1);
		}
		else
		{
			configuration[id] = std::stoi(val);
		}
	}
	return configuration;
}


//takes the populated samplePoints vector, gets the average of all points in that vector, and returns the averaged point
cv::Point pathSmoothing(cv::Mat &cameraFeed, std::vector<cv::Point> samplePoints, std::map<std::string, int> configuration) {
	cv::Point temp;
	int x = 0;
	int y = 0;
	for (int i = 0; i < configuration["SMOOTHING_SAMPLE"]; i++) {
		x += samplePoints[i].x;
		y += samplePoints[i].y;
	}
	temp.x = x / configuration["SMOOTHING_SAMPLE"];
	temp.y = y / configuration["SMOOTHING_SAMPLE"];
	return temp;
}


//draws the target reticle at given point
void drawTarget(cv::Point target, cv::Mat &cameraFeed) {
	int x = target.x;
	int y = target.y;
	cv::line(cameraFeed, cv::Point(x, y + 25), cv::Point(x, y - 25), cv::Scalar(200, 255, 0), 2);
	line(cameraFeed, cv::Point(x + 25, y), cv::Point(x - 25, y), cv::Scalar(200, 255, 0), 2);
	putText(cameraFeed, "(" + std::to_string(x) + "," + std::to_string(y) + ")", cv::Point(x - 38, y - 35), 1, 1, cv::Scalar(255, 100, 0), 2);
}

//uses some trigonometry magic to update the position of the current point based on a limited speed given by SPEED_OF_MOVEMENT
cv::Point speedGovernor(cv::Point current, cv::Point destination, double dist) {
	double x_dist = destination.x - current.x;
	double y_dist = destination.y - current.y;
	double distance = sqrt(pow((x_dist), 2) + pow((y_dist), 2));
	double angle = atan2(y_dist, x_dist);
	if (distance > dist) {
		distance = dist;
		x_dist = distance*(cos(angle));
		y_dist = distance*(sin(angle));
	}
	current.x = current.x + x_dist;
	current.y = current.y + y_dist;
	return current;
}

//collects a set of destination points to be be averaged
void collectSamples(cv::Mat thresholdImage, cv::Mat &cameraFeed, std::vector<cv::Point> &samplePoints, cv::Point &targ, std::map<std::string, int> configuration) {
	bool objectDetected = false;
	cv::Mat temp;
	thresholdImage.copyTo(temp);
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	//find contours of processed image (areas of motion)
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);// retrieves external contours

	std::vector<cv::Moments> contour_moments(contours.size());
	std::vector<cv::Point> mass_centers(contours.size());

	//gets centroids of contours
	for (int i = 0; i < contours.size(); i++) {
		contour_moments[i] = moments(contours[i], false);
		mass_centers[i] = cv::Point(contour_moments[i].m10 / contour_moments[i].m00, contour_moments[i].m01 / contour_moments[i].m00);
	}

	if (contours.size()>0)objectDetected = true;
	else objectDetected = false;

	//only finds averages of centroids if contours exist - prevents target from being set to 0,0 if no contours
	if (objectDetected) {
		double xpos = 0;
		double ypos = 0;
		for (int i = 0; i < contours.size(); i++) {
			xpos += mass_centers[i].x;
			ypos += mass_centers[i].y;
		}
		//find the average of the centroids of all contours. This could be improved by using K-means or some other kind of clustering algorithm
		if (!(abs(xpos) > 10000 || abs(ypos) > 10000)) {
			targ.x = xpos / contours.size(), targ.y = ypos / contours.size();
		}
		//std::cout << targ << std::endl;
	}

	samplePoints.push_back(targ);

	//simple drawing showing a visualisation of target
	drawContours(cameraFeed, contours, -1, cv::Scalar(255, 255, 255), 1);

}

void drawTrails(cv::Mat &cameraFeed, cv::Point current, std::vector<cv::Point> &targetPoints,std::map<std::string,int> configuration) {
	targetPoints.push_back(current);
	std::vector<cv::Point> temp;
	int counter = 0;

	//Draw Past motion of target//
	//if vector target only has one point, skip over it
	if (targetPoints.size() > 1) {
		if (configuration["TRAILS"] >= targetPoints.size()) {
			counter = targetPoints.size();
			//target size hasn't gotten that many trails yet
			//Loop through past points and draw last 3 lines of motion
			for (int i = 0; i + 1<counter - 1; i++) {
				line(cameraFeed, targetPoints[i], targetPoints[i + 1], (0, 0, i * 10), 2);
			}
		}
		else {
			counter = configuration["TRAILS"];

			//Loop through past points and draw last lines of motion
			for (int i = 0; i + 1<counter; i++) {
				line(cameraFeed, targetPoints[targetPoints.size() - 1 - i], targetPoints[targetPoints.size() - 2 - i], (0, 0, 255 - i * 20), 2);
				targetPoints[i] = targetPoints[i + 1];
			}
		}
	}
}

#endif