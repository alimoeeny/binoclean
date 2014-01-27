#import <stdio.h>
#import <math.h>
#import <stdlib.h>
#import <OpenGL/gl.h>
#import "commdefs.h"
#import "stimuli.h"
#import "sprotos.h"

extern int verbose, debug;
static int nreplaced = 0;
/*#define LEFT	-1
 #define RIGHT	1*/

extern Expt expt;
extern Monitor mon;
extern int YOFFSET;
static int aamode = 0;
static vcoord cylrect[8];
static vcoord clrect[4];

float calc_newoffset(float maxdisparity, float xpos);
OneStim *NewCylinder(Stimulus *st, Substim *sst, Substim  *copy);
int init_cylinder(Stimulus *st,  Substim *sst);                  
ball_s *get_mem(int ndots);
void fill_balls(int ndots, ball_s *balls, int flag, int lifeframes); 
void free_cylinder(Substim *st);
float calc_offset(float minaxis, float x, float viewdist, float halfiod, int left_right);
void calc_cylinder(Stimulus *st); 
void plc_paint_cylinder(Stimulus *st);
void rect_paint_cylinder(Stimulus *st, int mode);
void rect_calc_cylinder(Stimulus *st);
void draw_dot(float rotatefactor[XY], float hdotsize[XY], float xpos, float ypos, float offset, float standing_disp,  float dotwidth, int mode, Locator *pos, int flag, ball_s *ball);
void calc_cyl_motion(ball_s *balls, float vel, int ndots, int flag, int lifeframes, float deathchance, int width);
void calc_subpix_disp(ball_s *balls, int numdots, int flag, float disparity, Locator *pos, float widthfactor, float heightfactor);

/*
 * for painting a color that is blended funny. This must be left over from attempts to eliminate cross talk?
 * DO NOT use with antialiasing - just makes it all low contrast
 */
#define mybcolor(x) glColor4f((float)(x[0]),(float)(x[1]),(float)(x[2]), 0.09)

extern int optionflag,  option2flag;

/*******************************************************************************/			
OneStim *NewCylinder(Stimulus *st, Substim *sst, Substim  *copy)
{
    OneStim *pcyl;
    Locator *pos = &st->pos;
    
    FreeStimulus(sst);
    /* initialise values (i.e pcyl values, which points to Cylinder structure */
    
    if(sst->ptr == NULL){
        sst->ptr = (OneStim *)malloc(sizeof(OneStim));
        sst->ptr->velocity=0.003;
    }
    if(sst->ptr->velocity > 0.1)
        sst->ptr->velocity=0.003;
    pcyl = sst->ptr; 
    pcyl->numdots=0;
    if(pos->density != 0)
        pcyl->density=pos->density;
    else
        pcyl->density=20.0; 				        
    
    pcyl->olddotsize[X]=-3;/* must be different to dotsize initially to run primitive list */
    pcyl->olddotsize[Y]=-3;
    if(pos->dotsize > 0.5)/*j*/
        pcyl->dotsiz[X] = pcyl->dotsiz[Y]= pos->dotsize;
    else
        pcyl->dotsiz[X] = pcyl->dotsiz[Y] = 5;
    pcyl->direction_r=GL_TRUE; 
    pcyl->front_intensity=255; 
    pcyl->back_intensity=255; 
    pcyl->deathchance=0.02; /* for each dot, range 0.0 - 1.0 */
    pcyl->lifeframes=10;
    pcyl->balls=NULL;
    pcyl->primitive=NULL;
    /*pcyl->primitive[1]=NULL; was originally put in to do negative contrast dots*/
    pcyl->fixdots=80;  
    pcyl->standing_disp=0;
    sst->ptr = pcyl;
    init_cylinder(st,sst);
    st->type = STIM_CYLINDER;
    optionflag |= SQUARE_RDS;
    sst->type = STIM_CYLINDER;
    
    pcyl->trackball.pos[X]=0.0;
    pcyl->trackball.pos[Y]=0.0;
    pcyl->trackball.left_right=1;
    
    /* short term solution */
    
    srand48(getpid());
    return(pcyl);
}

