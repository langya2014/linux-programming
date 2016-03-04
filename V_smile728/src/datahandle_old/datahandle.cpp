#include <datahandle.h>
#include <string.h>
#include <stdlib.h>

extern struct user* privalUser;

void datahandle(struct user *puser)
{
	
	Value root;
	Reader reader;
	int typecode;
	string data = puser->data;
	cout<<"in datahandle: "<<data<<endl;
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
		case 500: add_job(pool, privatemsg ,puser); break;
		case 600: add_job(pool, tofocus,puser); break;
		case 620: add_job(pool, getfocuslist ,puser); break;
		case 640: add_job(pool, getfanslist ,puser); break;

		case 1000: 										//get private qunlist
		case 1100: add_job(pool, getqunlist, puser); break;	//get public qunlist
		case 1020: add_job(pool, deletequn ,puser); break;
		case 1040: add_job(pool, quitqun, puser); break;
		case 1060: add_job(pool, addqun, puser); break;
		case 1061:
		case 1062: add_job(pool, respaddqun, puser); break;
		case 1120: add_job(pool, serqun ,puser); break;
		case 1300: add_job(pool, createqun ,puser); break;
		case 1200: add_job(pool, chat, puser); break;
		case 1210: add_job(pool, getqunrecord, puser); break;
		case 2000: add_job(pool, createhottie, puser); break;//create hot tie
		case 2100: add_job(pool, getsavedhotielist, puser); break;
		case 2020: add_job(pool, gethottielist, puser); break;
		case 2080: add_job(pool, getonehottie, puser); break;
		case 2040: add_job(pool, getprivatehottielist, puser); break;
		case 2060: add_job(pool, savehottie, puser); break;

		case 3000: add_job(pool, askpk, puser); break;
		case 3001: add_job(pool, quitpk, puser); break;
		case 3002: add_job(pool, quitgame, puser); break;
		case 3003: add_job(pool, finishgame, puser); break;

		default: break;
	}
}

/*******************  start of server  ****************************/
void regist_change(void *arg) //0
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
		delete pusrinfor;
		string strdata = writer.write(ret);
		
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

void login(void *arg)//0
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

		//?
		//int p = 10;
		cout<<"ulong:"<<(unsigned long)arg<<endl;
		long p = (long)arg;
		cout<<"long:"<<p<<endl;
		plogin = dblogin(cname, cpaswd, p);
		//cout << "hhhhhhh" << endl;
		cout << plogin->username << endl;
		if(plogin->Islginsuccess == 1)
		{
			ret["typecode"] = 30;
			ret["username"] = plogin->username;
			//ret["username"] = "finder\nfinder2";
			ret["age"] = plogin->age;
			ret["headpicID"] = plogin->portrait;
			ret["facetype"] = plogin->facetype;
			ret["sex"] = plogin->sex;

		}
		else if(plogin->Islginsuccess == 2)
		{
			ret["typecode"] = 31;	
		}
		else if(plogin->Islginsuccess ==3)
		{
			ret["typecode"] = 32;
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


void privatemsg(void *arg)//-1
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		string username = root["username"].asString();
		string reciver = root["reciver"].asString();
		string content = root["Content"].asString();
	}
}
void tofocus(void *arg)//
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		string username = root["username"].asString();
		string focused = root["focused"].asString();
		int num = 0;
		if(add_focus(username, focused, num) == 1)
		{
			Value object;
			FastWriter writer;
			object["typecode"] = 610;
			object["num"] = num;
			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);			//add data to send vector
			pthread_mutex_unlock(&(puser->msg_lock));
		}
	}
}

void getfocuslist(void *arg)//
{
	Value root;
	Reader reader;
	FastWriter writer;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		Value object,flist,item;
		string username = root["username"].asString();
		int num = root["num"].asInt();
		struct fanslist* list = show_guanzhu(username, num);
		int len = list->Isquerysuccess;
		struct FAN* fanarray = list->pfanslist;
		for(int i=0;i<len;++i)
		{
			item["img"] = list->pfanslist[i].portrait;
			item["name"] = list->pfanslist[i].username;
			flist.append(item);
		}
		object["typecode"] = 0630;
		object["focused"] = flist;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
		delete[] list->pfanslist;
		delete list;
	}	
}
void getfanslist(void *arg)//
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		Value object,flist,item;
		FastWriter writer;
		string username = root["username"].asString();
		int num = root["num"].asInt();
		struct fanslist* list = show_fans(username, num);
		int len = list->Isquerysuccess;
		for(int i=0;i<len;++i)
		{
			item["img"] = list->pfanslist[i].portrait;
			item["name"] = list->pfanslist[i].username;
			flist.append(item);
		}
		object["typecode"] = 0650;
		object["focused"] = flist;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
		delete[] list->pfanslist;
		delete list;
	}	
}

