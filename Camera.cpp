#include "Camera.hpp"

Camera::Camera(int _cameraID, int _apiID) : cameraID(_cameraID), apiID(_apiID)
{}

void Camera::open() {
    cap.open(cameraID, apiID);
}

bool Camera::isOpened() {
    return cap.isOpened();
}

cv::Mat Camera::getFrame()
{
    return this->frame;
}

void Camera::run() {

    int frame_nmb  = 0;
    int print_freq = 10;
    std::vector<cv::Rect> faces;

    while(true)
    {
        frame_nmb++;

        cap.read(frame);
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        //faces = faceDetector.getFaces(frame);

        if (frame_nmb % print_freq == 0) {
            frame_nmb = 0;

            std::cout << faces.size() << " faces detected" << std::endl;            
        }

        for (int i = 0; i < faces.size(); ++i) {
            cv::Mat faceROI = frame(faces[i]);

            int x = faces[i].x;
            int y = faces[i].y;
            int h = y + faces[i].height;
            int w = x + faces[i].width;

            //Drawing a rectangle using around the faces//
            cv::rectangle(frame,
                cv::Point(x, y),
                cv::Point(w, h),
                cv::Scalar(255, 0, 255),
                2,
                8,
                0);
        }

        imshow("Live", frame);
        if (cv::waitKey(5) >= 0) {
            break;
        }
    }
}

void Camera::showFaces(std::vector<cv::Rect> faces)
{

     cap.read(frame);

    if (frame.empty()) {
        std::cerr << "ERROR in showFaces()! blank frame grabbed\n";
    }


    for (int i = 0; i < faces.size(); ++i) {
        cv::Mat faceROI = frame(faces[i]);

        int x = faces[i].x;
        int y = faces[i].y;
        int h = y + faces[i].height;
        int w = x + faces[i].width;

        //Drawing a rectangle using around the faces//
        cv::rectangle(frame,
            cv::Point(x, y),
            cv::Point(w, h),
            cv::Scalar(255, 0, 255),
            2,
            8,
            0);
    }

    imshow("Live", frame);
    
}

int Camera::thread_run(int frame_nmb, int print_freq)
{
    frame_nmb++;

    cap.read(frame);
    cv::Size frame_size = frame.size();

    //cv::imwrite("Klata.jpg", frame);

    if (frame.empty()) {
        std::cerr << "ERROR in thread_run! blank frame grabbed\n";
        return 1; // frame error
    }

    if (frame_nmb % print_freq == 0) {
        frame_nmb = 0;
        //std::cout<< frame_size.height << std::endl;
                 
    }


    return 0;

}
