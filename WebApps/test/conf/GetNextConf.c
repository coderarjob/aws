#include "../header/conf.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define STEP_GARBAGE 0
#define STEP_COMMENT 1
#define STEP_SETTING 2
#define STEP_VALUE 3
#define STEP_END 4
#define STEP_EOF 5
#define STEP_BEGIN 6
#define STEP_NEWLINE 7

#define GOT_BOTH 2

#define LINE_MAX_LENGTH 500

static char *SETTING = "Setting:";
static char *VALUE = "Value:";

void print(char *t);

struct conf *GetNextConf(int fd,int *error)
{
    char c;
    int step = STEP_GARBAGE;
    int complete = 0;
    char *confName, *confValue;
    
    char *line = malloc(sizeof(char)*LINE_MAX_LENGTH);
    char *lp = line;
    
    struct conf *newconf = (struct conf *)malloc(sizeof(struct conf));
    confName = newconf->setting;
    confValue = newconf->value;
    
    while(step != STEP_END && step != STEP_EOF )
    {
		int ret;
		
		if ((ret = read(fd,&c,1)) <= 0){
			if (ret < 0)
			perror("GetNextConf");
			return NULL;
		}

		switch(c)
		{
			case '\0':
			step = STEP_EOF;
			break;
			case '#':
			step = STEP_COMMENT;
			break;
			case '\n':
			*confName='\0';
			*confValue='\0';
			
			lp = line;
			
			if (complete == GOT_BOTH)
				step = STEP_END;
			else
				step = STEP_NEWLINE;
			break;
			case ' ':
			case '\t':
			//Tab and Space are only valid in Comments, not
			//in Name or Path
			if (step != STEP_COMMENT)
				step = STEP_GARBAGE;
			break;		
			default:
			if (step == STEP_NEWLINE)
				step = STEP_BEGIN;
		} //switch
		
		switch(step)
		{
			case STEP_SETTING:
				//printf("%c", c);
				*confName++ = c;
				break;
			case STEP_VALUE:
				//printf("%c", c);
				*confValue++ = c;
				break;
		}
		
		if (step == STEP_BEGIN)
		{
			*lp++ = c;
			*lp='\0';
			
			if (strcmp(line,SETTING) == 0){
				//printf("\nNAME=\n");
				step=STEP_SETTING;
				complete++;
			}
			else if (strcmp(line,VALUE) == 0){
				//printf("\nPATH=\n");
				step = STEP_VALUE;
				complete++;
			}
		}
			
    }

    return newconf;
}


