#include "../header/debug.h"
#include "../header/keyvalue.h"
#include <stdio.h>

void display_bytes(char *bytes, int length)
{
  static int byte_count = 0;
  
  for(;length > 0; length--, bytes++, byte_count++)
  {
    if (byte_count%8 == 0)
	printf("%s %d \t",
	       (byte_count==0?"":"\n"), 
	       byte_count
	      );
    
    char printedchar;
    if (*bytes == 0xa || *bytes == 0xd)
      printedchar = '*';
    else
      printedchar = *bytes;
      
    printf("%x(%c) ",*bytes, printedchar) ;
  }
} 

void display(struct linkedlist *ll)
{
    printf("\n\t---DISPLAY----\n");
  
    struct llnode *this_g_node = ll->head;
    struct keyvalue *thisnode;
    
    if (this_g_node == NULL)
    {
      printf("\n\tNo item in list.\n");
	  printf("\n\t--------------\n");
      return;
    }
    
    while(this_g_node != NULL){
		thisnode = (struct keyvalue *)this_g_node->item;
		printf("\t%s: %s\n", thisnode->key, thisnode->value);
		this_g_node = this_g_node->next;
    }
    printf("\n\t--------------\n");
}
