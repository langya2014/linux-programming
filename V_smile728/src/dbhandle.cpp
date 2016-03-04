#include "../include/dbhandle.h"

#define maxdscrb 128

struct RECORD records[20];

/***********************  start about database  ******************************/
struct USER *dblogin(char name[32], char passwd[24], long socket, string facetype)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	my_ulonglong numrow;

	struct USER *lginres;
	//lginres = (struct USER *)malloc(sizeof(struct USER *));
	lginres = new USER();
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		lginres->Islginsuccess = 0;
		mysql_close(&mysql);
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
			if(!strcmp(row[6], passwd))
			{
				cout << "login success!" << endl;				
				//passwd correct
				lginres->Islginsuccess = 1;
				lginres->username = row[1];
				lginres->sex = atoi(row[2]);
				//cout << row[2] << endl;
				//cout << *row[2] <<endl;
				lginres->age = atoi(row[3]);
				//cout << lginres->age << endl;
				//lginres->facetype = atoi(row[4]);
				lginres->facetype = facetype;
				//cout << lginres->facetype << endl;
				lginres->mail = row[5];
				lginres->portrait = atoi(row[7]);
				//cout << lginres->portrait << endl;
				
				/* store the user's socket */
				stringstream ss;
				ss<<socket;
				string skt = ss.str();
				string username = name;
				string cmd = "update users set socket='"+skt+"',facetype='"+facetype+"' where username='"+username+"'";
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
				return lginres;
			}
			
		}
		++i;
		if(i == numrow)
		{
			cout << "this user does not exist, please check the user name!" << endl;
			lginres->Islginsuccess = 3;
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
	int sex, age, facetype, portrt;
	name = pusrinfor->username;
	pwd = pusrinfor->passwd;
	mail = pusrinfor->mail;
	sex = pusrinfor->sex;
	age = pusrinfor->age;
	facetype = pusrinfor->facetype;
	portrt =pusrinfor->portrait;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
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
	
	stringstream ss1, ss2, ss3, ss4;
	ss1<<sex; ss2<<age; ss3<<facetype; ss4<<portrt;
	string ssex = ss1.str();
	string sage = ss2.str();
	string sfacetype = ss3.str();
	string sportrt = ss4.str();
	
	string comd = "insert into users (username,sex,age,facetype,mail,passwd,portrait) values('"+name+"','"+ssex+"','"+sage+"','"+sfacetype+"','"+mail+"','"+pwd+"','"+sportrt+"')";
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
		return 1;
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int pwdmodify(char name[16], char newpwd[24])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
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
		pname = strcmp(row[1], name);
		if(pname == 0)
		{
			string spwd, sname;
			spwd = newpwd; 
			sname = name;
			string comd = "update users set password='"+spwd+"' where username='"+sname+"'";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				
				cout << "mysql_query failure!" << endl;
				return 0;
			}
			else
			{
				cout << "passwd modify success!" << endl;
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

int changeinfo(struct USER *pchginfor)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	string pwd, name, mail;
	int sex, age, facetype, portrt;
	name = pchginfor->username;
	pwd = pchginfor->passwd;
	mail = pchginfor->mail;
	sex = pchginfor->sex;
	age = pchginfor->age;
	facetype = pchginfor->facetype;
	portrt = pchginfor->portrait;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
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

			stringstream ss1, ss2, ss3, ss4;
			ss1<<sex; ss2<<age; ss3<<facetype; ss4<<portrt;
			string ssex = ss1.str();
			string sage = ss2.str();
			string sfacetype = ss3.str();
			string sportrt = ss4.str();

			string comd = "update users set mail='"+mail+"',facetype='"+sfacetype+"',age='"+sage+"',sex='"+ssex+"',password='"+pwd+"',portrait='"+sportrt+"' where username='"+name+"'";
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
/*
struct qunlist *publicqun(string username)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;

	struct qunlist *p_qunlist;
	//p_qunlist = (struct qunlist *)malloc(sizeof(struct qunlist));
	p_qunlist = new qunlist();
	struct quninfor *qungroup = new quninfor[10];
	p_qunlist->qungroup = qungroup; 
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}

	if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
	{
		cout << "mysql_real_query failure!" <<endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
	
	res2 = mysql_store_result(&mysql);
	if(res2 == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
		
	int i = 0;
	string joinedgrp;
	while(row2 = mysql_fetch_row(res2))
	{	
		if(strcmp(username.c_str(), row2[1]) == 0)
		{
			joinedgrp = row2[2];
			string cmd = "select * from qun_tab where qunname!='"+joinedgrp+"'";
			if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
			{
				cout << "mysql_real_query failure!" <<endl;
				p_qunlist->Issuccess = 0;
				return p_qunlist;
			}
	
			res = mysql_store_result(&mysql);
			if(res == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				p_qunlist->Issuccess = 0;
				return p_qunlist;
			}
		
			int i = 0;
			while(row = mysql_fetch_row(res))
			{	
				struct quninfor *p_quninfor;
				//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
				p_quninfor = new quninfor();
				p_quninfor = &(p_qunlist->qungroup[i]);
				p_quninfor->qunportrait = atoi(row[3]); 	
				p_quninfor->qunname = row[1];
				p_quninfor->qundescrible = row[4];

				++i;
				if(i == 10)
				{
					delete p_quninfor;
					return p_qunlist;
					//break;
				}
			}
		}
	}
				
	//free(p_qunlist);
	delete p_qunlist;
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);

}
*/

struct qunlist *publicqun(string username, int num)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2, *res3;
	MYSQL_ROW row, row3;
	my_ulonglong res2_num;

	struct qunlist *p_qunlist;
	//p_qunlist = (struct qunlist *)malloc(sizeof(struct qunlist));
	p_qunlist = new qunlist();
	struct quninfor *qungroup = new quninfor[10];
	p_qunlist->qungroup = qungroup; 
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		p_qunlist->Issuccess = 0;
		mysql_close(&mysql);
		return p_qunlist;
	}

	string create_qun_flag = "select qunname from qun_tab where qunmaster = '"+username+"'";
	if(mysql_query(&mysql, create_qun_flag.c_str()))
	{
		cout << "mysql_query failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}

	res2 = mysql_store_result(&mysql);
	if(res2 == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
	}
	res2_num = mysql_num_rows(res2);
	mysql_free_result(res2);

	if(res2_num == 0)
	{
		cout << "res2 table is empty!" << endl;
		string query_qun = "select * from qun_tab";
		if(mysql_query(&mysql, query_qun.c_str()))
		{
			cout << "mysql_query failure!" << endl;
			p_qunlist->Issuccess = 0;
			return p_qunlist;
		}

		res3 = mysql_store_result(&mysql);

		int i = 0, j = 0;
		cout<<"num:"<<num<<endl;
		struct quninfor *p_quninfor;
		while(row3 = mysql_fetch_row(res3))
		{	
		//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
		//p_quninfor = new quninfor();
			if(i >= num)
			{
				p_quninfor = &(p_qunlist->qungroup[j]);
				p_quninfor->qunname = row3[1];
				//cout<<"qunname"<<row3[1]<<endl;
				p_quninfor->qunportrait = atoi(row3[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
				p_quninfor->qundescrible = row3[4];
				p_quninfor->type = 2; 	//added in 20150418
				++j;
			}
		
			++i;
			if(j == 10)
			{
			//free(p_quninfor);
			//delete p_quninfor;
				break;
			}
		}
		mysql_free_result(res3);
		p_qunlist->Issuccess = j; 	//store the number of the privatequn.
		return p_qunlist;
	}

	string creat_view = "create view tt as select qunname from user_qun where username='"+username+"'";
	if(mysql_query(&mysql, creat_view.c_str()))
	{
		cout << "create view tt failure!" << endl;
		//p_qunlist->Issuccess = 0;
		//return p_qunlist;
	}
	string comd = "select distinct qun_tab.qunname,qun_tab.qunportrait,qun_tab.qundescrible from qun_tab,tt where qun_tab.qunname != tt.qunname"; 	//create view tt as select qunname from qun_tab where qunname=
	// string comd = "select distinct qun_tab.qunname,qun_tab.qunportrait,qun_tab.qundescrible from qun_tab,test.v where not exists(select * from test.v where qun_tab.qunname = test.v.qunname)";
	if(mysql_real_query(&mysql, comd.c_str(), (unsigned long)strlen(comd.c_str())))
	{
		cout << "mysql_real_query failure!" <<endl;
		p_qunlist->Issuccess = 0;

		string drop_view = "drop view tt";
		if(mysql_query(&mysql, drop_view.c_str()))
			cout << "drop view tt failure!" << endl;

		return p_qunlist;
	}
	/*string cmd = "select * from qun_tab where qunname!=(select qunname from user_qun where username='"+username+"')";
	if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
	{
		cout << "mysql_real_query failure!" <<endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}*/
	
	string drop_view = "drop view tt";
	if(mysql_query(&mysql, drop_view.c_str()))
		cout << "drop view tt failure!" << endl;

	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
		
	int i = 0, j = 0;
	cout<<"num:"<<num<<endl;
	struct quninfor *p_quninfor;
	while(row = mysql_fetch_row(res))
	{	
		//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
		//p_quninfor = new quninfor();
		if(i >= num)
		{
			p_quninfor = &(p_qunlist->qungroup[j]);
			p_quninfor->qunname = row[0];
			cout<<"qunname"<<row[0]<<endl;
			p_quninfor->qunportrait = atoi(row[1]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
			p_quninfor->qundescrible = row[2];
			p_quninfor->type = 2; 	//added in 20150418
			++j;
		}
		
		++i;
		if(j == 10)
		{
			//free(p_quninfor);
			//delete p_quninfor;
			break;
		}
	}
	
	p_qunlist->Issuccess = j; 	//store the number of the privatequn.
	mysql_free_result(res);
	mysql_close(&mysql);
	return p_qunlist;
	//free(p_qunlist);
	delete p_qunlist;
	delete[] qungroup;
}

struct qunlist *privatequn(string username, int num)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;

	struct qunlist *p_qunlist; 	//the maximun number of privatequn is 10
	//p_qunlist = (struct qunlist *)malloc(sizeof(struct qunlist));
	p_qunlist = new qunlist();
	struct quninfor *qungroup = new quninfor[10];
	p_qunlist->qungroup = qungroup; 
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}

	if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
	{
		cout << "mysql_real_query failure!" <<endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
		
	//int i = 0;
	if(!(row = mysql_fetch_row(res)))
	{
		p_qunlist->Issuccess = 0;
		return p_qunlist;
	}
	//while(row = mysql_fetch_row(res))
	//do
	int i = 0;
	while(row)
	{
		int pusrname;
		pusrname = strcmp(username.c_str(), row[1]);
		if(pusrname == 0)
		{
			string qunname = row[2];
			if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
			{
				cout << "mysql_real_query failure!" <<endl;
				p_qunlist->Issuccess = 0;
				return p_qunlist;
			}
			
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				p_qunlist->Issuccess = 0;
				return p_qunlist;
			}
			
			//int i = 0;
			while(row2 = mysql_fetch_row(res2))
			{
				int pqunname;
				pqunname = strcmp(qunname.c_str(), row2[1]);
				if(pqunname == 0)
				{
					struct quninfor *p_quninfor;
					//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
					//p_quninfor = new quninfor();
					p_quninfor = &(p_qunlist->qungroup[i]);
					p_quninfor->qunportrait = atoi(row2[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
					p_quninfor->qunname = row2[1];
					p_quninfor->qundescrible = row2[4];
					p_quninfor->type = atoi(row[3]); 	//added in 20150418, represent the relationship between user and group.

					++i;
					//free(p_quninfor);
					//delete p_quninfor;
				}
				if(i == 10)
				{
					//break;
					p_qunlist->Issuccess = i; 	//store the number of the privatequn.
					return p_qunlist;
				}	
			}
			
		}
		row = mysql_fetch_row(res);
	}//while(row = mysql_fetch_row(res));
	p_qunlist->Issuccess = i; 	//store the number of the privatequn.
	return p_qunlist;	
				
	//free(p_qunlist);
	delete p_qunlist;
	delete[] qungroup;
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);

}
/*
struct quninfor *joinqun(string username, string qunname)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	struct quninfor *p_quninfor;
	//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
	p_quninfor = new quninfor();
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return p_quninfor;
	}

	string comd = "insert into user_qun (username, qunname) values('"+username+"','"+qunname+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return p_quninfor;
	}
	else
	{
		cout << "join in the qun success!" << endl;
		if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
		{
			cout << "mysql_real_query failure!" <<endl;
			return p_quninfor;
		}
	
		res = mysql_store_result(&mysql);
		if(res == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return p_quninfor;
		}
		
		while(row = mysql_fetch_row(res))
		{
			int pqunname;
			pqunname = strcmp(qunname.c_str(), row[1]);
			if(pqunname == 0)
			{
				p_quninfor->qunportrait = atoi(row[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
				p_quninfor->qunname = row[1];
				p_quninfor->qundescrible = row[4];
					
				return p_quninfor;
			}
				
		}
		//return 1;
	}
	
	//free(p_quninfor);
	delete p_quninfor;
	mysql_free_result(res);
	mysql_close(&mysql);

}
*/

struct quninfor *searchqun(string qunname)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	struct quninfor *p_quninfor;
	//p_quninfor = (struct quninfor *)malloc(sizeof(struct quninfor));
	p_quninfor = new quninfor();
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return p_quninfor;
	}

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return p_quninfor;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return p_quninfor;
	}
		
	while(row = mysql_fetch_row(res))
	{
		int pqunname;
		pqunname = strcmp(qunname.c_str(), row[1]);
		if(pqunname == 0)
		{
			p_quninfor->qunportrait = atoi(row[3]); 	//in the qun_tab.sql, the qunportrait has type of char(2),why?
			p_quninfor->qunname = row[1];
			p_quninfor->qundescrible = row[4];
					
			return p_quninfor;
		}
				
	}

	//free(p_quninfor);
	delete p_quninfor;
	mysql_free_result(res);
	mysql_close(&mysql);

}

