#include <stdio.h>
#include "mymath.h"
#include <math.h>
#import <OpenGL/OpenGL.h>
#include <sys/time.h> 
#include "commdefs.h"
#include "stimuli.h"
#include "sprotos.h"
#include "misc.h"

extern FILE *seroutfile,*psychlog;
extern int option2flag;
extern double gammaval;
extern int optionflag,optionflags[],testflags[];
extern Expt expt;
extern int verbose;

static GLubyte *images[MAXFRAMES] = {NULL};
static GLubyte *rightimages[MAXFRAMES] = {NULL};
static int imagelengths[MAXFRAMES] = {0};
static int imagews[MAXFRAMES] = {0};
static int imagehs[MAXFRAMES] = {0};
int imageseed[MAXFRAMES] = {0};


Substim *NewImage(Stimulus *st, Substim *sst)
{
    Locator *pos = &st->pos;/*j*/
    
    FreeStimulus(sst);
    if(sst->ptr == NULL)
        sst->ptr = (OneStim *) (malloc(sizeof(OneStim)));
    if(verbose)
        printf("New Image addr %d\n",sst->ptr);
    //    sst->uimage = NULL;
    init_image(st,sst);
    st->type = STIM_IMAGE;
    sst->type = STIM_IMAGE;
    return(sst);
}


int init_image(Stimulus *st,  Substim *sst)
{
    
	int i,j,*p;
	double val,xval,yval,x,y,cval,cm,deg,sx,sy;
	Locator *pos = &st->pos;
	int ndots;
    
	if(sst->dotsiz[0] < 0.01)
        sst->dotsiz[0] = deg2pix(0.08);
	if(sst->dotsiz[1] < 0.01)
        sst->dotsiz[1] = deg2pix(0.08);
	if(pos->radius[0] < 0.1)
        pos->radius[0] = deg2pix(2);
	if(pos->radius[0] > 2000)
        pos->radius[0] = deg2pix(2);
	if(pos->radius[1] < 0.1)
        pos->radius[1] = deg2pix(2);
    
    
	sst->pos.radius[0] = pos->radius[0];
	sst->pos.radius[1] = pos->radius[1];
    
    
    
	st->ndisps = 0;
	st->pos.locn[0] = 0;
	pos->phase = 0;
	st->phasedisp[0] = 0;
    memcpy(&sst->pos,&st->pos,sizeof(Locator));
	return(0);
}

int PreloadPGM(char *name, Substim *sst, int frame){
    FILE *imfd;
    char buf[BUFSIZ];
    static char lastname[BUFSIZ];
    int w,h,imax;
    
    static int *astate = NULL;
    
    if((imfd = fopen(name,"r")) == NULL){
        if(strncmp(name,lastname,strlen(name)-5) != 0){
            sprintf(buf,"Can't Read Image %s\n",name);
            if(astate == NULL || *astate == 0){ // no confirmer up now
            }
            fputs(buf,stderr);
            if(seroutfile)
                fprintf(seroutfile,"Can't Read Image %s\n",name);
            if(psychlog)
                fprintf(psychlog,"Can't Read Image %s\n",name);
            strcpy(lastname,name);
        }
        return(NULL);
    }
    fgets(buf, BUFSIZ, imfd);
    sscanf(buf,"P5 %d %d %d",&w,&h,&imax);
    imagews[frame] = w;
    imagehs[frame] = h;
    
    if (sst->mode & RIGHTMODE){
        if(w * h > imagelengths[frame]){
            if(rightimages[frame] != NULL)
                free(rightimages[frame]);
            rightimages[frame] = (GLubyte *)malloc(w * h);
            imagelengths[frame] = w * h;
        }
        else{
            if(rightimages[frame] != NULL)
                free(rightimages[frame]);
            rightimages[frame] = (GLubyte *)malloc(w * h);
            imagelengths[frame] = w * h;
        }
        fread(rightimages[frame],1,w*h,imfd);
    }
    else
    {
        if(w * h > imagelengths[frame]){
            if(images[frame] != NULL)
                free(images[frame]);
            images[frame] = (GLubyte *)malloc(w * h);
            imagelengths[frame] = w * h;
        }
        else{
            if(images[frame] != NULL)
                free(images[frame]);
            images[frame] = (GLubyte *)malloc(w * h);
            imagelengths[frame] = w * h;
        }
        fread(images[frame],1,w*h,imfd);
    }
    fclose(imfd);
    return(w*h);
}


