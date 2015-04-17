#include "../include/datahandle.h"

//#include <iostream>
#include <string.h>
#include <stdlib.h>

void datahandle(struct user *puser)
{
	
	Value root;
	Reader reader;
	int typecode;
	string data = puser->data;
	cout<<"in datahandle"<<data<<endl;
	if(reader.parse(data,root))
		typecode = root["typecode"].asInt();
	cout<<"typecode:"<<typecode<<endl;
	if(typecode == 0)
	{
		cout<<"data is wrong"<<endl;
	}
	switch(typecode)
	{
		case 1:												//regist
		case 40: add_job(pool, regist_change, puser); break;	//change information
		case 20: add_job(pool, login, puser); break;		//login
		case 60: 											//get private qunlist
		case 80: add_job(pool, getqunlist, puser); break;	//get public qunlist
		case 100:											//add qun
		case 120: add_job(pool, ser_add_qun, puser); break;	//search qun
		case 140: add_job(pool, gethottie, puser); break;	//get hot tie
		case 160: add_job(pool, createqun, puser); break; 	//create qun
		case 180: add_job(pool, getqunrecord, puser); break; //get private qun records
		case 200: add_job(pool, chat, puser); break; 		//chat in qun
		case 210: add_job(pool, createhottie, puser); break;//create hot tie
		default: break;
	}
}