int createqun(string qunname, string qunmaster, int qunportrt, string qundscrb)//0
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select qunname from qun_tab", (unsigned long)strlen("select qunname from qun_tab")))
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
		pname = strcmp(row[0], qunname.c_str());
		if(pname == 0)
		{
			cout << "this qun name exist!" << endl;
			return 2;
		}
		//break;
	}

	/*string smaster, sname, sdscrb;
	smaster = qunmaster; 
	sname = qunname;
	sdscrb = qundscrb;
	string sportrt = qunportrt;*/
	stringstream ss;
	ss<<qunportrt;
	string sportrt = ss.str();
	string comd = "insert into qun_tab (qunname, qunmaster, qunportrait, qundescrible) values('"+qunname+"','"+qunmaster+"','"+sportrt+"','"+qundscrb+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "create qun success!" << endl; 
		/************* added in 20150418 *****************/
		string cmd = "insert into user_qun (username, qunname, relationship) values('"+qunmaster+"', '"+qunname+"', 0)";
		cout << cmd.c_str() << endl;
		if(mysql_query(&mysql, cmd.c_str()))
		{
			cout << "insert record to user_qun failure!" <<endl;
			return 0;
		}
		else
		{
			cout << "insert record to user_qun success!" <<endl;
			return 1;
		}
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

struct recordlist *showchatrcd(string username, string qunname, int num)
{
	MYSQL mysql;
	MYSQL_RES *res2, *res3;
	MYSQL_ROW row2, row3;
	
