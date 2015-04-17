#ifndef EVENTSTRUCT_H
#define EVENTSTRUCT_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <event.h>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <memory.h>
#include "threadpool.h"
#include "datahandle.h"

#define BUFFSIZE  1024

using namespace Json;
using namespace std;

extern struct event_base *base;
extern struct threadpool *pool;
	
struct user
{
	//string username;
	//int alive;
	//struct event alivetimer;
	struct event *read_ev, *write_ev;
	pthread_mutex_t msg_lock;
	vector<string> msg;
	string recvstr;
	char data[BUFFSIZE];
	int msg_len;
};

void on_accept(int sock, short event, void *arg);

void on_read(int sock, short event, void *arg);

void on_write(int sock, short event, void *arg);

//void on_timer(int sock, short event ,void *arg);

void sendback(void *arg);

#endif