void getqunlist(void *arg)//0
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
		int num = root["num"].asInt();
		Value object,qunlist,qunitem;
		FastWriter writer;
		if(typecode == 1000)	//private group
		{
			struct qunlist *p_privatequn;
			p_privatequn = privatequn(username,num); 
			int qunnum = p_privatequn->Issuccess; // get the number of private qun.
			struct quninfor *p_onequn;
			for(int i=0; i<qunnum; ++i)
			{
				p_onequn = &(p_privatequn->qungroup[i]);
				qunitem["name"] = p_onequn->qunname;
				qunitem["img"] = p_onequn->qunportrait;
				qunitem["infor"] = p_onequn->qundescrible;
				qunitem["type"] = p_onequn->type;
				qunlist.append(qunitem);
			}
			object["typecode"] = 1010;
			object["group_list"] = qunlist;
			string strdata = writer.write(object);
			
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);			//add data to send vector
			pthread_mutex_unlock(&(puser->msg_lock));
			delete[] p_privatequn->qungroup;
			delete p_privatequn;
		}
		else if(typecode == 1100)	//get the number of public qun
		{
			struct qunlist *p_publicqun; //= new qunlist();
			p_publicqun = publicqun(username, num);
			int qunnum = p_publicqun->Issuccess; // get the number of public qun.
			cout<<"qunnum:"<<qunnum<<endl;
			struct quninfor *p_onequn; //= new quninfor();
			for(int i=0;i<qunnum;++i)
			{
				p_onequn = &(p_publicqun->qungroup[i]);
				qunitem["name"] = p_onequn->qunname;
				qunitem["img"] = p_onequn->qunportrait;
				qunitem["infor"] = p_onequn->qundescrible;
				qunitem["type"] = p_onequn->type;
				qunlist.append(qunitem);
			}
			object["typecode"] = 1110;
			object["group_list"] = qunlist;
			string strdata = writer.write(object);
			
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);			//add data to send vector
			pthread_mutex_unlock(&(puser->msg_lock));
			//delete p_onequn;
			delete[] p_publicqun->qungroup;
			delete p_publicqun;
		}
	}
}

void deletequn(void *arg)//
{
	Value root;
	Reader reader;
	FastWriter writer;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		string username = root["username"].asString();
		string groupname = root["groupname"].asString();
		if(deletegroup(username, groupname) > 0)
		{
			root["typecode"] = 1030;
		}
		string strdata = writer.write(root);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

void quitqun(void *arg)//
{
	Value root,object;
	Reader reader;
	FastWriter writer;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		string username = root["username"].asString();
		string groupname = root["group_name"].asString(); 
		outofgroup(username, groupname);
		object["typecode"] = 1040;
		object["group_name"] = groupname; 
		string strdata = writer.write(root);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

void addqun(void *arg)//
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
		string requester = root["requester"].asString();
		string qunname = root["group_name"].asString();
		int groupid;
		long pmaster = requestjoin(requester, qunname, groupid);
		if(pmaster == -1)	//qunmaster is offline
		{
			object["typecode"] = 1073;
			cout<<"qunmaster is offlime"<<endl;
		}
		else 
		{
			object["typecode"] = 1070;
			object["group_name"] = qunname;			
			object["requester"] = requester;			
			puser = (struct user*)pmaster;
		}

		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	
	}
}

void respaddqun(void *arg)
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
		string requester = root["requester"].asString();
		string master = root["master"].asString();
		string qunname = root["group_name"].asString();
		int typecode = root["typecode"].asInt();
		int groupid;
		if(typecode == 1601)	//agree to add qun
		{
			long pmaster = responsejoin(1, requester, qunname);
			struct user* puser = (struct user*)pmaster;
			object["typecode"] = 1070;
			object["group_name"] = qunname;			
			object["master"] = master;			

			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);			//add data to send vector
			pthread_mutex_unlock(&(puser->msg_lock));
			
		}
		else if(typecode == 1602)
		{
			long pmaster = responsejoin(0, requester, qunname);
			struct user* puser = (struct user*)pmaster;

			object["typecode"] = 1071;
			object["group_name"] = qunname;			
			object["master"] = master;			

			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);			//add data to send vector
			pthread_mutex_unlock(&(puser->msg_lock));			
		}
	
	}	
}
void serqun(void *arg)//
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
		string groupname = root["group_name"].asString();
		struct quninfor* pgroup = searchqun(groupname);
		object["typecode"] = 1130;
		object["group_name"] = pgroup->qunname;
		object["img"] = pgroup->qunportrait;
		object["infor"] = pgroup->qundescrible;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
		delete pgroup;
	}
}