	/* define the responding struct */
	struct recordlist *p_recordlist;
	p_recordlist = new recordlist();
	struct RECORD *precordlist = new RECORD[10];
	p_recordlist->precordlist = precordlist;
			
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return NULL;
	}

	/*if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}*/
	
	string comd = "select * from qun_record where qunname = '"+qunname+"' order by id DESC";
	cout << comd.c_str() << endl;
	if(mysql_real_query(&mysql, comd.c_str(), (unsigned long)strlen(comd.c_str())))
	{
		cout << "mysql_real_query chat record failure!" <<endl;
		//return 0;
	}
	else
	{
		cout << "query chat record success!" << endl;
	}	

	res2 = mysql_store_result(&mysql);
	if(res2 == NULL)
	{
		cout << "mysql_store_result chat record failure!" << endl;
		//return 0;
	}
			
	int i = 0, j = 0;
	int count = 0;
	struct RECORD *p_rcd;
	while(row2 = mysql_fetch_row(res2))
	{
		++i;
		//cout<<"i: "<<i<<endl;
		if(i >= num)
		{
			cout<<"i>=num"<<endl;
					//p_rcd = (struct RECORD *)malloc(sizeof(struct RECORD));
			p_rcd = new RECORD();
			p_rcd = &(p_recordlist->precordlist[j]);
			p_rcd->username = row2[2];
			p_rcd->cur_time = row2[3];
			p_rcd->message = row2[4];
				
			string speaker;
			speaker = p_rcd->username;
			if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
			{
				cout << "mysql_real_query failure!" <<endl;
				//return;
			}
	
			res3 = mysql_store_result(&mysql);
			if(res3 == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				//return;
			}

			while(row3 = mysql_fetch_row(res3))
			{
				int pname;
				pname = strcmp(speaker.c_str(), row3[1]);
				if(pname == 0)
				{
					p_rcd->usrportrt = atoi(row3[7]);
					break;
				}
			}
			++j;
			++count;
			mysql_free_result(res3);
			if(count == 10)
				break;
		
		}
	}	
	mysql_free_result(res2);	
	p_recordlist->Isquerysuccess = count;
	//cout << "count: "<<count <<endl;
	
	mysql_close(&mysql);
	return p_recordlist;
	
	delete p_recordlist;
	delete[] precordlist;
	//free(precord);	
	//mysql_free_result(res);
}

char *gettime(void)
{
	time_t now;
	//char *ctime();

	(void) time(&now);
	return ctime(&now);
}

/************************* new add group chat record function ************************************/
int addchatrecord(string username, string qunname, string msg, long *&skt, long &online_num)
{
	MYSQL mysql;
	MYSQL_RES *res2, *res3;
	MYSQL_ROW row2, row3;
	my_ulonglong numrow;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return 0;
	}

	/*if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}*/

	string stime;
	stime = gettime();
	string comd = "insert into qun_record (qunname,username,cur_time,arecord) values('"+qunname+"','"+username+"','"+stime+"','"+msg+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		mysql_close(&mysql);
		return 0;
	}
	else
	{
		cout << "add chat record success!" << endl;
		if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
		{
			cout << "mysql_real_query failure!" <<endl;
			mysql_close(&mysql);
			return 0;
		}
	
		res2 = mysql_store_result(&mysql);
		if(res2 == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			//mysql_free_result(res2);
			mysql_close(&mysql);
			return 0;
		}
				
		numrow = mysql_num_rows(res2);
		int num = numrow - 1;
		long *socket = new long[num];
		//socket = (long *)malloc(num*sizeof(long));
		int i = 0, membernum = 0; 	//add the number of group member, this number equal the total - 1.
		while(row2 = mysql_fetch_row(res2))
		{
			if(strcmp(qunname.c_str(), row2[2]) == 0)
			{
				membernum++;
				string otheruser = row2[1]; 	//get other users in this group
				if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
				{
					cout << "mysql_real_query users failure!" <<endl;
					mysql_close(&mysql);
					return 0;
				}
	
				res3 = mysql_store_result(&mysql);
				if(res3 == NULL)
				{
					cout << "mysql_store_result users failure!" << endl;
					//mysql_free_result(res3);
					mysql_close(&mysql);
					return -1;
				}
			
				while(row3 = mysql_fetch_row(res3))
				{
					if(strcmp(otheruser.c_str(), row3[1]) == 0)
					{
									//int socket = atoi(row3[8]);
						if(atoi(row3[8]) != -1)
						{
							socket[i] = atoi(row3[8]);
							++i;
							break;
						}
					}
				}
				mysql_free_result(res3);
			}
		}
		mysql_free_result(res2);
		online_num = i;
		skt = socket;
		//mysql_free_result(res);
		mysql_close(&mysql);
		return 1;
	}
}

int insert_hottie(int picID, string author, string title, string text)
{
	MYSQL mysql;
	//MYSQL_RES *res;
	//MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return 0;
	}
	
	stringstream ss;
	ss<<picID; 
	string spicID = ss.str();

	string comd = "insert into hottie (author, picture, title, text) values('"+author+"','"+spicID+"','"+title+"','"+text+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "insert hot tie failure!" <<endl;
		mysql_close(&mysql);
		return 0;
	}
	else
	{
		cout << "insert hot tie success!" << endl;
		mysql_close(&mysql);
		return 1;
	}

	//mysql_free_result(res);
}

struct hottielist *send_hottie(int num)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	struct hottielist *p_hottielist;
	p_hottielist = new hottielist();
	struct hottie *phottielist = new hottie[10]; //store 10 hotties.
	p_hottielist->phottielist = phottielist;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return 0;
	}
	
	if(mysql_real_query(&mysql, "select * from hottie", (unsigned long)strlen("select * from hottie")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}

	int i = 0, j=0, count = 0;
	//p_hottie = new hottie();
	while(row = mysql_fetch_row(res))
	{
		++i;
		if(i >= num)
		{
			struct hottie *p_hottie;
			p_hottie = &(p_hottielist->phottielist[j]);
			p_hottie->picture = atoi(row[2]);
			p_hottie->author = row[1];
			p_hottie->title = row[3];
			p_hottie->text = row[4];
			++j;
			++count;
		}
		if(count >= 10)
		{
			break;
		}
	}
	//delete p_hottie;
	p_hottielist->Isquerysuccess = count;
	
	return p_hottielist;
	
	delete p_hottielist;
	delete[] phottielist;
	mysql_free_result(res);
	mysql_close(&mysql);
	
}

/*************************  new function from version 1.0  ************************************/
long add_focus(string myusername, string focusedname, int &focusnum)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row;
	my_ulonglong numrow;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
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
	
	long socket;
	string respondtab;
	string ssex, sage, sportrt;
	while(row = mysql_fetch_row(res))
	{
		int pname, pfocuse;
		char *userID;
		userID = (char *)malloc(4*sizeof(char));
		pfocuse = strcmp(row[1], focusedname.c_str());
		if(pfocuse == 0)
		{
				//stringstream ss1, ss2, ss3;
				//ss1<<sex; ss2<<age; //ss3<<facetype; 
				//ss4<<portrt;
				ssex = row[2];
				sage = row[3];
				sportrt = row[7];
				socket = atoi(row[8]);
		
		}
		pname = strcmp(row[1], myusername.c_str());
		if(pname == 0)
		{
			userID = row[0];
			switch((int)(*userID)){
				case '1':
					respondtab = "user_1_guanzhu";
					break;
				case '2':
					respondtab = "user_2_guanzhu";
					break;
				case '3':
					respondtab = "user_3_guanzhu";
					break;
				case '4':
					respondtab = "user_4_guanzhu";
					break;
				case '5':
					respondtab = "user_5_guanzhu";
					break;
				case '6':
					respondtab = "user_6_guanzhu";
					break;
				case '7':
					respondtab = "user_7_guanzhu";
					break;
				case '8':
					respondtab = "user_8_guanzhu";
					break;
				case '9':
					respondtab = "user_9_guanzhu";
					break;
				/*case '10':
					respondtab = "user_10_record";
					break;*/
				default:
					cout << "exceed the maximun numberof the user, error!" << endl;
			}
			
		}

	}
			
	string comd = "insert into "+respondtab+" (username,sex,age,portrait) values('"+focusedname+"','"+ssex+"','"+sage+"','"+sportrt+"')";
