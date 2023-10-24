#include "CameraThread.hpp"


extern pthread_mutex_t faceDetectorThreadMutex;
extern pthread_barrier_t initBarrier;
pthread_mutex_t frameMutex;
struct GlobalVariables;
extern GlobalVariables globalVariables;



int initCameraThread(int camera_number, int api_ID) {
    pthread_t      cameraThread;
    pthread_attr_t cameraThreadAttr;
    struct mq_attr cameraMQueueAttr;

    CameraThArgs cameraThArgs;

    
    frameMutex = PTHREAD_MUTEX_INITIALIZER;
    cameraThArgs.cam_num = camera_number;
    cameraThArgs.api_ID = api_ID;

    pthread_attr_init(&cameraThreadAttr);

    cameraMQueueAttr.mq_maxmsg  = 1000;
    cameraMQueueAttr.mq_msgsize = sizeof(cv::Mat);


    if (pthread_create(&cameraThread, &cameraThreadAttr, tCameraThreadFunc, (void*)&cameraThArgs)) {
        return 1;
    }

    return 0;
}

int finalizeCameraThread() {


    return 0;
}

void* tCameraThreadFunc(void* cookie) {

    int policy = SCHED_FIFO;
    struct sched_param param;
    std::vector<cv::Rect> faces;
    cv::Mat frame;

    int frame_nmb  = 0;
    int print_freq = 10;
    int result;

    
    int camera_number = 0;
    int apiID = cv::CAP_ANY;

    Camera camera(camera_number, apiID);

    camera.open();

    if(!camera.isOpened())
    {
        std::cerr << "Cannot open Camera \n";
        return NULL;
    }
    //real time thread
    param.sched_priority = sched_get_priority_max(policy);
    pthread_setschedparam(pthread_self(), policy, &param);

    std::cout<<"CameraThread: waiting\n";

    pthread_barrier_wait(&initBarrier);
    std::cout<<"CameraThread: started while\n";
    while(true)
    {

        
        // reading frame
        result = camera.thread_run(frame_nmb, print_freq);

        pthread_mutex_lock(&frameMutex);
        globalVariables.frame= camera.getFrame();
        pthread_mutex_unlock(&frameMutex);


        if(result != 0)
        {
            std::cerr << "ERROR in CameraThread! blank frame grabbed\n";
            break;
        }


    }
    std::cout<<"CameraThread ended\n";
    return NULL;
}   
