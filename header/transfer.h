#ifndef TRANSFER_H
#define TRANSFER_H

#include "linkedlist.h"
#include "keyvalue.h"

#include <stdint.h>

void sendKeyvaluesToApplication(int writeToSocket, struct linkedlist *ll);
void recvKeyvaluesToApplication(int readFromSocket, struct linkedlist *ll);

void WriteRequest(int socketToWrite, struct linkedlist *request_header, 
		  struct linkedlist *queryString, struct linkedlist *PostStrings, 
		  struct linkedlist *response_header);

void ReadRequest(int socketToRead, struct linkedlist *request_header, 
		  struct linkedlist *queryString, struct linkedlist *PostStrings, 
		  struct linkedlist *response_header);

void WriteResponse(int writeToSocket, struct linkedlist *response_header, int16_t statuscode);

void ReadResponse(int readFromSocket,struct linkedlist *response_header, int16_t *statuscode);

void ReceiveBlock(int fd, void *dest, uint32_t totalsize);
uint32_t StartReceiveBlock(int fd);
void TransmitBlock(int fd, void *obj,size_t size);
void StartTransmitBlock(int fd, uint32_t totalsize);

#endif