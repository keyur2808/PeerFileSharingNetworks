/*************************************************************************************************************************************

	                               C PROGRAM TO CONNECT PEERS AND DOWNLOAD FILES
	                                           KEYUR JOSHI(keyurjos)
					           keyurjos@buffalo.edu

**************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int mode,info;
int port;
int peer;
int sockfd,listen_sd,max_sd,new_sd,connsockfd;
int desc_ready,end_server=FALSE;
int close_conn;
int k=0;
int i,len,yes=1;
int stdc;
int  sock[3],l[3];
int p[10];
int flag;
unsigned int j,m,rc=4;
long fsize;

char *server;
char *chnkptr;
char *PORT;
char *ip_host;
char lst[100],table[300],buf[4][200];
char cmd[1150];
char buffer[1150];
char remoteIP[INET6_ADDRSTRLEN];
char s[10][INET6_ADDRSTRLEN];

fd_set master_set,working_set;

struct timeval timeout,time_bef,time_af;
struct sockaddr_storage remoteaddr; // client address
struct addrinfo hints, *loadinfo, *ptr;
struct sockaddr ip;

socklen_t addrlen;//client address length




void console_read(char *);
void MyIP();



//-------------------------------------------------CONVERT IP ADDRESS-----------------------------------------------------------------
void *get_in_addr(struct sockaddr *sa)
{
if (sa->sa_family == AF_INET) {
return &(((struct sockaddr_in*)sa)->sin_addr);
}
return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//------------------------------------------------------CONVERT ----------------------------------------------------------------------
unsigned int get_in_port(struct sockaddr *sa)
{
if (sa->sa_family == AF_INET) {
return (((struct sockaddr_in*)sa)->sin_port);
}
return (((struct sockaddr_in6*)sa)->sin6_port);
}

//------------------------------------------------SET THE SERVER STRUCTURE TO---------------------------------------------------------
void servcl_memset(){
memset(&hints, 0, sizeof(hints));
hints.ai_family= AF_UNSPEC;
hints.ai_socktype= SOCK_STREAM;
}





void ldesc(){
             info= fcntl(0, F_SETFL,O_NONBLOCK);
             if (info < 0)
	       {
               perror("fl failed");
	       close(0);
	       exit(-1);
               }
	     max_sd = listen_sd;
	     FD_SET(listen_sd, &master_set);
             if (mode==1)FD_SET(sockfd,&master_set);
             }


//---------------------------------------------------PEER LIST PRINT & SEND-----------------------------------------------------------
void printpeer(){
                char tm[200];
		printf("%s\n",lst);
                printf("%s\n",table);
                if (mode==2){stdc=3;}else{stdc=4;if(flag==0)printf("\n\t%d \t%s \t%d ",1,server,port);}
                
                for (k=0;k<max_sd-stdc;k++)
                   {
                   sprintf(tm,"\n\t%d \t%s \t%d ",k+stdc-2,s[k],p[k]);
                   strcpy(buf[k],tm);
                   printf("%s\n",buf[k]);

                   }
               }

void sendlist(){int g;char *slist="\t\tServer List Update\n";
                for(g=stdc+1;g<max_sd+1;g++){if(mode==2){strcpy(buffer,slist);if(send(g,buffer,sizeof buffer,0)<0){perror("");}}
                strcpy(buffer,table);
                if(send(g,buffer,sizeof buffer,0)<0){perror("");}
                for(k=0;k<max_sd-stdc;k++){strcpy(buffer,buf[k]);if(send(g,buffer,sizeof buffer,0)<0){perror("");}} }
                }
//--------------------------------------------------------PARSE INCOMING DATA---------------------------------------------------------
void read_data(char *cm){
                 char cmdf[20],arg1[128],arg2[5],arg3[4],arg4[1000];
                 sscanf(cm,"%s %s %s %s %s",cmdf,arg1,arg2,arg3,arg4);

              if (strcmp(cmdf,"list") ==0){sendlist();return;}


               if (strcmp(cmdf,"Error")==0){printf("%s",cm);return;}

                /*if (strcmp(cmdf,"File")==0){cm=cm+(strlen(cmdf)+strlen(arg1)+strlen(arg2)+strlen(arg3)+4);
                if (sock[atoi(arg3)-1]==0){int t=(atoi(arg3))*(atoi(arg2));sock[atoi(arg3)]=sock[atoi(arg3)]+t;}
                int fp=open(arg1,O_WRONLY|O_CREAT|O_APPEND,00777);lseek(fp,sock[atoi(arg3)],SEEK_SET);
                if (fp<0){printf("%d",fp);close(fp);};int rbytes;
                 if((rbytes=write(fp,cm,strlen(cm)))<0){
                perror("File:");
                   } close(fp);
               //printf("%d %d\n",arg3,arg2);
                     
              return;}*/

                if (strcmp(cmdf,"Size")==0){FILE *tmpf=fopen(arg1,"r");if(tmpf!=NULL){
                fseek(tmpf,0,SEEK_END);
                fsize=ftell(tmpf);rewind(tmpf);
                fclose(tmpf);
                unsigned int f=htonl(fsize);
                //printf("%s",buffer);
                send(i,&f,sizeof f,0);}
                else{
                char *temp="Error in file/doesnot exists";strcpy(buffer,temp);
                send(i,buffer,sizeof buffer,0);}

                return;}

                if (strcmp(cmdf,"download")==0){
                //char data[1000];
                FILE *fs=fopen(arg1,"r");
                if (fs==NULL){char *temp="Error in file/doesnot exists";strcpy(buffer,temp);
                send(i,buffer,sizeof buffer,0);fclose(fs);
		//free(temp);
                return;
		}
                else{int bytes;
                FILE *fs=fopen(arg1,"r");
                while (!feof(fs)){bzero(buffer,sizeof buffer);
                      bytes=fread(buffer,1,atoi(arg2),fs);
                      printf("\n%s",buffer);					 
           if((send(i,buffer,sizeof buffer,0))<0){perror("tx fail");}
            
                };
                char *t="ENDF";strcpy(buffer,t);if((send(i,buffer,sizeof buffer,0))<0){perror("tx fail");}else{printf("\tFile sent");}
                fclose(fs);}//End else

               return ;}



}

