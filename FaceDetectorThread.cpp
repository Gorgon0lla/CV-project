#include "FaceDetectorThread.hpp"

pthread_barrier_t initBarrier;
pthread_barrier_t cameraBarrier;

pthread_mutex_t faceDetectorThreadMutex;
extern pthread_mutex_t frameMutex;

GlobalVariables globalVariables;

int initFaceDetectorThread(std::string classificator_location) {
    pthread_t      faceDetectorThread;
    pthread_attr_t faceDetectorThreadAttr;
    struct mq_attr faceDetectorMQueueAttr;

    FaceDetectorThArgs faceDetectorThArgs;

    faceDetectorThreadMutex = PTHREAD_MUTEX_INITIALIZER;

    faceDetectorThArgs.classificator_loc = classificator_location;

    pthread_attr_init(&faceDetectorThreadAttr);

    faceDetectorMQueueAttr.mq_maxmsg  = 1000;
    faceDetectorMQueueAttr.mq_msgsize = sizeof(std::vector<cv::Rect>);


    if (pthread_create(&faceDetectorThread, &faceDetectorThreadAttr, tFaceDetectorThreadFunc, (void*)&faceDetectorThArgs)) {
        return 1;
    }

    return 0;
}

int finalizeFaceDetectorThread() {

    return 0;
}

void* tFaceDetectorThreadFunc(void* cookie) {

    int policy = SCHED_FIFO;
    struct sched_param param;
    std::string classifier_location = std::string(get_current_dir_name()) + "/haarcascade_frontalface_alt.xml";
    cv::Mat frame4rec; // frame to get from Camera class
    std::vector<cv::Rect> faces; //faces to send for Camera class

    //std::cout << classifier_location << " it is path \n";

    FaceDetector faceDetector(classifier_location);


    pthread_setschedparam(pthread_self(), policy, &param);

    std::cout << "FaceDetectorThread: waiting\n";
    pthread_barrier_wait(&initBarrier);
    std::cout << "FaceDetectorThread: started while\n";
    while (true) {

        pthread_mutex_lock(&frameMutex);
        pthread_mutex_lock(&faceDetectorThreadMutex);
        
        frame4rec = globalVariables.frame;
        // Face detections based on camera frame
        faces = faceDetector.getFaces(frame4rec);

        globalVariables.faces = faces;

        pthread_mutex_unlock(&frameMutex);
        pthread_mutex_unlock(&faceDetectorThreadMutex);
        pthread_barrier_wait(&cameraBarrier);

        //std::cout << "Detected faces number: " << faces.size() << std::endl;
               
    }
    std::cout<<"FaceDetectorThread ended\n";
    return NULL;
}   
