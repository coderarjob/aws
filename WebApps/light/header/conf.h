#ifndef APPFILE_HH
#define APPFILE_HH

#include "linkedlist.h"
#include <fcntl.h>
#include "panic.h"
#include <unistd.h>

#define NAME_MAXLENGTH 50
#define PATH_MAXLENGTH 100

struct conf
{
    char setting[NAME_MAXLENGTH];
    char value[PATH_MAXLENGTH];
};

struct linkedlist *ReadConfFile(char *filename);

#endif //APPFILE_HH