/*******************************************************************************/			
int init_cylinder(Stimulus *st,  Substim *sst) /* passed st, st->left (pointer to left substim */
{
    
	OneStim *cyl = (OneStim *)sst->ptr;       /* specifics of stimulus*/
	Locator *pos = &st->pos;
    int ndots=0, first, i, meanintensity;
	int numlevels = SUBPIXLEVELS;
    
    
	if(st->flag & FIXED_DOTNUM)  
		ndots = cyl->fixdots;
	else {		
		if(cyl->density < 0.0 || cyl->density >=100)
			cyl->density = 5.0;  	
		ndots = (int)floor((cyl->density/100) * (pos->imsize[X] * pos->imsize[Y]) / (cyl->dotsiz[X] * cyl->dotsiz[Y])); 
	}	
    
	if(cyl->numdots < ndots || cyl->balls == NULL) {
		if(cyl->balls != NULL)
			free(cyl->balls);
		cyl->balls = get_mem(ndots);
	} 
    
	if(ndots >= 0)
		cyl->numdots = ndots;
    
	
    myrnd_init(st->left->baseseed);
    srand48(st->left->baseseed);
	fill_balls(cyl->numdots, cyl->balls, st->flag, cyl->lifeframes);      /* fill balls structure with random numbers */
    /*flag will give whether flat or cylindriical motion */
	/* if subpix mode and calprims set ie. new or size has changed set new list of primitives */
	/*if((option2flag & JSUBPIX) && (cyl->dotsiz[X] != cyl->olddotsize[X])){
     if(cyl->primitive != NULL){
     free(cyl->primitive);
     cyl->primitive = NULL;
     }
     
     */ /* in future can change numlevels */
    /*cyl->primitive = (unsigned long **)malloc(numlevels * sizeof(unsigned long *));*/
    /*for(i=0; i<numlevels; i++){
     cyl->primitive[i] = rgb_calc_primitive(cyl->dotsiz[X], cyl->dotsiz[Y], i, numlevels, cyl->front_intensity);
     }
     cyl->olddotsize[X] = cyl->dotsiz[X];
     }	*/
    
	memcpy(&sst->pos,&st->pos,sizeof(Locator)); /* hmmmmmmmmm just in case */
	return(0);
}

/*******************************************************************************************/
ball_s *get_mem(int ndots)
{
    
	ball_s *balls;
	if(ndots <=0)
        return(NULL);
	if (!(balls = (ball_s *)malloc(ndots * sizeof(ball_s)))) {
	    fprintf(stderr, "bounce: malloc failed\n");
	    return NULL;
	}
	return(balls);
}

/*******************************************************************************************/
void fill_balls(int ndots, ball_s *balls, int flag, int lifeframes) 
{
	int i;
	double drnd;
	
    
	if(verbose)
        printf("%d dots replaced, %d dots each frame\n",nreplaced,ndots);
	nreplaced = 0;
	if(flag & FLAT_SURFACES){
		for (i = 0; i < ndots; i++) {
            
			balls[i].pos[X] = 2.0 * (mydrand() - 0.5); /* -1 to +1 */
			drnd = mydrand();
			balls[i].pos[Y] = 2.0 * (drnd - 0.5); /* -1 to +1 */
			if (mydrand() >= 0.5)
			    balls[i].left_right = JONRIGHT;
			else 
			    balls[i].left_right = JONLEFT;
			balls[i].lrnd = myrnd_u();
			balls[i].countdown = balls[i].lrnd % (lifeframes+1);
		}  
	}
	else{
		for (i = 0; i < ndots; i++) {
			balls[i].pos[X] = sinf(M_PI * (mydrand() - 0.5)); /* sin(-pi/2 -> pi/2 */
			drnd = mydrand();
			balls[i].pos[Y] = 2.0 * (drnd - 0.5); /* -1 to +1 */
			if (mydrand() >= 0.5) 
			    balls[i].left_right = JONRIGHT;
			else 
			    balls[i].left_right = JONLEFT;
			balls[i].lrnd = myrnd_u();
			balls[i].countdown = balls[i].lrnd % (lifeframes+1);
		}
	}
}

/*******************************************************************************/			
void free_cylinder(Substim *st)
{
    OneStim *ptr;
    
    if((ptr = st->ptr) != NULL)
    {
        free(ptr->balls);
        ptr->balls = NULL;
        /*	free(ptr);
         st->ptr = 0;
         */
    }
}



