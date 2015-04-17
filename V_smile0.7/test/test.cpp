#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std;

void *fun(void *arg)
{
	cout<<"in fun:"<<pthread_self()<<endl;
}

int main(void)
{
	pthread_t a;
	pthread_create(&a,NULL,fun,NULL);
	sleep(1);
	cout<<"in main:"<<a<<endl;
}