//
//  MotionTracking.cpp
//  PortalTurret
//
//  Created by Laurin Fisher on 11/4/15.
//  Copyright © 2015 Laurin Fisher. All rights reserved.
//

#include <stdio.h>


//Lucas, Laurin, Tom
//Motion Tracking using OpenCV

//TUTORIALS
/*
 http://www.pyimagesearch.com/2015/05/25/basic-motion-detection-and-tracking-with-python-and-opencv/
 
 http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html
 */

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using std::abs;
using std::sqrt;
using std::pow;
using std::atan2;
using std::sin;
using std::cos;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;
using namespace cv;

//sensitivity value to be used in the threshold() function
const static int SENSITIVITY_VALUE = 30;
//size of blur used to smooth the image to remove possible noise
const static int BLUR_SIZE = 30;
//camera resolution values used to weed out extraneous x and y target positions (set to large value by default. Fine tune by setting to your camera resolution)
const static int CAM_RES_X = 15000;
const static int CAM_RES_Y = 15000;
//number of points to use to average direction of motion
const static int SMOOTHING_SAMPLE = 3;
//
const static double SPEED_OF_MOVEMENT = 20;
//
const static int SIZE_OF_TRAIL = 10;
//object xy
Point targ;

//variables for target path smoothing and rate governor
Point current;
Point destination;
vector<Point> samplePoints;

//variable for trail data
vector<Point> trailPoints;

vector<Point> targetPoints;
const int TRAILS = 30;

//function takes the populated samplePoints vector, gets the average of all points in that vector, and returns the averaged point
Point pathSmoothing(Mat &cameraFeed) {
    Point temp;
    int x = 0;
    int y = 0;
    for (int i = 0; i < SMOOTHING_SAMPLE; i++) {
        x += samplePoints[i].x;
        y += samplePoints[i].y;
    }
    temp.x = x / SMOOTHING_SAMPLE;
    temp.y = y / SMOOTHING_SAMPLE;
    return temp;
}

//draws the line trail
void drawTrail() {
}
//draws the target at given point
void drawTarget(Point target, Mat &cameraFeed) {
    int x = target.x;
    int y = target.y;
    line(cameraFeed, Point(x, y + 25), Point(x, y - 25), Scalar(0, 0, 0), 2);
    line(cameraFeed, Point(x + 25, y), Point(x - 25, y), Scalar(0, 0, 0), 2);
    putText(cameraFeed, "Tracking object at (" + to_string(x) + "," + to_string(y) + ")", Point(x - 100, y - 30), 1, 1, Scalar(0, 0, 255), 2);
}

//uses some trigonometry magic to update the position of the current point based on a limited speed given by SPEED_OF_MOVEMENT
Point speedGovernor(Point current, Point destination, double dist) {
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

void collectSamples(Mat thresholdImage, Mat &cameraFeed) {
    bool objectDetected = false;
    Mat temp;
    thresholdImage.copyTo(temp);
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    //find contours of processed image (areas of motion)
    findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);// retrieves external contours
    
    vector<Moments> contour_moments(contours.size());
    vector<Point> mass_centers(contours.size());
    
    //gets centroids of contours
    for (int i = 0; i < contours.size(); i++) {
        contour_moments[i] = moments(contours[i], false);
        mass_centers[i] = Point(contour_moments[i].m10 / contour_moments[i].m00, contour_moments[i].m01 / contour_moments[i].m00);
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
        if (!(abs(xpos) > CAM_RES_X || abs(ypos) > CAM_RES_Y)) {
            targ.x = xpos / contours.size(), targ.y = ypos / contours.size();
        }
        cout << targ << endl;
    }
    
    samplePoints.push_back(targ);
    
    //simple drawing showing a visualisation of target
    drawContours(cameraFeed, contours, -1, Scalar(255, 255, 255), 1);
    
    
}