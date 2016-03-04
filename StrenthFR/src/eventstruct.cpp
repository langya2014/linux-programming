#include "eventstruct.h"
#include <dbhandle.h>
void on_accept(int sock, short event, void *arg)
{
	cout<<"accept an client"<<endl;
	int client = accept(sock,(struct sockaddr*)NULL,NULL);
	if(client == -1)
	{
		cout<<"accept client error!"<<endl;
		return;
	}
	struct user *puser = new struct user();
	puser->username ="";
	puser->prival = NULL;
	puser->sock = client;
	puser->read_ev = (struct event*)malloc(sizeof(struct event));
	event_set(puser->read_ev,client,EV_READ|EV_PERSIST,on_read,puser);
	event_base_set(base, puser->read_ev);
	event_add(puser->read_ev, NULL);

	puser->write_ev = (struct event*)malloc(sizeof(struct event));
	event_set(puser->write_ev,client,EV_WRITE|EV_PERSIST,on_write,puser);
	event_base_set(base, puser->write_ev);
	event_add(puser->write_ev, NULL);

	pthread_mutex_init(&(puser->msg_lock), NULL);

}

void on_read(int sock, short event, void *arg)
{
	//cout<<"on read"<<endl;
	struct user *puser = (struct user*)arg;
	//pthread_mutex_lock(&(puser->msg_lock));
	memset(puser->data, '\0',BUFFSIZE);
	int i = recv(sock, puser->data, 2, 0);
	//int i = recv(sock, puser->data, 4, 0);
	if(i<0)
		return;
	short int len = *(short int*)(puser->data);
	len = ntohs(len);
	//int len = *(int*)(puser->data+2);
	//int len = (puser->data[0]-'0')*1000+(puser->data[1]-'0')*100+(puser->data[2]-'0')*10+(puser->data[3]-'0');
	//cout << "data4:"<<puser->data[0]  << puser->data[1] << puser->data[2]  << puser->data[3] <<endl;
	//cout << "befor byte order transfer: "<< len << endl;
	//len = ntohl(len);
	//cout<<"length of data:"<<len<<endl;
	memset(puser->data, '\0', BUFFSIZE);
	if(len < 0)
		return;

	//puser->msg_len = recv(sock, puser->data, len, 0);
	puser->msg_len = recv(sock, puser->data, len, MSG_WAITALL);
	//cout << "msg_len: " << puser->msg_len << endl;
	if(puser->msg_len > 0)
	{
		datahandle(puser);
		//cout<<puser->data<<endl;
		//pthread_mutex_unlock(&(puser->msg_lock));
	}
	
	if(puser->msg_len <= 0)
	{
		cout<<"a client disconnect"<<endl;
		if(puser->username != "")
		{
			cout<<"setsocket re:"<<setsocket(puser->username)<<endl;

		}
		event_free(puser->read_ev);
		event_free(puser->write_ev);
		//pthread_mutex_unlock(&(puser->msg_lock));
		pthread_mutex_destroy(&(puser->msg_lock));
		delete(puser);
		close(sock);
	}
}

void on_write(int sock, short event, void *arg)
{
	struct user *puser = (struct user*)arg;
	//pthread_mutex_lock(&(puser->msg_lock));
	if(puser->msg.size() > 0)
	{
		string sendstr = puser->msg.front();
		puser->msg.erase(puser->msg.begin());
		/*short int len = sendstr.length();
		cout<<"send data length:"<<len<<endl;
		memset(puser->data,'\0',BUFFSIZE);
		len = htons(len);
		*(puser->data) = len;
		sendstr.copy((puser->data)+2, len, 0);
		cout<<"send data:"<<(char*)((puser->data)+2)<<endl;
		//cout<<"write msg"<<puser->data<<endl;		//for testing
		send(sock, puser->data, len+2, 0);*/
		cout<<"send data:"<<sendstr<<endl;
		int len = sendstr.length();
		sendstr.copy(puser->data,len,0);
		//cout << "puser->data: "<< puser->data << endl;
		send(sock,puser->data, len, 0);
	}
	//pthread_mutex_unlock(&(puser->msg_lock));
}

void sendback(void *arg)
{
	struct user *puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	puser->recvstr = puser->data;
	puser->msg.push_back(puser->recvstr);
	//cout<<"sendback msg:"<<puser->recvstr<<endl;		//for testing 
	pthread_mutex_unlock(&(puser->msg_lock));
}
