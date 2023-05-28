#ifndef MIME_H
#define MIME_H

#include <stddef.h>

#define MAX_EXT_LENGTH 260

struct filetype
{
    char *ext;
    char *mime;
};

extern struct filetype mimes[];

char *getmime(char *ext);
char *getextension(char *filename);

#endif //MIME_H