void createqun(void *arg)//0
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
		string groupname = root["group_name"].asString();
		int img = root["img"].asInt();
		cout<<"img"<<img<<endl;
		string infor = root["group_infor"].asString();

		//call dbfunction():
		cout << "group infor:"<< infor << endl;
		int ret = createqun(groupname, username, img, infor); //attention the type of qunportrait.
		if (ret == 1)
		{
			object["typecode"] = 1310;
			object["group_name"] = groupname;
		}
		else
			object["typecode"] = 1311;

		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

void chat(void *arg)//
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
		string qunname = root["group_name"].asString();
		string msg = root["msg"].asString();
		long *pointer;
		long num = 0;
		addchatrecord(username, qunname, msg, pointer,num);
cout<<"num:"<<num<<endl;
cout<<"*pointeer:"<<pointer[0]<<endl;
		for(int i=0;i<num;++num)
		{
			struct user* puser = (struct user*)pointer[i];
			if(i == num-1)
				delete pointer;
			Value object;
			object["typecode"] = 1200;
			object["username"] = username;
			object["group_name"] = qunname;
			object["msg"] = msg;

			string strdata = writer.write(object);
			cout << "huibuhui baocuo" << endl;
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);
			pthread_mutex_unlock(&(puser->msg_lock));
		}
		//delete pointer;
	}
}

void getqunrecord(void *arg)//
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		Value object, recordlist, item;
		FastWriter writer;
		string groupname = root["group_name"].asString();
		string username = root["username"].asString();
		int num = root["num"].asInt();
		
		struct recordlist* qunrcd = showchatrcd(username, groupname,num); //!should input the username
		int len = qunrcd->Isquerysuccess;
		struct RECORD* precord = qunrcd->precordlist;
		for(int i=0; i<len; ++i)
		{
			item["username"] = precord[i].username;
			item["msg"] = precord[i].message;
			item["time"] = precord[i].cur_time;
			item["img"] = precord[i].usrportrt; //add in the dbhandle.cpp.
			recordlist.append(item);
		}
			object["typecode"] = 1220;
			object["group_name"] = groupname; //qunname is above
			object["recordlist"] = recordlist;
			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);
			pthread_mutex_unlock(&(puser->msg_lock));
		
		delete[] precord;
		delete qunrcd;
	}
}

void createhottie(void *arg)//0
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
		string title = root["title"].asString();
		string text = root["content"].asString();
		int  picID = root["img"].asInt();
		string author = root["username"].asString();

		//call dbfunction():
		int ret =insert_hottie(picID, author, title, text); //where is the input parameter?
		if(ret == 1)
		{
			object["typecode"] = 2010;
		}
		else
		{
			object["typecode"] = 2011;
		}
		object["title"] = title;
	
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}
void gethottielist(void *arg)//0
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		Value object,list, item;
		FastWriter writer;
		int num = root["num"].asInt();
		struct hottielist* hotlist = send_hottie(num);
		int len = hotlist->Isquerysuccess;
		for(int i=0;i<len;++i)
		{
			item["title"] = hotlist->phottielist[i].title;
			item["img"] = hotlist->phottielist[i].picture;
			item["writer"] = hotlist->phottielist[i].author;
			list.append(item);
		}
		object["typecode"] = 2030;
		object["list"] = list;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
		delete[] hotlist->phottielist;
		delete hotlist;
	}
}

void getonehottie(void *arg)//0
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
		string title = root["title"].asString();
		string wri = root["writer"].asString();
		struct hottie *p_hottie = onehottie(wri, title); 	
		object["typecode"] = 2090;
		object["content"] = p_hottie->text;
		object["title"] =  p_hottie->title;
		object["img"] =  p_hottie->picture;

		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
	
		free(p_hottie);
	}

}

void getprivatehottielist(void *arg)//0
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		Value object,list, item;
		FastWriter writer;
		string writ = root["writer"].asString();
		int num = root["num"].asInt();
		struct hottielist* hotlist = myhottie(writ, num);
		int len = hotlist->Isquerysuccess;
		//cout<<"len:"<<len<<endl;
		for(int i=0;i<len;++i)
		{
			item["title"] = hotlist->phottielist[i].title;
			item["img"] = hotlist->phottielist[i].picture;
			list.append(item);
		}
		object["writer"] = writ;
		object["typecode"] = 2050;
		object["list"] = list;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
		delete[] hotlist->phottielist;
		delete hotlist;
	}
}

