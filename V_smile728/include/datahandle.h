#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include "eventstruct.h"
#include "threadpool.h"
#include <sstream>
#include <dbhandle.h>

using namespace std;
using namespace Json;

extern struct threadpool* pool;

void datahandle(struct user *puser);

void regist_change(void *arg);
void login(void *arg);
void logout(void* arg);
void privatemsg(void *arg);//-1
void tofocus(void *arg);//-1
void getfocuslist(void *arg);//-1
void getfanslist(void *arg);//-1

void getqunlist(void *arg);
void deletequn(void *arg);
void quitqun(void *arg);
void addqun(void *arg);
void respaddqun(void *arg);
void serqun(void *arg);
void createqun(void *arg);
void chat(void *arg);
void getqunrecord(void *arg);
void getmsg(void *arg);

void createhottie(void *arg);
void gethottielist(void *arg);
void getonehottie(void *arg);
void getprivatehottielist(void *arg);
void savehottie(void *arg);
void getsavedhotielist(void *arg);

void askpk(void *arg);
void quitpk(void *arg);
void quitgame(void *arg);
void finishgame(void *arg);

void addfen(void *arg);
#endif
