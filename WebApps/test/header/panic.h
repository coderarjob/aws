#ifndef PANIC_H
#define PANIC_H

    #define  PANIC(call) do{ perror(call); exit(-1);}while(0)
    #define  MSG printf
//MSG(msg) do{ printf("Message: %s\n",msg);}while(0)
#endif