#include "FaceDetector.hpp"

FaceDetector::FaceDetector(std::string _classifierLocation) : classifierLocation(_classifierLocation) {
    cascadeClassifier.load(classifierLocation);
}

std::vector<cv::Rect> FaceDetector::getFaces(cv::Mat frame) {
    cascadeClassifier.detectMultiScale(frame, faces, scaleFactor, 4, cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    return faces;
}