#include <string.h>
#include <malloc.h>
#include "../header/mime.h"

struct filetype mimes[] = 	{
				    {"bm", "image/bmp"},
				    {"bmp", "image/bmp"},
				    {"c", "text/plain"},
				    {"txt", "text/plain"},
				    {"h", "text/plain"},
				    {"php", "text/plain"},
				    {"jpeg", "image/jpeg"},
				    {"jpg", "image/jpeg"},
				    {"png", "image/png"},
				    {"ico", "image/x-icon"},
				    {"html", "text/html"},
				    {"htm", "text/html"},
				    {NULL, "application/octet-stream"}
				};
				
char *getmime(char *ext)
{
    struct filetype *current = mimes;
    do{
	if (strcasecmp(current->ext,ext) ==0)
	    break;
	current++;
    }while(current->ext !=  NULL);
	
    return current->mime;
}

char *getextension(char *filename)
{
    char *extension = malloc(sizeof(char) * MAX_EXT_LENGTH);
    char *ext_ptr = extension;
    
    char c;
    while((c = *filename++) != '\0' && c != '.');
    
    if (c != '\0'){
	int ext_length_rem = MAX_EXT_LENGTH;
	for(;ext_length_rem > 0 && (*ext_ptr++ = *filename++) != '\0';ext_length_rem--);
    }
    
    *ext_ptr='\0';
    return extension;
}
