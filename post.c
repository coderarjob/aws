#include <stdio.h>
#include "header/webrequest.h"
#include "header/linkedlist.h"
#include "header/keyvalue.h"
#include "header/panic.h"
#include "header/appfile.h"
#include "header/debug.h"

static int readPost(int fd, char *buffer, int length)
{
    int t_len = length;
    int read_len;
    while(length > 0 && (read_len = readFromSocket(fd,buffer,length,SOCK_TIMEOUT_SEC)) > 0)
    {
	length -= read_len;
	buffer+=read_len;
    }
    *buffer='\0';
    
    if (read_len > 0)
	return read_len;
    else
	return t_len - length;
}


void HandlePostRequest()
{
    
    /*struct llnode *the_gnode = search_ll(request_headers,"Content-Length");
    if (the_gnode == NULL)
    {
	MSG("Content-Length not found. Invalid POST request");
	exit(-1);
    }
    struct keyvalue *thenode = (struct keyvalue *)the_gnode->item;
    printf("POST conisits of (%d) bytes\n",atoi(thenode->value));
    
    int content_length = atoi(thenode->value);
    char *post = malloc(sizeof(char) * (content_length+1));
    readPost(clientfd,post,content_length);
    printf("%s\n",post);*/
    
}
