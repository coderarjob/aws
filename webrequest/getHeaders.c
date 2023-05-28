#include "../header/webrequest.h"

#define WTKEY 0
#define WTVALUE 1
#define WTCRLF1 2
#define WTCRLF2 3
#define WTTERM 4
#define WTSPERTOR 5

void getHeaders(char *request, struct linkedlist *ll)
{
    char key[MAX_HEADDER_KEY_LENGTH], value[MAX_HEADDER_VALUE_LENGTH];
    char *currentword = key;
    
    int wordtype = WTKEY; //0 = key, 1 = values, 2 = 1st cr, 3 = 2nd cr, 4 = terminate; 5 = whitespace
    char c;
    
    for(; wordtype != 4 && (c = *request) != '\0';  request++)
    {
	switch(c)
	{
	    case '\r':
		if(wordtype==WTCRLF1)
		    wordtype=WTCRLF2;
		else
		    wordtype=WTCRLF1;
		break;
	    case '\n':
		switch(wordtype)
		{
		    case WTCRLF2: 
			wordtype = WTTERM; //terminate;
			break;
		    case WTCRLF1:
			//a key value pair was found
			*currentword='\0';
			currentword=key;
			trim(key,' ',key);
			trim(value,' ',value);
			AddToList(ll,key,value);
			break;
		}
		   
		break;
	    case ':':
		if (wordtype == WTKEY)
		{
		    *currentword = '\0';
		    currentword = value;
		    wordtype = WTSPERTOR;
		}
		break;
	    default:
		if (wordtype == WTSPERTOR)
		    wordtype=WTVALUE;
		if(wordtype == WTCRLF1)
		    wordtype=WTKEY;
		
	}
	
	//printf("%d, %c\n", wordtype, c=='\r'?'*':(c=='\n'?'+':c));
	
	if (wordtype == WTKEY || wordtype== WTVALUE)
	    *currentword++ = c;
    }
}