/*******************************************************************************************/
void calc_cylinder(Stimulus  *st) 
{         
    OneStim *cyl = (OneStim *)st->left->ptr;       /* points to st->left->ptr */
    ball_s *balls = cyl->balls;
    Locator *pos = &st->pos;
    int i;
    float delta, vel, fraction, sign;
    int flag = st->flag;	
    float disparity, deathchance;
    int countdown;
    float widthfactor, heightfactor;
    float dx,dy;
    float rotatefactor[2];
    
    rotatefactor[X]=cosf(pos->angle-(M_PI/2));
    rotatefactor[Y]=sinf(pos->angle-(M_PI/2));
    
    disparity= st->disp*2;
    dx = (disparity * rotatefactor[X])/2;
    dy = (disparity * rotatefactor[Y])/2;
    widthfactor = (pos->imsize[Y]/2) - (cyl->dotsiz[X]); /*j width and height swapped from earlier so direction same as sine and rds*/
    heightfactor = (pos->imsize[X]/2) - (cyl->dotsiz[Y]); 
    
    
    for(i = 0; i < 2; i++){ /*j*/
        if(pos->imsize[i] == 0)
            pos->imsize[i] = 256;
        pos->radius[i] = pos->imsize[i]/2;
    }
    
    /*j 3/2/97 motion calculation now in separate function
     does calculation for all balls i=0 i<cyl.numdots + plus tests whether dot should die or not*/
    calc_cyl_motion(balls, cyl->velocity, cyl->numdots, flag, cyl->lifeframes, cyl->deathchance, pos->imsize[X] );      	
    
    /*if(option2flag & JSUBPIX){
     calc_subpix_disp(balls, cyl->numdots, flag, st->disp, pos, widthfactor, heightfactor);
     }
     else{*/
	for(i=0;i<cyl->numdots;i++){		
	    if (flag & FLAT_DISPARITY)
		    balls[i].offset = disparity * balls[i].left_right; 
	    else
		    balls[i].offset = disparity * balls[i].proportion * balls[i].left_right; 		
	    
	    /*j convert to position in pixels*/		
	    balls[i].position[X] = balls[i].pos[X] * widthfactor;
	    balls[i].position[Y] = balls[i].pos[Y] * heightfactor; 
	    if(st->flag & CARDS)
            balls[i].dot=1 - balls[i].proportion;
	    else if(st->flag & TEXTURE)
            balls[i].dot=balls[i].proportion;
	    else
            balls[i].dot=1;
	}
    return;
}
/*******************************************************************************************/

int paint_balls(Stimulus *st, int mode, OneStim *cyl,float *vcolor, float *bcolor,
                float *rotatefactor, float *hdotsize, int aa)
{
    ball_s *balls = cyl->balls;
    Locator *pos = &st->pos;
    int i=0,j,r;
    
    if((st->flag & (FRONT_ONLY | BACK_ONLY | FRONT_SURFACE_OCCLUDES)) == 0){
        for (i = 0; i <cyl->numdots; i++) { 
            /* calculate parameters for hole */
            if(st->prev != NULL && st->prev->type == STIM_CYLINDER && !(optionflag | (BACKGROUND_FIXED_BIT))){
                if(fabsf(balls[i].position[X]) < (st->prev->pos.imsize[Y]/2) &&
                   fabsf(balls[i].position[Y]) < (st->prev->pos.imsize[X]/2))
		    		continue;
            }
            if(!(optionflag & SQUARE_RDS)){ /*j test to see if dots fall in elipse*/
                if ( ((balls[i].pos[X] * balls[i].pos[X]) 
                      + (balls[i].pos[Y] * balls[i].pos[Y])) >= 1){
                    continue;
                }
            }
            if(((balls[i].lrnd >>16) & 0xff) > st->dotfrac*255)
                mycolor(vcolor);      
            else
                mycolor(bcolor);
            draw_dot(rotatefactor, hdotsize, balls[i].position[X], balls[i].position[Y], balls[i].offset, cyl->standing_disp, balls[i].dot, mode,pos,st->flag,&balls[i]);
        }
        return(i);
    }
    
    
    mycolor(vcolor);
    if ( !(st->flag & FRONT_ONLY)){ /*j saves energy if only want one plane!*/
        for (i = 0; i <cyl->numdots; i++) { 
            /* calculate parameters for hole */
            if(st->prev != NULL && st->prev->type == STIM_CYLINDER && !(optionflag | (BACKGROUND_FIXED_BIT))){
                if(fabsf(balls[i].position[X]) < (st->prev->pos.imsize[Y]/2) &&
                   fabsf(balls[i].position[Y]) < (st->prev->pos.imsize[X]/2))
		    		continue;
            }
            if(!(optionflag & SQUARE_RDS)){ /*j test to see if dots fall in elipse*/
                if ( ((balls[i].pos[X] * balls[i].pos[X]) 
                      + (balls[i].pos[Y] * balls[i].pos[Y])) >= 1){
                    continue;
                }
            }
            if(balls[i].left_right == JONRIGHT){
                if(((balls[i].lrnd >>16) & 0xff) > st->dotfrac * 255)
                    mycolor(vcolor);      
                else
                    mycolor(bcolor);
                if(st->flag & HOLD_PLANE)/*j holds the 'NULL direction' plane at the standing disparity*/
                    draw_dot(rotatefactor, hdotsize, balls[i].position[X], balls[i].position[Y], 0, cyl->standing_disp, balls[i].dot, mode,pos,st->flag,&balls[i]);
                else    
                    draw_dot(rotatefactor, hdotsize, balls[i].position[X], balls[i].position[Y], balls[i].offset, cyl->standing_disp, balls[i].dot, mode,pos,st->flag,&balls[i]);
            }
        }
    }
    mycolor(bcolor);
    
    if ( !(st->flag & BACK_ONLY)){ 
        for (i = 0; i <cyl->numdots; i++) { 
            if(st->prev != NULL && st->prev->type == STIM_CYLINDER && !(optionflag | (BACKGROUND_FIXED_BIT))){
                if(fabsf(balls[i].position[X]) < (st->prev->pos.imsize[Y]/2) &&
                   fabsf(balls[i].position[Y]) < (st->prev->pos.imsize[X]/2))
		    		continue;
            }
            if(!(optionflag & SQUARE_RDS)){ /*j test to see if dots fall in elipse*/
                if ( ((balls[i].pos[X] * balls[i].pos[X]) 
                      + (balls[i].pos[Y] * balls[i].pos[Y])) >= 1){
                    continue;
                }
            }
            if(balls[i].left_right == JONLEFT){
                if(((balls[i].lrnd >>16) & 0xff) > st->dotfrac*255)
                    mycolor(vcolor);      
                else
                    mycolor(bcolor);
                draw_dot(rotatefactor, hdotsize, balls[i].position[X], balls[i].position[Y], balls[i].offset, cyl->standing_disp, balls[i].dot, mode,pos,st->flag,&balls[i]);
            }
        }
    }
    
    return(i);
}


