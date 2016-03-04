#include "../include/dbhandle.h"

/***********************  start about database  ******************************/
struct USER *dblogin(char name[64], char passwd[32], long socket)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	my_ulonglong numrow;

	struct USER *lginres;
	//lginres = (struct USER *)malloc(sizeof(struct USER *));
	lginres = new USER();
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "facetech", "strenthFR", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		lginres->Islginsuccess = 0;
		return lginres;
	}

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		lginres->Islginsuccess = 0;
		return lginres;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		lginres->Islginsuccess = 0;
		return lginres;
	}

	numrow = mysql_num_rows(res);
	int i = 0;
	while(row = mysql_fetch_row(res))
	{
		int pname; 
		//cout<<row[1]<<"  "<<name<<endl;
		pname = strcmp(row[1], name);
		if(pname == 0)
		{
			//int ppasswd;
			if(!strcmp(row[3], passwd))
			{
				cout << "login success!" << endl;				
				//passwd correct
				lginres->Islginsuccess = 1;
				lginres->username = row[1];
				//lginres->sex = atoi(row[2]);
				//lginres->mail = row[3];
				
				/* store the user's socket */
				stringstream ss;
				ss<<socket;
				string skt = ss.str();
				string username = name;
				string cmd = "update users set socket='"+skt+"' where username='"+username+"'";
				cout << cmd.c_str() << endl;
				if(mysql_query(&mysql, cmd.c_str()))
				{
					cout << "update socket failure!" << endl;
					lginres->Islginsuccess = 0;
					return lginres;
				}
				else
					return lginres;
			}
			else
			{
				cout << "passwd error, login failure!" << endl;
				//passwd error
				lginres->Islginsuccess = 2;
				lginres->username = name;
				return lginres;
			}
			
		}
		++i;
		if(i == numrow)
		{
			cout << "this user does not exist, please check the user name!" << endl;
			lginres->Islginsuccess = 3;
			lginres->username = name;
			return lginres;
		}
	}
	
	//free(lginres);
	delete lginres;
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int registration(struct USER *pusrinfor)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	/********get the information of the user *********/
	string pwd, name, mail;
	int sex;
	name = pusrinfor->username;
	pwd = pusrinfor->passwd;
	//mail = pusrinfor->mail;
	sex = pusrinfor->sex;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "facetech", "strenthFR", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select username from users", (unsigned long)strlen("select username from users")))
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

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[0], name.c_str());
		if(pname == 0)
		{
			cout << "this username exist!" << endl;
			return 2;
		}
		//break;
	}
	
	stringstream ss1;
	ss1<<sex; 
	string ssex = ss1.str();
	
	string comd = "insert into users (username,sex,passwd) values('"+name+"','"+ssex+"','"+pwd+"')";
	//string comd = "insert into users (username,passwd) values('"+name+"','"+pwd+"')";
//	string comd = "insert into users (username, mail, passwd)"
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "register success!" << endl;
		
		string class_tab = name + "_" + "class";
		string cmd = "create table "+class_tab+" (id int unsigned not null auto_increment primary key,classname varchar(128) not null,roll_call int not null,attendace int not null)";
		cout << cmd.c_str() << endl;
		if(mysql_query(&mysql, cmd.c_str()))
		{
			cout << "create "<< class_tab << "failure!" << endl;
			return 3;
		}
		
		string cmd2 = "insert into "+class_tab+" (classname, roll_call, attendace) values('ImageProcessing', 2, 0)";
		if(mysql_query(&mysql, cmd2.c_str()))
		{
			cout << "insert into initial class information failure!" << endl;
			return 3;
		}
		
		return 1;
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int changeinfo(struct USER *pchginfor)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	string pwd, name, mail;
	int sex;
	name = pchginfor->username;
	pwd = pchginfor->passwd;
	//mail = pchginfor->mail;
	sex = pchginfor->sex;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "facetech", "strenthFR", 3306, 0, 0))
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

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[1], name.c_str());
		if(pname == 0)
		{
			/*string spwd, sname;
			spwd = newpwd; 
			sname = name;*/

			stringstream ss1;
			ss1<<sex;
			string ssex = ss1.str();

			string comd = "update users set sex='"+ssex+"',password='"+pwd+"' where username='"+name+"'";
			//string comd = "update users set password='"+pwd+"' where username='"+name+"'";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				
				cout << "change information failure!" << endl;
				return 0;
			}
			else
			{
				cout << "information modify success!" << endl;
				return 1;
			}
			//row[9] = newpwd;
			//cout << "psd modify success!" << endl;
			//return 1;	
		}
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int update_class(string name, struct CLASS_INFOR * pClassinfor)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	my_ulonglong numrow;
	
	string classname = pClassinfor->classname;
	int roll_call, attendace;
	roll_call = pClassinfor->roll_call;
	attendace = pClassinfor->attendace;
	
	string class_tab = name + "_" + "class";
	stringstream ss1, ss2;
	ss1<<roll_call;		ss2<<attendace;
	string s_rc = ss1.str();
	string s_atd = ss2.str();
	//cout << "s_atd: "<< s_atd<< " attendace: "<< attendace<< endl;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "facetech", "strenthFR", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return -1;
	}
	
	string cmd = "select * from "+class_tab+" where classname='"+classname+"'";
	cout << cmd.c_str() << endl;
	if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
	{
		cout << "query "<< class_tab << "failure!" << endl;
		mysql_close(&mysql);
		return -1;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		mysql_close(&mysql);
		return -1;
	}
	
	numrow = mysql_num_rows(res);
	if(numrow == 0)
	{
		string cmd = "insert into "+class_tab+" (classname,roll_call,attendace) values('"+classname+"', 2,"+s_atd+")";
		cout << cmd.c_str() << endl;
		if(mysql_query(&mysql, cmd.c_str()))
		{
			cout << "insert into " << class_tab << " failure!" << endl;
			mysql_close(&mysql);
			return -1;
		}
		else{
			cout << "update(insert) class table success!" << endl;
			mysql_close(&mysql);
			return 0;
		}
	}
	else{
		row = mysql_fetch_row(res);
		int cur_attendance = atoi(row[3]);
		
		if(attendace <= cur_attendance)
		{
			cout << "update class table success!" << endl;
			mysql_close(&mysql);
			return 0;
		}
		
		string cmd = "update "+class_tab+" set roll_call="+s_rc+",attendace="+s_atd+" where classname='"+classname+"'";
		cout << cmd.c_str() << endl;
		if(mysql_query(&mysql, cmd.c_str()))
		{
			cout << "update class table failure!" << endl;
			mysql_close(&mysql);
			return -1;
		}
		else{
			cout << "update class table success!" << endl;
			mysql_close(&mysql);
			return 0;
		}
	}
	
}

