#include "../header/webrequest.h"

#define WTWORD 0
#define WTWS 1
#define WTTERM 2

/*
 * Based on HTTP 1.1 spec.
 * ref: Request-Line   = Method SP Request-URI SP HTTP-Version CRLF (5.1 HTTP/1.1 spec)
 * ref:  No CR or LF is allowed except in the final CRLF sequence.  (5.1 HTTP/1.1 spec)
 */
void getStatusLine(char **request, char *method, char *page, 
			  char *version)
{
    char c, prev_c;    
    int currenttoken = WTWORD;
    
    char *currentwords[] = {method,page,version}; //an array of 3 pointer to chars
    char **currentword = currentwords;
    
    trim(*request, ' ',*request); //trim out extra spaces.
    
    for( ; currenttoken != WTTERM && (c = **request) != '\0' ; prev_c = c, (*request)++)
    {
	switch(c)
	{
	    case '\r':
		currenttoken = WTWS;	//whitespace
		break;
	    case '\n':
		if (prev_c == '\r'){
		    currenttoken = WTTERM; 	//termination detected
		    **currentword = '\0';
		}
		break;
	    case ' ':
		currenttoken = WTWS;	 //whitespace
		break;
	    default:
		if (prev_c == ' ')
		{
		    **currentword = '\0';
		    currentword++;
		}
		
		currenttoken = WTWORD; 	//normal word
		break;
	}
	
	//printf("%d, %c\n", currenttoken, c=='\r'?'*':(c=='\n'?'+':c));
	
	if (currenttoken == WTWORD){
	    //putchar(c);
	    **currentword = c;    
	    (*currentword)++;
	}
	    
    } //for lloop
}
