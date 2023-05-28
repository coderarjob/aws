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

#define PAGE_FILE "./html/page.html"

#define STD_READ_FD 0 
#define STD_WRITE_FD 1

int main()
{
	struct linkedlist *headers = CreateKeyValueList();
	struct linkedlist *qString = CreateKeyValueList();
	struct linkedlist *response = CreateKeyValueList();
	struct linkedlist *postString = CreateKeyValueList();
	char *filename = PAGE_FILE;

	ReadRequest(STD_READ_FD,headers,qString,postString,response);
	
	AddToList(response,"Content-Type", getmime("htm"));
	WriteResponse(STD_WRITE_FD,response,HTTP_OK);
	return WriteBody(STD_WRITE_FD,filename);
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