#ifndef HTTPCODES_H
#define HTTPCODES_H

//Informational - Request received, continuing process
#define HTTP_CONTINUE 100
#define HTTP_SWITCHING_PROTOCOLS 101
#define HTTP_PROCESSING 102

//Success - The action was successfully received, 
//understood, and accepted
#define HTTP_OK 200
#define HTTP_CREATED 201
#define HTTP_ACCEPTED 202
#define HTTP NONAUTHINFO 203 //Non-Authoritative Information
#define HTTP_NO_CONTENT 204 //No Content

//Redirection - Further action must be taken in 
//order to complete the request
#define HTTP_MOVED_PERMANENTLY 301
#define HTTP_FOUND 302
#define HTTP_TEMPORARY_REDIRECT 307
#define HTTP_PERMANENT_REDIRECT 308

//Client Error - The request contains bad syntax 
//or cannot be fulfilled
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_METHOD_NOT_ALLOWED 405
#define HTTP_REQUEST_TIMEOUT 408         
#define HTTP_URI_TOO_LONG 414
#define HTTP_TOO_MANY_REQUESTS 429

//Server Error - The server failed to fulfill an 
//apparently valid request
#define HTTP_INTERNAL_SERVER_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501

#endif