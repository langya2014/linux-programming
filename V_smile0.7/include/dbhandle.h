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
using namespace std;

#define maxdscrb 128

//NOTICE: no element of email
struct USER{	
	int Islginsuccess;
	string username;
	string passwd;
	string mail; //add the mail to this struct.
	int sex;
	int age;
	int facetype;
	int portrait;
};

struct quninfor{
	int qunportrait;
	string qunname;
	string qundescrible;
};

struct qunlist{
	int Issuccess;
	struct quninfor *qungroup;
};

struct RECORD{
	string username;
	int usrportrt;
	string cur_time;
	string message;
};

//struct RECORD records[20];

struct hottie{
	int picture;
	string author;
	string title;
	string text;
};

/***********the declaration of functions*************/
struct USER *dblogin(char name[32], char passwd[24]);
int registration(struct USER *pusrinfor);	// NOTICE:no parameters for sex age headpicID and email
int pwdmodify(char name[16], char newpwd[24]);		// NOTICE:?
int changeinfo(struct USER *pchginfor);// NOTICE:no parameter for sex
int createqun(string qunname, string qunmaster, int qunportrt, string qundscrb);
struct qunlist *publicqun(void);
struct qunlist *privatequn(string username);
struct quninfor *joinqun(string username, string qunname);
struct quninfor *searchqun(string qunname);
struct RECORD *showchatrcd(string username, string qunname);
char *gettime(void);
int addchatrecord(string username, string qunname, string msg);
int insert_hottie(int picID, string author, string title, string text);
struct hottie *send_hottie(int times);

#endif
