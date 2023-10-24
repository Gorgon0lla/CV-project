#pragma once

#include <iostream>
#include <pthread.h>
#include <mqueue.h>
#include <opencv2/core.hpp>
#include "Camera.hpp"
#include "FaceDetectorThread.hpp"



struct CameraThArgs
{
    int cam_num;
    int api_ID;

};

int initCameraThread(int ,int);
int finalizeCameraThread();
void* tCameraThreadFunc(void*);