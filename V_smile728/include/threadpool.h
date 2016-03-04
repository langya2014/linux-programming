#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <pthread.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include "eventstruct.h"

using namespace std;


struct job
{
	void (*job)(void * arg);
	void *arg;		//argument of job(void* arg)
};

struct threadpool
{
	pthread_mutex_t jobs_lock;
	pthread_cond_t jobs_cond;

	int shutdown;	//0--shutdonw, 1--running
	vector<job*> jobs;
	map<pthread_t*,int> mapthread;		//map of thread,int==1--running, int ==0--sleep
	int maxnum;	//max number of thread
	int maxsleepnum;	//max number of sleep thread
	int minsleepnum;	//min number of sleep thread
	pthread_t *add_thread;	////create thread if cursleepnum smaller than minsleepnum

};

struct threadpool* init_pool(int imaxnum, int imaxsleepnum ,int iminsleepnum);
void add_job(struct threadpool *pool,void (*process)(void *arg),void *arg);
void *add_thread_runtine(void *arg);
void *thread_runtine(void *arg);
void destroy_pool(struct threadpool* pool);


#endif