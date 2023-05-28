#include "../header/keyvalue.h"

static int ismatch(void *in, void *key)
{
    struct keyvalue *_in = (struct keyvalue *)in;
    return strcmp(_in->key, (char *)key) == 0;
}

static void freenode(void *item)
{
    free((struct keyvalue *)item);
}

static char *tostring(void *item)
{
    struct keyvalue *_item = (struct keyvalue *)item;
    return _item->key;
}

struct linkedlist *CreateKeyValueList()
{
    return create_ll(ismatch,freenode,tostring);
}

void AddToList(struct linkedlist *ll, char *key, char *value)
{
    struct keyvalue newnode;
    strcpy(newnode.value,value);
    strcpy(newnode.key,key);
    
    insert_to_ll(ll,&newnode,sizeof(newnode));
}
