#pragma once

#include <iostream>
#include <pthread.h>
#include <mqueue.h>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include "FaceDetector.hpp"


struct GlobalVariables
{
    cv::Mat frame;
    std::vector<cv::Rect> faces;
};

struct FaceDetectorThArgs
{
    std::string classificator_loc;
};

int initFaceDetectorThread(std::string);
int finalizeFaceDetectorThread();
void* tFaceDetectorThreadFunc(void*);

