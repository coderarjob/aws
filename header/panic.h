#ifndef PANIC_H
#define PANIC_H

	#include "errors.h"
	#include <unistd.h>

    #define  PANIC(call) do{ perror(call); _exit(ERR_UNKNOWN);}while(0)
    #define  MSG printf
//MSG(msg) do{ printf("Message: %s\n",msg);}while(0)
#endif