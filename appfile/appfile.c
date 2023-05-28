
#include "../header/appfile.h"

struct app *GetNextApp(int fd,int *error);

int ismatch(void *in, void *key)
{
    struct app *thisitem = (struct app *)in;
    return strcmp((char *)key, thisitem->name)==0;
}

void freeNode(void *item)
{
    free((struct app *)item);
}

char *tostring(void *item)
{
    struct app *thisitem = (struct app *)item;
    return thisitem->name;
}

struct linkedlist *ReadAppFile(char *filename)
{
    int fd;
    struct linkedlist *newll = create_ll(ismatch, freeNode, tostring);
    
    
    if ((fd = open(filename,O_RDONLY)) == -1){
	PANIC("ReadAppFile");
	return NULL;
    }
    
    struct app *nwApp;    
    while((nwApp = GetNextApp(fd,NULL)) != NULL)
	insert_to_ll(newll, nwApp, sizeof(struct app));
    
    return newll;
}