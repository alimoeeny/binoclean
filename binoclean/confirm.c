#include <stdio.h>
#include "misc.h"
#include "myview.h"
#include <stdarg.h>
#define SCREENCHARWIDTH 80
#define MAXMSG 5

int confirmer_up[MAXMSG] = {0};

static int confirm();
static void yes_no();
char *myscopy(),*mysncopy();
static char *msg[MAXMSG] = {0};
static int init = 0;
void response();