void nonblk(int *sock_ptr){info= fcntl(*sock_ptr, F_SETFL,O_NONBLOCK);
		  if (info < 0)
		  {
		   perror("fcntl() failed");
		   close(*sock_ptr);
		   exit(-1);
                  }
                  }

//------------------------------------------------------BIND SOCKET TO PORT-----------------------------------------------------------
void sock_bind(){info= bind(listen_sd,loadinfo->ai_addr,loadinfo->ai_addrlen);
 		   if (info < 0){
		   perror("bind() failed");
		   close(listen_sd);
		   return;}
 		  }
//-----------------------------------------------------START LISETING ON PORT---------------------------------------------------------
void set_listen(){info = listen(listen_sd, 4);
		 if (info < 0)
		 {
		 perror("listen() failed");
		 close(listen_sd);
		 exit(-1);
		 }
		 ldesc();
                }
//------------------------------------------------------SELECT FUNCTION---------------------------------------------------------------
void start_listen(){
		    do
			{
			memcpy(&working_set, &master_set, sizeof(master_set));
			printf("\nWaiting for new connections...\n");
			info = select(max_sd + 1, &working_set, NULL, NULL, NULL);

			if (info < 0){
			perror(" select() failed");break;
			}

			if (info == 0)
			{
		         printf(" Function timed out. End program.\n");
			break;
			}

                        desc_ready = info;
			for (i=0; i <= max_sd && desc_ready > 0; ++i)
			 {
			  if (FD_ISSET(i, &working_set)){
			  desc_ready -= 1;
			  if (i == listen_sd)
                           {
                            printf(" Listening socket is readable\n");

			    do
                              {
			       addrlen = sizeof remoteaddr;
			       new_sd = accept(listen_sd, (struct sockaddr *)&remoteaddr,&addrlen);
                               if (new_sd < 0){
                                 if (errno != EWOULDBLOCK)
                                 {
                                  perror(" accept() failed");
                                  end_server = TRUE;
                                  }
                                  break;
                                 }
                                 //int len=sizeof ip;


                               printf(" New incoming connection - %d\n", new_sd);

                               if (mode==2){j=new_sd-4;
                               if((info=recv(new_sd,buffer,sizeof buffer,0))<=0)
                                {perror("recv:");}

                                 printf("Server: new connection from %s on "
				 "socket %d from client port %d\n",
				 inet_ntop(remoteaddr.ss_family,
                                 get_in_addr((struct sockaddr*)&remoteaddr),
                                 remoteIP, INET6_ADDRSTRLEN),
                                 new_sd,atoi(buffer));strcpy(s[j],remoteIP);p[j]=atoi(buffer);}//Print incoming details server;

                                 else{j=new_sd-5;
                                 printf("Peer: new connection from %s on "
				 "socket %d from client port %d\n",
				 inet_ntop(remoteaddr.ss_family,
                                 get_in_addr((struct sockaddr*)&remoteaddr),
                                 remoteIP, INET6_ADDRSTRLEN),
                                 new_sd,ntohs(get_in_port((struct sockaddr *)&remoteaddr)));

                                 strcpy(s[j],remoteIP);p[j]=ntohs(get_in_port((struct sockaddr *)&remoteaddr));

                                 }//Print for client;



				FD_SET(new_sd, &master_set);//Add new socket to list
                                if (new_sd > max_sd)//Update id
                                 max_sd = new_sd;

                               // Print peer list
                              printpeer();
                              //Sendlist to clients;
                              sendlist();


                              } while (new_sd != -1);
                            }//If readable

                      else{//i!=listen_sd/Operate on existing connections
                          close_conn = FALSE;
                          do{ if (i==0) {
                                         info=read(i,buffer,sizeof buffer);
                                         if (info <= 0) break;
                                         console_read(buffer);break;}

                              info = recv(i, buffer, sizeof buffer, 0);
			      if (info < 0){
			       if (errno != EWOULDBLOCK){
		                 perror(" recv() failed");
				 close_conn = TRUE;}
                              break;}
                              if (info == 0){
			      printf(" Connection closed\n");
			      close_conn = TRUE;
			      break;}



                          if (mode==1){if (i==sockfd){printf("%s",buffer);continue;}printf("\nData rcvd");strcpy(cmd,buffer);
                           read_data(cmd);break;
                           }

                         if(mode==2){printf("\nSending peer list");strcpy(cmd,buffer);read_data(cmd);break;
                              }

                          }while(TRUE );
                          if (close_conn){if(i==sockfd){flag=1;printf("Server disconnected");}
                          close(i);
                          FD_CLR(i, &master_set);if(mode==2){strcpy(s[i-4],"");p[i-4]=0;}else{strcpy(s[i-5],"");p[i-5]=0;}
                            if (i == max_sd){
                                          while (FD_ISSET(max_sd, &master_set) == FALSE)
                                          max_sd -= 1;

                                         }
                              printpeer();sendlist();
                              }
                          }//END of else
                        }//If new conn
                     }//For loop
          end_server==TRUE;} while (end_server == FALSE);

                }