int ReadPGMstim(char *name, Substim *sst)
{
    FILE *imfd;
    char buf[BUFSIZ];
    static char lastname[BUFSIZ];
    int w,h,imax;
    
    static int *astate = NULL;
    
    if((imfd = fopen(name,"r")) == NULL){
        if(strncmp(name,lastname,strlen(name)-5) != 0){
            sprintf(buf,"Can't Read Image %s\n",name);
            if(astate == NULL || *astate == 0){ // no confirmer up now
            }
            fputs(buf,stderr);
            if(seroutfile)
                fprintf(seroutfile,"Can't Read Image %s\n",name);
            if(psychlog)
                fprintf(psychlog,"Can't Read Image %s\n",name);
            strcpy(lastname,name);
        }
        return(NULL);
    }
    fgets(buf, BUFSIZ, imfd);
    sscanf(buf,"P5 %d %d %d",&w,&h,&imax);
    sst->imw = w;
    sst->imh = h;
    if(w * h > sst->uimlen){
        if(sst->uimage != NULL)
            free(sst->uimage);
        sst->uimage = (GLubyte *)malloc(w * h);
        sst->uimlen = w * h;
    }
    fread(sst->uimage,1,w*h,imfd);
    fclose(imfd);
    return(w*h);
}


int CopyPGMstim(Substim *src, Substim *tgt)
{
    FILE *imfd;
    char buf[BUFSIZ];
    static char lastname[BUFSIZ];
    int w,h,imax;
    
    w = tgt->imw = src->imw;
    h = tgt->imh = src->imh;
    if(w * h > tgt->uimlen){
        if(tgt->uimage != NULL)
            free(tgt->uimage);
        tgt->uimage = (GLubyte *)malloc(w * h);
        tgt->uimlen = w * h;
    }
    memcpy(tgt->uimage,src->uimage,w*h);
    return(w*h);
}

