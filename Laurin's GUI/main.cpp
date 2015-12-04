
//
//Laurin Fisher
//

//I used the SOIL library to load an image

//All the includes so it works-
#include <cstdlib>       // Do this before GL/GLUT includes
using std::exit;
#ifndef __APPLE__
# include <GL/glew.h>
# include <GL/glut.h>    // Includes OpenGL headers as well
#else
# include <GL/glew.h>
# include <GLUT/glut.h>  // Apple puts glut.h in a different place
#endif
#ifdef _MSC_VER          // Tell MS-Visual Studio about GLEW lib
# pragma comment(lib, "glew32.lib")
#endif

#include <iostream>
#include <string>
#include "lib381/bitmapprinter.h" //to print text to screen
#include "libSOIL/SOIL.h"
//credit to Glenn Chappell
#include "lib381/glslprog.h"//for shaders
#include "lib381/tshapes.h"
#include "lib381/globj.h" //class Tex2D

#include "MotionTracking.cpp" //OpenCV File
using std::cout;
using std::endl;
using std::exit;
using std::cerr;
using std::string;

const int windowStartSize=1024;
int wind_w, wind_h =1; //for viewport(pixels)

GLhandleARB prog1; //for shaders
string vshadername0;
string fshadername0;

string status; //Lucas' opencv status updates
Mat video; //the image that is being loaded as texture for opengl

//OPENCV/////////////////////////////////////////////////
//Lucas' Target code
//some boolean variables for added functionality
bool objectDetected = false;
//these two can be toggled by pressing 'd' or 't'
bool debugMode = false;
bool trackingEnabled = false;
//pause and resume code
bool pauses = false;
//set up the matrices that we will need
//the two frames we will be comparing
Mat frame1, frame2;
//their grayscale images (needed for absdiff() function)
Mat grayImage1, grayImage2;
//resulting difference image
Mat differenceImage;
//thresholded difference image (for use in findContours() function)
Mat thresholdImage;

 cv::VideoCapture stream1(0);

void loadCVimageAsTexture()
{
    if(video.empty()){
        cout << "Image is empty" << endl;
    }
    else{
        //align the opencv image to opengl 4-byte alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, (video.step & 3) ? 1:4);
        
        //set length of one complete row
        glPixelStorei(GL_UNPACK_ROW_LENGTH, video.step/video.elemSize());
        
        //OpenCv and Opengl stores images differently, flip the image so it is correct
        cv::flip(video, video, 0);
        
        
        GLuint tex_2d;
        glGenTextures(1, &tex_2d);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_2d);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, video.cols, video.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, video.ptr());
    }
}

void opencvLoop(){
    //read first frame
    stream1.read(frame1);
    if(!frame1.empty()){
        //convert frame1 to gray scale for frame differencing
        cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
    }
    //copy second frame
    stream1.read(frame2);
    //convert frame2 to gray scale for frame differencing
    cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);
    //perform frame differencing with the sequential images. This will output an "intensity image"
    //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
    absdiff(grayImage1, grayImage2, differenceImage);
    //threshold intensity image at a given sensitivity value
    threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
    if (debugMode == true) {
        //show the difference image and threshold image
        video = differenceImage;            // imshow("Difference Image", differenceImage);
        //Can only have one window at time -can show threshold only?
        //video = thresholdImage;             //imshow("Threshold Image", thresholdImage);
    }
    else {
        //if not in debug mode, destroy the windows
        destroyWindow("Difference Image");
        destroyWindow("Threshold Image");
    }
    //blur() to smooth the image, remove noise
    blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
    //threshold again to obtain binary image from blur output
    threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
    //verifies that image is 8 bit for findcontours()
    thresholdImage.convertTo(thresholdImage, CV_8U);
    if (debugMode == true) {
        //show the threshold image after it's been "blurred"
        //->            imshow("Final Threshold Image", thresholdImage);
    }
    else {
        //destroyWindow("Final Threshold Image");
    }
    
    //if tracking enabled, search for contours in our thresholded image
    if (trackingEnabled) {
        //Collects a number of sample averages specified by SMOOTHING_SAMPLE and sticks them in the samplePoints global vector
        collectSamples(thresholdImage, frame1);
    }
    //when samplePoints vector reaches the size specified by SMOOTHING_SAMPLE, updates the current point and clears samplePoints
    if (samplePoints.size() == SMOOTHING_SAMPLE) {
        destination = pathSmoothing(frame1);
        samplePoints.clear();
    }
    
    if(trackingEnabled){
        targetPoints.push_back(current);
        vector<Point> temp;
        int counter = 0;
        
        //Draw Past motion of target//
        //if vector target only has one point, skip over it
        if(targetPoints.size() > 1){
            if(TRAILS >= targetPoints.size()){
                counter = targetPoints.size();
                //target size hasn't gotten that many trails yet
                //Loop through past points and draw last 3 lines of motion
                for(int i = 0; i+1<counter-1; i++){
                    line(frame1, targetPoints[i], targetPoints[i+1], (0,0,255), 2);
                    cout << "building up" << endl;
                }
            }
            else{
                counter = TRAILS;
                
                //Loop through past points and draw last lines of motion
                for(int i = 0; i+1<counter; i++){
                    line(frame1, targetPoints[targetPoints.size()-1-i], targetPoints[targetPoints.size()-2-i], (0,0,255), 2);
                    targetPoints[i] = targetPoints[i+1];
                }
            }
        }
    }
    
    
    //limits the speed of movement of the target crosshair. We can tweak SPEED_OF_MOVEMENT to accurately reflect the actual position of the laser in project 2
    //so that we can draw an obscuring circle over the laser
    current = speedGovernor(current, destination, SPEED_OF_MOVEMENT);
    Point predictLine = speedGovernor(current, destination, 100);
    
    //draw the target
    drawTarget(current, frame1);
    line(frame1, current, predictLine, Scalar(0, 255, 0), 2);
    
    video = frame1;         //imshow("Frame1", frame1);
    
    //hotkey code from tutorial at (https://dl.dropboxusercontent.com/u/28096936/tuts/motionTrackingTut/finalCode/motionTracking.cpp)
    switch (waitKey(10)) {
        case 116: //'t' has been pressed. this will toggle tracking
            trackingEnabled = !trackingEnabled;
            if (trackingEnabled == false) status = "Tracking disabled.";
            else status = "Tracking enabled.";
            break;
        case 100: //'d' has been pressed. this will debug mode
            debugMode = !debugMode;
            if (debugMode == false) status = "Debug mode disabled.";
            else status = "Debug mode enabled.";
            break;
        case 112: //'p' has been pressed. this will pause/resume the code.
            pauses = !pauses;
            if (pauses == true) {
                status = "Code paused, press 'p' again to resume";
                while (pauses == true) {
                    //stay in this loop until
                    switch (waitKey()) {
                        case 112:
                            //change pause back to false
                            pauses = false;
                            status = "Code resumed.";
                            break;
                    }
                }
            }
            
    }
    
    glutPostRedisplay();
    
}



