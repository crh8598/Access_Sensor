#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

using namespace std;
void error_handling(char* message);

int main(int argc,char **argv)
{
    int serv_sock,clnt_sock;
    // sockaddr_in 은 소켓 주소의 츨을 형성하며 구조체로 AF_INET 일 경우 사용됨
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_size;

    // TCP 연결 지항형이고 ipv4 도메인을 위한 소켓을 생성 
    serv_sock= socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1) error_handling("socket error");

// 주소를 초기화 한 후 ip주소와 포트지정
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                 // ipv4 타입
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // ip주소
    serv_addr.sin_port = htonl(atoi(argv[1]));      // port

    
    if(bind(serv_sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr)==-1))
        error_handling("bind_error");

    if(lisen(serv_sock,5)==-1)
        error_handling("listen error");
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*) &clnt_addr, &clnt_addr_size);
    
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}