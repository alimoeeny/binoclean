#include <stdio.h>

char *myfree(s)
char *s;
{
	if(s != NULL) 
	{
		free(s);
		s = NULL;
	}
	return(s);
}
