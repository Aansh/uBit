/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#include "reqs.h"
#include "env.h"
#include "bool.h"
#include "user.h"
#include "device.h"

bool init_env(int argc, char* argv[]) {
      status = __TRUE;

      if(argc != DEFAULT_ARGS) {
           status = __FALSE;
           #ifdef DEBUG
                fprintf(stderr, "(ERROR) init_env(): Usage %s <device path> <%d(create boot parameter block) OR %d(write boot information)>\n", 
                                 argv[0], WRITE_BPB_INFO, WRITE_BOOT_INFO);
           #endif
           goto _end;
      }

      p_device_info = (struct s_device_info*)calloc(1, sizeof(struct s_device_info));
      if(!p_device_info) {
           status = __FALSE;
           #ifdef DEBUG
                fprintf(stderr, "(ERROR) init_env(): out of memory\n");
           #endif
           goto _end;
      }

      if(!set_device_path(argv[1])) {
           status = __FALSE;
           #ifdef DEBUG
                fprintf(stderr, "(ERROR) init_env(): unable to set device path!\n");
           #endif
           goto _end;
      }

      if(!set_user_choice(atoi(argv[2]))) {
           status = __FALSE;
           #ifdef DEBUG
                fprintf(stderr, "(ERROR) init_env(): unable to set user choice\n");
           #endif
           goto _end;
      }

_end:
      return status;
}
