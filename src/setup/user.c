/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#include "reqs.h"
#include "bool.h"
#include "user.h"

int user_choice = 0;

uint8_t set_user_choice(int choice) {
     status = __TRUE;

     if(!choice) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) set_user_choice(): user choice cannot be 0!\n");
          #endif
          goto _end;
     }

     user_choice = choice;
_end:
     return status;
}
