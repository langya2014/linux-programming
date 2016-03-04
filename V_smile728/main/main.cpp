#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

#include "threadpool.h"
#include "eventstruct.h"
#include <dbhandle.h>
#include <fstream>

using namespace std;

#define IP 			"123.57.78.19"
// #define IP 			"121.49.97.136"

#define PORT 		40001
#define BACKLOG 	2

struct user* privalUser = NULL;
//global variable
struct event_base *base;
struct threadpool *pool;

void do_at_exit(int sign_no) 
{ 
	printf("You can see the output when the program terminates\n"); 
	ofstream ff("log.txt",ofstream::out|ofstream::ate|ofstream::app);
	ff<<gettime()<<endl;
	ff.close();
	set_allskt();
	raise(SIGTERM); 
} 

int main(void)
{
	signal(SIGBUS,do_at_exit);
	signal(SIGSEGV,do_at_exit);
	signal(SIGINT,do_at_exit);
	/*max number of thread is 1000000,
	  max number sleep thread is 10,
	  min number sleep thread is 5.
	*/
	//pool = init_pool(1000000,10,5);
	pool = init_pool(10,5,3);
	/*for(int i =0;i<4;i++)
	add_job(pool,pro,NULL);
	sleep(2);
	destroy_pool(pool);
	cout<<"after destroy:"<<pool->mapthread.size()<<endl;*/

	//*************init the server socket
	int lissock;
	lissock = socket(AF_INET,SOCK_STREAM,0);
	cout<<"listen socket"<<lissock<<endl;
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
