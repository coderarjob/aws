/* Build with -D DUMMY_SERIAL to simulate a serial device */

#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>

#include "header/keyvalue.h"
#include "header/transfer.h"
#include "header/mime.h"
#include "header/httpcodes.h"

void Display(struct linkedlist *ll);
int WriteBody(int writefd, char *filename);
void set_blocking (int fd, int should_block);
int set_interface_attribs (int fd, int speed, int parity);
int InitializeSerialPort();
unsigned int GetLedStatus(int fd);
void TurnLedON(int fd);
void TurnLedOff(int fd);

#define LIGHT_ON_FILE "./html/light_on.html"
#define LIGHT_OFF_FILE "./html/light_off.html"

#define STD_READ_FD 0 
#define STD_WRITE_FD 1

#define PORT "/dev/ttyUSB0"
#define SPEED 9600

#ifdef DUMMY_SERIAL
unsigned int g_dummyCurrentLedStatus;
#endif

int main()
{
	struct linkedlist *headers = CreateKeyValueList();
	struct linkedlist *qString = CreateKeyValueList();
	struct linkedlist *response = CreateKeyValueList();
	struct linkedlist *postString = CreateKeyValueList();
	char *filename;
	unsigned int CurrentLedStatus;

	int fd = InitializeSerialPort();
	if (fd < 0)
		return -1;
	
	ReadRequest(STD_READ_FD,headers,qString,postString,response);
	
	struct keyvalue *UsrInputNode;
    UsrInputNode = (struct keyvalue *) search_ll(qString,"light");
    if (UsrInputNode == NULL){
		CurrentLedStatus = GetLedStatus(fd);
		if (CurrentLedStatus == 1)
			filename = LIGHT_ON_FILE;
		else
			filename = LIGHT_OFF_FILE;
		fprintf(stderr,"WEBAPP: NO QSTRING FOUND.\n");
	}
	else{
		if (strcmp(UsrInputNode->value,"on")==0){
			filename = LIGHT_ON_FILE;
			TurnLedON(fd);
			fprintf(stderr,"WEBAPP: Asked to turn on the light.\n");
		}
		else{
			filename = LIGHT_OFF_FILE;
			TurnLedOff(fd);
			fprintf(stderr,"WEBAPP: Asked to turn off the light.\n");
		}
	}
	
	AddToList(response,"X-IsModified","TRUE");
	AddToList(response,"Content-Type", getmime("htm"));
	WriteResponse(STD_WRITE_FD,response,HTTP_OK);
	return WriteBody(STD_WRITE_FD,filename);
}

int InitializeSerialPort()
{
#ifndef DUMMY_SERIAL
	int fd = open (PORT, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
			perror("InitializeSerialPort");
			fprintf(stderr, "Failed to open port.\n");
			return -1;
	}

	set_interface_attribs (fd, SPEED, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking
	return fd;
#else
    return 0;
#endif //DUMMY_SERIAL
}

void TurnLedOff(int fd)
{
#ifndef DUMMY_SERIAL
	write(fd,"F",1);
#else
    g_dummyCurrentLedStatus = 0;
#endif
}

void TurnLedON(int fd)
{
#ifndef DUMMY_SERIAL
	write(fd,"O",1);
#else
    g_dummyCurrentLedStatus = 1;
#endif
}

unsigned int GetLedStatus(int fd)
{
#ifndef DUMMY_SERIAL
	char c;
	write(fd,"S",1);
	read(fd,&c,1);
	return c;
#else
    return g_dummyCurrentLedStatus;
#endif
}

long GetFileSize(char *file)
{
    FILE *fd;
    int sz;
    
    if ((fd = fopen(file,"r")) == NULL){
		perror("GetFileSize");
		return -1;
    }
    
    fseek(fd, 0L, SEEK_END);
    sz = ftell(fd);
    fclose(fd);
    
    return sz;
}

int WriteBody(int writefd, char *filename)
{  
	fprintf(stderr,"WEBAPP: File = %s\n", filename);

	int32_t filesize= GetFileSize(filename);
	if (writefd < 0)
		return -1;
	
	fprintf(stderr,"WEBAPP: FileSize = %d.\n",filesize);
	
	StartTransmitBlock(writefd,filesize);
	
	char buff[100];
	int filefd = open(filename,O_RDONLY);
	int len;
	for(;filesize > 100;filesize-=len)
	{
		len = read(filefd,buff,100);
		TransmitBlock(writefd,buff,len);
	}
	read(filefd,buff,filesize);
	TransmitBlock(writefd,buff,filesize);
	
	fprintf(stderr,"WEBAPP: WriteBody exited sucessfully\n");
	return 0;
  
}
void Display(struct linkedlist *ll)
{
  struct llnode *currentnode = ll->head;
  struct keyvalue *currentpair;
  
  while(currentnode != NULL){
    currentpair = (struct keyvalue*)currentnode->item;
    fprintf(stderr,"%s: %s.\n",currentpair->key, currentpair->value);
    currentnode = currentnode->next;
  }
}
