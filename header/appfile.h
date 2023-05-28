#ifndef APPFILE_HH
#define APPFILE_HH

#include "linkedlist.h"
//#include <stdio.h>
#include <fcntl.h>
#include "panic.h"
#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//#include <errno.h>

#define NAME_MAXLENGTH 50
#define PATH_MAXLENGTH 100

struct app
{
    char name[NAME_MAXLENGTH];
    char path[PATH_MAXLENGTH];
};

struct linkedlist *ReadAppFile(char *filename);

#endif //APPFILE_HH