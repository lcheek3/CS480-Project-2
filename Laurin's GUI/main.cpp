//main.cpp
//GUI for Motion Tracking Robot
//Laurin Fisher

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

//lib381 credit to Glenn Chappell
#include "lib381/bitmapprinter.h" // print text to screen
#include "libSOIL/SOIL.h"
#include "lib381/glslprog.h"//for shaders
#include "lib381/tshapes.h"
#include "lib381/globj.h" //class Tex2D

#include "MotionTracking.cpp" //OpenCV File
#include "ArduinoSerialMac.hpp" //Talk to Arduino on serial, for mac

using std::cout;
using std::endl;
using std::exit;
using std::cerr;
using std::string;





//Global Variables//

bool keys[4]; //boolean array to toggle arrow key inputs

const int windowStartSize=1024;
int wind_w, wind_h =1; //for viewport(pixels)
int wind_w1, wind_h1 =1; //for viewport(pixels)
int wind_w2, wind_h2 =1; //for viewport(pixels)
int wind_w3, wind_h3 =1; //for viewport(pixels)

//For Multiple Windows
static int mainWindow;
static int debugWindow1; //difference image
static int debugWindow2; //threshold image
static int debugWindow3; //Final image

GLhandleARB prog1; //for shaders
string vshadername0;
string fshadername0;

string status; //Lucas' opencv status updates
Mat video[4]; //the image that is being loaded as texture for opengl, video[0]=main,[1]=difference,[2]=thresh,[3]=final

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






//Functions//

//http://stackoverflow.com/questions/16809833/opencv-image-loading-for-opengl-texture
void loadCVimageAsTexture(Mat img)
{
    if(img.empty()){
        cout << "Image is empty" << endl;
    }
    else{
        //align the opencv image to opengl 4-byte alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, (img.step & 3) ? 1:4);
        
        //set length of one complete row
        glPixelStorei(GL_UNPACK_ROW_LENGTH, img.step/img.elemSize());
        
        //OpenCv and Opengl stores images differently, flip the image so it is correct
        cv::flip(img, img, 0);
        
        
        GLuint tex_2d;
        glGenTextures(1, &tex_2d);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_2d);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
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
        video[1] = differenceImage;            // imshow("Difference Image", differenceImage);
//        glutSetWindow(debugWindow1);
//        glutShowWindow();
        video[2] = thresholdImage;             //imshow("Threshold Image", thresholdImage);
    }
    else {
        //if not in debug mode, destroy the windows
        //glutHideWindow();
       // glutHideWindow(debugWindow2);
    }
    //blur() to smooth the image, remove noise
    blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
    //threshold again to obtain binary image from blur output
    threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
    //verifies that image is 8 bit for findcontours()
    thresholdImage.convertTo(thresholdImage, CV_8U);
    if (debugMode == true) {
        //show the threshold image after it's been "blurred"
        video[3] = thresholdImage;             //imshow("Final Threshold Image", thresholdImage);
    }
    else {
       // glutHideWindow(debugWindow3);
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
    
    video[0] = frame1;         //imshow("Frame1", frame1);
    glutPostRedisplay();
    
}