//------------------------------------------------------Initialize Listening Socket---------------------------------------------------
void sock_init(){
                 servcl_memset();
                 hints.ai_flags=AI_PASSIVE;
                 hints.ai_family=AF_INET;

                 if ((info = getaddrinfo(INADDR_ANY, PORT, &hints, &loadinfo)) != 0) {
                 fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(info));
                 return;}

		 if ((listen_sd = socket(loadinfo->ai_family, loadinfo->ai_socktype,loadinfo->ai_protocol)) == -1) {
		 perror("server: socket");
             	 return;}

                 info = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(int));
		 if (info < 0){
	 	 perror("setsockopt() failed");
		 close(listen_sd);return;}


              }
//------------------------------------------------------CONNECT TO PEERS--------------------------------------------------------------
void cl_connect(char *arg1,char *arg2){int port=atoi(arg2); //Connect function to access other clients/peers

                  connsockfd=socket(AF_INET,SOCK_STREAM,0);

                  struct sockaddr_in serv;
                  memset(&serv, 0, sizeof(serv));
                  serv.sin_family = AF_INET;
                  inet_pton(AF_INET,arg1,&serv.sin_addr.s_addr);
                  serv.sin_port = htons(port);

                  if (connect(connsockfd,(struct sockaddr*)&serv,sizeof serv) == -1) {
                  close(sockfd);
                  perror("client: connect");
                  return;}
                  nonblk(&connsockfd);
                  FD_SET(connsockfd,&master_set);
                  if (connsockfd>max_sd)max_sd=connsockfd;p[k]=port;strcpy(s[k],arg1);
                  }
