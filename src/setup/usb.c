/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#include "bool.h"
#include "reqs.h"
#include "user.h"
#include "env.h"
#include "device.h"
#include "fat32.h"

int main(int argc, char* argv[]) {
     status = __TRUE;

     if(!init_env(argc, argv)) {
          status = __FALSE;
          fprintf(stderr, "(ERROR) could not initialize the environment!\n");
          goto _end;
     }

     if(!get_device_info(argv[1])) {
          status = __FALSE;
          fprintf(stderr, "(ERROR) could not get device information!\n");
          goto _end;
     }

     switch(user_choice) {
          case WRITE_BPB_INFO:
          if(!write_bpb_info()) {
               status = __FALSE;
               fprintf(stderr, "(ERROR) could not write volume boot parameter block information to file!\n");
               goto _end;
          }
          break;

          case WRITE_BOOT_INFO:
          if(!write_boot_info()) {
               status = __FALSE;
               fprintf(stderr, "(ERROR) could not write volume boot parameter block information to USB!\n");
               goto _end;
          }
          break;

          default:
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) wrong choice %d\n", user_choice);
          #endif
          goto _end;
          break;
     }

_end:
     close_device(p_device_info->fd_device);
     return 0;
}
