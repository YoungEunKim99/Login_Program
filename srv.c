////////////////////////////////////////////////////////////////////////////////
// File name	:srv.c   				 
// Date		:2020/06/06						
// Os		:Ubuntu 16.04.5 64bits                      
// Author	:Kim Young Eun					
// Student ID	:2018202037					
//                         			
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
#include <pwd.h>
struct passwd *fgetpwent(FILE* stream);
#define MAX_BUF 20
/*///////////////////////////////////////////////////////////////////////////////
//determine user may log-in or not					     	
// 							                     
// input:  connfd  			      				     
// return: int 	0: fail							      
// 		1: success					             
//Perpose: get user id,pawword from client and determine user may log-in or not				     
///////////////////////////////////////////////////////////////////////////////*/
int log_auth(int connfd);
/*//////////////////////////////////////////////////////////////////////////////
//compare registered id,password with id,password that user entered id						     	
// 							                     
// input: char*user char*passwd -> id,passwd that user entered	    
// return: int  0: fail							      
// 		1: success					             
//Perpose: compare registered id,password in passwd file with id,password that user entered				     
//////////////////////////////////////////////////////////////////////////////*/
int user_match(char *user,char*passwd);
/*///////////////////////////////////////////////////////////////////////////////
//determine user may connect to server or not					     	
// 							                     
// input:  ciladdr,connfd			      				     
// return: int 	0: fail							      
// 		1: success					             
//Perpose:determine user may connect to server or not		     
///////////////////////////////////////////////////////////////////////////////*/
int client_info(struct sockaddr_in *cliaddr,int connfd);
int main(int argc,char**argv)
{

	
	int listenfd, connfd;
	int clilen;
	struct sockaddr_in servaddr, cliaddr;
	listenfd=socket(PF_INET,SOCK_STREAM,0); //make socket
	memset(&servaddr,0,sizeof(servaddr));
	//set server's socket address
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(atoi(argv[1]));
	
	bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));//bind server

	listen(listenfd,5);
	while(1){
		//accept client's connection 
		connfd=accept(listenfd,(struct sockaddr *) &cliaddr, &clilen);
		
		if(client_info(&cliaddr,connfd)==0){
		continue;
		}
		
		if(log_auth(connfd)==0){//if fail to login
			printf("** Fail to log-in **\n");
			
		}
		else{//if success to login
			printf("** Success to log-in **\n");
			
		}
		
		close(connfd);
		
		
	}	
	
		
	return 0;
}

int log_auth(int connfd){
	char user[MAX_BUF], passwd[MAX_BUF];
	int n, count=1;

	while(1){
		n=read(connfd,user,MAX_BUF);//get user id
		user[n]='\0';
		n=read(connfd,passwd,MAX_BUF);//get password
		passwd[n]='\0';
		
		write(connfd,"OK",MAX_BUF);
		printf("** User is trying to log-in (%d/3) **\n",count);
		if((n=user_match(user,passwd))==1){//if user enter correct id,password
			write(connfd,"OK",MAX_BUF);
			return 1;
		}
		else if(n==0){
			if(count>=3){//if user enter wrong id,password 3 times
			printf("** Log-in failed **\n");
			write(connfd,"DISCONNECTION",MAX_BUF);
			return 0;
			}
			
			write(connfd,"FAIL",MAX_BUF);//if user enter wrong id,password
			printf("** Log-in failed **\n");
			count++;
			
		}
	}
	
}
int user_match(char *user,char*passwd){
	FILE *fp;
	struct passwd *pw;
	int exist=0;
	fp=fopen("passwd","r");
	while((pw=fgetpwent(fp))!=NULL){
		//compare id,password to registered id,password
		if(!strcmp(user,pw->pw_name)&&!strcmp(passwd,pw->pw_passwd)){
			exist=1;
			
		}
	}
	if(exist==1)
		return 1;//if correct passwrd,id
	else 
		return 0;
}

int client_info(struct sockaddr_in *cliaddr,int connfd){
	char buf[MAX_BUF];
	FILE *fd;
	char* ptr;
	char* ptr2;
	char ip[MAX_BUF];
printf("** Client is trying to connect **\n");
printf(" -IP: %s\n",inet_ntoa((*cliaddr).sin_addr));//print client ip
printf(" -Port:%d\n",ntohs((*cliaddr).sin_port));//print client port number

	fd=fopen("access.txt","r");
		if(fd!=NULL){
			
			int success=0;
			while(!feof(fd)){
				fgets(buf,MAX_BUF,fd);
				if(!strcmp(buf,"\n")){// if access.txt has space line
					continue;
				}
				if((ptr2=strstr(buf,".*"))!=NULL){
					ptr=strtok(buf,"*");
					strcpy(ip,ptr);
					ip[strlen(ip)-1]='\0';
				}
				else{
					ptr=strtok(buf,"\n");
					strcpy(ip,ptr);
				}
				if(strstr(inet_ntoa((*cliaddr).sin_addr),ip)){
					success=1;//if ip address exist
					
				}
			}
			if(success==1){//if client may connect to server
				fclose(fd);
				write(connfd,"ACCEPTED",MAX_BUF);
				printf("** Client is connected **\n");
				return 1;
			}
			else{
				write(connfd,"REJECTION",MAX_BUF);
				printf("** It is NOT authenticated client **\n");
				return 0;
			}
			
		}
}