//Display function
void DisplayFunc()
{
    
    opencvLoop();
    
    glClearColor(1.0,1.0,1.0,0.0); //clearing screen to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    GLhandleARB theprog=prog1;
    glUseProgramObjectARB(theprog);
    loadCVimageAsTexture();
    glColor3d(0.5, 0.5, 0.50);
    glScaled(0.75, 0.75, 0.0);
    glTranslated(-0.50, 0.50, 0.0);
    drawSubdividedSquare(1);
    
    glUseProgramObjectARB(0); //no shaders for text
    //draw text to screen using Glenn Chappell's bitmap printer
    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glColor3d(0.0,0.0,0.0);
    glTranslated(0.0, -1.30, 0.0);
    BitmapPrinter p(-0.87, 0.8, 0.1);
    
    //prints instructions
    
    p.print("ESC        Quit");
    p.print("Use left and right to move obj");
    p.print(status);
    glPopMatrix();     //restore projection
    glMatrixMode(GL_MODELVIEW);
    
    //send it down the pipline
    glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: //ESC
            exit(0);
            break;
        case 'r':
        case 'R':
            break;
        case 'c':
        case 'C':
            break;
        case 'f':
        case 'F':
            break;
    }
}
void specialKey(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            glLoadIdentity();
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            glLoadIdentity();
            glutPostRedisplay();
            break;
    }
}

void init()
{
    prog1 = makeProgramObjectFromFiles(vshadername0, fshadername0);
    
    //video capture object.
    //VideoCapture capture;
   
    if (!stream1.isOpened()) {
        cout << "Cannot open camera." << endl;
    }

}

void Reshape(int w, int h)
{
    //set viewport for changes in window size
    glViewport(0,0,w,h);
    wind_w=w;
    wind_h=h;
    
    glMatrixMode(GL_MODELVIEW);
    
}

//error checking
void Idle()
{
    static int error_count =0;
    if(GLenum err= glGetError())
    {
        ++error_count;
        cerr << "OpenGL Error" << error_count << ": "
        << gluErrorString(err) << endl;
    }
    //Can put image loading here for textures?
    
    glutPostRedisplay();  
}

int main(int argc, char** argv)
{
    //OPENGL//////////////////////////////////////////////////
    glutInit(&argc, argv);
    getShaderFilenames(vshadername0, fshadername0, argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    //creating a window
    glutInitWindowSize(windowStartSize,windowStartSize);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Motion Tracking");
    
    if(glewInit() != GLEW_OK)
    {
        cerr << "glewInit failed" << endl;
        exit(1);
    }
    
    //call backs
    init();
    glutDisplayFunc(DisplayFunc);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(specialKey);
    glutReshapeFunc(Reshape);
    glutIdleFunc(Idle);
    
    //for glut to do its thing
    glutMainLoop();
    
    return 0;
}
                             
