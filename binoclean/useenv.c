#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "misc.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/dir.h>

#ifndef MAXPATHLEN
#include <sys/param.h>
#endif
#include <pwd.h>

char *useenv(instring)
char *instring;
{
    char outstr[300];
    char *inptr,*outptr,*tempptr;
    char tempstr[100];
    char *environ;
    
	if(instring==NULL)
	{
		warn("NULL string",1);
		return(NULL);
	}
	inptr=instring;
	outptr = outstr;
	while (*inptr != 0) {
		tempptr= (char *)tempstr;
        /* don't do anything is the tilde is just a backup file */
		if (*inptr=='~' && strlen(inptr) > 2) 
        /*		if (*inptr=='~') */
		{
			if((environ = tilde(inptr)) != NULL)
				strcpy(outptr,environ);
			outptr+= strlen(environ);
			*outptr = 0;
			free(environ);
			break;
		}
		if (*inptr=='$') {
			inptr++;
			while (isalnum(*inptr)) 
				*tempptr++ = *inptr++;
			*tempptr=0;
			environ= (char *)getenv(tempstr);
			if (environ==0) {
				;
			} else {
				strcpy(outptr,environ);
				outptr+= strlen(environ);
			}
		}
		else {
			*(outptr++) = *(inptr++);
		}
	}
	*outptr=0;
	return((char *)strcpy((char *)malloc(strlen(outstr)+1),outstr));
}


char *CompleteFilename(path)
char *path;
{
	char *p = NULL,*name,*dir,buf[256],*rindex();
	DIR *dirp;
	struct stat s_buf;
#ifdef SunOS
	struct dirent *dp;
#else
	struct direct *dp;
#endif	
    
	if((name = rindex(path,'/')) == NULL)
	{
		name = path;
		dir = myscopy(dir,".");
	}
	else
	{
		*name++ = '\0';
		dir = path;
	}
	
	if(stat(dir, &s_buf) == -1 || !(s_buf.st_mode & S_IREAD))
		return(path);
	if (s_buf.st_mode & S_IFDIR)
	{
		if(!(dirp = opendir(dir)))
            return(path);
		while(dp = readdir(dirp))
            if(strncmp(dp->d_name, name,strlen(name)) == 0)
            {
                sprintf(buf,"%s/%s",dir,dp->d_name);
                p = myscopy(p,buf);
                break;
            }
		closedir(dirp);
	}
	free(dir);
	return((p == NULL) ? path : p);
}

char *tilde(char *s)
{
	char *o,*p,buf[256],*start;
	struct passwd *pw = NULL;
	
	o = s;
	for(p = buf, o = s+1; *o && *o != '/'; *p++ = *o++)
		*p = 0;
	*p = 0;
	if(buf[0] == 0)
		start = useenv("$HOME");
	else
	{
        pw = getpwnam(buf);
        if(pw == NULL)
            return(NULL);
        else
            start = pw->pw_dir;
	}
    
	sprintf(buf,"%s%s",start,o);
	free(start);
	return((char *)strcpy((char *)malloc(strlen(buf)+1),buf));
}
