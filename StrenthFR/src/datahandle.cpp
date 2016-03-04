#include <datahandle.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include "../include/base64.h"

extern struct user* privalUser;

void datahandle(struct user *puser)
{
	
	Value root;
	Reader reader;
	int typecode;
	string data = puser->data;
	//cout<<"in datahandle: "<<data<<endl;
	if(reader.parse(data,root))
		typecode = root["typecode"].asInt();
	//cout<<"typecode:"<<typecode<<endl;
	if(typecode == 0)
	{
		cout<<"data is wrong"<<endl;
	}
	//if(typecode == 90)
		//cout <<"in datahandle: typecode is 90, client is sending image!" << endl;
	if(typecode != 90)
		cout << "in datahandle: " << data << endl;
	switch(typecode)
	{
		case 1:												//regist
		case 40: add_job(pool, regist_change, puser); break;	//change information
		case 20: add_job(pool, login, puser); break;		//login
		case 50: add_job(pool, UpdateClass, puser); break;
		case 60: add_job(pool, RequestClass, puser); break;
		case 70: add_job(pool, RequestImage, puser); break;
		case 80: add_job(pool, logout, puser); break;
		case 90: add_job(pool, ClientSendImage, puser); break;
		
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
		//string email = root["email"].asString();
		int sex = root["sex"].asInt();
		
		struct USER *pusrinfor;
		pusrinfor = new USER();
		//pusrinfor = (struct USER *)malloc(sizeof(struct USER));
		pusrinfor->username = name;
		pusrinfor->passwd = paswd;
		//pusrinfor->mail = email;
		pusrinfor->sex = sex;

		if((name == "")||(paswd == ""))		//data is wrong
		{
			cout<<"in regist,data is wrong"<<endl;
			ret["typecode"] = 11;
		}
		else if(typecode == 1)		//regist
		{	
			int flag = registration(pusrinfor);
			if(flag == 1)
			{
				ret["typecode"] = 10;
				
				string image_name = name + ".png";
				string image_path = "/home/StrenthFR/user_image/" + image_name;
				FILE *fd = fopen(image_path.c_str(), "wb");
				if(fd == NULL)
					cout << "create the image, error!" << endl;
				
				fclose(fd);
			}	
			else 
				ret["typecode"] = 11;
		}
			
		else						//change information
		 {
		 	int flag;
			//cout << "ccccccc" << endl;
			flag = changeinfo(pusrinfor);
		 	if(flag == 1)
				ret["typecode"] = 41;
			else
				ret["typecode"] = 42;
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
	long temp = (long)puser;
	puser = (struct user*)temp;

	cout << "in login function\n";
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		/******************登陆*********************/
		Value ret,object,ret1;
		FastWriter writer;
		string name = root["username"].asString();
		puser->username = name;
		string paswd = root["password"].asString();
		if((name == "")||(paswd == ""))			//data is wrong
		{
			cout<<"in login,data is wrong"<<endl;
			return ;
		}
		
		//cout << "hahahahahhahaah\n";
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
		long p = (long)puser;
		plogin = dblogin(cname, cpaswd, p);
		//cout << "after dblogin function" << endl;
		cout << "username: "<< plogin->username << endl;
		if(plogin->Islginsuccess == 1)
		{
			ret["typecode"] = 30;
			//ret["username"] = plogin->username;
			//ret["username"] = "finder\nfinder2";
			//ret["sex"] = plogin->sex;

				/**********************************返回积分********************************/
			/*int tatol = add_scores(name,0);
			ret1["typecode"] = 4010;
			ret1["score"] = tatol;
			string strdata = writer.write(ret1);
			pthread_mutex_lock(&(puser->msg_lock));
			puser->msg.push_back(strdata);
			pthread_mutex_unlock(&(puser->msg_lock));*/
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

void UpdateClass(void *arg)
{
	Value root;
	Reader reader;
	
	struct user *puser = (struct user *)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data, root))
	{
		Value ret;
		FastWriter writer;
		
		int typecode = root["typecode"].asInt();
		string name = root["username"].asString();
		string classname = root["classname"].asString();
		int roll_call = root["roll_call"].asInt();
		int attendace = root["attendance"].asInt();
		
		if((name == "")||(classname == ""))		//data is wrong
		{
			cout<<"in UpdateClass, data is wrong"<<endl;
			ret["typecode"] = 52;
		}
		else
		{
			struct CLASS_INFOR *pClassInfor;
			pClassInfor = new CLASS_INFOR();
			pClassInfor->classname = classname;
			pClassInfor->roll_call = roll_call;
			pClassInfor->attendace = attendace;
			
			cout << "attendace:" << attendace<< " pClassInfor->attendace:"<< pClassInfor->attendace << endl;
			int flag = update_class(name, pClassInfor);
			if(flag == -1)
			{
				cout << "in UpdateClass, update failure!" << endl;
				ret["typecode"] = 52;
			}
			else{
				ret["typecode"] = 51;
			}
			
			delete pClassInfor;
		}
		
		string strdata = writer.write(ret);
		
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	}	
}

void RequestClass(void *arg)
{
	Value root;
	Reader reader;
	
	struct user *puser = (struct user *)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data, root))
	{
		Value ret;
		FastWriter writer;
		int typecode = root["typecode"].asInt();
		string name = root["username"].asString();
		string classname = root["classname"].asString();
		
		if((name == "")||(classname == ""))		//data is wrong
		{
			cout<<"in RequestClass, data is wrong"<<endl;
			ret["typecode"] = 62;
		}
		else
		{
			struct CLASS_INFOR *pClassInfor;
			pClassInfor = new CLASS_INFOR();
			pClassInfor = RequestClassInfor(name, classname);
			
			if(pClassInfor->classname == "NULL")
			{
				cout << "classname is Null, request failure!" << endl;
				ret["typecode"] = 62;
			}
			else{
				ret["typecode"] = 61;
				ret["username"] = name;
				ret["classname"] = classname;
				ret["roll_call"] = pClassInfor->roll_call;
				ret["attendace"] = pClassInfor->attendace;
			}
			
			delete pClassInfor;
		}
		
		string strdata = writer.write(ret);
		
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

void RequestImage(void *arg)
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
		
		string image_name = name + ".png";	//the format of image is not sure.
		string image_path = "/home/StrenthFR/user_image/" + image_name;
		FILE *fd = fopen(image_path.c_str(), "r");
		if(fd == NULL)
		{
			cout << "this user did not have image, error!" << endl;
			ret["typecode"] = 72;
		}
		/*int fd = open(image_path.c_str(), O_RDWR);
		if(fd == -1)
		{
			cout << "this user did not have image, error!" << endl;
			ret["typecode"] = 72;
		}*/
		else{
			int image_size = get_file_size(image_path.c_str());
			ret["typecode"] = 71;
			ret["image"] = image_name;
			ret["image_size"] = image_size;
			
			char ch[1030];
			int seq = 0;
			int sent_bytes = 0;
			string content;
			while(!feof(fd))
			{
				//int len = fread(ch,1,1024,fd);
				//int len = fread(ch,1024, 1,fd);
				int len = fread(ch, 1, 693,fd);
				
				//int len = read(fd, ch, 1024);
				//int len = fread((void *)const_cast<char *>(content.c_str()),1024, 1, fd);
				vector<char> vec_buf;
				for(int i=0; i<len; i++)
				{
					vec_buf.push_back(ch[i]);
				}
				string s;
				s.assign(vec_buf.begin(), vec_buf.end());
				
				//content = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
				content = s;
				
				ret["seq"] = ++seq;
				ret["content"] = content;
				sent_bytes += content.size();
				cout << "len: " << len << "content size: " << content.size() << endl;
				
				
				/*ret["content"] = ch;
				sent_bytes += strlen(ch);
				cout << "len: " << len << "content size: " << sizeof(ch) << endl;*/
				
				string strdata = writer.write(ret);
		
				pthread_mutex_lock(&(puser->msg_lock));
				puser->msg.push_back(strdata);			//add data to send vector
				pthread_mutex_unlock(&(puser->msg_lock));
				//cout << "send "<< lenth << "bytes data!" << endl;
				
			}
			cout << "image send "<< sent_bytes << "bytes!" << endl;
		}
		
		fclose(fd);
	}
}

