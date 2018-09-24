#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLINE 1024

int main(int argc,char **argv)
{
char *servInetAddr = "127.0.0.1";
int socketfd;
struct sockaddr_in sockaddr;
char recvline[MAXLINE], sendline[MAXLINE];
int n;
fd_set read_fds;


if(argc != 2)
{
printf("client <ipaddress> \n");
exit(0);
}

socketfd = socket(AF_INET,SOCK_STREAM,0);
// initial memory space for socketaddr struct
memset(&sockaddr,0,sizeof(sockaddr));
sockaddr.sin_family = AF_INET;
sockaddr.sin_port = htons(1088);

// Convert Internet address format from text to binary
inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);

if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )
{
printf("connect error %s errno: %d\n",strerror(errno),errno);
exit(0);
}

/*
while (1){

printf("send message to server\n");
// reads a line from the specified stream and stores it into the string pointed to by str.
fgets(sendline,1024,stdin);

if((send(socketfd,sendline,strlen(sendline),0)) < 0)
{
printf("send mes error: %s errno : %d",strerror(errno),errno);
exit(0);
}
else{
    printf("success in sending message: %s", sendline);
    recv(socketfd, recvline, MAXLINE, 0);
    puts("Reply received\n");
    puts(recvline);
}
}
*/
while(1){

    int fd_max = STDIN_FILENO;
    /* Set the bits for the file descriptors you want to wait on. */
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(socketfd, &read_fds);
    /* The select call needs to know the highest bit you set. */
    if( socketfd > fd_max ) { fd_max = socketfd; }
    /* Wait for any of the file descriptors to have data. */
    if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1)
    {
      perror("select:");
      exit(1);
    }

    /* After select, if an fd's bit is set, then there is data to read. */
    if(FD_ISSET(socketfd, &read_fds))
    {
        /* There is data waiting on your socket.  Read it with recv(). */
        recv(socketfd, recvline, MAXLINE, 0);
        puts("Reply received:\n");
        puts(recvline);
    }

    if(FD_ISSET(STDIN_FILENO, &read_fds))
    {
        /* The user typed something.  Read it fgets or something.
           Then send the data to the server. */
           fgets(sendline,1024,stdin);
           if((send(socketfd,sendline,strlen(sendline),0)) < 0)
           {
           printf("send mes error: %s errno : %d",strerror(errno),errno);
           exit(0);
         }
       }
     }
   }
