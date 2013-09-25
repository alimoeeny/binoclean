#include <stdio.h>
#include <strings.h>
#include "misc.h"

struct typename{
	char *s;
	int code;
	struct typename *next;
} first;

static char firstline[256];

SetFileType(s,i)
char *s;
int i;
{
	struct typename *new = &first;

	if(first.s != NULL)
	{
		while (new->next != NULL)
			new = new->next;
		new->next = (struct typename *)calloc(1,sizeof(struct typename));
		new = new->next;
	}
	new->s = myscopy(new->s,s);
	new->code = i;
}

int myfiletype(s)
char *s;
{
	FILE *fd;
	struct typename *next = &first;
	
	if((fd = fopen(s,"r")) == NULL)
		return(-1);
	else if(fgets(firstline,256,fd) == NULL)
		return(0);
	fclose(fd);

	while(next != NULL)
	{
		if(strncmp(next->s,firstline,strlen(next->s)) == NULL)
			return(next->code);
		next = next->next;
	}
	if(isascii(firstline[0]))
		return(ASCII);
	return(NULL);

}

char *getfilecomment()
{
	char *s,*p;
	
	if((s = index(firstline,':')) == NULL)
		return(NULL);
	s++;
	p = myscopy(p,s);
	return(p);
}

char *getfilename(path)
char *path;
{
	char *p;
	if(p = rindex(path, '/'))
		p++;
	else
		p = path;
	return(strcpy((char *)malloc(strlen(p)+1), p));
}