void ClientSendImage(void *arg)
{
	//cout << "enter ClientSendImage function()\n";
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	//cout << "aaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	//cout << "bbbbbbbbbbbbbbbbbbbbbbbbbb" << endl;
	if(reader.parse(data,root))
	{
		Value ret;
		FastWriter writer;
		int typecode = root["typecode"].asInt();
		string name = root["username"].asString();
		int seq = root["seq"].asInt();
		string content = root["content"].asString();
		//cout << "name: "<< name << " seq: " << seq << endl;
		
		if(name == "" || content == "")
		{
			cout << "in ClientSendImage, data is wrong!" << endl;
			ret["typecode"] = 92;
			ret["seq"] = seq;
		}
		else
		{
			string image_name = name + ".png";
			string image_path = "/home/StrenthFR/user_image/" + image_name;
			FILE *fd = fopen(image_path.c_str(), "ab");
			if(fd == NULL)
			{
				cout << "open the image, error!" << endl;
				ret["typecode"] = 92;
				ret["seq"] = seq;
			}
			
			int fseek_ret = fseek(fd, 0L, SEEK_END);
			//int fwrite_ret = fwrite(content.c_str(), 1, content.size(), fd);
			int fwrite_ret = fwrite(content.c_str(), content.size(), 1, fd);
			/*string decoded = base64_decode(content);
			int fwrite_ret = fwrite(decoded.c_str(), 1, decoded.size(), fd);
			cout << "fseek_ret:" << fseek_ret <<" content length: " << content.length() << endl; 
			cout << "decoded size: "<< decoded.size() << " decoded length: " << decoded.length() << " ret: " << fwrite_ret << endl;
			cout << "after fwrite!*****" << endl;*/
			//fwrite(content.c_str(), 1, content.size(), fd);
			
			ret["typecode"] = 91;
			ret["seq"] = seq;
			
			fclose(fd);
		}
		
		string strdata = writer.write(ret);
		
		pthread_mutex_lock(&(puser->msg_lock));
		puser->msg.push_back(strdata);			//add data to send vector
		pthread_mutex_unlock(&(puser->msg_lock));
	}
}

void logout(void *arg)
{
	Value root;
	Reader reader;
	struct user * puser = (struct user*)arg;
	long temp = (long)puser;
	puser = (struct user*)temp;


	pthread_mutex_lock(&(puser->msg_lock));
	string data = puser->data;
	pthread_mutex_unlock(&(puser->msg_lock));
	if(reader.parse(data,root))
	{
		string name = root["username"].asString();
		setsocket(name);
	}
}

int get_file_size(const char *path)
{
	int filesize = -1;
	struct stat statbuff;
	if(stat(path, &statbuff) < 0)
	{
		return filesize;
	}
	else{
		filesize = statbuff.st_size;
		return filesize;
	}
}