void paint_cylinder(Stimulus *st, int mode, double subtracting)

{   
    
    Locator *pos = &st->pos;                 /* pos should point to dimention data (locator structure) */
    OneStim *cyl = (OneStim *)st->left->ptr; 
    ball_s *balls = cyl->balls;              /* balls should now point to balls structure */
    float angle  = pos->angle * 180/M_PI;
    float rotatefactor[XY];
    int i, j, r;
    float vcolor[3] = {0, 0, 0};       /* yes yankee spelling!! */
    float bcolor[3] = {0, 0, 0};       /* yes yankee spelling!! */
    float dimvcolor[3] = {0, 0, 0};       /* yes yankee spelling!! */
    float dimbcolor[3] = {0, 0, 0};       /* yes yankee spelling!! */
    float hdotsize[XY];
    float xpos;
    float cosa,sina,h,w;
    float lum[2];
    float plcscale = expt.vals[PLC_MAG];
    
    if (st->flag & DIRECTION_L || st->disp < 0)
    	cyl->direction_r = 0;
    else 	
    	cyl->direction_r = 1;	
    
    w = hdotsize[X] = cyl->dotsiz[X]/2;
    h = hdotsize[Y] = cyl->dotsiz[Y]/2;
    rotatefactor[X]=cosf(pos->angle-(M_PI/2));
    rotatefactor[Y]=sinf(pos->angle-(M_PI/2));
    
    cosa = cos(pos->angle+M_PI/2);
    sina = sin(pos->angle+M_PI/2);
    cylrect[0] = -w * cosa - h * sina;
    cylrect[1] = -h * cosa + w * sina;
    cylrect[2] = -w * cosa + h * sina;
    cylrect[3] = h * cosa + w * sina;
    cylrect[4] = w * cosa + h * sina;
    cylrect[5] = h * cosa - w * sina;
    cylrect[6] = w * cosa - h * sina;
    cylrect[7] = -h * cosa - w * sina;
    h = h+0.5;
    clrect[0] = -h * sina;
    clrect[1] = -h * cosa;
    clrect[2] = h * sina;
    clrect[3] = h * cosa;
    
    
    /*if (mode == JONRIGHT) used if want diff postions on monitors
     pos = &st->right->pos;
     else
     pos = &st->left->pos;*/
	
    glPushMatrix();
    if(mode == JONRIGHT)
        glTranslatef(pos->xy[X], pos->xy[Y]-st->vdisp, 0.0);
    else
        glTranslatef(pos->xy[X], pos->xy[Y]+st->vdisp, 0.0);
    
    glRotatef((angle+90.0),0.0,0.0,1.0); /*j temp to fix direction so same as rest*/
	
    if(st->background > 0.1){
        if((lum[0] = st->background - st->pos.contrast * st->background) < 0)
            lum[0] = 0;
        if((lum[1] = st->background + st->pos.contrast * st->background) > 1)
            lum[1] = 1;
    }
    else{ 
        lum[0] = 0;
        lum[1] = st->pos.contrast;
    }
    if(st->flag & CONTRAST_NEGATIVE && st->flag & CONTRAST_POSITIVE){
        cyl->back_intensity = lum[0];
        cyl->front_intensity = lum[1];
    }
    else if(st->flag & CONTRAST_NEGATIVE){
        cyl->back_intensity = cyl->front_intensity = lum[0];
    }
    else if(st->flag & CONTRAST_POSITIVE){
        cyl->back_intensity = cyl->front_intensity = lum[1];
    }
    if(st->flag & ANTICORRELATE && mode == JONRIGHT)
    {
        vcolor[0] = vcolor[1] = vcolor[2] = cyl->back_intensity;
        bcolor[0] = bcolor[1] = bcolor[2] = cyl->front_intensity;
    }
    else
    {
        if(subtracting){
            vcolor[0] = vcolor[1] = vcolor[2] = st->background +
            (st->background - cyl->front_intensity) * subtracting;
            bcolor[0] = bcolor[1] = bcolor[2] = st->background +
            (st->background - cyl->back_intensity) * subtracting ;
        }
        else{
            vcolor[0] = vcolor[1] = vcolor[2] = cyl->front_intensity;
            bcolor[0] = bcolor[1] = bcolor[2] = cyl->back_intensity ;
        }
    }
    aamode = 0;
    if(st->flag & CYL_PLC){
        for(i =0; i < 3; i++)
        {
            dimvcolor[i] = st->background + (vcolor[i]-st->background) * plcscale;
            dimbcolor[i] = st->background + (bcolor[i]-st->background) * plcscale;
        }
    }
    if(st->flag & TEXTURE)
    {
        glLineWidth(1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        aamode = 2;
        paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 0);
    }
    else if(st->flag & CYL_PLC){
        glLineWidth(hdotsize[0]*balls[0].dot*2);
        glBegin(GL_LINES);
        st->flag |= BACK_ONLY;
        paint_balls(st, mode, cyl, dimvcolor, dimbcolor, rotatefactor, hdotsize, 0);
        st->flag &= (~(BACK_ONLY | FRONT_ONLY));
        
        st->flag |= FRONT_ONLY;
        paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 0);
        st->flag &= (~(BACK_ONLY | FRONT_ONLY));
        glEnd();
    }
    else
    { 
        if (~optionflag & ANTIALIAS_BIT){
            glLineWidth(hdotsize[0]*balls[0].dot*2);
            glBegin(GL_LINES);
            paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 0);
            glEnd();
        }
    }
    if(optionflag & ANTIALIAS_BIT && st->flag & TEXTURE)
    {
        aamode = 2;
        paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 0);
    }
    else if(optionflag & ANTIALIAS_BIT)
    {
        aamode = 1;
        w = hdotsize[0]*balls[0].dot;
        if(w < 1)
            w = 1;
        else
            w = w*2;
        glLineWidth(1.0);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (st->aamode == AAPOLYLINE) { //Line smoothing
            glEnable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 1);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 1);
        }
        else if (st->aamode == AAPOLYGON){  // Polygon AA - get diagonal articact, but geometry correct.
            glDisable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 1);
        }
        else if (st->aamode == AABOTH){  // Do Line then poly for each. Best but slow
            glEnable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 1);
        }
        else if (st->aamode == AALINE){  // Drawn as Thick lines. No good on Mac
            glLineWidth(w);
            glEnable(GL_LINE_SMOOTH);
            glBegin(GL_LINES);
            paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
        }
        else if(st->aamode == AAPOLYGON_AND_LINE){ //Draws ppolygon then uses GL_LINES
            glLineWidth(1.0);
            glEnable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            paint_balls(st, mode, cyl, vcolor, bcolor, rotatefactor, hdotsize, 1);
        }
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}

