#ifndef HANDLERS
#define HANDLERS

/* IMPORTS */

#include <stdio.h>
#include <stdlib.h>
#include "../scellutils/constructs/boolean.h"
#include "../scellutils/constructs/sh_state.h"

/* EXTERNS */

extern char* sh_last_command;

/* FUNCTIONS */

SH_STATE arguments(int a);
SH_STATE init(int first, int second);
SH_STATE sub(int first, int second);
SH_STATE quit();

#endif