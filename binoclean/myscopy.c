#include <stdio.h>
#include "misc.h"
#include <string.h>

char *myscopy(char *s1, char *s2)
{
    
    int i;
    
    if(s1 == s2)
        return(s1);
	if(s1 != NULL)
		free(s1);
	if(s2 == NULL)
		return(NULL);
	else if((i = strlen(s2)) == 0)
		return(NULL);
	else
	{
        s1 = (char *)malloc(sizeof(char) *(i+3));
        s1 = strcpy(s1,s2);
	}
	return(s1);
}

char *mysncopy(char *s1,char *s2, int n)
{
	char *calloc();
    
	if(s1 != NULL)
		free(s1);
	if(s2 == NULL)
		return(NULL);
	else if(strlen(s2) == 0)
		return(NULL);
	else
	{
        s1 = calloc(1,(strlen(s2)+1));
        strncpy(s1,s2,n);
	}
	return(s1);
}

char *nonewline(char *s)
{
	int i;
	if(s == NULL) return(NULL);
	i = strlen(s) - 1;
	while(s[i] == '\n' && i >= 0)
		s[i--] = '\0';
	return(s);
}