/*******************************************************************************/			

void plc_paint_cylinder(Stimulus *st)

{   
    
    Locator *pos = &st->pos;                 /* pos should point to dimention data (locator structure) */
    OneStim *cyl = (OneStim *)st->left->ptr;    /* cyl should now point to stim specific OneStim structure */
    ball_s *balls = cyl->balls;              /* balls should now point to balls structure */
    float vcolor[3] = {0, 0, 0};      
    int i, fmapcolour, first, meanintensity;
    float xpospix, ypospix, dot[XY]; /* conversion to pixels and dot[XY] used when CARDS or TEXTURE */
    float angle  = pos->angle * 1800/M_PI;
    
    float radpix = (pos->imsize[X]/2) - (cyl->dotsiz[X]);           /* radius (1/2 image width) of cylinder in pixels */ 
    float halfheightpix = (pos->imsize[Y]/2) - (cyl->dotsiz[Y]);    /* plus the dotsize so all drawn within stim area */
    int factor = 1;
    int flag = st->flag;
    
    if(flag & FLAT_DISPARITY) 
        radpix -= st->disp; /* to avoid painting outside boundary square */
  	
    pos = &st->left->pos;
    meanintensity=128;
    
	
    glPushMatrix();
    glTranslatef(pos->xy[X], pos->xy[Y], 0);
    glRotatef(angle,0.0,0.0,1.0);
    
    if (flag & DIRECTION_L)
    	cyl->direction_r = 0;
    else 	
    	cyl->direction_r = 1;	
    
    dot[X]=1;
    for (i = 0; i <cyl->numdots; i++) { 
        if ( !((cyl->direction_r && balls[i].left_right == RIGHT) || (!cyl->direction_r && balls[i].left_right == LEFT)) ){
            xpospix = floorf(balls[i].pos[X] * radpix);
            ypospix = floorf(balls[i].pos[Y] * halfheightpix); 
            
            vcolor[0] = vcolor[1] = vcolor[2] = ( -(balls[i].proportion) * LEVELS) + meanintensity;
            mycolor(vcolor);
            
            if(flag & CARDS)
                dot[X]=1 - balls[i].proportion;
            if(flag & TEXTURE)
                dot[X]=balls[i].proportion;
            
            glRects(xpospix - cyl->dotsiz[X]/2*dot[X], ypospix - cyl->dotsiz[Y]/2,
                    xpospix + cyl->dotsiz[X]/2*dot[X], ypospix + cyl->dotsiz[Y]/2);
        }   
    }
    for (i = 0; i <cyl->numdots; i++) { 
        if ( ((cyl->direction_r && balls[i].left_right == RIGHT) || (!cyl->direction_r && balls[i].left_right == LEFT)) ){
            xpospix = floorf(balls[i].pos[X] * radpix);
            ypospix = floorf(balls[i].pos[Y] * halfheightpix); 
            
            vcolor[0] = vcolor[1] = vcolor[2] = ( (balls[i].proportion) * LEVELS) + meanintensity;
            mycolor(vcolor);
            
            if(flag & CARDS)
                dot[X]=1 - balls[i].proportion;
            if(flag & TEXTURE)
                dot[X]=balls[i].proportion;
            
            glRects(xpospix - cyl->dotsiz[X]/2*dot[X], ypospix - cyl->dotsiz[Y]/2,
                    xpospix + cyl->dotsiz[X]/2*dot[X], ypospix + cyl->dotsiz[Y]/2);
        }   
    }
    
    glPopMatrix();
    return;
}


