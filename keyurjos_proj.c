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


#define TRUE 1
#define FALSE 0

int mode,info;
int port;
int count;
int peer;
int sockfd,listen_sd,max_sd,new_sd,connsockfd;
int desc_ready,end_server=FALSE;
int close_conn;
int k=0;
int i,len,yes=1;
int stdc;
int  sock[3],l[3];
int flag,nm;
int reg;
unsigned int j,m,rc=4;
long fsize;

char *svc;
char *server;
char *chnkptr;
char *PORT;
char *ip_host;
char lst[100],table[300],buf[4][200],name[4][200];
char cmdf[20],arg1[128],arg2[5];
char cmd[1150];
char buffer[1150];
char remoteIP[INET6_ADDRSTRLEN];
char sl[INET6_ADDRSTRLEN];

fd_set master_set,working_set;

struct timeval timeout;
struct sockaddr_storage remoteaddr; // client address
struct addrinfo hints, *loadinfo, *ptr;
struct sockaddr ip;
struct clist;


socklen_t addrlen;//client address length


void console_read(char *);
void MyIP();


struct clist{
int id;
int  p;
char ip[INET6_ADDRSTRLEN];
char name[100];
}cl[4],apr[4];


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
 void printpeer(struct clist clptr[]){
                 if ( (mode==1) && (i==sockfd) )printf("\nServer Update\n");
                 printf("%s\n",lst);
                 printf("%s\n",table);
                 if (mode==2){stdc=3;}else{stdc=4;if(flag==0)printf("\n\t%d \t%s \t%s \t%d ",1,cl[0].name,server,port);}
                
        for (k=0;k<max_sd-stdc;k++)			
                   {
                   printf("\n\t%d \t%s    \t%s \t%d ",k+stdc-2,clptr[k].name,clptr[k].ip,clptr[k].p);
                   
                   }
               }


void update(struct clist clptr[]){printf("\t\t Server Update \n");
                                  printf("%s\n",table);
                     for (k=0;k<4;k++)
                   {
                   printf("\n\t%d \t%s    \t%s \t%d ",clptr[k].id,clptr[k].name,clptr[k].ip,clptr[k].p);
                   
                   }

             }

void sendlist(){int g;
                for(g=stdc+1;g<max_sd+1;g++){if (FD_ISSET(g,&master_set)){
                                                if(send(g,cl,sizeof cl,0)<0){perror("");} 
                                              }
                                            } 
               }
//--------------------------------------------------------PARSE INCOMING DATA---------------------------------------------------------

void chk1(){
            sprintf(buffer,"download %s %s %d",arg1,arg2,0);
            send(5,buffer,sizeof buffer,0);
           }

void chk2(){
            sprintf(buffer,"download %s %s %d",arg1,arg2,1);
            send(6,buffer,sizeof buffer,0);
           }


void chk3(){
            sprintf(buffer,"download %s %s %d",arg1,arg2,2);
            send(7,buffer,sizeof buffer,0);
           }

