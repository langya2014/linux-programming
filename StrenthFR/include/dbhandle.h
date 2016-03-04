#ifndef DBHANDLE_H
#define DBHANDLE_H

#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <mysql.h>
#include <stdlib.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
#include <sstream>
#include <map>
using namespace std;

#define maxdscrb 128

//NOTICE: no element of email
struct USER{	
	int Islginsuccess;
	string username;
	string passwd;
	//string mail; //add the mail to this struct.
	int sex;
};

struct CLASS_INFOR{
	string classname;
	int roll_call;
	int attendace;
};

/***********basic functions*************/
struct USER *dblogin(char name[32], char passwd[24], long socket);
int registration(struct USER *pusrinfor);	// NOTICE:no parameters for sex age headpicID and email
int changeinfo(struct USER *pchginfor);// NOTICE:no parameter for sex

/**************about class*************/
//int update_class(string name, string classname, int roll_call, int attendace);
int update_class(string name, struct CLASS_INFOR * pClassinfor);
struct CLASS_INFOR *RequestClassInfor(string name, string classname);
 
int setsocket(string username);
int set_allskt(void);

char *gettime(void);

#endif