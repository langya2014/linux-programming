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
	int type; 	//0 for user created, 1 for user joined, 2 for user didn't join.
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

/*********** join gourp msg 20150517 *************/
struct joinmsg{
	string requester;
	string groupname;
};

struct msglist{
	int num;
	struct joinmsg *pjoinmsg;
};

/*********** new struct 20150414 *************/
struct FAN{	
	//int Isquerysuccess;
	string username;
	int sex;
	int age;
	int portrait;
};

struct fanslist{
	int Isquerysuccess;
	struct FAN *pfanslist;
};

struct hottielist{
	int Isquerysuccess;
	struct hottie *phottielist;
};

struct recordlist{
	int Isquerysuccess;
	struct RECORD *precordlist;
};

struct collection{
	string author;
	string title;
};

/***********basic functions*************/
struct USER *dblogin(char name[32], char passwd[24], long socket);
int registration(struct USER *pusrinfor);	// NOTICE:no parameters for sex age headpicID and email
int pwdmodify(char name[16], char newpwd[24]);		// NOTICE:?
int changeinfo(struct USER *pchginfor);// NOTICE:no parameter for sex

/***********group functions*************/
int createqun(string qunname, string qunmaster, int qunportrt, string qundscrb);
struct qunlist *publicqun(string username, int num); 
struct qunlist *privatequn(string username, int num);
//struct quninfor *joinqun(string username, string qunname);
struct quninfor *searchqun(string qunname);
struct recordlist *showchatrcd(string username, string qunname, int num); //show group chat record.
char *gettime(void);
//int addchatrecord(string username, string qunname, string msg);
int addchatrecord(string username, string qunname, string msg, long *&skt, long &online_num);
/********************** join group function *********************/
long requestjoin(string username, string groupname, int &groupID); //return the socket of group master.
long responsejoin(int agreeflag, string username, string groupname); //return the socket of requester
//agreeflag = 1 mean the group master agree with the join request.
/************************* quit the group function  ************************************/
int deletegroup(string username, string groupname); 
int outofgroup(string username, string groupname);

/***********hottie functions*************/
int insert_hottie(int picID, string author, string title, string text);
struct hottielist *send_hottie(int num);
//int add_collect(int picID, string author, string title, string text);	//did the collect represent the hottie.
string add_collect(string username, string title);
struct collection *show_collect(string collector, int num, int &return_num);  //return_num represents the number of return collection.
struct hottielist *myhottie(string writer, int num); 	//success return 10 hottie.
struct hottie *onehottie(string writer, string title); 	//look a single hottie.

/************************* focus and fans function  ************************************/
long add_focus(string myusername, string focusedname, int &focusnum); 	//return the socket of focused user.
struct fanslist *show_guanzhu(string myusername, int num); //show 10 focused user;
long add_fans(string myusername, string fansname, int &fansnum); //return the fans socket.
struct fanslist *show_fans(string myusername, int num); 	//show 10 fans user;

/************************* scores function  ************************************/
int add_scores(string username, int addscore); 	//input the username, return the total scores.

/************************* game function  ************************************/
int creategame(string requester); 	//return the game_id, search the qun the requester belong to.
int joingame(int game_id, string player, long &socket); 		//success return the game_id 
int setwinner(int game_id, string player, long &winnerskt, long &loserskt); 		//success return the game_id

/************************* private chat function  ************************************/
int addprivatechat(string speaker, string listener, string content); 	//return the socket of the listener.

/************************* message handle function about join qun  ************************************/
long Istheremsg(string groupmaster);	//according the groupmaster search the join gourp msg. 
long msghandle(string groupmaster, string &requester, string &groupname);		//return requeser and groupname
long getusrskt(string username);
struct msglist *query(string groupmaster);		//return the join group list.
#endif