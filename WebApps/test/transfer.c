#include"header/transfer.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static int readString(int socket, char *out)
{
	int count;
	for (count =0;read(socket,out,1);out++, count++)
		if (*out == '\0')
			break;
			
	*++out = '\0';
	return count;
}

void sendKeyvaluesToApplication(int writeToSocket, struct linkedlist *ll)
{
	struct llnode *thisnode;
	struct keyvalue *thiskeyvalue;
	
	for(thisnode = ll->head;thisnode != NULL;thisnode = thisnode->next)
	{
		thiskeyvalue = (struct keyvalue *)thisnode->item;
		write(writeToSocket,thiskeyvalue->key,strlen(thiskeyvalue->key)+1);
		write(writeToSocket,thiskeyvalue->value,strlen(thiskeyvalue->value)+1);
	}
	
	char a = '\0';
	write(writeToSocket, &a ,sizeof(char));
}

void recvKeyvaluesToApplication(int readFromSocket, struct linkedlist *ll)
{
	char key[MAX_HEADDER_KEY_LENGTH];
	char value[MAX_HEADDER_VALUE_LENGTH];
	
	while(readString(readFromSocket,key) > 0)
	{	
		//key was read, now read value
		readString(readFromSocket,value);
		AddToList(ll,key,value);
	}
	
}

void WriteRequest(int socketToWrite,struct linkedlist *request_header, 
		  struct linkedlist *queryString, struct linkedlist *PostStrings, 
		  struct linkedlist *response_header)
{
	sendKeyvaluesToApplication(socketToWrite, request_header);
	sendKeyvaluesToApplication(socketToWrite, queryString);
	sendKeyvaluesToApplication(socketToWrite, PostStrings);
	sendKeyvaluesToApplication(socketToWrite, response_header);
}

void ReadRequest(int socketToRead, struct linkedlist *request_header, 
		  struct linkedlist *queryString, struct linkedlist *PostStrings, 
		  struct linkedlist *response_header)
{	
	recvKeyvaluesToApplication(socketToRead, request_header);
	recvKeyvaluesToApplication(socketToRead, queryString);
	recvKeyvaluesToApplication(socketToRead, PostStrings);
	recvKeyvaluesToApplication(socketToRead, response_header);
}

void WriteResponse(int writeToSocket, struct linkedlist *headers, int16_t statuscode)
{
	sendKeyvaluesToApplication(writeToSocket, headers);
	write(writeToSocket,&statuscode,sizeof(int16_t));
}

void ReadResponse(int readFromSocket, struct linkedlist *headers,int16_t *statuscode)
{
	recvKeyvaluesToApplication(readFromSocket, headers);
	read(readFromSocket,statuscode,sizeof(int16_t));
}

void StartTransmitBlock(int fd, uint32_t totalsize)
{
  write(fd,(void *)&totalsize,sizeof(uint32_t));
}

void TransmitBlock(int fd, void *obj,size_t size)
{
    write(fd,obj,size);
}

uint32_t StartReceiveBlock(int fd)
{
   uint32_t Size;
   read(fd,&Size,sizeof(uint32_t));
   return Size;
}

void ReceiveBlock(int fd, void *dest, uint32_t totalsize)
{
    uint32_t remainingSize = totalsize;

    while(remainingSize > 100)
    {
      read(fd,dest,100);
      dest+=100;
      remainingSize-=100;
    }
    if (remainingSize > 0)
      read(fd,dest,remainingSize);
}