//	string comd = "insert into users (username, mail, passwd)"
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "add guanzhu success!" << endl;
		string cmd = "select * from "+respondtab+"";
		cout << cmd.c_str() << endl;
		if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
		{
			cout << "mysql_real_query failure!" <<endl;
			return 0;
		}
	
		res2 = mysql_store_result(&mysql);
		if(res2 == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return 0;
		}
		
		numrow = mysql_num_rows(res2); 	//query the number of the focuse of username.
		focusnum = numrow;
		return socket;
	}

	
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);

	//return 0;	
}

long add_fans(string myusername, string fansname, int &fansnum)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row;
	my_ulonglong numrow;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
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
	
	long socket;
	string respondtab;
	string ssex,sage,sportrt;
	while(row = mysql_fetch_row(res))
	{
		int pname, pfans;
		char *userID;
		userID = (char *)malloc(4*sizeof(char));
		pfans = strcmp(row[1], fansname.c_str());
		if(pfans == 0)
		{
				ssex = row[2];
				sage = row[3];
				sportrt = row[7];
				socket = atoi(row[8]);
		}
		pname = strcmp(row[1], myusername.c_str());
		if(pname == 0)
		{
			userID = row[0];
			switch((int)(*userID)){
				case '1':
					respondtab = "user_1_fans";
					break;
				case '2':
					respondtab = "user_2_fans";
					break;
				case '3':
					respondtab = "user_3_fans";
					break;
				case '4':
					respondtab = "user_4_fans";
					break;
				case '5':
					respondtab = "user_5_fans";
					break;
				case '6':
					respondtab = "user_6_fans";
					break;
				case '7':
					respondtab = "user_7_fans";
					break;
				case '8':
					respondtab = "user_8_fans";
					break;
				case '9':
					respondtab = "user_9_fans";
					break;
				/*case '10':
					respondtab = "user_10_record";
					break;*/
				default:
					cout << "exceed the maximun numberof the user, error!" << endl;
			}
			
		}

	}
	//squnno = respondtab;
	//string comd = "insert into '"+squnno+"' (qunname, username, cur_time, arecord) values('"+squnname+"','"+suser+"', '"+stime+"','"+srecd+"')";
	string comd = "insert into "+respondtab+" (username,sex,age,portrait) values('"+fansname+"','"+ssex+"','"+sage+"','"+sportrt+"')";

	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "add fans success!" << endl;
		string cmd = "select * from "+respondtab+"";
		cout << cmd.c_str() << endl;
		if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
		{
			cout << "mysql_real_query failure!" <<endl;
			return 0;
		}
	
		res2 = mysql_store_result(&mysql);
		if(res2 == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return 0;
		}
		
		numrow = mysql_num_rows(res2); 	//query the number of the focuse of username.
		fansnum = numrow;
		return socket;
	}

	
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);
	
}

struct fanslist *show_guanzhu(string myusername, int num)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;
	
	/* define the point for the FANs*/
	/*struct FAN *p_fan;
	p_fan = new FAN();
	struct FAN *fans;
	fans = new FAN[10];*/
	struct fanslist *p_fanslist;
	p_fanslist = new fanslist();
	struct FAN *pfanslist = new FAN[10]; 	//store 10 fans once
	p_fanslist->pfanslist = pfanslist;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return 0;
	}

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		string focusetab;
		char *tableID;
		tableID = (char *)malloc(4*sizeof(char));
		pname = strcmp(row[1], myusername.c_str());
		if(pname == 0)
		{
			tableID = row[0];
			switch((int)(*tableID)){
				case '1':
					focusetab = "user_1_guanzhu";
					break;
				case '2':
					focusetab = "user_2_guanzhu";
					break;
				case '3':
					focusetab = "user_3_guanzhu";
					break;
				case '4':
					focusetab = "user_4_guanzhu";
					break;
				case '5':
					focusetab = "user_5_guanzhu";
					break;
				case '6':
					focusetab = "user_6_guanzhu";
					break;
				case '7':
					focusetab = "user_7_guanzhu";
					break;
				case '8':
					focusetab = "user_8_guanzhu";
					break;
				case '9':
					focusetab = "user_9_guanzhu";
					break;
				/*case '10':
					focusetab = "user_10_guanzhu";
					break;*/
				default:
					cout << "exceed the maximun numberof the user, error!" << endl;
			}

			string comd = "select * from "+focusetab+"";
			cout << comd.c_str() << endl;
			if(mysql_real_query(&mysql, comd.c_str(), (unsigned long)strlen(comd.c_str())))
			{
				cout << "mysql_real_query focused table failure!" <<endl;
				//return 0;
			}
			else
			{
				cout << "query focused table success!" << endl;
			}	

			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result focused table failure!" << endl;
				//return 0;
			}
			
			int i = 0, j = 0, count = 0;
			while(row2 = mysql_fetch_row(res2))
			{
				++i;
				if(i >= num)
				{
					struct FAN *p_fan;
					p_fan = new FAN();
					p_fan = &(p_fanslist->pfanslist[j]);
					p_fan->username = row2[1];
					p_fan->sex = atoi(row2[2]);
					p_fan->age = atoi(row2[3]);
					p_fan->portrait = atoi(row2[4]);
					//p_fan->Isquerysuccess = 1;
					++j;
					++count;
					
					if(count == 10)
					{
						//delete p_fan;
						break;
					}
					delete p_fan;
				}
			}
		p_fanslist->Isquerysuccess = count;	 	//store the number of fans.
			
		free(tableID);
		return p_fanslist;
		}
	//return p_fanslist;
	}
	/*delete p_fan;
	return fans;
	delete[] fans;*/
	delete p_fanslist;
	delete[] pfanslist;
	
	mysql_free_result(res2);
	mysql_free_result(res);
	mysql_close(&mysql);
}

struct fanslist *show_fans(string myusername, int num)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;
	
	/* define the point for the FANs*/
	struct fanslist *p_fanslist;
	p_fanslist = new fanslist();
	struct FAN *pfanslist = new FAN[10]; 	//store 10 fans once
	p_fanslist->pfanslist = pfanslist;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return 0;
	}

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		string fanstab;
		char *tableID;
		tableID = (char *)malloc(4*sizeof(char));
		pname = strcmp(row[1], myusername.c_str());
		if(pname == 0)
		{
			tableID = row[0];
			switch((int)(*tableID)){
				case '1':
					fanstab = "user_1_fans";
					break;
				case '2':
					fanstab = "user_2_fans";
					break;
				case '3':
					fanstab = "user_3_fans";
					break;
				case '4':
					fanstab = "user_4_fans";
					break;
				case '5':
					fanstab = "user_5_fans";
					break;
				case '6':
					fanstab = "user_6_fans";
					break;
				case '7':
					fanstab = "user_7_fans";
					break;
				case '8':
					fanstab = "user_8_fans";
					break;
				case '9':
					fanstab = "user_9_fans";
					break;
				/*case '10':
					fanstab = "user_10_fans";
					break;*/
				default:
					cout << "exceed the maximun numberof the user, error!" << endl;
			}

			string comd = "select * from "+fanstab+"";
			cout << comd.c_str() << endl;
			if(mysql_real_query(&mysql, comd.c_str(), (unsigned long)strlen(comd.c_str())))
			{
				cout << "mysql_real_query fans table failure!" <<endl;
				//return 0;
			}
			else
			{
				cout << "query fans table success!" << endl;
			}	

			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result fans table failure!" << endl;
				//return 0;
			}
			
			int i = 0, j = 0, count = 0;
			while(row2 = mysql_fetch_row(res2))
			{
				++i;
				if(i >= num)
				{
					struct FAN *p_fan;
					p_fan = new FAN();
					p_fan = &(p_fanslist->pfanslist[j]);
					p_fan->username = row2[1];
					p_fan->sex = atoi(row2[2]);
					p_fan->age = atoi(row2[3]);
					p_fan->portrait = atoi(row2[4]);
					//p_fan->Isquerysuccess = 1;
					++j;
					++count;
					
					if(count == 10)
					{
						//delete p_fan;
						break;
					}
					delete p_fan;
				}
			}
		p_fanslist->Isquerysuccess = count;	
		free(tableID);
		return p_fanslist;
		}
	}
	delete p_fanslist;
	delete[] pfanslist;
	
	mysql_free_result(res2);
	mysql_free_result(res);
	mysql_close(&mysql);
}

