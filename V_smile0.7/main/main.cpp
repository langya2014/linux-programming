#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include "threadpool.h"
#include "eventstruct.h"

#define IP 			"112.74.105.139"
#define PORT 		40000
#define BACKLOG 	2


//global variable
struct event_base *base;
struct threadpool *pool;

int main(void)
{
	/*max number of thread is 1000000,
	  max number sleep thread is 10,
	  min number sleep thread is 5.
	*/
	pool = init_pool(1000000,10,5);
	/*for(int i =0;i<4;i++)
	add_job(pool,pro,NULL);
	sleep(2);
	destroy_pool(pool);
	cout<<"after destroy:"<<pool->mapthread.size()<<endl;*/

	//*************init the server socket
	int lissock;
	lissock = socket(AF_INET,SOCK_STREAM,0);
	int yes = 1;
	setsockopt(lissock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));	//reuse the port
	if(lissock == -1)
	{
		cout<<"create listen socket error!"<<endl;
		return 0;
	}
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = inet_addr(IP);

	if(bind(lissock,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr))==-1)
	{
		cout<<"bind to server address error!"<<endl;
		return 0;
	}
	listen(lissock, BACKLOG);

	//*******************init the libevevnt
	base = event_base_new();
	struct event lisevent;
	event_set(&lisevent, lissock, EV_READ|EV_PERSIST, on_accept,NULL);
	event_base_set(base, &lisevent);
	event_add(&lisevent, NULL);
	event_base_dispatch(base);

	return 0;
}
