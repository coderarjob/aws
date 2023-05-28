#include "../header/webrequest.h"
#include "../header/errors.h"

int readFromSocket(int sd, char *buffer, int buffer_length, int timeout_seconds)
{
    int read_len = 0;
    int total_read = 0;
    
    struct timeval waittime;
    fd_set rfds ;

    FD_ZERO(&rfds);
    FD_SET(sd,&rfds);
    
    do
    {
		waittime.tv_sec = timeout_seconds;
		waittime.tv_usec=0;
		
		if (select(sd+1,&rfds,NULL,NULL,&waittime) == 0)
			return ERR_TIMEOUT;
		
		if ((read_len = read(sd, buffer, buffer_length)) == -1)
			PANIC("read");
		
		buffer_length -= read_len;
		total_read += read_len;
		buffer+=read_len;
    }while(buffer_length > 0 && read_len > 0);
    
    return total_read;
    
}

char *getHTTPrequest(int sd, int *errorcode)
{
  char *message = calloc(MAX_REQUEST_LENGTH, sizeof(char)); 
  char *message_t = message;
  int wordtype = 0; //0=normal, 1= 1st crlf, 2= 2nd cr, 3=terminate
  int len;
  char c;
  while(wordtype != 3 && 
      (len = readFromSocket(sd,&c,1,SOCK_TIMEOUT_SEC)) > 0)
  {
	switch(c)
	{
	    case '\r':
		wordtype = wordtype==0?1:2;
		break;
	    case '\n':
		if (wordtype == 2)
		    wordtype=3; //terminate
		break;
	    default:
		wordtype=0;
		break;		
	};
	
	
	*message_t++ = c;
  }
  
  *message_t = '\0';
  
	if (len <= 0){
		switch(len)
		{
			case 0:
				*errorcode = ERR_NO_DATA;
				break;
			default:
				*errorcode = len;
				break;
	  }
      return NULL;
	}
	else
		*errorcode = ERR_NONE;
	
	return message;
}