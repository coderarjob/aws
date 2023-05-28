
#include "header/response.h"
#include "header/panic.h"

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define SERVER "Obsidian v1b"

static struct HttpCode *GetReasonPhrase(int code);
static void SendResponceHeader(int fd, struct linkedlist *headers);
static void SendStatusLine(int fd, int HttpErrorCode);


struct HttpCode HttpCodes[] = {
    //Informational - Request received, continuing process
    {100, "Continue"},
    {101, "Switching Protocols"},
    {102, "Processing"},
    
    //Success - The action was successfully received, 
    //understood, and accepted
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    
    //Redirection - Further action must be taken in 
    //order to complete the request
    {301, "Moved Permanently"},
    {302, "Found"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},
    
    //Client Error - The request contains bad syntax 
    //or cannot be fulfilled
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {408, "Request Timeout"},         
    {414, "URI Too Long"},
    {429, "Too Many Requests"},
    
    //Server Error - The server failed to fulfill an 
    //apparently valid request
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {0,NULL}
};

void AddGeneralHeaders(struct linkedlist *headers)
{
	AddToList(headers,"Connection","Keep-Alive");
    AddToList(headers,"Server",SERVER_NAME);
    AddToList(headers,"x-ServerVer",SERVER_VER);
}

struct HttpCode *GetReasonPhrase(int code)
{
    struct HttpCode *thecode = &HttpCodes[0];
    int index = 0;
    
    while(thecode->code != 0)
    {
	if (thecode->code == code){
	    return thecode;
	}
	thecode=&HttpCodes[++index];
    }
    
    return NULL;
}


void SendResponseHeaders(int fd, int HttpErrorCode, struct linkedlist *headers)
{
    if (headers == NULL){
		MSG("ERROR: No header. Cannot send response.\n");
		PANIC("SendResponse");
    }
    
    AddGeneralHeaders(headers);
    
    SendStatusLine(fd, HttpErrorCode);
    SendResponceHeader(fd,headers);
}

static void SendResponceHeader(int fd, struct linkedlist *headers){
    struct llnode *thenode = headers->head;
    struct keyvalue *thenodevalue = NULL;
    int wlength, length;
    
    char headerline[MAX_RESPONSE_HEADERLINE_LENGTH_LENGTH];
    
    while(thenode != NULL)
    {
		thenodevalue = (struct keyvalue *)thenode->item;
		headerline[0] = '\0';
		length = sprintf(headerline,"%s:%s\r\n", thenodevalue->key, thenodevalue->value);
		
		MSG("RESPONSE: %s", headerline);
		
		if ((wlength = write(fd,headerline,length)) != length){
			MSG("ERROR: Write failed to header line. %d or %d bytes writen.\n", wlength,length);
			PANIC("SendResponceHeader");
		}
		
		thenode = thenode->next;
    }
    
    length = 2;
    if ((wlength = write(fd,"\r\n", length)) != length){
	    MSG("ERROR: Write failed to header line. %d or %d bytes writen.\n", wlength,length);
	    PANIC("SendResponceHeader");
	}	
}

static void SendStatusLine(int fd, int HttpErrorCode)
{
    char statustext[MAX_RESPONSE_STATUSLINE_LENGTH];

    struct HttpCode *status = GetReasonPhrase(HttpErrorCode);
    if (status == NULL){
	MSG("WARNING: HttpCode (%d) not found. Selecting default response error.\n", HttpErrorCode);
	status = GetReasonPhrase(DEFAULT_RESPONSE_HTTP_ERROR_CODE);
    }
    
    int length = sprintf(statustext, "%s %d %s\r\n", "HTTP/1.1", status->code, status->reason);
   
    MSG("RESPONSE: %s", statustext);
     
    int wlength;
    if ((wlength = write(fd,statustext,length)) != length){
		MSG("ERROR: Write failed to send status line. %d or %d bytes writen.\n", wlength,length);
		PANIC("SendStatusLine");
    }
}

long GetFileSize(char *file)
{
    FILE *fd;
    int sz;
    
    if ((fd = fopen(file,"r")) == NULL){
	MSG("ERROR: File (%s) is not found.\n",file);
	PANIC("GetFileSize");
    }
    
    fseek(fd, 0L, SEEK_END);
    sz = ftell(fd);
    fclose(fd);
    
    return sz;
}

unsigned int IsFilePresent(char *file) 
{
    FILE *fd;
    if ((fd = fopen(file,"r")) != NULL)
    {
	fclose(fd);
	return TRUE;
    }
    
    return FALSE;
}