/*******************  start of server  ****************************/
void regist_change(void *arg)
{	
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		Value ret;
		FastWriter writer;
		int typecode = root["typecode"].asInt();
		string name = root["username"].asString();
		string paswd = root["password"].asString();
		string email = root["email"].asString();
		int age = root["age"].asInt();
		int facetype = root["facetype"].asInt();
		int sex = root["sex"].asInt();
		int headpicID = root["headpicID"].asInt();
		
		//cout << "sssssstrut" << endl;
		struct USER *pusrinfor;
		pusrinfor = new USER();
		//pusrinfor = (struct USER *)malloc(sizeof(struct USER));
		pusrinfor->username = name;
		pusrinfor->passwd = paswd;
		pusrinfor->mail = email;
		pusrinfor->sex = sex;
		pusrinfor->age = age;
		pusrinfor->facetype = facetype;
		pusrinfor->portrait = headpicID;

		if((name == "")||(paswd == ""))		//data is wrong
		{
			cout<<"in login,data is wrong"<<endl;
			ret["typecode"] = 11;
		}
		else if(typecode == 1)		//regist
		{
			//cout << "rrrrrr" << endl;	
			int flag = registration(pusrinfor);
			if(flag == 1)
				ret["typecode"] = 10;
			else 
				ret["typecode"] = 11;
		}
			
		else						//change information
		 {
		 	int flag;
			//cout << "ccccccc" << endl;
			flag = changeinfo(pusrinfor);
		 	if(flag == 1)
				ret["typecode"] = 50;
			else
				ret["typecode"] = 51;
		}
		//cout << "why????" << endl;	
		//free(pusrinfor);
		delete pusrinfor;
		//cout << "hhhhhhh" << endl;
		string strdata = writer.write(ret);
		
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
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
		Value ret;
		FastWriter writer;
		string name = root["username"].asString();
		string paswd = root["password"].asString();
		if((name == "")||(paswd == ""))			//data is wrong
		{
			cout<<"in login,data is wrong"<<endl;
			return ;
		}
		int len = name.length();
		char* cname = (char*)malloc(len);
		name.copy(cname,len,0);
		cname[len] = '\0';
		len = paswd.length();
		char* cpaswd = (char*)malloc(len);
		paswd.copy(cpaswd,len,0);
		cpaswd[len] = '\0';
		
		struct USER *plogin;
		//plogin = (struct USER *)malloc(sizeof(struct USER *));
		plogin = new USER();
		plogin = dblogin(cname, cpaswd);
		//cout << "hhhhhhh" << endl;
		cout << plogin->username << endl;
		if(plogin->Islginsuccess == 1)
		{
			ret["typecode"] = 30;
			ret["username"] = plogin->username;
			ret["age"] = plogin->age;
			ret["headpicID"] = plogin->portrait;
			ret["facetype"] = plogin->facetype;
			ret["sex"] = plogin->sex;

		}
		else
		{
			ret["typecode"] = 31;	
		}

		free(cname);
		free(cpaswd);
		//free(plogin);
		delete plogin;
		string strdata = writer.write(ret);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

void getqunlist(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		int typecode = root["typecode"].asInt();
		string username = root["username"].asString();
		Value object;
		FastWriter writer;
		if(typecode == 60)
		{
			//call dbfuntion();
			struct qunlist *p_privatequn;
			//p_privatequn = (struct qunlist *)malloc(sizeof(struct qunlist));
			//p_privatequn = new qunlist();
			p_privatequn = privatequn(username); 
			int qunnum = p_privatequn->Issuccess; // get the number of private qun.
			struct quninfor *p_onequn;
			//p_onequn = (struct quninfor*)malloc(sizeof(struct quninfor));
			p_onequn = new quninfor();
			for(int i=0; i<qunnum; ++i)
			{
				p_onequn = &(p_privatequn->qungroup[i]);
				object["typecode"] = 70;
				object["Qunname"] = p_onequn->qunname;
				object["touxiangID"] = p_onequn->qunportrait;
				object["Describe"] = p_onequn->qundescrible;

				string strdata = writer.write(object);
				
				pthread_mutex_lock(&(puser->msg_lock));
				puser->msg.push_back(strdata);			//add data to send vector
				pthread_mutex_unlock(&(puser->msg_lock));
			}
			//free(p_onequn);
			delete p_onequn;
			//free(p_privatequn);
			delete[] p_privatequn->qungroup;
			delete p_privatequn;
		}
		else if(typecode == 80)
		{
			//call dbfunction();
			struct qunlist *p_publicqun; //= new qunlist();
			//p_publicqun = (struct qunlist *)malloc(sizeof(struct qunlist));
			
			p_publicqun = publicqun();
			struct quninfor *p_onequn;
			//p_onequn = (struct quninfor*)malloc(sizeof(struct quninfor));
			p_onequn = new quninfor();
			for(int i=0;i<10;++i)
			{
			p_onequn = &(p_publicqun->qungroup[i]);
			object["typecode"] = 90;
			object["Qunname"] = p_onequn->qunname;
			object["touxiangID"] = p_onequn->qunportrait;
			object["Describe"] = p_onequn->qundescrible;

			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);			//add data to send vector
			pthread_mutex_unlock(&(puser->msg_lock));
			}
			//free(p_onequn);
			delete p_onequn;
			//free(p_publicqun);
			delete[] p_publicqun->qungroup;
			delete p_publicqun;
		}
	}
}

void ser_add_qun(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{	
		Value object;
		FastWriter writer;
		string username = root["username"].asString();
		string qunname = root["Qunname"].asString();
		
		struct quninfor *p_quninfor;
		//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
		p_quninfor = new quninfor();

		int typecode;
		if(typecode == 100)	//add qun
		{
			//call dbfunction():
			p_quninfor = joinqun(username, qunname); 
			object["typecode"] = 110;
			object["Qunname"] = p_quninfor->qunname;
			object["Touxiang"] = p_quninfor->qunportrait;
			object["Describe"] = p_quninfor->qundescrible;
			
		}
		else if(typecode == 120)	//search
		{
			//call dbfuntion()
			p_quninfor = searchqun(qunname);
			object["typecode"] = 130;
			object["Qunname"] = p_quninfor->qunname;
			object["Touxiang"] = p_quninfor->qunportrait;
			object["Describe"] = p_quninfor->qundescrible;
		}

		//free(p_quninfor);
		delete p_quninfor;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	
	}
}

