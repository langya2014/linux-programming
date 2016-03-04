#include <iostream>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

using namespace std;
using namespace Json;

#define PORT 40001
#define IP  "139.129.27.120"

void senddata(string &msg, int client);
int main(void)
{
	int client;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if(client == -1)
	{
		cout<<"create client error"<<endl;
		return 0;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);

	connect(client,(struct sockaddr*)&addr, sizeof(struct sockaddr));
	cout<<strerror(errno);
	perror("error:");
	Value login,regist,changeinf,qunchat,addqun;
	FastWriter writer;

	regist["typecode"] = 1; 
	regist["username"] = "finder";
	regist["password"] = "123";
	regist["email"] = "1397812937@qq.com";

	login["typecode"] = 20;
	login["username"] = "finder";
	login["password"] = "123";
	
	changeinf["typecode"] = 40;
	changeinf["username"] = "finder";
	changeinf["password"] = "456";

	qunchat["typecode"] = 1200;
	qunchat["username"] = "finder";
	qunchat["img"] = 1;
	qunchat["group_name"] = "finderhu";
	qunchat["msg"] = "hello";

	addqun["typecode"] = 1060;
	addqun["requester"] = "finder";
	addqun["group_name"] = "newtest";

	string strregist = writer.write(regist);
	string strlogin = writer.write(login);
	string strchange = writer.write(changeinf);
	string strqunchat = writer.write(qunchat);
	string straddqun = writer.write(addqun);

	cout<<"1--regist   2--login   3--changeinf    4--qunchat	5--addqun"<<endl;
	while(1)
	{
		cout<<"your choice:"<<endl;
		int i = 0;
		cin>>i;
		switch(i)
		{
			case 1:senddata(strregist,client); break;
			case 2:senddata(strlogin,client); break;
			case 3:senddata(strchange,client); break;
			case 4:senddata(strqunchat,client); break;
			case 5:senddata(straddqun,client); break;
			default:break;
		}
	}
	close(client);		//close socket
	return 0;
}

void senddata(string &msg, int client)
{	
	short int datalen = msg.length();
	char* data = (char*)malloc(datalen+2);
	*data = datalen;
	msg.copy(data+2,datalen,0);
	cout<<"data length:"<<*(short int*)data<<endl;
	cout<<"send data:"<<(data+2)<<endl;
	int sendlen = send(client,data,datalen+2,0);
	cout<<"length of sended data:"<<sendlen<<endl;
	free(data);
}