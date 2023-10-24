#include "CameraThread.hpp"
#include "FaceDetectorThread.hpp"
#include "Logger.hpp"

#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <queue>
#include <atomic>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <charconv>

extern pthread_mutex_t faceDetectorThreadMutex;
extern pthread_barrier_t initBarrier;
extern pthread_barrier_t cameraBarrier;
extern GlobalVariables globalVariables;
pthread_mutex_t cameraThreadMutex;





int main(int argc, char** argv)
{
   // Komenda ktora nalezy uruchomic przed rozpoczeciem programu
   // sudo sysctl fs.mqueue.msg_max=3000

   std::string classifierLocation = std::string(get_current_dir_name()) + "/haarcascade_frontalface_alt.xml";

   std::cout << classifierLocation << "\n";

   int camera_number = 0;
   int api_ID = cv::CAP_ANY;
   bool isInitTrue = true;
   std::vector<cv::Rect> faces;
   char status[1];
   status[0] = 'w';
   char facesNum[1];
   //int facesNum;

   char shutDown[1];
   shutDown[0] = 'f';

   cameraThreadMutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_barrier_init(&initBarrier, NULL, 3);
   pthread_barrier_init(&cameraBarrier, NULL, 2);


   pid_t pid;
   int procCom[2];
   int loggerPipe[2];
   int shutPipe[2];


  if(pipe(procCom) == -1){

      std::cerr<< "Cannot create pipe \n";

  }

    if(pipe(loggerPipe) == -1){

      std::cerr<< "Cannot create logger pipe \n";

  }

      if(pipe(shutPipe) == -1){

      std::cerr<< "Cannot create shut pipe \n";

  }

  pid = fork();

  if(pid < 0){

      std::cerr << "Cannot create second process \n";
      return 0;
  }

   //////////////////////////////////////////////////////////
  if(pid != 0){// first process

   close(procCom[1]);
   close(loggerPipe[0]);
   close(shutPipe[0]);

   cv::Mat frame;
   int size_of_faces = 0;


   if(initCameraThread(camera_number, api_ID))
   {
      std::cerr << "Cannot create Camera Thread \n";
      finalizeCameraThread();
      shutDown[0] = 't';
      write(shutPipe[0], shutDown, 1);
      return 1;
   }

   if(initFaceDetectorThread(classifierLocation))
   {
      std::cerr << "Cannot create Face Detector Thread \n";
      shutDown[0] = 't';
      write(shutPipe[0], shutDown, 1);
      finalizeFaceDetectorThread();
      return 1;
   }

   std::cout<<"Waiting for second process to start successfully \n";
   read(procCom[0],status, 1);
   std::cout<<"Status of second process: "<< status << std::endl;

   if(status == "q")
   {
      std::cerr<<"Second process failed\n";
      return 1;
   }
   std::cout<<"Starting while in main thread\n";
   while(1)
   {
      
      pthread_mutex_lock(&faceDetectorThreadMutex);
      faces = globalVariables.faces;
      pthread_mutex_unlock(&faceDetectorThreadMutex);

      
      
      pthread_mutex_lock(&cameraThreadMutex);
      frame = globalVariables.frame;
      pthread_mutex_unlock(&cameraThreadMutex);

      if(isInitTrue == false){

      pthread_barrier_wait(&cameraBarrier);

      }
      else{

         std::cout << "Initialization...\n";
         isInitTrue = false;
         pthread_barrier_wait(&initBarrier);

      }

      if(frame.empty()){

         std::cerr << "Empty frame in main thread! \n";

      }
      else{

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
      
      size_of_faces = faces.size();
      std::to_chars(facesNum, facesNum + 1, size_of_faces);

      //std::cout<<"Faces main thread: "<<facesNum[0]<<std::endl;
      write(loggerPipe[1], facesNum, 1);
      write(shutPipe[1], shutDown, 1);


      if(cv::waitKey(10) >= 0){

         std::cout << "Program ended, user interupt \n";
         shutDown[0] = 't';
         write(shutPipe[1], shutDown, 1);
         break;

      }
   }
   finalizeCameraThread();
   finalizeFaceDetectorThread();
   std::cout<<"First process ended\n";

   close(procCom[0]);
   close(loggerPipe[1]);
   close(shutPipe[1]);
   }
   /////////////////////////////////////////////////////////////////////
   else if(pid == 0){ // second process
      close(procCom[0]);
      close(loggerPipe[1]);
      close(shutPipe[1]);

      int numFace = 0;
      char test[1];
      test[0] = 'a';
      //==========================================================

      
      if(initLogger())
      {
         status[0] = 'q';
         std::cerr << "Logger init failed\n";
         write(procCom[1], status, 1);
         return 1;
      }
      else{
         status[0] = 'g';
      }
      write(procCom[1], status, 1);

      while(1){

         read(loggerPipe[0], facesNum, 1);
         numFace = (int)(facesNum[0]) - 48; //ASCII Code
         
         //std::cout<<"Second process faces: "<<numFace<<std::endl;
         read(shutPipe[0], shutDown, 1);
         prctl(PR_SET_PDEATHSIG, SIGKILL);
         if(shutDown[0] == 't')
         {
            std::cout<<"Second process shut down\n";
            break;
         }
         mq_send(loggerMQ, (char*)&numFace, sizeof(int), 0);

      } 

      finalizeLogger();
      std::cout<<"Second process ended\n";

     close(procCom[1]);
     close(loggerPipe[0]);
     close(shutPipe[0]);
   }
   
   std::cout<<"Program ended normally !\n";

   waitpid(pid, NULL, 0);

   return 0;
}