void calc_image(Stimulus *st, Substim *sst)
{
    char imname[BUFSIZ],buf[BUFSIZ],impref[BUFSIZ];
    int ival;
    double val,ori;
    char eye;
    int seed = 0;
    FILE *fd;
    struct timeval now,then;
    time_t tval;
    int i,id,nf,fo = 0;
    
    if(sst->calculated)
        return;
    /*
     * calc_image is called directly when preloading, so this calculation needs
     * to be done here also, for an SF Fast sequence to work
     */
    
    st->left->pos.sf = st->pos.sf + st->sf_disp;
    st->right->pos.sf =  st->pos.sf - st->sf_disp;
    if(st->preload && st->preloaded)
    {
        if(st->xstate == INTERLEAVE_EXPT_BLANK)
        {
        }
        return;
    }
    
    if(sst->mode == RIGHTMODE)
        eye = 'R';
    else if(sst->mode == LEFTMODE)
        eye = 'L';
    ori = sst->pos.angle*180/M_PI;
    
    if(st->immode == IMAGEMODE_ORBW){ // Size and Ori in path
        if(sst->seedloop != 1)
        /*
         *  only set new seed for left image (called first), so the same image is
         * loaded for both eyes
         */
            if(sst->mode == LEFTMODE){
                if(optionflags[RANDOM_PHASE]){
                    if(expt.mode == ORIENTATION){
                        id = rnd_i()%(expt.nstim[0]+expt.nstim[2]);
                        if(id < expt.nstim[2])
                            st->xstate = INTERLEAVE_EXPT_BLANK;
                        else{
                            ori = rint(i2expval(id,expt.nstim[2],0,0));
                            st->xstate = 0;
                        }
                    }
                    else{
                        ori = (rnd_i()%18) * 10;
                        id = ori;
                    }
                }
                seed = sst->seed = 1 + (rnd_i() % st->nimseed);
            }
            else{
                if(st->flag & UNCORRELATE)
                    seed = 1+ (st->left->seed%st->nimseed);
                else
                    seed =  st->left->seed;
            }
            else
                seed = 1+ (sst->baseseed % st->nimseed);
        /*    gettimeofday(&then,NULL);*/
        
        if(sst->mode == LEFTMODE){
            if(optionflags[RANDOM_PHASE])
                st->stimid = id;
            else
                st->stimid = seed;
        }
        
        if(st->xstate == INTERLEAVE_EXPT_BLANK || ori == INTERLEAVE_EXPT_BLANK && !expt.st->preload)
            return;
        
        if(sst->orbw > MAXORBW)
            sprintf(impref,"%s/orinf/sf%.2f/sz%.2f",st->imprefix,sst->pos.sf,sst->size);
        else if(sst->orbw < 0)
            sprintf(impref,"%s/or%.0f/sf%.2f/sz%.2f/bw%.0f",st->imprefix,ori,sst->pos.sf,sst->size,fabs(sst->orbw));
        else if(sst->orbw < 1)
            sprintf(impref,"%s/or%.0f/sf%.2f/sz%.2f/bw10",st->imprefix,ori,sst->pos.sf,sst->size);
        else
            sprintf(impref,"%s/or%.0f/sf%.2f/sz%.2f/bw%.0f",st->imprefix,ori,sst->pos.sf,sst->size,sst->orbw);
        
        /*
         * if this is the call from PrepareExptStim, get the param list, and
         * record the date by creating a blank file "lastused" in the directory
         * if seedloop is > 0 do ot write the date. That way files are not re-created and psych can be done on a repeated set over and over.
         */
        if(st->prepared == 0 && st->framectr ==0 && st->xstate != INTERLEAVE_EXPT_BLANK){
            sprintf(imname,"%s/lastused",impref);
            if(sst->seedloop  == 0){
                if((fd = fopen(imname,"w")) != NULL){
                    /* need to write something, else timestamp not changed */
                    tval = time(NULL);
                    fprintf(fd,"%s\n",ctime(&tval));
                    fclose(fd);
                }
                else
                    fprintf(stderr,"Can't make %s\n",imname);
            }
            sprintf(imname,"%s/paramlist",impref);
            if((fd = fopen(imname,"r")) != NULL){
                while(fgets(buf, BUFSIZ, fd)){
                    if(!strncmp(buf,"SeedOffset",7)){
                        sscanf(buf,"SeedOffset %d",&ival);
                        st->seedoffset = ival;
                    }
                    else if(!strncmp(buf,"pix2deg",7)){
                        sscanf(buf,"pix2deg %lf",&val);
                        st->pix2deg = val;
                    }
                    else if(!strncmp(buf,"imsz",4)){
                        sscanf(buf,"imsz %d sd %lf",&ival,&val);
                        st->npix = ival;
                        st->imsd = val;
                    }
                    else if(!strncmp(buf,"Version",7)){
                        sscanf(buf,"Version %lf",&val);
                        st->stimversion = val;
                    }
                }
                fclose(fd);
                /*
                 took 0.4ms - we can spare this...
                 gettimeofday(&now, NULL);
                 printf("Params took %.4f\n",timediff(&now,&then));
                 */
            }
        }
        sprintf(imname,"%s/se%d.pgm",impref,seed);
    }
    else if(sst->baseseed > 0)
        sprintf(imname,"%s%.0f.%d%c.pgm",st->imprefix,sst->xshift,sst->baseseed,eye);
    else
        sprintf(imname,"%s%.0f%c.pgm",st->imprefix,sst->xshift,eye);
    
    
    gettimeofday(&then,NULL);
    nf = 1;
    if(st->preload){
        if(st->prev != NULL)
            fo = st->prev->framectr+200;
        else
            fo = st->framectr;
        imageseed[fo] = seed;
        
        if(st->xstate > INTERLEAVE_EXPT_BLANK){
            PreloadPGM(imname,sst,fo);
        }
        nf = st->nframes;
    }
    else if(st->immode == IMAGEMODE_ORBW && sst->mode == RIGHTMODE)
        CopyPGMstim(st->left,st->right);
    else if(st->imname && strcmp(imname,st->imname) == 0) /* same imgage - don't re-read */
        ;
    else if(ReadPGMstim(imname,sst))
        st->imname = myscopy(st->imname, imname);
    gettimeofday(&now, NULL);
    val = timediff(&now,&then);
    if(val > 0.1 && seroutfile){
        fprintf(seroutfile,"#Imread (%d,%d) %s took %.4f\n",sst->mode,nf,imname,val);
    }
    sst->calculated = 1;
    /*
     * Painting image will paint over old one, so no need for clearing the
     * screen. This may change if images of different size are loaded in one
     * sequence
     st->noclear = 1; 
     * but this causes problems when image is dragged. Need to use
     st->mode & STIMULUS_NEED_CLEAR
     */
    st->mode &= (~STIMULUS_NEEDS_CLEAR);
}

