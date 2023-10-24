#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

class FaceDetector {
    private:        
        cv::CascadeClassifier cascadeClassifier;
        std::vector<cv::Rect> faces;
        std::string           classifierLocation;
        double                scaleFactor = 1.2;

    public:
        FaceDetector(std::string);
        std::vector<cv::Rect> getFaces(cv::Mat);
};