int add_scores(string username, int addscore)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	my_ulonglong numrow;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}

	if(mysql_real_query(&mysql, "select * from scores", (unsigned long)strlen("select * from scores")))
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
	
	numrow = mysql_num_rows(res);
	if(res == 0)
	{
		cout << "There are not any scores record in the scores table!" << endl;
		int total_scores = addscore;
		stringstream ss;
		ss<<total_scores;
		string total = ss.str();
		string comd = "insert into scores (username, scores) values('"+username+"','"+total+"')";
		cout << comd.c_str() << endl;
		if(mysql_query(&mysql, comd.c_str()))
		{
				
			cout << "update scores failure!" << endl;
			return -1;
		}
		else
		{
			cout << "update scores success!" << endl;
			return total_scores;
		}
	}
	else
	{
		int i = 0;
		while(row = mysql_fetch_row(res))
		{
			int pname; 
			pname = strcmp(row[1], username.c_str());
			if(pname == 0)
			{
				int curr_scores = atoi(row[2]);
				int total_scores = curr_scores + addscore;
				stringstream ss;
				ss<<total_scores;
				string total = ss.str();
				string comd = "update scores set scores='"+total+"' where username='"+username+"'";
				cout << comd.c_str() << endl;
				if(mysql_query(&mysql, comd.c_str()))
				{
				
					cout << "update scores failure!" << endl;
					return -1;
				}
				else
				{
					cout << "update scores success!" << endl;
					return total_scores;
				}
				//break;
			}
			++i;
		}
		if(i == numrow)
		{
			cout << "There are not any scores record in the scores table!" << endl;
			int total_scores = addscore;
			stringstream ss;
			ss<<total_scores;
			string total = ss.str();
			string comd = "insert into scores (username, scores) values('"+username+"','"+total+"')";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				cout << "update scores failure!" << endl;
				return -1;
			}
			else
			{
				cout << "update scores success!" << endl;
				return total_scores;
			}
		}
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);
}


/************************* new add collection function  ************************************/
string add_collect(string username, string title)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return title;
	}


	
	if(mysql_real_query(&mysql,"select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return 0;
	}
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return 0;
	}
	
	//int i = 0, j = 0, count = 0;
	string author;
	int img;
	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[3], title.c_str());
		if(pname == 0)
		{
			author = row[1];
			img = atoi(row[2]);
			break;
		}
	}
	
	stringstream ss;
	ss<<img;
	string simg = ss.str();
	string comd = "insert into collection (collector, author, title, img) values('"+username+"','"+author+"','"+title+"','"+simg+"')";
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return title;
	}
	else
	{
		cout << "insert collection record success!" << endl;
		return title;
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);
}

struct collection *show_collect(string collector, int num, int &return_num)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	struct collection *p_collect;
	//p_collect = new collection();
	struct collection *collections;
	collections = new collection[10];
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return 0;
	}

	if(mysql_real_query(&mysql, "select * from collection", (unsigned long)strlen("select * from collection")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return 0;
	}
	
	int i = 0, j = 0, count = 0;
	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[1], collector.c_str());
		if(pname == 0)
		{
			++i;
			if(i >= num)
			{
				p_collect = &(collections[j]);
				p_collect->author = row[2];
				p_collect->title = row[3];
				p_collect->img = atoi(row[4]);
				
				++j;
				++count;
				if(count == 10)
					break;
			}
		}
	}
	
	return_num = count;
	
	//delete p_collect;
	return collections;
	delete[] collections;
	
	mysql_free_result(res);
	mysql_close(&mysql);
}


int addprivatechat(string speaker, string listener, string content)
{
	MYSQL mysql;
	//MYSQL_RES *res;
	//MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}
	
	string stime = gettime();
	string table = "private_chat_recd";
	string comd = "insert into "+table+" (speaker, listener, cur_time, content) values('"+speaker+"','"+listener+"', '"+stime+"','"+content+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "add chat record success!" << endl;
		return 1;
	}
		
	mysql_close(&mysql);
}

struct hottielist *myhottie(string writer, int num)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	/*struct hottie *p_hottie;
	p_hottie = new hottie();
	struct hottie *hotties;
	hotties = new hottie[10];*/
	struct hottielist *p_hottielist;
	p_hottielist = new hottielist();
	struct hottie *phottielist = new hottie[10]; //store 10 hotties.
	p_hottielist->phottielist = phottielist;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return 0;
	}
	
	if(mysql_real_query(&mysql, "select * from hottie", (unsigned long)strlen("select * from hottie")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}

	int i = 0, j=0, count = 0;
	while(row = mysql_fetch_row(res))
	{
		if(strcmp(row[1], writer.c_str()) == 0)
		{
			++i;
			//cout<<"num:"<<num<<"i:"<<i<<endl;
			if(i >= num)
			{
				struct hottie *p_hottie;
				//p_hottie = new hottie();
				p_hottie = &(p_hottielist->phottielist[j]);
				p_hottie->picture = atoi(row[2]);
				p_hottie->author = row[1];
				p_hottie->title = row[3];
				p_hottie->text = row[4];
				
				++j;
				++count;
				if(count == 10)
					break;
				//delete p_hottie;
			}
		}
	}
	p_hottielist->Isquerysuccess = count;
	
	return p_hottielist;
	
	delete p_hottielist;
	delete[] phottielist;
	
	mysql_free_result(res);
	mysql_close(&mysql);
}

struct hottie *onehottie(string writer, string title)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	struct hottie *p_hottie;
	p_hottie = new hottie();
	//struct hottie *hotties;
	//hotties = new hottie[10];

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return 0;
	}
	
	if(mysql_real_query(&mysql, "select * from hottie", (unsigned long)strlen("select * from hottie")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}
	while(row = mysql_fetch_row(res))
	{
		int pwriter, ptitle;
		//pwriter = strcmp(row[1], writer.c_str());
		ptitle = strcmp(row[3], title.c_str());
//		if((pwriter== 0) && (ptitle == 0))
		if(ptitle == 0)
		{
			//p_hottie = &(hotties[i]);
			p_hottie->picture = atoi(row[2]);
			p_hottie->author = row[1];
			p_hottie->title = row[3];
			p_hottie->text = row[4];
		return p_hottie;
		}
		
	}
	//free(p_hottie);
	delete p_hottie;
	mysql_free_result(res);
	mysql_close(&mysql);
}

//delete the responding record in the table user-qun to be undo.
int deletegroup(string username, string groupname)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
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
	int groupID;
	while(row = mysql_fetch_row(res))
	{
		int puser, pgroup;
		puser = strcmp(row[2], username.c_str());
		pgroup = strcmp(row[1], groupname.c_str());
		cout<<groupname.c_str()<<endl;
		if((puser == 0) && (pgroup == 0))
		{
			groupID = atoi(row[0]);
			break;
		}	
	}
	
	stringstream ss;
	ss<<groupID;
	string group_id = ss.str();
	string comd = "delete from qun_tab where qunname = '"+groupname+"'";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
				
		cout << "delete group failure!" << endl;
		return -1;
	}
	return groupID;
	
	
	mysql_free_result(res);
	mysql_close(&mysql);
}

int outofgroup(string username, string groupname)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
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

	int ID;
	while(row = mysql_fetch_row(res))
	{
		int puser, pgroup;
		puser = strcmp(row[1], username.c_str());
		pgroup = strcmp(row[2], groupname.c_str());
		if((puser == 0) && (pgroup == 0))
		{
			ID = atoi(row[0]);
			break;
		}	
	}
	
	stringstream ss;
	ss<<ID;
	string id = ss.str();
	string comd = "delete from user_qun where id = "+id+"";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{		
		cout << "quit group failure!" << endl;
		return -1;
	}
	else
	{
		cout << "quit group success!" << endl;
	}
	
	return ID;
	mysql_free_result(res);
	mysql_close(&mysql);
}