//--------------------------------------------------------------------------------------------------------------------------------------
void read_data(char *cm){
                 char arg3[4],arg4[1000];
                 sscanf(cm,"%s %s %s %s %s",cmdf,arg1,arg2,arg3,arg4);

              if (strcmp(cmdf,"list") ==0){sendlist();return;}


               if (strcmp(cmdf,"Error")==0){printf("%s",cm);return;}

                if (strcmp(cmdf,"File")==0){cm=cm+(strlen(cmdf)+strlen(arg1)+strlen(arg2)+strlen(arg3)+4);int nn=0;
                  if (sock[atoi(arg3)]==0){int t=(atoi(arg3))*(atoi(arg2));sock[atoi(arg3)]=sock[atoi(arg3)]+t;}
                  int fp=open(arg1,O_WRONLY|O_CREAT|O_APPEND,00777);lseek(fp,sock[atoi(arg3)],SEEK_SET);
                  if (fp<0){printf("%d",fp);close(fp);};int rbytes;
                  if((rbytes=write(fp,cm,atoi(arg2)))<0){
                  perror("File:");
                   }
                  nn=fp; close(fp);chnkptr[sock[atoi(arg3)]]='1';sock[atoi(arg3)]=sock[atoi(arg3)]+3*atoi(arg2);
                      if(nn<j){
                        switch(atoi(arg3)){
                          case 1:
                                chk1();break;
                          case 2:
                                chk2();break;
                          default:
                                  chk3();
                                       }
                               }
                return;}

                if (strcmp(cmdf,"Size")==0){FILE *tmpf=fopen(arg1,"r");if(tmpf!=NULL){
                fseek(tmpf,0,SEEK_END);
                fsize=ftell(tmpf);rewind(tmpf);
                fclose(tmpf);
                unsigned int f=htonl(fsize);
                //printf("%s",buffer);
                send(i,&f,sizeof f,0);}
                else{
                char *temp="Error in file/doesnot exists";strcpy(buffer,temp);
                send(i,buffer,sizeof buffer,0);
                }
                return;}

                if (strcmp(cmdf,"download")==0){
                char data[1000];
                int fs=open(arg1,O_RDONLY);
                if (fs<0){char *temp="Error in file/doesnot exists";strcpy(buffer,temp);
                send(i,buffer,sizeof buffer,0);close(fs);
		//free(temp);
                return;
		}
                else{int bytes;int t=(atoi(arg3))*(atoi(arg2));
                int fs=open(arg1,O_RDONLY);if (l[atoi(arg3)]==0){l[atoi(arg3)]=l[atoi(arg3)]+t;}
                lseek(fs,l[atoi(arg3)],SEEK_SET); bzero(data,sizeof data);
                if ((bytes=read(fs,data,atoi(arg2)*sizeof (char)))>0){
                      bzero(buffer,sizeof buffer);
                      sprintf(buffer,"File %s %s %s %s",arg1,arg2,arg3,data);printf("\n%s",data);
                      if((send(i,buffer,sizeof buffer,0))<0){perror("tx fail");}
                }l[atoi(arg3)]=l[atoi(arg3)]+(3*(atoi(arg2)));//
                close(fs);}//End else

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
                                 new_sd,atoi(buffer));
                                 
                                if((getnameinfo((struct sockaddr*)&remoteaddr,sizeof remoteaddr,cl[j].name,100 ,svc,sizeof svc,0))!=0)
                                 fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(info));
                 
                                 strcpy(cl[j].ip,remoteIP);cl[j].p=atoi(buffer);//Load structure
                                 cl[j].id=j+1;

                                
                                }//Print incoming details server;

                                 else{j=new_sd-5;
                                 printf("Peer: new connection from %s on "
				 "socket %d from client port %d\n",
				 inet_ntop(remoteaddr.ss_family,
                                 get_in_addr((struct sockaddr*)&remoteaddr),
                                 remoteIP, INET6_ADDRSTRLEN),
                                 new_sd,ntohs(get_in_port((struct sockaddr *)&remoteaddr)));

                           if((info=getnameinfo((struct sockaddr*)&remoteaddr,sizeof remoteaddr,cl[j].name,100 ,svc,sizeof svc,0))!=0)
                                 fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(info));
                 
                                 strcpy(cl[j].ip,remoteIP);cl[j].p=ntohs(get_in_port((struct sockaddr *)&remoteaddr));
			         cl[j].id=j+2;
                                 }//Print for client;



				FD_SET(new_sd, &master_set);//Add new socket to list
                                if (new_sd > max_sd)//Update id
                                 max_sd = new_sd;

                               // Print peer list
                              printpeer(cl);
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



                          if (mode==1){if (i==sockfd){memcpy(&apr,&buffer,sizeof (apr));update(apr);break;}printf("\nData rcvd");
                           strcpy(cmd,buffer);
                           read_data(cmd);break;
                           }

                         if(mode==2){printf("\nSending peer list");strcpy(cmd,buffer);read_data(cmd);break;
                              }

                          }while(TRUE );
                          if (close_conn){if((mode==1)&&(i==sockfd)){flag=1;printf("Server disconnected");}
                          close(i);
                          FD_CLR(i, &master_set);{strcpy(cl[i-4].ip,"");cl[i-4].p=0;cl[i-4].id=0;strcpy(cl[i-4].name,"");}
                           //else{strcpy(cl[i-5].ip,"");cl[i-5].p=0;}
                             if (i == max_sd){
                                          while (FD_ISSET(max_sd, &master_set) == FALSE)
                                          max_sd -= 1;

                                         }
                             if (mode==1)
                              printpeer(cl);sendlist();
                              }
                          }//END of else
                        }//If new conn
                     }//For loop
          /*end_server==TRUE;*/} while (end_server == FALSE);

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
void cl_connect(char *arg1,char *arg2){ int port=atoi(arg2); //Connect function to access other clients/peers
                                        int e;struct addrinfo hints,*loadinfo;
					memset(&hints, 0, sizeof(hints));
					hints.ai_family= AF_INET;
					hints.ai_socktype= SOCK_STREAM;
                 
    
                 if (count>3){printf("\nConnection limit exceeded");return;}

                    if ( (strcmp(sl,arg1)==0) || (strcmp(arg1,"127.0.0.1")==0) ){printf("\nSelf Connect\n");return;}

                  for (e=0;e<4;e++){
                  if ( (strcmp(apr[e].ip,arg1)==0) && (apr[e].p==atoi(arg2)) )break;//Loop through server list
                  if ( (strcmp(apr[e].name,arg1)==0) && (apr[e].p==atoi(arg2)) )break;//Loop through server list
                  }
                  
                  if (e>3){printf("\nSpecified Client Not Connected To Server\n");return;}
                  
                  
                  for (e=0;e<4;e++){
                  if( (strcmp(arg1,cl[e].ip)==0)||(strcmp(apr[e].name,arg1)==0) ){
                  printf("\nAlready connected to this peer\n");return;}
                  }
                  
                  

                   if ((info = getaddrinfo(arg1, arg2, &hints, &loadinfo)) != 0) {
                                   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(info));
                                   return;}
                                 


                      connsockfd=socket(AF_INET,SOCK_STREAM,0);//Setup socket
                  if (connect(connsockfd,(struct sockaddr*)loadinfo->ai_addr,loadinfo->ai_addrlen) == -1) {
                  close(sockfd);
                  perror("client: connect");
                  return;}
                  nonblk(&connsockfd);
                  FD_SET(connsockfd,&master_set);
                  if (connsockfd>max_sd)max_sd=connsockfd;

                  
                  struct sockaddr_in *tmp=(struct sockaddr_in *)loadinfo->ai_addr;
                  if((info=getnameinfo((struct sockaddr*)loadinfo,sizeof hints,cl[count+1].name,100 ,svc,sizeof svc,0))!=0)
                              fprintf(stderr, "getnameinfo: %s\n", gai_strerror(info));
                  if (count<2){
                  inet_ntop(AF_INET,&tmp->sin_addr, cl[count+1].ip,INET6_ADDRSTRLEN );
                  cl[count+1].p=port;cl[count+1].id=count+2;count++;}
                  printf("\nConnected");
                  }
