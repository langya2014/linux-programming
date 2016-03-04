#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

int main(void)
{
        int lissock;
        lissock = socket(AF_INET,SOCK_STREAM, 0);
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(40000);
        addr.sin_addr.s_addr = inet_addr("112.74.105.139");
        bind(lissock, (struct sockaddr*)&addr, sizeof(struct sockaddr));
        listen(lissock, 2);
        int client = accept(lissock, (struct sockaddr*)NULL,NULL);

}
