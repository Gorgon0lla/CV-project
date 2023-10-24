#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>


class Camera {
    private:
        cv::Mat frame;
        cv::VideoCapture cap;

        int cameraID;
        int apiID;

    public:
        Camera(int, int);
        void open();
        bool isOpened();
        void run();
        void showFaces(std::vector<cv::Rect>);
        cv::Mat getFrame();
        int thread_run(int, int);
};