void savehottie(void *arg)//0
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
		string title = root["title"].asString();
		add_collect(username,title);
		object["typecode"] = 2070;
		object["title"] = title;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
		
	}
}

void getsavedhotielist(void *arg)//
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if (reader.parse(data, root))
	{
		Value object,list, item;
		FastWriter writer;
		string username = root["username"].asString();
		int num = root["num"].asInt();
		//struct hottielist* hotlist = myhottie(writ, num);
		int ret_len;
		struct collection* pcollect = show_collect(username,num,ret_len); 
		for(int i=0;i<ret_len;++i)
		{
			item["title"] = pcollect[i].title;
			item["writer"] = pcollect[i].author;
			list.append(item);
		}
		object["typecode"] = 2110;
		object["list"] = list;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
		delete[] pcollect;
	}
}


void askpk(void *arg)//
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
		puser->username = username;
		if(privalUser == NULL)
		{
			privalUser = puser;
			object["typecode"] = 3010;
			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);
			pthread_mutex_unlock(&(puser->msg_lock));
		}
		else if(puser != privalUser)
		{
			puser->prival = privalUser;
			object["typecode"] = 3011;
			object["rivalname"] = privalUser->username;
			string strdata = writer.write(object);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);
			pthread_mutex_unlock(&(puser->msg_lock));
			
			privalUser->prival = puser;
			object["rivalname"] = puser->username;
			strdata = writer.write(object);
			pthread_mutex_lock(&(privalUser->msg_lock));
			privalUser->msg.push_back(strdata);
			pthread_mutex_unlock(&(privalUser->msg_lock));

			privalUser = NULL;
		}
	}
}

void quitpk(void *arg)
{
//	Value object;
//	FastWriter writer;
	struct user * puser = (struct user*)arg;
	if(privalUser == puser)
		privalUser = NULL;
/*	int tatol = add_scores(username,-5);
	object["typecode"] = 4010;
	object["score"] = tatol;
	string strdata = writer.write(object);
	pthread_mutex_lock(&(puser->msg_lock));
	puser->msg.push_back(strdata);
	pthread_mutex_unlock(&(puser->msg_lock));
	*/
}

void quitgame(void *arg)
{
	struct user * puser = (struct user*)arg;
	Value object, object1;
	FastWriter writer;
	struct user* prival = puser->prival;
	object["typecode"] = 3012;
	object["rivalname"] = puser->username;
	string strdata = writer.write(object);
	pthread_mutex_lock(&(prival->msg_lock));
	prival->msg.push_back(strdata);
	pthread_mutex_unlock(&(prival->msg_lock));
	
	int tatol = add_scores(puser->username,-5);
	object1["typecode"] = 4010;
	object1["score"] = tatol;
	strdata = writer.write(object1);
	pthread_mutex_lock(&(puser->msg_lock));
	puser->msg.push_back(strdata);
	pthread_mutex_unlock(&(puser->msg_lock));

}

void finishgame(void *arg)
{
	struct user* puser = (struct user*)arg;
	Value object, object1;
	FastWriter writer;
	struct user* prival = puser->prival;
	object["typecode"] = 3013;
	object["winner"] = puser->username;
	string strdata = writer.write(object);

	int tatol = add_scores(puser->username,30);
	object1["typecode"] = 4010;
	object1["score"] = tatol;
	string strdata1 = writer.write(object1);

	pthread_mutex_lock(&(puser->msg_lock));
	puser->msg.push_back(strdata);
	puser->msg.push_back(strdata1);
	pthread_mutex_unlock(&(puser->msg_lock));
	//puser->prival = NULL;

	tatol = add_scores(prival->username,15);
	object1["score"] = tatol;
 	strdata1 = writer.write(object1);

	pthread_mutex_lock(&(prival->msg_lock));
	prival->msg.push_back(strdata);
	prival->msg.push_back(strdata1);
	pthread_mutex_unlock(&(prival->msg_lock));
	//prival->prival = NULL;
}

void addfen(void*arg)
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

		int tatol = add_scores(username,10);
		object["typecode"] = 4010;
		object["score"] = tatol;
		string strdata = writer.write(object);
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);
		pthread_mutex_unlock(&(puser->msg_lock));
	}	
}