struct CLASS_INFOR *RequestClassInfor(string name, string classname)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	my_ulonglong numrow;
	
	struct CLASS_INFOR *pClassinfor;
	pClassinfor = new CLASS_INFOR();
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "facetech", "strenthFR", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		pClassinfor->classname = "NULL";
		mysql_close(&mysql);
		return pClassinfor;
	}
	
	string class_tab = name + "_" + "class";
	string cmd = "select * from "+class_tab+" where classname='"+classname+"'";
	cout << cmd.c_str() << endl;
	if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
	{
		cout << "query "<< class_tab << "failure!" << endl;
		pClassinfor->classname = "NULL";
		mysql_close(&mysql);
		return pClassinfor;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		pClassinfor->classname = "NULL";
		mysql_close(&mysql);
		return pClassinfor;
	}
	
	numrow = mysql_num_rows(res);
	if(numrow == 0)
	{
		cout << "this class record not exist!" << endl;
		pClassinfor->classname = "NULL";
		mysql_close(&mysql);
		return pClassinfor;
	}
	else{
		row = mysql_fetch_row(res);
		cout << "query class information success!" << endl;
		pClassinfor->classname = row[1];
		pClassinfor->roll_call = atoi(row[2]);
		pClassinfor->attendace = atoi(row[3]);
		cout<<pClassinfor->classname<<" "<<pClassinfor->roll_call<<" "<<pClassinfor->attendace<<endl;

		mysql_close(&mysql);
		return pClassinfor;
	}
}

char *gettime(void)
{
	time_t now;
	//char *ctime();

	(void) time(&now);
	return ctime(&now);
}

int setsocket(string username)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "facetech", "strenthFR", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return -1;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return -1;
	}

	//int readmsgid;
	while(row = mysql_fetch_row(res))
	{
		int puser = strcmp(row[1], username.c_str());
		//int pgroup = strcmp(row[2], groupname.c_str());
		if(puser == 0)
		{
			//readmsgid = atoi(row[4]);
			string cmd = "update users set socket=-1 where username='"+username+"'";
			cout << cmd.c_str() << endl;
			if(mysql_query(&mysql, cmd.c_str()))
			{
				cout << "update users' socket failure!" << endl;
				return -1;
			}
			else{
				cout << "update users' socket success!" << endl;
				return 0;
			}
			//break;
		}
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);
}

int set_allskt(void)
{
	MYSQL mysql;
	//MYSQL_RES *res;
	//MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "facetech", "strenthFR", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	string cmd = "update users set socket=-1";
	cout << cmd.c_str() << endl;
	if(mysql_query(&mysql, cmd.c_str()))
	{
		cout << "set all socket -1 failure!" << endl;
		return -1;
	}
	else{
		cout << "set all socket -1 success!" << endl;
		return 0;
	}	
	
	//mysql_free_result(res);
	mysql_close(&mysql);
}