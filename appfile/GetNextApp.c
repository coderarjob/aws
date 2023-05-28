#include "../header/appfile.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define STEP_GARBAGE 0
#define STEP_COMMENT 1
#define STEP_NAME 2
#define STEP_APP 3
#define STEP_END 4
#define STEP_EOF 5
#define STEP_BEGIN 6
#define STEP_NEWLINE 7

#define GOT_BOTH 2

#define LINE_MAX_LENGTH 500

static char *NAME = "Name:";
static char *APP = "App:";

void print(char *t);

struct app *GetNextApp(int fd,int *error)
{
    char c;
    int step = STEP_GARBAGE;
    int complete = 0;
    char *appName, *appPath;
    
    char *line = malloc(sizeof(char)*LINE_MAX_LENGTH);
    char *lp = line;
    
    struct app *newapp = (struct app *)malloc(sizeof(struct app));
    appName = newapp->name;
    appPath = newapp->path;
    
    while(step != STEP_END && step != STEP_EOF )
    {
	int ret;
	
	if ((ret = read(fd,&c,1)) <= 0){
	    if (ret < 0)
		perror("GetNextApp");
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
		*appName='\0';
		*appPath='\0';
		
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
	    case STEP_NAME:
		//printf("%c", c);
		*appName++ = c;
		break;
	    case STEP_APP:
		//printf("%c", c);
		*appPath++ = c;
		break;
	}
	
	if (step == STEP_BEGIN)
	{
	    *lp++ = c;
	    *lp='\0';
	    
	    if (strcmp(line,NAME) == 0){
		//printf("\nNAME=\n");
		step=STEP_NAME;
		complete++;
	    }
	    else if (strcmp(line,APP) == 0){
		//printf("\nPATH=\n");
		step = STEP_APP;
		complete++;
	    }
	}
		
    }

    return newapp;
}