/********************** join group function *********************/
long requestjoin(string username, string groupname, int &groupID)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
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
	
	long socket;
	while(row = mysql_fetch_row(res))
	{
		int pqunname;
		pqunname = strcmp(groupname.c_str(), row[1]);
		if(pqunname == 0)
		{
			string groupmaster = row[2];
			cout << "cccccc------" << endl;
			groupID = atoi(row[0]);
			if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
			{
				cout << "mysql_real_query users failure!" <<endl;
				return -1;
			}
	
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result users failure!" << endl;
				return -1;
			}
			
			while(row2 = mysql_fetch_row(res2))
			{
				if(strcmp(groupmaster.c_str(), row2[1]) == 0)
				{
					cout << "dddddd-------" << endl;
					socket = atoi(row2[8]);
					if(socket == -1)
					{
						cout << "group master is offline!" << endl;
						string cmd = "insert into grp_request_msg (img,request,qunname,qunmaster) values(0,'"+username+"','"+groupname+"','"+groupmaster+"')";
						cout << cmd.c_str() << endl;
						if(mysql_query(&mysql, cmd.c_str()))
						{
							cout << "insert group request msg failure!" << endl;
							return -1;
						}
						else
						{
							cout << "insert group request msg success!" << endl;
							return socket;
						}
						//return -1;
					}
					else
					{
						cout << "group master is online!" << endl;
						string cmd = "insert into grp_request_msg (img,request,qunname,qunmaster) values(0,'"+username+"','"+groupname+"','"+groupmaster+"')";
						cout << cmd.c_str() << endl;
						if(mysql_query(&mysql, cmd.c_str()))
						{
							cout << "insert group request msg failure!" << endl;
							return -1;
						}
						else
						{
							cout << "insert group request msg success!" << endl;
							return socket;
						}
					}
						//return socket;
				}
			}
		}
	}
	
	mysql_free_result(res2);
	mysql_free_result(res);
	mysql_close(&mysql);
	
}

long responsejoin(int agreeflag, string username, string groupname)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;
	//MYSQL_RES *res2;
	//MYSQL_ROW row2;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query users failure!" <<endl;
		return -1;
	}
	
	res2 = mysql_store_result(&mysql);
	if(res2 == NULL)
	{
		cout << "mysql_store_result users failure!" << endl;
		return -1;
	}
	
	
	long socket;
	if(agreeflag == 1)
	{
		cout << "this user is permitted to join group!" << endl;
		while(row2 = mysql_fetch_row(res2))
		{
			if(strcmp(username.c_str(), row2[1]) == 0)
			{
				socket = atoi(row2[8]);
				if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
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

				string groupmaster;
				while(row = mysql_fetch_row(res))
				{
					int pqunname;
					pqunname = strcmp(groupname.c_str(), row[1]);
					if(pqunname == 0)
					{
						groupmaster = row[2];
						break;
					}
				}

				// string cmd = "delete from grp_request_msg where img=0 && request='"+username+"' && qunname='"+groupname+"'";
				// cout << cmd.c_str() << endl;
				// if(mysql_query(&mysql, cmd.c_str()))
				// 	cout << "delete from grp_request_msg failure!" << endl;
				// else
				// 	cout << "delete from grp_request_msg success!" << endl;
				if(socket == -1)
				{
					cout << "requester is offline!" << endl;
					string cmd = "insert into grp_request_msg (img,request,qunname,qunmaster) values(1,'"+username+"','"+groupname+"','"+groupmaster+"')";
					cout << cmd.c_str() << endl;
					if(mysql_query(&mysql, cmd.c_str()))
					{
						cout << "insert group request msg failure!" << endl;
						return -1;
					}
					else
					{
						cout << "insert group request msg success!" << endl;
						//return socket;
					}
				}
				else{
					cout << "requester is online!" << endl;
					string cmd = "insert into grp_request_msg (img,request,qunname,qunmaster) values(1,'"+username+"','"+groupname+"','"+groupmaster+"')";
					cout << cmd.c_str() << endl;
					if(mysql_query(&mysql, cmd.c_str()))
					{
						cout << "insert group request msg failure!" << endl;
						return -1;
					}
					else
					{
						cout << "insert group request msg success!" << endl;
						//return socket;
					}
				}
				
				
				
				string comd = "insert into user_qun (username, qunname, relationship) values('"+username+"', '"+groupname+"', 1)";
				cout << comd.c_str() << endl;
				if(mysql_query(&mysql, comd.c_str()))
				{
					cout << "insert user_qun relationship failure!" << endl;
					return -1;
				}
				else
				{
					cout << "insert user_qun relationship success!" << endl;
					return socket;	//the request's socket.
				}
				//break;
			}
		}
	}
	else
	{
		cout << "this user is refused to join group!" << endl;
		while(row2 = mysql_fetch_row(res2))
		{
			if(strcmp(username.c_str(), row2[1]) == 0)
			{
				socket = atoi(row2[8]);
				string cmd = "delete from grp_request_msg where request='"+username+"' && qunname='"+groupname+"'";
				cout << cmd.c_str() << endl;
				if(mysql_query(&mysql, cmd.c_str()))
					cout << "delete from grp_request_msg failure!" << endl;
				else
					cout << "delete from grp_request_msg success!" << endl;
					
				return socket;
				//break;
			}
		}
		
	}
	
	mysql_free_result(res2);
	mysql_free_result(res);
	mysql_close(&mysql);
	
}

/* if there are msg, return the group-master's socket, otherwise return 0  */
long Istheremsg(string groupmaster)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}

	if(mysql_real_query(&mysql, "select * from grp_request_msg", (unsigned long)strlen("select * from grp_request_msg")))
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

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[3], groupmaster.c_str());
		if(pname == 0)
		{
			if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
			{
				cout << "mysql_real_query users failure!" <<endl;
				return -1;
			}
	
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result users failure!" << endl;
				return -1;
			}
			
			while(row2 = mysql_fetch_row(res2))
			{
				if(strcmp(groupmaster.c_str(), row2[1]) == 0)
				{
					//cout << "dddddd-------" << endl;
					long socket = atoi(row2[8]);
					return socket;
				}	
			}
			
			break;
		}
	}
	
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);
}

/* note: this function return a request msg once! */
long msghandle(string groupmaster, string &requester, string &groupname)
{
	
}

long getusrskt(string username)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
	{
		cout << "mysql_real_query users failure!" <<endl;
		return -1;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result users failure!" << endl;
		return -1;
	}
			
	while(row = mysql_fetch_row(res))
	{
		if(strcmp(username.c_str(), row[1]) == 0)
		{
			long socket = atoi(row[8]);
			return socket;
		}	
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);
	
}