void paint_track(Stimulus *st)
{
    Locator *pos = &st->pos;                 /* pos should point to dimention data (locator structure) */
    OneStim *cyl = (OneStim *)st->left->ptr;    
    float radpix = (pos->imsize[X]/2) - (cyl->dotsiz[X]+2);           /* radius (1/2 image width) of cylinder in pixels */ 
    float vcolor[3] = {255, 255, 255};       /* yes yankee spelling!! */
    float xpospix;
    glPushMatrix();
    glTranslatef(pos->xy[X], pos->xy[Y], 0);
    mycolor(vcolor);
    xpospix = cyl->trackball.pos[X]*radpix;
    myrect(xpospix-(cyl->dotsiz[X]*1.5), 0.0-(cyl->dotsiz[Y]*1.5),
           xpospix+(cyl->dotsiz[X]*1.5), 0.0+(cyl->dotsiz[Y]*1.5));
    glPopMatrix();
}
/*******************************************************************************************/

float calc_newoffset(float maxdisparity, float xpos)
{
	return(maxdisparity*(cos(asin(xpos))) ); /* offset calculated as projection of circle */
}
/*******************************************************************************************/
float calc_offset(float minaxis, float x, float viewdist, float halfiod, int left_right)

/* valid if -1.0 < x < 1.0, (i.e a proportion of the radius from origin) and other variables in the same units */
{
    float disp, depth, offset;
    
    depth = minaxis*sqrt(1-(x*x)); /* d=b*sqrt(1-(x*x)/(a*a)) a=1.0 in radius units*/
    depth *= left_right; 
    disp = (atan(halfiod/(viewdist-depth)) - atan(halfiod/viewdist))*2;
    offset = (viewdist*tan(disp))/2;
    
    return (offset);
}

