/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#ifndef __ENV_H
#define __ENV_H 0x01

#define NULL_TERM 1

#include "bool.h"

#define DEFAULT_ARGS        3
#define WRITE_BPB_INFO      1 
#define WRITE_BOOT_INFO     2 

bool init_env(int argc, char* argv[]);

#endif
