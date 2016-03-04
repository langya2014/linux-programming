#include <iostream>
#include <pthread.h>
#include <vector>
#include <signal.h>
using namespace std;

void do_at_exit(int sign_no) 
{ 
	cout<<"You can see the output when the program terminates"<<endl;
	raise(SIGINT); 
} 

int main(void)
{
	//signal(SIGBUS,do_at_exit);
	signal(SIGSEGV,do_at_exit);
	char * p;
	char a = *p;
	cout<<"in main:"<<a<<endl;
}