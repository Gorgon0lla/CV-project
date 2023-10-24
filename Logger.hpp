#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <mqueue.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <errno.h>


extern mqd_t loggerMQ;


int initLogger();
void *tLoggerThreadFunc(void *);
int finalizeLogger();