struct msglist *query(string username)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return NULL;
	}

	if(mysql_real_query(&mysql, "select * from grp_request_msg", (unsigned long)strlen("select * from grp_request_msg")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return NULL;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return NULL;
	}

	int msgnum = 0;
	while(row = mysql_fetch_row(res))
	{
		int prequest, pmaster; 
		prequest = strcmp(row[2], username.c_str());
		pmaster = strcmp(row[4], username.c_str());
		if(((prequest == 0) && (atoi(row[1]) == 1)) || ((pmaster == 0) && (atoi(row[1]) == 0)))
		{
			if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
			{
				cout << "mysql_real_query users failure!" <<endl;
				return NULL;
			}
	
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result users failure!" << endl;
				//return -1;
			}
			
			while(row2 = mysql_fetch_row(res2))
			{
				if(strcmp(username.c_str(), row2[1]) == 0)
				{
					long socket = atoi(row2[8]);
					if(socket > 0)
					{
						msgnum++;
						break;
					}
					//return socket;
				}	
			}
			
		}
	}
	
	struct msglist *p_msglist;
	p_msglist = new msglist();
	struct joinmsg *pjoinmsg = new joinmsg[msgnum];
	p_msglist->pjoinmsg = pjoinmsg;
	
	//cout<<"before fetch result!"<<endl;
	if(mysql_real_query(&mysql, "select * from grp_request_msg", (unsigned long)strlen("select * from grp_request_msg")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return NULL;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return NULL;
	}
	int i = 0;
	while(row = mysql_fetch_row(res))
	{
		int prequest, pmaster; 
		prequest = strcmp(row[2], username.c_str());
		pmaster = strcmp(row[4], username.c_str());
		if(((prequest == 0) && (atoi(row[1]) == 1)) || ((pmaster == 0) && (atoi(row[1]) == 0)))
		{
			//msgnum++;
			/*if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
			{
				cout << "mysql_real_query users failure!" <<endl;
				return NULL;
			}
	
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result users failure!" << endl;
				return -1;
			}
			
			while(row2 = mysql_fetch_row(res2))
			{
				if(strcmp(username.c_str(), row2[1]) == 0)
				{
					//cout << "dddddd-------" << endl;
					socket = atoi(row2[8]);
					return socket;
				}	
			}*/
			struct joinmsg *p_join;
			p_join = &(p_msglist->pjoinmsg[i]);
			p_join->img = atoi(row[1]);
			p_join->requester = row[2];
			//cout<<"dbdata requester:"<<row[1]<<endl;
			p_join->groupname = row[3];
			p_join->groupmaster =row[4];
			++i;
			
			if(i == msgnum)
				break;
		}
	}
	
	p_msglist->num = msgnum;
	
	return p_msglist;
	
	delete p_msglist;
	delete[] pjoinmsg;
	
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);
}

int cleangrpmsg(int img, string requester, string groupname, string groupmaster)
{
	MYSQL mysql;
	//MYSQL_RES *res;
	//MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
				
	stringstream ss;
	ss<<img;
	string simg = ss.str();
	string cmd = "delete from grp_request_msg where img='"+simg+"' && request='"+requester+"' && qunname='"+groupname+"' && qunmaster='"+groupmaster+"'";
	cout << cmd.c_str() << endl;
	if(mysql_query(&mysql, cmd.c_str()))
	{
		cout << "delete from grp_request_msg failure!" << endl;
		return -1;
	}
	else{
		cout << "delete from grp_request_msg success!" << endl;
		return 0;
	}
	
	//mysql_free_result(res);
	mysql_close(&mysql);
}

map<string, int> query_grpmsg(string username)
{
	MYSQL mysql;
	MYSQL_RES *res, *res3;
	MYSQL_ROW row, row3;
	
	map<string, int> newmsglist;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		map<string,int> temp;
		return temp;
	}

	if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return NULL;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return NULL;
	}

	int readmsgid, grpnum, msgnum;
	grpnum = 0;
	string groupmaster, groupname;
	while(row = mysql_fetch_row(res))
	{
		if(strcmp(row[1], username.c_str()) == 0)
		{
			groupname = row[2];
			readmsgid = atoi(row[4]);
			cout<<"readmsgid:"<<readmsgid<<endl;

			/*if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
			{
				cout << "mysql_real_query failure!" <<endl;
				//return NULL;
			}
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				//return NULL;
			}*/
				
					string cmd = "select * from qun_record where qunname = '"+groupname+"' order by id DESC";
					cout << cmd.c_str() << endl;
					if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
					{
						cout << "mysql_real_query failure!" <<endl;
						//return NULL;
					}
					res3 = mysql_store_result(&mysql);
					if(res3 == NULL)
					{
						cout << "mysql_store_result failure!" << endl;
						//return NULL;
					}
					
					while(row3 = mysql_fetch_row(res3))
					{
						if(readmsgid < atoi(row3[0]))
						{
							cout<<"row3[0]:"<<row3[0]<<endl;

							cout << "There are new chat message!" << endl; 
							msgnum = atoi(row3[0]) - readmsgid;
							grpnum++;
							newmsglist[groupname] = msgnum;
							
							break;
						}
					}
					mysql_free_result(res3);
		}
	}
	
	mysql_free_result(res);
	return newmsglist;
	
}

struct recordlist *querynewrcd(string username, string groupname, int newmsgnum)
{
	MYSQL mysql;
	MYSQL_RES *res2, *res3;
	MYSQL_ROW row2, row3;
	
	/* define the responding struct */
	struct recordlist *p_recordlist;
	p_recordlist = new recordlist();
	struct RECORD *precordlist = new RECORD[newmsgnum];
	p_recordlist->precordlist = precordlist;
			
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "querynewrc: mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return NULL;
	}

	/*if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}
	mysql_free_result(res);
	*/
					
	string cmd = "select * from qun_record where qunname = '"+groupname+"' order by id DESC";
	cout << cmd.c_str() << endl;
	if(mysql_real_query(&mysql, cmd.c_str(), (unsigned long)strlen(cmd.c_str())))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return NULL;
	}
	res2 = mysql_store_result(&mysql);
	if(res2 == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return NULL;
	}
	int i = 0, j = 0;
	struct RECORD *p_rcd;
	while(row2 = mysql_fetch_row(res2))
	{
		//p_rcd = (struct RECORD *)malloc(sizeof(struct RECORD));
		p_rcd = new RECORD();
		p_rcd = &(p_recordlist->precordlist[j]);
		p_rcd->username = row2[2];
		p_rcd->cur_time = row2[3];
		p_rcd->message = row2[4];
					
		string speaker;
		speaker = p_rcd->username;
		cout<<"speaker:"<<speaker<<endl;
		if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
		{
			cout << "mysql_real_query failure!" <<endl;
			//return;
		}
	
		res3 = mysql_store_result(&mysql);
		if(res3 == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			//return;
		}

		while(row3 = mysql_fetch_row(res3))
		{
			int pname;
			pname = strcmp(speaker.c_str(), row3[1]);
			if(pname == 0)
			{
				p_rcd->usrportrt = atoi(row3[7]);
				break;
			}
		}
		++j;
		mysql_free_result(res3);
		//++count;
		//delete p_rcd;
		if(j == newmsgnum)
			break;
	}
	mysql_free_result(res2);
	p_recordlist->Isquerysuccess = newmsgnum;
	
	mysql_close(&mysql);
	return p_recordlist;
		
	delete p_recordlist;
	delete[] precordlist;
	//free(precord);	
}

int update_msgid(string username, string groupname, int newmsgnum)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return -1;
	}
	
	if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
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

	int readmsgid;
	while(row = mysql_fetch_row(res))
	{
		int puser = strcmp(row[1], username.c_str());
		int pgroup = strcmp(row[2], groupname.c_str());
		if((puser == 0) && (pgroup == 0))
		{
			readmsgid = atoi(row[4]);
			break;
		}
	}
	
	readmsgid += newmsgnum;
	stringstream ss;
	ss<<readmsgid;
	string smsgid = ss.str();
	string cmd = "update user_qun set readmsgid='"+smsgid+"' where username='"+username+"' && qunname='"+groupname+"'";
	cout << cmd.c_str() << endl;
	if(mysql_query(&mysql, cmd.c_str()))
	{
		cout << "update user_qun readmsgid failure!" << endl;
		return -1;
	}
	else{
		cout << "update user_qun readmsgid success!" << endl;
		return 0;
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);
}