int paint_image(Stimulus *st, Substim *sst)
{
    int w,h,frame;
    double c;
    if(st->xstate == INTERLEAVE_EXPT_BLANK){
        if(testflags[TEST_RC])
            glClearColor(0, 0, 0, 1.0);
        else
            glClearColor(st->gammaback, st->gammaback, st->gammaback, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        return(0);
    }
    if (st->prev != NULL)
        frame = st->prev->framectr+200;
    else{
        frame = st->framectr;
        if(st->preload & frame > st->preloaded)
            frame = st->preloaded;
    }
    /*
     if(optionflags[FAST_SEQUENCE] & expt.vals[FAST_SEQUENCE_RPT] > 1)
     frame = floor(st->framectr/expt.vals[FAST_SEQUENCE_RPT]);
     */
    if(st->stimid > -1000){
        glPixelZoom(1.0,-1.0);
        if(st->preload){
            w= imagews[frame];
            h= imagews[frame];
        }
        else{
            w = sst->imw;
            h = sst->imh;
        }
        
        if(sst->pos.contrast < 0.99){
            c = sst->pos.contrast;
            glPixelTransferf(GL_RED_BIAS,0.5-c/2);
            glPixelTransferf(GL_RED_SCALE,c);
            glPixelTransferf(GL_BLUE_BIAS,0.5-c/2);
            glPixelTransferf(GL_BLUE_SCALE,c);
            glPixelTransferf(GL_GREEN_BIAS,0.5-c/2);
            glPixelTransferf(GL_GREEN_SCALE, c);
            //    glPixelTransferf(GL_GREEN_SCALE, 1.0);
        }
        else{
            glPixelTransferf(GL_RED_BIAS,0);
            glPixelTransferf(GL_RED_SCALE,1);
            glPixelTransferf(GL_BLUE_BIAS,0);
            glPixelTransferf(GL_BLUE_SCALE,1);
            glPixelTransferf(GL_GREEN_BIAS,0);
            glPixelTransferf(GL_GREEN_SCALE, 1);
        }
        glRasterPos2i(sst->pos.xy[0]-w/2,sst->pos.xy[1]+h/2);
        if(st->preload){
            if (sst->mode == RIGHTMODE && st->flag & UNCORRELATE)
                glDrawPixels(imagews[frame], imagehs[frame], GL_LUMINANCE, GL_UNSIGNED_BYTE, rightimages[frame]);
            else
                glDrawPixels(imagews[frame], imagehs[frame], GL_LUMINANCE, GL_UNSIGNED_BYTE, images[frame]);
        }
        else
            glDrawPixels(sst->imw, sst->imh, GL_LUMINANCE, GL_UNSIGNED_BYTE, sst->uimage);
    } 
    sst->calculated = 0;
    return(sst->imw);
}