void drawArrowKeys()
{
    //Top Arrow Key
    glPushMatrix();
    if(keys[2]==true){
        glColor3d(0.7, 0.7, 0.7);
    }
    else{
        glColor3d(0.5, 0.5, 0.50);
    }
    glTranslated(0.60, -0.50, 0.0);
    glScaled(0.1, 0.1, 0.1);
    drawSubdividedSquare(1);
    glPopMatrix();
    
    //Draw arrow on Top Key
    glPushMatrix();
    glColor3d(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(0.60, -0.55);
    glVertex2d(0.60, -0.45);
    glEnd();
    glPopMatrix();
    
    //draw arrow triangle
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex2d(0.55, -0.50);
    glVertex2d(0.65, -0.50);
    glVertex2d(0.60, -0.45);
    glEnd();
    glPopMatrix();
    
    //Bottom Arrow Key
    glPushMatrix();
    if(keys[3]==true){
        glColor3d(0.7, 0.7, 0.7);
    }
    else{
        glColor3d(0.5, 0.5, 0.50);
    }
    glTranslated(0.60, -0.75, 0.0);
    glScaled(0.1, 0.1, 0.1);
    drawSubdividedSquare(1);
    glPopMatrix();
    
    //Draw arrow on Bottom Key
    glPushMatrix();
    glColor3d(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(0.60, -0.80);
    glVertex2d(0.60, -0.70);
    glEnd();
    glPopMatrix();
    
    //draw arrow triangle
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex2d(0.60, -0.80);
    glVertex2d(0.65, -0.75);
    glVertex2d(0.55, -0.75);
    glEnd();
    glPopMatrix();
    
    //Left Arrow Key
    glPushMatrix();
    if(keys[0]==true){
        glColor3d(0.7, 0.7, 0.7);
    }
    else{
        glColor3d(0.5, 0.5, 0.50);
    }
    glTranslated(0.35, -0.75, 0.0);
    glScaled(0.1, 0.1, 0.1);
    drawSubdividedSquare(1);
    glPopMatrix();
    
    //Draw arrow on Left Key
    glPushMatrix();
    glColor3d(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(0.30, -0.75);
    glVertex2d(0.40, -0.75);
    glEnd();
    glPopMatrix();
    
    //draw arrow triangle
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex2d(0.30, -0.75);
    glVertex2d(0.35, -0.70);
    glVertex2d(0.35, -0.80);
    glEnd();
    glPopMatrix();
    
    //Right Arrow Key
    glPushMatrix();
    if(keys[1]==true){
        glColor3d(0.7, 0.7, 0.7);
    }
    else{
        glColor3d(0.5, 0.5, 0.50);
    }
    glTranslated(0.85, -0.75, 0.0);
    glScaled(0.1, 0.1, 0.1);
    drawSubdividedSquare(1);
    glPopMatrix();
    
    //Draw arrow on Right Key
    glPushMatrix();
    glColor3d(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(0.90, -0.75);
    glVertex2d(0.80, -0.75);
    glEnd();
    glPopMatrix();
    
    //draw arrow triangle
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex2d(0.90, -0.75);
    glVertex2d(0.85, -0.70);
    glVertex2d(0.85, -0.80);
    glEnd();
    glPopMatrix();
}



//Display function
void DisplayFunc()
{
    glutSetWindow(mainWindow);
    
    opencvLoop();
    
    glClearColor(1.0,1.0,1.0,0.0); //clearing screen to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    GLhandleARB theprog=prog1;
    glUseProgramObjectARB(theprog);
    glPushMatrix();
    loadCVimageAsTexture(video[0]);
    glColor3d(0.5, 0.5, 0.50);
    glScaled(0.75, 0.75, 0.0);
    glTranslated(-0.50, 0.50, 0.0);
    drawSubdividedSquare(1);
    glPopMatrix();
    
    glUseProgramObjectARB(0); //no textures here!
    drawArrowKeys();
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
    
    p.print("Use Arrow Keys to Move Robot");
    p.print("T          Enable Tracking");
    p.print("D          Debug Mode");
    p.print("ESC        Quit");
    p.print(status);
    glPopMatrix();     //restore projection
    glMatrixMode(GL_MODELVIEW);
    
    //send it down the pipline
    glutSwapBuffers();
}

void differenceDisplay()
{
    glutSetWindow(debugWindow1);
    glClearColor(1.0,1.0,1.0,0.0); //clearing screen to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Draw difference Image texture onto object
    GLhandleARB theprog=prog1;
    glUseProgramObjectARB(theprog);
    loadCVimageAsTexture(video[1]);
    drawSubdividedSquare(1);
    
    //send it down the pipline
    glutSwapBuffers();
    
}

void threshDisplay()
{
    glutSetWindow(debugWindow2);
    glClearColor(1.0,1.0,1.0,0.0); //clearing screen to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Draw difference Image texture onto object
    GLhandleARB theprog=prog1;
    glUseProgramObjectARB(theprog);
    loadCVimageAsTexture(video[2]);
    drawSubdividedSquare(1);
    
    //send it down the pipline
    glutSwapBuffers();
    
}

void finalDisplay()
{
    glutSetWindow(debugWindow2);
    glClearColor(1.0,1.0,1.0,0.0); //clearing screen to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Draw difference Image texture onto object
    GLhandleARB theprog=prog1;
    glUseProgramObjectARB(theprog);
    loadCVimageAsTexture(video[3]);
    drawSubdividedSquare(1);
    
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
        case 't':   //'t' has been pressed. this will toggle tracking
        case 'T':
            trackingEnabled = !trackingEnabled;
            if (trackingEnabled == false) status = "Tracking disabled.";
            else status = "Tracking enabled.";
            break;
        case 'p':   //'p' has been pressed. this will pause/resume the code.
        case 'P':
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
            break;
        case 'd':
        case 'D': //'d' has been pressed. this will debug mode
            debugMode = !debugMode;
            if (debugMode == false) status = "Debug mode disabled.";
            else status = "Debug mode enabled.";
            break;
            break;
    }
}
void specialKey(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            keys[0] = true;
            keys[1] = false;
            keys[2] = false;
            keys[3] = false;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            keys[1] = true;
            keys[0] = false;
            keys[2] = false;
            keys[3] = false;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            keys[2]=true;
            keys[0] = false;
            keys[3] = false;
            keys[1] = false;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            keys[3]=true;
            keys[0] = false;
            keys[2] = false;
            keys[1] = false;
            glutPostRedisplay();
            break;
    }
}

void init()
{
    prog1 = makeProgramObjectFromFiles(vshadername0, fshadername0);
   
    if (!stream1.isOpened()) {
        cout << "Cannot open camera." << endl;
    }

//To load arrows as texture to put on object (keyboard arrows)
    
    //Image from: http://packdog.com/static/images/blog/keys-for-happiness/arrows.gif
    GLuint tex_2d = SOIL_load_OGL_texture
    (		"plainArrows.png",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
     );  //get image
    if(0==tex_2d)
    {
        cout << "soil error" << endl;
    }

}

void Reshape(int w, int h)
{
    //set viewport for changes in window size
    glutSetWindow(mainWindow);
    glViewport(0,0,w,h);
    wind_w=w;
    wind_h=h;
    
    glMatrixMode(GL_MODELVIEW);
    
}


void differenceReshape(int w, int h)
{
    //set viewport for changes in window size
    glutSetWindow(debugWindow1);
    glViewport(0,0,w,h);
    wind_w1=w;
    wind_h1=h;
    
    glMatrixMode(GL_MODELVIEW);
    
}

void threshReshape(int w, int h)
{
    //set viewport for changes in window size
    glutSetWindow(debugWindow2);
    glViewport(0,0,w,h);
    wind_w1=w;
    wind_h1=h;
    
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
    
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    //OPENGL//////////////////////////////////////////////////
    glutInit(&argc, argv);
    getShaderFilenames(vshadername0, fshadername0, argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    //Create the Main Window
    glutInitWindowSize(windowStartSize,windowStartSize);
    glutInitWindowPosition(50,50);
    mainWindow = glutCreateWindow("Motion Tracking");
    
    if(glewInit() != GLEW_OK)
    {
        cerr << "glewInit failed" << endl;
        exit(1);
    }
    
        init();
        glutDisplayFunc(DisplayFunc);
        glutReshapeFunc(Reshape);
        glutKeyboardFunc(Keyboard);
        glutSpecialFunc(specialKey);
        glutIdleFunc(Idle);
    
debugWindow1 = glutCreateSubWindow(mainWindow, windowStartSize/2 + 250, 10, 150, 150);
    init();
    glutDisplayFunc(differenceDisplay);

debugWindow2 = glutCreateSubWindow(mainWindow, windowStartSize/2 + 250, 170, 150, 150);
    init();
    glutDisplayFunc(threshDisplay);
    
debugWindow3 = glutCreateSubWindow(mainWindow, windowStartSize/2 + 250, 330, 150, 150);
    init();
    glutDisplayFunc(finalDisplay);
    
    
//    //Create Debug Mode Difference Image window
//    glutInitWindowSize(windowStartSize,windowStartSize);
//    glutInitWindowPosition(100, 100);
//    debugWindow1 = glutCreateWindow("Debug Mode: Difference Image");
//    
//    if(glewInit() != GLEW_OK)
//    {
//        cerr << "glewInit failed" << endl;
//        exit(1);
//    }
//    
//        init();
//        glutDisplayFunc(differenceDisplay);
//        glutReshapeFunc(differenceReshape);
//        glutKeyboardFunc(Keyboard);
//        glutSpecialFunc(specialKey);
//        glutIdleFunc(Idle);
//    
//    glutHideWindow();
//    
//    //Create Debug Mode Threshold Image window
//    glutInitWindowSize(windowStartSize,windowStartSize);
//    glutInitWindowPosition(100, 100);
//    debugWindow2 = glutCreateWindow("Debug Mode: Threshold Image");
//    
//    if(glewInit() != GLEW_OK)
//    {
//        cerr << "glewInit failed" << endl;
//        exit(1);
//    }
//    
//    init();
//    glutDisplayFunc(threshDisplay);
//    glutReshapeFunc(threshReshape);
//    glutKeyboardFunc(Keyboard);
//    glutSpecialFunc(specialKey);
//    glutIdleFunc(Idle);
//    
//    glutHideWindow();
//    
    

    //for glut to do its thing
    glutMainLoop();
    
    return 0;
}