/*******************************************************************************************/
void draw_dot(float rotatefactor[XY], float hdotsize[XY], float xpos, float ypos, float offset, float standing_disp,  float dotwidth, int mode, Locator *pos, int flag, ball_s *ball)
{
    float dotdata[4][XY];
    float dx,dy,wx,hy,vsr = expt.vals[VSR];
    int j;
    int rnd = ball->lrnd;
    
    /*
     * Quick Fix by BGC July 2001 to make dots wrap around so that disparity
     * can be applied without a background
     * really all these calculations should be done at calc_cyl time for maximum
     * efficiency. But it works.
     * check to see if when the disparity is added, the dot will fall outside
     * the stimulus rectagle. If so, the dot is moved to the other side of the
     * rectanle.
     * N.B. because this is done each time a dot is painted, calc_cyls idea
     * of the dot-position has not changed, so we can't alter the
     * Y co-ordinate here.
     */
    if((flag & FLAT_SURFACES) && (flag & FLAT_DISPARITY)){
        if(mode == JONLEFT){
            dx = ((offset/2) - standing_disp) * rotatefactor[X];
            dy = -((offset/2) - standing_disp) * rotatefactor[Y];
        }
        else{
            dx = -((offset/2) -standing_disp)* rotatefactor[X];
            dy = ((offset/2) - standing_disp) * rotatefactor[Y];
        }
        wx = (pos->imsize[X] - (2 *hdotsize[X]));
        if(xpos+dx > wx/2){
            xpos -=  wx;
            ypos = (ball->lrnd % (int)pos->imsize[Y]) - pos->imsize[Y]/2;
        }
        if(xpos+dx < -wx/2){
            xpos +=  wx;
            ypos = (ball->lrnd % (int)pos->imsize[Y]) - pos->imsize[Y]/2;
        }
        hy = (pos->imsize[Y] - (2 *hdotsize[Y]));
        if(ypos+dy > hy/2)
            ypos -=  hy;
        if(ypos+dy < -hy/2)
            ypos +=  hy;
    }
    
    /*j its - standing cos of a fudge factor in calc cylinder - this was necessary to make same dir sign as other stims*/
    /*j standing disp is already in pixels, half total*/ 
    
    if(mode==JONLEFT){
        xpos = xpos + ((offset/2-standing_disp) * rotatefactor[X]);
        ypos = ypos - ((offset/2-standing_disp) * rotatefactor[Y]);
        ypos = ypos * vsr;
    }
    else{
        xpos = xpos - ((offset/2-standing_disp) * rotatefactor[X]);
        ypos = ypos + ((offset/2-standing_disp) * rotatefactor[Y]);
        ypos = ypos/vsr;
    }    
    
    /*
     if(fabsf(xpos) > pos->imsize[X]/2 || fabsf(ypos) > pos->imsize[Y]/2)
     return;
     */
    if(aamode == 1)
        aarotrect(cylrect,xpos,ypos);
    else if(aamode == 2)
    {
        dotdata[0][X] = dotdata[1][X] = (xpos - hdotsize[X]*dotwidth);  
        dotdata[0][Y] = dotdata[3][Y] = (ypos - hdotsize[Y]);  
        dotdata[1][Y] = dotdata[2][Y] = (ypos + hdotsize[Y]);  
        dotdata[2][X] = dotdata[3][X] = (xpos + hdotsize[X]*dotwidth);  
        myrect(dotdata[0][X],dotdata[0][Y],dotdata[2][X],dotdata[2][Y]);
    }
    else{
        ypos = rint(ypos);
        xpos = rint(xpos);
        rotrect(clrect,xpos,ypos);
    }
}   
/*******************************************************************************************/

