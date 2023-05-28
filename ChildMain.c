#include <stdio.h>
#include<sys/wait.h>

#include "header/webrequest.h"
#include "header/linkedlist.h"
#include "header/keyvalue.h"
#include "header/panic.h"
#include "header/appfile.h"
#include "header/debug.h"
#include "header/response.h"
#include "header/mime.h"
#include "header/transfer.h"

#define APPFILE "./default.app"

#define STD_READ_ID 0
#define STD_WRITE_ID 1

static int getRequest(int clientfd, char **request, struct linkedlist *request_headers, struct linkedlist* query_strings);
static struct app *GetApplicationFromName(char *app);
static int ValidateRequest(struct linkedlist *request);
static void GetFileName(const char *page, char *filename, int length);
static int StartWebApp(struct app *webapp, int fd[2], struct linkedlist *request_header, 
		 struct linkedlist *qstring, struct linkedlist *postStrings,
		 struct linkedlist *response_header);

void childMain(int clientfd)
{
    char *request;
    char fileSizeinText[20];
	
    struct linkedlist *request_headers, *query_strings, *response_header, *post_strings;
    
    request_headers = CreateKeyValueList();
    query_strings = CreateKeyValueList();
    post_strings = CreateKeyValueList();
    response_header = CreateKeyValueList();
	
	int errorCode;
    if ((errorCode = getRequest(clientfd, &request, request_headers, query_strings)) < 0)
    {		
		MSG("Error: Failed to get HTTP Request. err: %d\n", errorCode);
		_exit(errorCode);
	}
	
	MSG("INFO: Got HTTP request.\n");
	
	//display(request_headers);  
	//display(query_strings);

	unsigned int ret;
	if ((ret = ValidateRequest(request_headers)) != ERR_NONE){
		switch(ret)
		{
			case ERR_INVALID_REQUEST:
				SendResponseHeaders(clientfd,HTTP_BAD_REQUEST,response_header);
			break;
			case ERR_UNSUPPORTED_METHOD:
				SendResponseHeaders(clientfd,HTTP_METHOD_NOT_ALLOWED,response_header);
			break;
		}
		
		PANIC("childMain");
	}

	struct keyvalue *theNode;
	theNode = (struct keyvalue *) search_ll(request_headers,"File");

	if (IsFilePresent(theNode->value))
	{
		MSG("INFO: %s file was found.\n",theNode->value);
		
		char fileSizeinText[20];
		int filesize = GetFileSize(theNode->value);
		
		if (filesize > MAX_RESPONSE_BINARY_FILE_SIZE)
		{
			SendResponseHeaders(clientfd,HTTP_NOT_FOUND,response_header);
			
			MSG("File of size %d bytes exceeds the max length of %d bytes.\n",filesize,MAX_RESPONSE_BINARY_FILE_SIZE);
			PANIC("childMain");
		}
		
		sprintf(fileSizeinText, "%d", filesize);
		AddToList(response_header,"Content-Length", fileSizeinText);
		
		char *mime = getmime(getextension(theNode->value));
		AddToList(response_header,"Content-Type", mime);
		
		SendResponseHeaders(clientfd,HTTP_OK,response_header);
		
		MSG("INFO: Sending body. Of %d bytes.\n",filesize);
		
		char *buff[100];
		int filefd = open(theNode->value,O_RDONLY);
		int len;
		for(;filesize > 100;filesize-=100)
		{
			len = read(filefd,buff,100);
			write(clientfd,buff,len);
		}
		read(filefd,buff,filesize);
		write(clientfd,buff,filesize);
	}
	else
	{
		MSG("INFO: %s file was NOT found.\n",theNode->value);
		
		struct  app *thisapp = GetApplicationFromName(theNode->value);
		if (thisapp == NULL)
		{
			SendResponseHeaders(clientfd,HTTP_NOT_FOUND,response_header);
			PANIC("childMain");
		}
		else
		{
			int childpid, returnStatus,fd[2];
		
			MSG("INFO: An Application with the '%s' name was found.\n",theNode->value);
			MSG("INFO: App:- %s, Path:- %s\n", thisapp->name, thisapp->path);
			
			MSG("INFO: WebApp Started..\n");
			childpid = StartWebApp(thisapp,fd,request_headers, query_strings, post_strings, response_header);
			waitpid(childpid, &returnStatus,0);
			
			MSG("INFO: WebApp exited!\n");
			
			if (returnStatus != 0)
			{
				MSG("INFO: WebApp exited with %d return code.\n",returnStatus);
				SendResponseHeaders(clientfd,HTTP_INTERNAL_SERVER_ERROR,response_header);
				PANIC("childMain");
			}
			else
			{
				void *response_body;
				int body_length;
				int16_t responseStatusCode;
				
				free_ll(response_header);
				response_header = CreateKeyValueList();
				ReadResponse(fd[0],response_header,&responseStatusCode);
				
				if (responseStatusCode == HTTP_OK){
					body_length = StartReceiveBlock(fd[0]);
					response_body = malloc(sizeof(void)*body_length);
					ReceiveBlock(fd[0],response_body,body_length);
					sprintf(fileSizeinText, "%d", body_length);
					AddToList(response_header,"Content-Length", fileSizeinText);
				}
				
				SendResponseHeaders(clientfd,responseStatusCode,response_header);
				
				if (responseStatusCode == HTTP_OK){
					MSG("INFO: Sending body. Of %d bytes.\n",body_length);
				
					for(;body_length > 100;body_length-=100,response_body+=100)
						write(clientfd,response_body,100);
					
					write(clientfd,response_body,body_length);
				}
				
			}
		}
	}


	MSG("INFO: RESPONSE COMPLETE.\n");
}

