#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include "eventstruct.h"
#include "threadpool.h"
//#include "base64.h"
#include <sstream>
#include <dbhandle.h>

#include <sys/stat.h>   
#include <unistd.h> 
#include <fcntl.h>

using namespace std;
using namespace Json;

extern struct threadpool* pool;

void datahandle(struct user *puser);

void regist_change(void *arg);
void login(void *arg);
void logout(void* arg);

void UpdateClass(void *arg);
void RequestClass(void *arg);

void RequestImage(void *arg);
void ClientSendImage(void *arg);

int get_file_size(const char *path); 

#endif