//------------------------------------------------------RGEISTER WITH SERVER----------------------------------------------------------
void registercl(char *arg1,char *arg2){server=arg1;port=atoi(arg2);//Register to server and activate listening socket
                  sock_init();
                  sock_bind();
                  set_listen();
                  nonblk(&listen_sd);

                  sockfd=socket(AF_INET,SOCK_STREAM,0);

                  struct sockaddr_in serv;
                  memset(&serv, 0, sizeof(serv));
                  serv.sin_family = AF_INET;
                  inet_pton(AF_INET,arg1,&serv.sin_addr.s_addr);
                  serv.sin_port = htons(port);
                  //p[0]=port;
                  //strcpy(s[0],arg1);k++;

                  if (connect(sockfd,(struct sockaddr*)&serv,sizeof serv) == -1) {
                  close(sockfd);
                  perror("client: connect");
                  return;}

                  FD_SET(sockfd,&master_set);if (max_sd<sockfd)max_sd=sockfd;
                  nonblk(&sockfd);


                  if((info=send(sockfd,PORT,sizeof PORT,0))<=0)
                  {perror("send:");}

                  start_listen();

                  }


//----------------------------------------------------GET IP ADDRESS------------------------------------------------------------------
void MyIP(){char sl[INET6_ADDRSTRLEN];//Get my ip address
            int sockfdip;
            char *GoogleDns="8.8.8.8";
            socklen_t len=sizeof ip;
            struct sockaddr_in serv;
            memset(&serv, 0, sizeof(serv));
            serv.sin_family = AF_INET;
            inet_pton(AF_INET,GoogleDns,&serv.sin_addr.s_addr);
            serv.sin_port = htons(53);
            if((sockfdip=socket(AF_INET,SOCK_DGRAM,0))<0){
            perror("socket:");}
            if (connect(sockfdip,(struct sockaddr*)&serv,sizeof serv) == -1) {
             close(sockfdip);
             perror("client: connect");
             }
           getsockname(sockfdip,(struct sockaddr *)&ip , &len);
           inet_ntop(AF_INET, get_in_addr((struct sockaddr *)&ip),sl, sizeof sl);
           printf("My IP address is %s\n", sl);
	   close(sockfdip);}

//----------------------------------------------------CREATOR INFO-------------------------------------------------------------------
void creator(){
		printf("Keyur Joshi\n");
                printf("keyurjos\n");
                printf("keyurjos@buffalo.edu\n");

}

//------------------------------------------------------CLOSE SOCKET------------------------------------------------------------------
void terminate(int c){c=c+3;
                     if(close(c)<0){perror("Close fail");}
                     FD_CLR(c, &master_set);strcpy(s[c-5],"");p[c-5]=0;
                            if (c == max_sd){
                                          while (FD_ISSET(max_sd, &master_set) == FALSE)
                                          max_sd -= 1;
                                         }
                              printpeer();sendlist();
                             

 }

//----------------------------------------------------DISPLAY HELP-------------------------------------------------------------------
void help(){printf("usage:./prog1.out c/s(client|server mode) listening_port\n");
            printf("help-Displays this help\n");
            printf("myip-Displays the IP address\n");
            printf("register server_address port-Register client to server specified by IP address\n\t\t\t   & port no\n");
            printf("connect peerip port-Connect to peer with IP address and Port no\n");
            printf("download filename chunksize-download files from peer in given chunks\n");
            printf("terminate i-Close connection i\n");
            printf("exit-Exit the Program\n");
            }
