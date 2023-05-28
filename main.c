#include <stdio.h> 	//for printf
#include <unistd.h> 	//for close, read, write, close
#include <fcntl.h> 	//for open
#include <netdb.h> 	//for getaddrinfo
#include <sys/socket.h>
#include <arpa/inet.h> 	//for inet_pton, etc
#include <netinet/in.h> //for the struct sockaddr_in, struct sockaddr_in6, hton etc
#include <errno.h>
#include <string.h>	//for memset
#include <stdlib.h> 	//malloc, atof, atoi, exit
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "header/timequantum.h"
#include "header/webrequest.h"
#include "header/linkedlist.h"
#include "header/keyvalue.h"
#include "header/panic.h"

#define LISTEN_PORT 4400
#define TIMEQUANTUM_SEC 10

#define WELCOME_MSG() do{printf("Server is running.\nCurrently listening on %d port.\n",LISTEN_PORT);}while(0)

void childMain(int clientfd);

void childDead(int signal)
{
    int ret;
	if (signal == SIGCHLD){
		wait(&ret);
		ret &=0x377;
		printf("\t--Child died.(%d)--\n",ret);
    }
}

int main()
{  
    struct sockaddr_in sock,client;
    int serverfd, clientfd;
    char clientip[INET6_ADDRSTRLEN];
    size_t socket_len;
  
	WELCOME_MSG();
	
    //set stdio to unbuffered
    setvbuf(stdout, NULL, _IONBF, 0);
    //--
    
    if ((serverfd=socket(PF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto)) == -1)
		PANIC("socket");
 
    memset(&sock, 0, sizeof(struct sockaddr_in));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(LISTEN_PORT);
    sock.sin_addr.s_addr = htons(INADDR_ANY); 
    //INADDR_ANY = 0, so htons does nothing. 
    //It is there because we must use htons for any other constants
	
    if (bind(serverfd, (struct sockaddr *)&sock, sizeof(struct sockaddr_in)) == -1)
	PANIC("bind");
    
    signal(SIGCHLD,childDead);
  
    while(1)
    {
	    if (listen(serverfd, 10) == -1)
		PANIC("listen");
	
	    socket_len = sizeof(struct sockaddr_in);
	    if ((clientfd = accept(serverfd, (struct sockaddr *) &client, &socket_len)) == -1)
			PANIC("accept");
	    
	    if(fork())
			close(clientfd);
	    else
	    {
			close(serverfd);
			MSG("\n\t---Connected to %s---\n", inet_ntop(AF_INET, &client.sin_addr, clientip, sizeof(clientip)));
			MSG("\tStarting Time quantum: %d sec\n", TIMEQUANTUM_SEC);
			StartTimeQuantum(TIMEQUANTUM_SEC);
			
			while(!IsTimeQuantumExpired()){
				childMain(clientfd);
				MSG("\tTime quantum has not expired. Waiting for new requests\n");
			}
			
			MSG("\tTime quantum has expired.\n");
			exit(0);
	    }
    }

    close(serverfd);

    return 0;
}