//------------------------------------------------------REGISTER WITH SERVER----------------------------------------------------------
void registercl(char *arg1,char *arg2){server=arg1;port=atoi(arg2);//Register to server and activate listening socket
                  if(reg==1){printf("\nAlready registered\n");return;}
                  struct sockaddr_in serv;
                  memset(&serv, 0, sizeof(serv));
                  serv.sin_family = AF_INET;
                  int m=inet_pton(AF_INET,arg1,&serv.sin_addr.s_addr);
                  serv.sin_port = htons(port);
                  
                  if (m==0){printf("\nInvalid address/Bad format.Please enter a valid address");return;
                           }


                  sock_init();
                  sock_bind();
                  set_listen();
                  nonblk(&listen_sd);

                  sockfd=socket(AF_INET,SOCK_STREAM,0);

                  
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

                  strcpy(cl[0].ip,arg1);count++;cl[0].id=1;
                  
                 
                 if((info=getnameinfo((struct sockaddr*)&serv,sizeof serv,cl[0].name,100 ,svc,sizeof svc,0))!=0)
                 fprintf(stderr, "getnameinfo: %s\n", gai_strerror(info));
                 reg=1;
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
                     FD_CLR(c, &master_set);strcpy(cl[c-5].ip,"");cl[c-5].p=0;cl[c-5].id=0;
                            if (c == max_sd){
                                          while (FD_ISSET(max_sd, &master_set) == FALSE)
                                          max_sd -= 1;
                                         }
                              printpeer(cl);sendlist();
                             

 }

//----------------------------------------------------DISPLAY HELP-------------------------------------------------------------------
void help(){printf("usage:./prog1.out c/s(client|server mode) listening_port\n");
            printf("help-Displays this help\n");
            printf("myip-Displays the IP address\n");
            printf("register server_address port-Register client to server specified by IP address\n\t\t\t   & port no\n");
            printf("connect peerip port-Connect to peer with IP address and Port no\n");
            printf("download filename chnuksize-download files from peer in given chunks\n");
            printf("terminate i-Close connection i\n");
            printf("exit-Exit the Program\n");
            }
//----------------------------------------------------SCAN CONSOLE INPUT--------------------------------------------------------------
void console_read(char *cmd){//Read from console and parse the string command

               //strcpy(snd,cmd);*Here strcmp has been used ,for OS specifc prog strcasecmp can be used to ignore case*
               sscanf(cmd,"%s %s %s",cmdf,arg1,arg2);
               if ( (strcmp(cmdf,"exit")==0) ||(strcmp(cmdf,"exit")==0) )
                   {close(sockfd);exit(0);}

               if ( (strcmp(cmdf,"myport")==0) ||(strcmp(cmdf,"MYPORT")==0) )
                   {printf("%s",PORT);return;}
 
               if ( (strcmp(cmdf,"terminate")==0) ||(strcmp(cmdf,"TERMINATE")==0) )
                   {terminate(atoi(arg1));return;}


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

              if ( (strcmp(cmdf,"list")==0) ||(strcmp(cmdf,"list")==0) ){printpeer(cl);printf("\n\n");
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
               chnkptr=(char*)malloc(j*1);//Allocate buffer to store chunk status(filesize/chunksize)
               flags=fcntl(6,F_GETFL);fcntl(6,F_SETFL,flags&(~O_NONBLOCK));flags=fcntl(7,F_GETFL);
               fcntl(7,F_SETFL,flags&(~O_NONBLOCK));
               //flags=fcntl(5,F_GETFL);fcntl(5,F_SETFL,flags|(O_NONBLOCK));
               memset(chnkptr,0,strlen(chnkptr));
            
               chk1();chk2();chk3();
          
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
sprintf(table,"\tId \tName \tAddress \tPort");
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
/*-------------------------------------------------------References-----------------------------------------------------------------*/