//----------------------------------------------------SCAN CONSOLE INPUT--------------------------------------------------------------
void console_read(char *cmd){char cmdf[20],arg1[128],arg2[34];//Read from console and parse the string command

               //strcpy(snd,cmd);*Here strcmp has been used ,for OS specifc prog strcasecmp can be used to ignore case*
               sscanf(cmd,"%s %s %s",cmdf,arg1,arg2);
               if ( (strcmp(cmdf,"exit")==0) ||(strcmp(cmdf,"exit")==0) )
                   exit(0);

               
               if ( (strcmp(cmdf,"terminate")==0) ||(strcmp(cmdf,"TERMINATE")==0) )
                   terminate(atoi(arg1));


               if ( (strcmp(cmdf,"CREATOR")==0) ||(strcmp(cmdf,"creator")==0) ){
                    creator();return;}

               if ( (strcmp(cmdf,"HELP")==0) ||(strcmp(cmdf,"help")==0) ){
                    help();return;}

          
               if ( (strcmp(cmdf,"MYIP")==0) ||(strcmp(cmdf,"myip")==0) ){
               MyIP();return;}

               if ( (strcmp(cmdf,"register")==0) ||(strcmp(cmdf,"REGISTER")==0) ){
               registercl(arg1,arg2);return;
               }

               if ( (strcmp(cmdf,"connect")==0) ||(strcmp(cmdf,"CONNECT")==0) ){
               cl_connect(arg1,arg2);
               return;
               }

              if ( (strcmp(cmdf,"list")==0) ||(strcmp(cmdf,"list")==0) ){printpeer();printf("\n\n");
               send(sockfd,buffer,sizeof buffer,0);return;}

               if (strcmp(cmdf,"download")==0){
               printf("Download requested\n");//memset(buffer,0,sizeof buffer);
               strcpy(buffer,"Size ");strcat(buffer,arg1);
               send(5,buffer,sizeof buffer,0);unsigned int d;int flags=fcntl(5,F_GETFL);//Send file size request
               fcntl(5,F_SETFL,flags&(~O_NONBLOCK));
               while((recv(5,&d,sizeof d,0))<0);//Wait for filesize
               fsize=(long)ntohl(d);
               printf("File size is%ld\n",fsize);//Print filesize
               long j=fsize/(atoi(arg2));

               int fp=open(arg1,O_WRONLY|O_CREAT|O_APPEND,00777);
               if (fp<0){perror("File:");close(fp);return;};int rbytes; 
               

               sprintf(buffer,"download %s %s %d",arg1,arg2,0);
               send(5,buffer,sizeof buffer,0);gettimeofday(&time_bef, NULL);
               while(fp<j){bzero(buffer,sizeof buffer);         
               if((recv(5,buffer,sizeof buffer,0))<0){perror("rx fail");break;}
                  if(strcmp(buffer,"ENDF")==0)break;
                   if((rbytes=write(fp,buffer,atoi(arg2)))<0){
                     perror("File:");close(fp);break;} 
                   
               };gettimeofday(&time_af, NULL);close(fp);
              double time_taken = (time_af.tv_sec - time_bef.tv_sec)*1e6 + (time_af.tv_usec - time_bef.tv_usec);
              printf("Time taken %f \n",time_taken);
              
              return;}

             }

int main(int argc,char * argv[])
{
if (argc<3)
{
printf("usage:./prog1.out c/s(client|server mode) listening_port\n");
exit(0);
}

if ( (strcmp(argv[1],"s")==0) || (strcmp(argv[1],"S")==0) ){mode=2;}else {mode=1;}

//mode=atoi(argv[1]);
PORT=argv[2];
FD_ZERO(&master_set);
printf("\nMy Port %s\n",PORT);
sprintf(lst,"\n\t\tList of Peers\n");
sprintf(table,"\tId \tAddress \tPort");
timeout.tv_sec=0;
timeout.tv_usec=3;

help();//DISPLAY HELP

 if (mode ==1){
               printf("In client/peer mode\n");
	       FD_SET(0,&master_set);

               for(;;){
               printf("\nproj1>>");
               gets(cmd);
               console_read(cmd);
               }//END FOR
               }
else{sock_init();nonblk(&listen_sd);sock_bind();set_listen();MyIP();start_listen();
}
return 0;
}
