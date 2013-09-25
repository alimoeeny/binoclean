/*
 * Misc.c a set of miscellaneous routines
 * useful for many programs
 *
 *	strup(char *s) makes a string all upper case
 *	strflush(char *s) cleans out a string
 *	float set(float *f, int n, float val) sets n members
 *			of the float array f to val
 *	int_set(i,n,val) does the same for integers
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


char *strup(s)
char *s;
{
	int i;
	char c;
    
	for(i = 0; i < strlen(s); i++)
		s[i] = (islower(s[i])) ? toupper(s[i]): s[i];
	return(s);
}

char *strflush(s)
char *s;
{
	int i,n;
	n = strlen(s);
	for (i = 0; i < n; i++)
		s[i] = '\0';
	return(s);
}



void float_set(f,n,val)
float *f;
int n;
float val;
{
	float *fp;
	int i;
    
	fp = f;
	for(i = 0; i < n; i++)
		*fp++ = val;
}

void int_set(p,n,val)
int *p;
int n;
int val;
{
	int *fp;
	int i;
    
	fp = p;
	for(i = 0; i < n; i++)
		*fp++ = val;
}


/* sets n bytes to zero */
void byte_clear(p,n)
char *p;
int n;
{
	int i;
	for(i = 0; i < n/sizeof(char); i++)
		*(p + i) = 0;
}

/* returns the order (base 10) of x
 * i.e order 100 = 100, order 10 = 10, order 90 = 10
 */
float get_order(float x)
{
	int lg;
	double temp,order;
    
	if(x == 0)	
		return(0);
	temp = log10(fabs(x*1.03));
	lg = (temp < 0 )? (int)(temp) - 1 : (int)(temp);
	order = pow(10.0,(double)(lg));
	return((float)order);
}


/*
 * fplaces calculates the number of deimal places needed
 * for displaying a number to n sig. digits.
 */
int fplaces(float x, int n)
{
	int lg;
	double temp,order;
	int i;
    
	if(x == 0.)	
		return(n);
	temp = x;
	order = fabs(temp);
	temp = log10(order);
	lg = (temp < 0 )? (int)(temp) - 1 : (int)(temp);
	i = n - lg -1;
	if( i < 0) i = 0;
	return(i);
}

/* puts a string into s which appropriate
 *  for describing f		*/
int fstrcpy(s,f)
char *s;
float f;
{
    
	float lg;
	int i;
    
	if(f == 0.0)
	{
		sprintf(s,"0");
		return(1);
	}
	lg = log10(get_order(f));
	i = (int)lg;
	if(i > 0)
		sprintf(s,"%.d",(int)(f));
	else
		sprintf(s,"%.*f",(1-i),f);
}

/* gets a string from a stream, and puts it in s */
/* if there is a blank line, s is left unaltered */
char *fgetl(s,n,fd)
char *s;
int n;
FILE *fd;
{
	char c;
	int i;
    
	if(fgets(s,n,fd) != NULL)
	{
		i = strlen(s) - 1;
		c = s[i];
		s[i] = (c == '\n')? '\0' : c;
		if(strlen(s) > 0)
			return(s);
		else return((char *)EOF);
	}
	else return((char *)EOF);
}

/* scans for a float	*/
/* returns f unchanged if theres a blank line */
float getf(f)
float f;
{
	float ftmp;
	char s[256];
    
	fgets(s, 256, stdin);
	ftmp = (strlen(s) > 0)? atof(s) : f;
	return(ftmp);
}

/* checks for the existence of a file */
checkfil(s)
char *s;
{
	int i;
	char c;
    
	if(fopen(s,"r") != NULL)
	{
		fprintf(stderr,"%s already exists - continue?\n",s);
		scanf("%1s",&c);
		if(c == 'y' || c == 'Y')
			return(1);
		else
			return(NULL);
	}
	else
		return(1);
}
