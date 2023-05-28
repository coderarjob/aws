#include "../header/trim.h"

void trim(char *text, char ch, char *dest)
{
    char *startofString;
    char *endofString;
    char c;
    
    //trim leading 
    for(;*text == ch;text++)
	;
    
    startofString = text;
    endofString = text;
    
    //trim trailling 
    for (;(c = *text) != '\0';text++){
	if (c != ch)
	    endofString= text;
    }
    
    endofString++;
    *endofString='\0';
    
    //copy into destination array
    while((*dest++ = *startofString++) != '\0')
	;
}

