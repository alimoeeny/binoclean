


struct sliderdat {
  float min,max;
  int places, code;
  char *label;
  float val;
};
typedef struct sliderdat Sliderdat;


#define POPPED_UP 1
struct FPSLIDER {
  int id;
  float max,min,incr,val;
  int precision,width,index,nprops,code,state;
  int (*proc)(struct FPSLIDER *fp, float val, XEvent *event);
  int (*selproc)(Sliderdat *fpd);
  Widget text_w,scale,minlabel,maxlabel,label,rc,menu,subrc;
  struct FPSLIDER *next,*prev;
  Sliderdat *proplist;
};
typedef struct FPSLIDER Fpslider;



Fpslider *make_fpslider(Widget parent, char *label, float min,
float max, float val, int precision, Widget x, int y, int len,
void (*proc)(struct FPSLIDER *fp, float val, XEvent *event),
int id);

Fpslider *make_multislider(Widget parent, Sliderdat *proplist,
int start, float val,  Widget x, int y, int len,
void (*proc)(struct FPSLIDER *fp, float val, XEvent *event),
void (*selproc)(Sliderdat *fdp),
int id);

void set_fprange(Fpslider *fp, float min, float max, float val, int precision);

int set_fpitemrange(Fpslider *fp, int code, float min, float max);
int set_fpslider(Fpslider *fp, int code, float val);

#define SLIDERBORDER 10
#define NULLPROP {0.0, 0.0, -1, 0, NULL, 0.0}
