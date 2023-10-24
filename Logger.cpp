#include "Logger.hpp"


mqd_t loggerMQ;


pthread_t loggerThread;
pthread_attr_t loggerThreadAttr;



int initLogger() {

	int status;
	pthread_attr_init(&loggerThreadAttr);
	struct mq_attr loggerMQattr;


	loggerMQattr.mq_maxmsg = 1000;
	loggerMQattr.mq_msgsize = sizeof(int);


	if ((loggerMQ = mq_open("/loggerMQ", O_CREAT | O_RDWR, 777, &loggerMQattr)) == -1) {
		std::cout<<errno<<std::endl;
		std::cerr << "Creation of LoggerMQ failed\n";
		return 1;
	}


	if ((status = pthread_create( &loggerThread, NULL, tLoggerThreadFunc, &loggerMQattr))) {
		std::cerr << "Cannot create logger thread.\n";
		return 1;
	}

	return 0;
}


int finalizeLogger() {

	if(mq_close(loggerMQ) || mq_unlink("/loggerMQ")){
        return 1;
    }

	return 0;
}


void *tLoggerThreadFunc(void *cookie) {

	int policy = SCHED_FIFO;
	struct sched_param param;
    
    std::ofstream loggerFile;
	int  faceNum;
    std::time_t currentTime;
	tm* nowTime;
	int iter = 0;

	param.sched_priority = sched_get_priority_min(policy);
	pthread_setschedparam( pthread_self(), policy, &param);

    loggerFile.open("cameraLog.txt");

	while(1) {
		iter++;
		
		mq_receive(loggerMQ, (char*)&faceNum, sizeof(int), NULL);

		if(iter == 10){
        currentTime = time(0);
		nowTime = localtime(&currentTime);
		loggerFile << nowTime->tm_hour <<":"<< nowTime->tm_min << ":" << nowTime->tm_sec << " Detected faces number: " << faceNum << std::endl;
		//std::cout<<"Logger faces: "<<faceNum<<std::endl;
		iter = 0;
		}

	}

    loggerFile.close();
	std::cout<<"LoggerThread ended\n";

	return 0;
}