#ifndef RESPONSE_H
#define RESPONSE_H

#include "errors.h"
#include "linkedlist.h"
#include "keyvalue.h"
#include "httpcodes.h"

#define MAX_RESPONSE_STATUSLINE_LENGTH 100
#define MAX_RESPONSE_HEADERLINE_LENGTH_LENGTH 400
#define MAX_RESPONSE_BINARY_FILE_SIZE 52428800 //50 MB Upload limit

#define SERVER_NAME "aws"
#define SERVER_VER  "0.2a"

#define DEFAULT_RESPONSE_HTTP_ERROR_CODE 500 //INTERNAL SERVER ERROR

struct HttpCode
{
    unsigned int code;
    char *reason;
};

extern struct HttpCode HttpCodes[];

long GetFileSize(char *file);
unsigned int IsFilePresent(char *file);
void SendResponseHeaders(int fd, int HttpErrorCode, struct linkedlist *headers);

#endif //RESPONSE_H