/************* new ****************/

void gethottie(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		int typecode = root["typecode"].asInt();
		Value object;
		FastWriter writer;


		//call dbfunction(): 5 OR 10 times
		struct hottie *p_hottie;
		struct hottie *hotties;
		//struct hottie hotties[5];
		p_hottie = (struct hottie *)malloc(sizeof(struct hottie));
		hotties = (struct hottie *)malloc(5*sizeof(struct hottie));
		hotties = send_hottie(5); //return 5 hotties;
		
		int i = 0;
		for(int i=0; i<5; ++i)
		{
			p_hottie = &(hotties[i]);
			object["typecode"] = 150;
			object["Content"] = p_hottie->text;
			object["Title"] =  p_hottie->title;
			object["photo"] =  p_hottie->picture;

			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);
			pthread_mutex_unlock(&(puser->msg_lock));
		
		}
		free(p_hottie);
	}

}

void createqun(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		Value object;
		FastWriter writer;
		string qunname = root["Qunname"].asString();
		string username = root["username"].asString();
		int qunportrait = root["Touxiang"].asInt();
		string describe = root["Describe"].asString();

		//call dbfunction():
		createqun(qunname, username, qunportrait, describe); //attention the type of qunportrait.
		int ret = 1;
		if (ret == 1)
			object["typecode"] = Value(170);
		else
			object["typecode"] = 171;


		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
	}

}

void getqunrecord(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		Value object;
		FastWriter writer;
		string qunname = root["Qunname"].asString();
		string username = root["username"].asString();

		//call dbfunction():
		struct RECORD *onercd = new RECORD();
		//onercd = (struct RECORD *)malloc(sizeof(struct RECORD));
		struct RECORD *qunrcd = new RECORD[20];
		//struct RECORD qunrecord[20];
		//qunrcd = (struct RECORD *)malloc(20*sizeof(struct RECORD));
		//qunrcd = qunrecord;
		
		qunrcd = showchatrcd(username, qunname); //!should input the username
		
		for(int i=0; i<20; ++i)
		{
			onercd = &(qunrcd[i]);
			object["typecode"] = 190;
			object["username"] = onercd->username;
			object["msg"] = onercd->message;
			object["time"] = onercd->cur_time;
			object["Touxiang"] = onercd->usrportrt; //add in the dbhandle.cpp.
			object["Qunname"] = root["Qunname"]; //qunname is above


			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);
			pthread_mutex_unlock(&(puser->msg_lock));
		}
		
		delete onercd;
		//free(onercd);
		delete[] qunrcd;
		//free(qunrcd);
	}
}

void chat(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		Value object;
		FastWriter writer;
		string username = root["username"].asString();
		string qunname = root["Qunname"].asString();
		string msg = root["msg"].asString();

		//call dbfunction():
		addchatrecord(username, qunname, msg);
	/*	object["typecode"] = 190;
		object["username"] = "lingxisf";
		object["msg"] = "good morning,how are you? fuck you ,mother fucker,come on, bitch,son of bitch";
		object["time"] = "9.8";
		object["Touxiang"] = 23;
		object["Qunname"] = root["Qunname"];
		*/
		//!!!!!!need display the current chat msg?
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
			
	}
}

void createhottie(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	cout<<"aaaaaaaaa"<<endl;
	if(reader.parse(data,root))
	{
		int typecode = root["typecode"].asInt();
		Value object;
		FastWriter writer;
		cout<<"ccccccccccccc"<<endl;
		string title = root["Title"].asString();
		string text = root["Content"].asString();
		int  picID = root["photo"].asInt();
		string author = root["username"].asString();

		//call dbfunction():
		insert_hottie(picID, author, title, text); //where is the input parameter?
		object["typecode"] = 220;
	
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

/************** end of new ************/

/***********************  end of server  ***********************************/
