////////////////////////////////////////////////////////////////////////////////
// File name	:cli.c   			
// Date		:2020/06/06						
// Os		:Ubuntu 16.04.5 64bits                                      
// Author	:Kim Young Eun			
// Student ID	:2018202037		
// Title	:log-in Server		     
// Description	:client request log-in to server
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define MAX_BUF 20
#define CONT_PORT 20001
////////////////////////////////////////////////////////////////////////////////
//log-in to server						     	
// 							                     
// input: sockfd  ->socket				             
//Perpose: log-in to server					     
////////////////////////////////////////////////////////////////////////////////
void log_in(int sockfd);
int main(int argc,char**argv){


	int sockfd,n,p_pid;
	struct sockaddr_in servaddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);//make socket
	memset(&servaddr,0,sizeof(servaddr));
	//set the addresss of server
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(atoi(argv[2]));
	connect(sockfd,(struct SA *)&servaddr, sizeof(servaddr));//connet to server
	log_in(sockfd);
	close(sockfd);
	return 0;

}

void log_in(int sockfd){
 	int n;
	char user[MAX_BUF], passwd[MAX_BUF], buf[MAX_BUF];
	n=read(sockfd,buf,MAX_BUF); //recieve message from server
	buf[n]='\0';
	
	if(!strcmp(buf,"REJECTION")){
		printf("** Connection refused **\n");
		exit(0);
	}
	else if(!strcmp(buf,"ACCEPTED")){
		printf("** It is connected to Server **\n");
	}
	else{
		exit(0);
	}
 	
	 while(1){
		printf("Input ID : ");
		scanf("%s",user);//get user id
		write(sockfd,user,MAX_BUF);
		printf("Input Password : ");
		scanf("%s",passwd);//get password
		write(sockfd,passwd,MAX_BUF);
		n=read(sockfd, buf, MAX_BUF);
		buf[n]='\0';
		if(!strcmp(buf, "OK")){
			
			n=read(sockfd, buf, MAX_BUF);
			buf[n]='\0';

			if(!strcmp(buf,"OK")){//if user enter correct id,password
				printf("** User %s logged in **\n",user);
				break;
			}
			else if(!strcmp(buf,"FAIL")){//if user enter wrong id,password
				printf("** Log-in failed **\n");
			}
			else{//if user enter wrong id,password 3 times
				printf("** Connection closed **\n");
				break;
			}
		}
		
	}
 
}
	

