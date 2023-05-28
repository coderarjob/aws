#include "../header/webrequest.h"

#define WTNORM 0
#define WTTOKEN 1

void getQueryString(char *page, struct linkedlist *ll)
{
    int wordtype = WTNORM ; //0  = word, 1 = tokens
    char *thiscontent;
    char key[MAX_HEADDER_KEY_LENGTH], value[MAX_HEADDER_VALUE_LENGTH];
    char c;
    
    thiscontent = key;
    
    //get to the beginning of the querystring
    while((c = *page++) != '?' && c != '\0')
	;
    
    if (c == '\0') //no querystring is there.
	return;
    
    while((c = *page++) != '\0')
    {
	switch(c)
	{
	    case '=':
		wordtype = WTTOKEN;
		*thiscontent = '\0';
		//printf("keys[%d]%s\n",index, keys[index]);
		thiscontent = value;
		break;
	    case '&':
		wordtype = WTTOKEN;
		*thiscontent = '\0';
		AddToList(ll,key,value);
		//printf("values[%d]%s\n",index, values[index]);
		thiscontent = key;
		break;
	    default:
		wordtype = WTNORM;
		break;
	}
	
	//if (wordtype == 1)
	//    printf("--%s\n", thiscontent);

	if (wordtype == WTNORM )
	    *thiscontent++ = c;	    
    }
    
    *thiscontent = '\0';
    AddToList(ll,key,value);
}