#include <iostream>
#include <string>
#include <sstream>

#include <opencv2/imgcodecs.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/types_c.h>
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"

//defines
#define KEY_Q (113)
#define KEY_W (119)
const cv::Rect REC(240, 160, 260, 260);

//function definitions
void drawMultipleImages(cv::Mat& frame);
void exitProgram(cv::VideoCapture& cap, cv::VideoWriter& outVideo);
void addLogo(cv::Mat& frame, cv::Mat pythonLogo);

int main(int argc, char** argv) {
    cv::VideoCapture cap;
    cv::Mat frame;  //used for video capture
    cv::Mat grayImage, subImg, pythonLogo, Roi;
    bool isVideoGood = false, isMultiple = false;
    int16_t key = -1;
    const std::string outFilename = "outcpp.avi";
    double fps = 10.0;
    cv::VideoWriter outVideo;
    //cv::Rect WhereRec;
    cv::Mat whiteBgPhoto;

    cv::Mat img = imread("redBall.jpg", cv::IMREAD_UNCHANGED);  //IMREAD_GRAYSCALE
    if (img.empty()){
        std::cout << "!!! Failed imread(): image not found" << std::endl;
        // don't let the execution continue, else imshow() will crash.
    }

    pythonLogo = imread("mainlogo.png", cv::IMREAD_UNCHANGED);  //IMREAD_GRAYSCALE
    if (pythonLogo.empty()) {
        std::cout << "!!! Failed imread(): image not found" << std::endl;
        // don't let the execution continue, else imshow() will crash.
    }

    whiteBgPhoto = imread("3D-Matplotlib.png", cv::IMREAD_UNCHANGED);  //IMREAD_GRAYSCALE
    if (whiteBgPhoto.empty()) {
        std::cout << "!!! Failed imread(): image not found" << std::endl;
        // don't let the execution continue, else imshow() will crash.
    }


    cv::namedWindow("Output", 1);    //create a gui window:

    cv::Mat output = cv::Mat::zeros(120, 350, CV_8UC3);     //initialize a 120X350 matrix of black pixels:

    //write text on the matrix:
    putText(output, "Hello World :)", cvPoint(15, 70),
        cv::FONT_HERSHEY_PLAIN, 3, cvScalar(0, 255, 0), 4);

    //cv::imshow("Output", output);     //display the image  
    //cv::imshow("Apple", img);

    cap = cv::VideoCapture(0);      //to make capture inside file: VideoCapture cap("chaplin.mp4"); 
    outVideo.open(outFilename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(640, 480), true);
    while (1) {     //video capture      
        isVideoGood = cap.read(frame);
        if (!isVideoGood) {
            std::cout << "!!! Video was not found" << std::endl;
            return 0;
        }
        cv::cvtColor(frame, grayImage, cv::COLOR_BGR2GRAY);

        //cv::line(frame, cv::Size(0, 0), cv::Size(300, 400), cv::Scalar(255), 50);
        //cv::rectangle(frame, cv::Size(0, 0), cv::Size(300, 400), cv::Scalar(0, 0, 255), 15);
        //subImg = frame(cv::Range(0, 200), cv::Range(0, 150));
        //subImg.copyTo(frame);
        //cv::imshow("Onlly half", subImg);

        //rectangle(frame, Rec, cv::Scalar(255), 1, 8, 0);

        if (isMultiple) {
            drawMultipleImages(frame);
        }


        addLogo(whiteBgPhoto, pythonLogo);

        //cv::imshow("frame", frame);
        //cv::imshow("gray", grayImage);

        //cv::imshow("white", whiteBgPhoto);

        outVideo.write(frame);
        if (key = cv::waitKey(1)) {
            if (key == KEY_Q) {
                break;
            }
            if (key == KEY_W) {
                isMultiple = !isMultiple;
            }
        }
    }

    exitProgram(cap, outVideo);
    return 0;
}

void drawMultipleImages(cv::Mat &frame)
{
    cv::Mat Roi = frame(REC);
    cv::Rect WhereRec(0, 0, Roi.cols, Roi.rows);
    cv::Rect WhereRec2(200, 0, Roi.cols, Roi.rows);
    cv::Rect WhereRec3(380, 0, Roi.cols, Roi.rows);
    Roi.copyTo(frame(WhereRec));
    Roi.copyTo(frame(WhereRec2));
    Roi.copyTo(frame(WhereRec3));

}

void exitProgram(cv::VideoCapture &cap, cv::VideoWriter &outVideo )
{
    cap.release();
    outVideo.release();
    //wait for the user to press any key:
    cv::waitKey(0);  //Calling waitKey() is mandatory if you use imshow().
    cv::destroyAllWindows();
}

void addLogo(cv::Mat &frame, cv::Mat pythonLogo)
{
    cv::Mat Roi;
    cv::Mat img2gray, mask, mask_inv, frame_bg, frame_fg, dst;
    int logoRows = 0, logocols = 0;

    logoRows = pythonLogo.rows;
    logocols = pythonLogo.cols;

    Roi = frame(cv::Range(0, logoRows), cv::Range(0, logocols));

    cv::cvtColor(pythonLogo, img2gray, cv::COLOR_BGR2GRAY);
    cv::threshold(img2gray, mask, 220, 255, cv::THRESH_BINARY_INV);

    cv::bitwise_not(mask, mask_inv);
   // cv::imshow("mask", mask_inv);

    cv::bitwise_and(Roi, Roi, frame_bg, mask_inv);
    //cv::imshow("mask", frame_bg);
    cv::bitwise_and(pythonLogo, pythonLogo, frame_fg, mask);

   cv::imshow("frame_bg", frame_bg);
   cv::imshow("frame_fg", frame_fg);


            //cv::add(frame_bg, frame_fg, dst);

    cv::add(frame_bg, frame_fg, dst);
    cv::imshow("dst", dst);
    //frame.copyTo(dst);


    cv::Rect WhereRec3(0, 0, dst.cols, dst.rows);
    dst.copyTo(frame(WhereRec3));
    //dst = frame_bg + frame_fg;

    
    cv::imshow("frame", frame);

        //dst.copyTo(frame(cv::Range(0, logoRows), cv::Range(0, logocols)));

}