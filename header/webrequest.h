#include "linkedlist.h"
#include "stack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "trim.h"
#include "linkedlist.h"
#include "keyvalue.h"
#include "panic.h"
#include "errors.h"

#define SOCK_TIMEOUT_SEC 10

#define MAX_REQUEST_LENGTH 2000  //bytes
#define MAX_QSTRING_KEY 100	//QueryString key length
#define MAX_QSTRING_VALUE 400	//QueryString value length

char *getHTTPrequest(int sd, int *errorcode);
void getStatusLine(char **request, char *method, char *page, char *version);
void getHeaders(char *request, struct linkedlist *ll);
int readFromSocket(int sd, char *buffer, int b_length, 
			  int timeout_seconds);
void getQueryString(char *page, struct linkedlist *ll);

