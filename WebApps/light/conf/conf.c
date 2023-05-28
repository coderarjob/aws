
#include "../header/conf.h"

struct conf *GetNextConf(int fd,int *error);

int ismatch(void *in, void *key)
{
    struct conf *thisitem = (struct conf *)in;
    return strcmp((char *)key, thisitem->setting)==0;
}

void freeNode(void *item)
{
    free((struct conf *)item);
}

char *tostring(void *item)
{
    struct conf *thisitem = (struct conf *)item;
    return thisitem->setting;
}

struct linkedlist *ReadConfFile(char *filename)
{
    int fd;
    struct linkedlist *newll = create_ll(ismatch, freeNode, tostring);
    
    if ((fd = open(filename,O_RDONLY)) == -1){
		PANIC("ReadConfFile");
		return NULL;
    }
    
    struct conf *nwConf;    
    while((nwConf = GetNextConf(fd,NULL)) != NULL)
		insert_to_ll(newll, nwConf, sizeof(struct conf));
    
    return newll;
}