int setsocket(string username)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
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
				cout << "update user_qun readmsgid failure!" << endl;
				return -1;
			}
			else{
				cout << "update user_qun readmsgid success!" << endl;
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
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
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

int getqunmenbers(string qunname,string& master,vector<qunmenber>& menbers)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2, *res3;
	MYSQL_ROW row, row2, row3;
	my_ulonglong member_num;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return -1;
	}
	
	string query_master = "select * from qun_tab where qunname='"+qunname+"'";
	if(mysql_query(&mysql, query_master.c_str()))
	{
		cout << "select from qun_tab failure!" << endl;
		return -1;
	}
	else{
		cout << "select from qun_tab success!" << endl;
		res = mysql_store_result(&mysql);
		if(res == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return -1;
		}
		row = mysql_fetch_row(res);
		master = row[2];
		mysql_free_result(res);
		
		string query_member = "select * from user_qun where qunname='"+qunname+"'";
		if(mysql_query(&mysql,query_member.c_str()))
		{
			cout << "query user_qun failuer!" << endl;
			return -1;
		}
		else{
			cout << "query user_qun success!" << endl;
			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				return -1;
			}
			
			member_num = mysql_num_rows(res2);
			int ix = 0;
			while(row2 = mysql_fetch_row(res2))
			{
				string username = row2[1];
				int img, score, facetype, num_hottie;
				my_ulonglong hottie_num;
				//query the scores of this user;
				string query_score = "select * from scores where username='"+username+"'";
				if(mysql_query(&mysql,query_score.c_str()))
				{
					cout << "query scores failuer!" << endl;
					return -1;
				}
				else{
					cout << "query scores success!" << endl;
					res3 = mysql_store_result(&mysql);
					if(res3 == NULL)
					{
						cout << "mysql_store_result failure!" << endl;
						return -1;
					}
					row3 = mysql_fetch_row(res3);
					score = atoi(row3[2]);
					mysql_free_result(res3);	
				}
				//query the facetype of this user;
				string query_facetype = "select * from users where username='"+username+"'";
				if(mysql_query(&mysql,query_facetype.c_str()))
				{
					cout << "query facetype failuer!" << endl;
					return -1;
				}
				else{
					cout << "query facetype success!" << endl;
					res3 = mysql_store_result(&mysql);
					if(res3 == NULL)
					{
						cout << "mysql_store_result failure!" << endl;
						return -1;
					}
					row3 = mysql_fetch_row(res3);
					score = atoi(row3[4]);
					img = atoi(row3[7]);
					mysql_free_result(res3);	
				}
				
				//query the facetype of this user;
				string query_hottie_num = "select * from hottie where author='"+username+"'";
				if(mysql_query(&mysql,query_hottie_num.c_str()))
				{
					cout << "query hottie_num failuer!" << endl;
					return -1;
				}
				else{
					cout << "query hottie_num success!" << endl;
					res3 = mysql_store_result(&mysql);
					if(res3 == NULL)
					{
						cout << "mysql_store_result failure!" << endl;
						return -1;
					}
					
					hottie_num = mysql_num_rows(res3);
					num_hottie = hottie_num;
					mysql_free_result(res3);	
				}
				
				qunmenber menber(img, username,score,facetype,num_hottie);
				cout<<username<<score<<endl;
				cout<<member_num<<endl;
				menbers.push_back(menber); 
				++ix;
				//if(ix == member_num-1)
					//break;
			}
			mysql_free_result(res2);
		}
		
		mysql_close(&mysql);
		cout << "getqunmenbers success!" << endl;
		return 0;
	}	
}

/*
	功能：把指针requester指向对象的用户加入一个游戏，如果游戏表中存在待匹配游戏则加入其中并返回1和对手的地址。
		如果游戏表里不存在，则新建一个游戏数据项，标志为匹配状态，并返回0；失败返回-1
		game表中stat为0表示匹配状态，为1表是PK状态
*/
int join_game(long requester,long& rival)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	my_ulonglong unmatch_num;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return -1;
	}
	
	string query_game = "select * from game where stat=0";
	if(mysql_query(&mysql, query_game.c_str()))
	{
		cout << "select from game failure!" << endl;
		return -1;
	}
	else{
		cout << "select from game success!" << endl;
		res = mysql_store_result(&mysql);
		if(res == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return -1;
		}
		
		unmatch_num = mysql_num_rows(res);
		if(unmatch_num != 0)
		{
			cout << "exist unmatched game!" << endl;
			row = mysql_fetch_row(res);
			int game_id = atoi(row[0]);
			rival = atoi(row[1]);
			cout<<"rival address in dbhandle:"<<rival<<endl;
			mysql_free_result(res);
		
			stringstream ss, ss_request;
			ss<<game_id;
			ss_request<<requester;
			string gameID = ss.str();
			string player = ss_request.str();
			string query_join = "update game set player='"+player+"', stat=1 where id = '"+gameID+"'";
			if(mysql_query(&mysql, query_join.c_str()))
			{
				cout << "update game failure!" << endl;
				return -1;
			}
			else{
				cout << "update game success!" << endl;
				mysql_close(&mysql);
				return 1;
			}
		}
		else{
			cout << "There is not unmatched game!" << endl;
			
			stringstream ss_request;
			ss_request<<requester;
			string player1 = ss_request.str();
			string query_create = "insert into game (requester, stat) values('"+player1+"', 0)";
			if(mysql_query(&mysql, query_create.c_str()))
			{
				cout << "create game failure!" << endl;
				return -1;
			}
			else{
				cout << "create game  success!" << endl;
				mysql_close(&mysql);
				return 0;
			}
		}
	}	
}

/*
	功能：提供退出游戏操作，删除用户所在游戏。当用户所在游戏处于匹配态时，返回1。当
			用户所在游戏处于PK态时，返回2。失败返回-1；
	参数：requester请求者地址，rival用于返回对手地址。	
*/
int quit_game(long requester, long& rival)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	//my_ulonglong unmatch_num;
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return -1;
	}
	
	stringstream ss;
	ss<<requester;
	string player = ss.str();
	string query_game = "select * from game where requester='"+player+"' || player='"+player+"'";
	if(mysql_query(&mysql, query_game.c_str()))
	{
		cout << "select from game failure!" << endl;
		return -1;
	}
	else{
		cout << "select from game success!" << endl;
		res = mysql_store_result(&mysql);
		if(res == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return -1;
		}
		row = mysql_fetch_row(res);
		if(atoi(row[3]) == 0) 	//game under matching state;
		{
			cout << "game under matching state" << endl;
			string delete_game = "delete from game where requester='"+player+"'";
			if(mysql_query(&mysql, delete_game.c_str()))
			{
				cout << "delete from game failure!" << endl;
				return -1;
			}
			else{
				cout << "delete from game success!" << endl;
				mysql_free_result(res);
				mysql_close(&mysql);
				return 1;	
			}
		}
		else{ 	//game under PK state;
			cout << "game under PK state" << endl;
			if(requester == atoi(row[1]))
			{
				rival = atoi(row[2]);
			}
			else{
				rival = atoi(row[1]);
			}
			string delete_game = "delete from game where requester='"+player+"' || player='"+player+"'";
			if(mysql_query(&mysql, delete_game.c_str()))
			{
				cout << "delete from game failure!" << endl;
				return -1;
			}
			else{
				cout << "delete from game success!" << endl;
				mysql_free_result(res);
				mysql_close(&mysql);
				return 2;	
			}	
		}
	}
}

/*
	功能：游戏结束，删除游戏，返回对手地址，出错返回-1；
*/
long finish_game(long requester)
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	long rival;
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		mysql_close(&mysql);
		return -1;
	}
	
	stringstream ss;
	ss<<requester;
	string player = ss.str();
	string query_game = "select * from game where requester='"+player+"' || player='"+player+"'";
	if(mysql_query(&mysql, query_game.c_str()))
	{
		cout << "select from game failure!" << endl;
		return -1;
	}
	else{
		cout << "select from game success!" << endl;
		res = mysql_store_result(&mysql);
		if(res == NULL)
		{
			cout << "mysql_store_result failure!" << endl;
			return -1;
		}
		row = mysql_fetch_row(res);
		if(requester == atoi(row[1]))
		{
			rival = atoi(row[2]);
		}
		else{
			rival = atoi(row[1]);
		}
		string delete_game = "delete from game where requester='"+player+"' || player='"+player+"'";
		if(mysql_query(&mysql, delete_game.c_str()))
		{
			cout << "delete from game failure!" << endl;
			return -1;
		}
		else{
			cout << "delete from game success!" << endl;
			mysql_free_result(res);
			mysql_close(&mysql);
			return rival;	
		}	
	}
}