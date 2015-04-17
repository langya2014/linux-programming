#include "eventstruct.h"

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
	pthread_mutex_lock(&(puser->msg_lock));
	memset(puser->data, '\0',BUFFSIZE);
	puser->msg_len = recv(sock, puser->data, BUFFSIZE, 0);
	puser->data[puser->msg_len] = '\0';
	if(puser->msg_len > 0)
	{
		//add_job(pool,sendback,puser);
		datahandle(puser);
	}
	
	if(puser->msg_len <= 0)
	{
		cout<<"a client disconnect"<<endl;
		event_del(puser->read_ev);
		event_del(puser->write_ev);
		pthread_mutex_unlock(&(puser->msg_lock));
		pthread_mutex_destroy(&(puser->msg_lock));
		delete(puser);
	}
	/*if(puser->msg_len < 0)
	{
		cout<<"recv error"<<endl;
		return;
	}*/
	pthread_mutex_unlock(&(puser->msg_lock));
}

void on_write(int sock, short event, void *arg)
{
	struct user *puser = (struct user*)arg;
	pthread_mutex_lock(&(puser->msg_lock));
	if(puser->msg.size() > 0)
	{
		string sendstr = puser->msg.front();
		puser->msg.erase(puser->msg.begin());

		memset(puser->data,'\0',BUFFSIZE);
		int len = sendstr.length();
		cout<<len<<endl;

		puser->data[0] = len&0xff;
		puser->data[1] = (len>>8)&0xff;
		cout<<puser->data[0] + puser->data[1]*256<<endl;
		
		sendstr.copy((puser->data)+2, len, 0);
		cout<<(puser->data)+2<<endl;
		//cout<<"write msg"<<puser->data<<endl;		//for testing
		send(sock, puser->data, len+2, 0);
	}
	pthread_mutex_unlock(&(puser->msg_lock));
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