void calc_cyl_motion(ball_s *balls, float vel, int ndots, int flag, int lifeframes, float deathchance, int width)
{    
    float delta, fraction, sign, rnd;
    int i;
    for (i = 0; i < ndots; i++) {
        balls[i].proportion = cos(asin(balls[i].pos[X])); /* sinusoidal variation from -1 -> 1 */ 
        if (flag & FLAT_SURFACES) {
		    if (flag & COUNTDOWN){
                if (balls[i].countdown-- <= 0){ 
				    balls[i].pos[X] = (2.0) * (mydrand() - 0.5);	
				    balls[i].pos[Y] = (2.0) * (mydrand() - 0.5);
				    balls[i].countdown = (lifeframes - 1);
                    nreplaced++;
                    
			    }	    
		    }
		    else
                if (mydrand() < deathchance) { 
                    balls[i].pos[X] = (2.0) * (mydrand() - 0.5);
                    balls[i].pos[Y] = (2.0) * (mydrand() - 0.5);
                    nreplaced++;
                }
            
		    delta = vel;
        }
        else {
		    if (flag & COUNTDOWN){
                if (balls[i].countdown-- <= 0){ /* should do comparrison then subtract 1 */
				    balls[i].pos[X] = sinf(M_PI * (mydrand() - 0.5));	
				    balls[i].pos[Y] = (2.0) * (mydrand() - 0.5);
				    balls[i].countdown = (lifeframes - 1);	
			    }	    
		    }
		    else
                if ((rnd = mydrand()) < deathchance) { 
                    balls[i].pos[X] = sinf(M_PI * (mydrand() - 0.5)); /* sin(-pi/2 -> pi/2 */
                    balls[i].pos[Y] = (2.0) * (mydrand() - 0.5);
                    nreplaced++;
                }	
            delta = fabsf(vel * balls[i].proportion);  	/* theta=asin(x)/r, dotvel=cos(theta)) */  
        }
        balls[i].pos[X] += (balls[i].left_right * delta)/width*1000; 		/* adds or subtracts delta */
        
        if ((balls[i].pos[X] >= 1.0) || (balls[i].pos[X] <= -1.0)) {
            fraction = (modff(balls[i].pos[X], &sign));
            balls[i].pos[X] = sign - fraction;
            balls[i].left_right*=-1;		/* swaps LEFT and RIGHT */
            if (flag & NO_WRAP)
                balls[i].pos[Y] = (2.0) * (mydrand() - 0.5);    						     
        }
    }
}

void calc_subpix_disp(ball_s *balls, int numdots, int flag, float disparity, Locator *pos, float widthfactor, float heightfactor)
{
    /*j pix_xy centre of stim from bot left
     dotpospix temp store of individual dots position from in pixels from centre of stimulus
     balls[i].position to nearest pixel position from bot left of bot left corner of dot */
    int pix_xy[XY], i, j;
    float dotpospix, pixs; 
    int numlevels =  SUBPIXLEVELS;  
    /*pos->xy[] contains the screencoordinates of the CENTRE of the stimulus */   
    pix_xy[X] = pos->xy[X]+640;
    pix_xy[Y] = pos->xy[Y]+512;
    
    for (i = 0; i < numdots; i++){
    	dotpospix = balls[i].pos[X]*widthfactor;
        /* now work out disparity */
        /* if negative then take an extra pixel off and use appropriate primitive */
        /* e.g. minus 1.5 is minus 2 pixels, and the middle primitive */
    	if (flag & FLAT_DISPARITY){
		    balls[i].primitive_num[0] = (int)floorf(numlevels * modff( ((dotpospix+disparity) * balls[i].left_right), &pixs)); 		
		    balls[i].rectposition[0] = pixs + pix_xy[X];
		    balls[i].primitive_num[1] = (int)floorf(numlevels * modff( ((dotpospix-disparity) * balls[i].left_right), &pixs)); 		
		    balls[i].rectposition[1] = pixs + pix_xy[X];
		    for(j=0;j<2;j++){
                if (balls[i].primitive_num[j] < 0){
                    balls[i].primitive_num[j] = numlevels + balls[i].primitive_num[j];
                    balls[i].rectposition[j]--;
                }
		    }
        }
        else{   /* the modff frunction gives the fraction part to work out the prim number and the integer part to balls[i].offset both signed */
            balls[i].primitive_num[0] = (int)floorf(numlevels * modff( dotpospix+(disparity * balls[i].proportion * balls[i].left_right), &pixs)); 		
            balls[i].rectposition[0] = pixs + pix_xy[X];
            balls[i].primitive_num[0] = (int)floorf(numlevels * modff( dotpospix-(disparity * balls[i].proportion * balls[i].left_right), &pixs)); 		
            balls[i].rectposition[1] = pixs + pix_xy[X];
    		/*printf("\nprim: %d rawoff: %.3f", balls[i].primitive_num, balls[i].offset);*/
		    for(j=0;j<2;j++){
                if (balls[i].primitive_num[j] < 0){
                    balls[i].primitive_num[j] = numlevels + balls[i].primitive_num[j];
                    balls[i].rectposition[j]--;
                }
		    }
        }
        balls[i].position[Y]=(balls[i].pos[Y]*heightfactor)+pix_xy[Y];
    }	    
}

