#include "datahandle.h"

#include <sys/socket.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>

int dblogin(char name[16], char passwd[24]);

void datahandle(struct user *puser)
{
	cout<<"in datahandle"<<endl;
	Value root;
	Reader reader;
	int typecode;
	string data = puser->data;
	if(reader.parse(data,root))
		typecode = root["typecode"].asInt();
	/*if(typecode == 0)
	{
		cout<<"data is wrong"<<endl;
	}*/
	switch(typecode)
	{
		case 0: add_job(pool, login, puser); break;
		default: break;
	}
}

void login(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		string name = root["name"].asString();
		string paswd = root["paswd"].asString();
		if((name == "")||(paswd == ""))
		{
			cout<<"in login,data is wrong"<<endl;
			return ;
		}
		cout<<"name is:"+name+"  password is:"+paswd<<endl;		//for testing


		/*
			call database function with parameters :name, paswd
			and return string data;
		*/
		int len = name.length();
		char* cname = (char*)malloc(len);
		name.copy(cname,len,0);
		cname[len] = '\0';
		len = paswd.length();
		char* cpaswd = (char*)malloc(len);
		paswd.copy(cpaswd,len,0);
		cpaswd[len] = '\0';

		dblogin(cname, cpaswd);

		free(cname);
		free(cpaswd);

		//add data to send vector
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(data);
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

int dblogin(char name[16], char passwd[24])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	cout << "hhhhhh" << endl;
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "xirujun", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}
	cout << "iiiiii" << endl;
	while(row = mysql_fetch_row(res))
	{
		int pname; 
		cout<<row[1]<<"  "<<name<<endl;
		pname = strcmp(row[1], name);
		if(pname == 0)
		{
			//int ppasswd;
			if(!strcmp(row[9], passwd))
			{
				cout << "login success!" << endl;				
				return 1;	//passwd correct
			}
			else
			{
				cout << "passwd error, login failure!" << endl;
				return 2;	//passwd error
			}
			
		}
		//break;
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}