static void SplitFilePath(char *filename, char *path,char *filetitle)
{
	char *filename_ptr = filename;
	int length;
	
	for(length=0;*filename_ptr != '\0';filename_ptr++,length++)
		;
	for(;*filename_ptr != '/';length--,filename_ptr--)
		;
	
	if (filetitle != NULL)
		strcpy(filetitle,filename_ptr+1);
	
	while(length-->0)
		*path++ = *filename++;
	*path='\0';
}

static int StartWebApp(struct app *webapp, int fd[2], struct linkedlist *request_header, 
		 struct linkedlist *qstring, struct linkedlist *postStrings,
		 struct linkedlist *response_header)
{
  int childpid;
  pipe(fd);
  
  if ((childpid = fork()))
    return childpid;
  else
  {
	char filepath[PATH_MAXLENGTH];
	SplitFilePath(webapp->path,filepath,NULL);
	
	MSG("INFO: Changing CDIR to %s\n", filepath);
	chdir(filepath);
	
    dup2(fd[0],STD_READ_ID);
    dup2(fd[1],STD_WRITE_ID);
	
    WriteRequest(fd[1],request_header,qstring,postStrings,response_header);
    execl(webapp->path,webapp->name,NULL);
	return -1;
  }

}

static int ValidateRequest(struct linkedlist *request_headers)
{
    struct keyvalue *theNode;
    theNode = (struct keyvalue *) search_ll(request_headers,"Method");
    if (theNode == NULL)
    {
		MSG("ERROR: Invalid HTTP Request. No Method was found.\n");
		return ERR_INVALID_REQUEST;
    }
    
    if (strcasecmp(theNode->value,"POST") == 0)
    {
		MSG("ERROR: POST method is not supported.\n");
		return ERR_UNSUPPORTED_METHOD;
    }
    
    theNode = (struct keyvalue *)search_ll(request_headers,"File");
    if (theNode == NULL)
    {
		MSG("ERROR: File item not found.\n");
		return ERR_INVALID_REQUEST;
    }
    
    if (strcmp(theNode->value,"") == 0)
    {
		MSG("ERROR: File item is empty.\n");
		return ERR_INVALID_REQUEST;
    }
    
    return ERR_NONE;
}

static struct app *GetApplicationFromName(char *app)
{   
    struct app *theapp;
    struct linkedlist *applist = ReadAppFile(APPFILE);
    theapp = (struct app *)search_ll(applist,app);
    if (theapp == NULL)
	return NULL;
    else
	return theapp;   
    
}


static void GetFileName(const char *page, char *filename, int length)
{
    char c;
    
    while(--length > 0 && (c = *page++) != '/')
    ;
    
    while(--length > 0 && (c = *page++) != '\0' && c != '?')
	*filename++ = c;
    *filename = '\0';
}

static int getRequest(int clientfd, char **request, struct linkedlist *request_headers, struct linkedlist* query_strings)
{
    
    int errorCode;
    *request = getHTTPrequest(clientfd,&errorCode);
    if (errorCode < 0)
		return errorCode;
    
    char method[MAX_HEADDER_VALUE_LENGTH];
    char page[MAX_HEADDER_VALUE_LENGTH];
    char version[MAX_HEADDER_VALUE_LENGTH];
    char filename[MAX_HEADDER_VALUE_LENGTH];
    
    
    
    getStatusLine(request,method,page,version);
    GetFileName(page,filename,MAX_HEADDER_VALUE_LENGTH);
    
    AddToList(request_headers,"Method",method);
    AddToList(request_headers,"Page",page);
    AddToList(request_headers,"File",filename),
    AddToList(request_headers,"Version",version);
    
    getHeaders(*request,request_headers);
    getQueryString(page,query_strings);
    
    return ERR_NONE;
}