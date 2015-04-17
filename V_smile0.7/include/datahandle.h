#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include "eventstruct.h"
#include "threadpool.h"

#include "dbhandle.h"

using namespace std;
using namespace Json;

extern struct threadpool* pool;

void datahandle(struct user *puser);
void login(void *arg);
void regist_change(void *arg);
void ser_add_qun(void *arg);
void getqunlist(void *arg);

/***************  new  ***********/
void gethottie(void *arg);
void createqun(void *arg);
void getqunrecord(void *arg);
void chat(void *arg);
void createhottie(void *arg);
/*************  end new  ***********/

#endif
