/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#include "bool.h"
#include "env.h"
#include "fat32.h"
#include "device.h"

unsigned long size_in_sectors = 0;
size_t bytes_read = 0;
size_t bytes_written = 0;
struct s_device_info* p_device_info = NULL;

bool is_valid_args(int arg_cnt, char* p_args[]) {
     status = __TRUE;

     if(arg_cnt != __DEFAULT_ARGS) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) is_valid_args(): wrong number of arguments\n");
          #endif
          goto _end;
     }

     for(uint8_t i = 0; i < arg_cnt; i++) {
          if(strlen(p_args[i]) == 0) {
               status = __FALSE;
               #ifdef DEBUG
                    fprintf(stderr, "(ERROR) is_valid_args(): argument length cannot be 0 \n");
               #endif
               break;
          }
     }

     if(status == __FALSE) {
          goto _end;
     }

_end:
     return status;
}

bool open_device(int open_mode) {
     status = __TRUE;

     if(!p_device_info) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) open_device(): device information not initialized!\n");
          #endif
          goto _end;
     }

     if(p_device_info->fd_device != INVALID_HANDLE) {
          close(p_device_info->fd_device);
     }

     if((p_device_info->fd_device = open(p_device_info->p_device_path, open_mode)) == INVALID_HANDLE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) open_device(): unable to open device!\n");
          #endif
          goto _end;
     }

_end:
     return status;
}


void close_device(int device_id) {
     if(device_id != __INVALID_HANDLE) {
          close(device_id);
     }
}

bool set_device_path(const char* p_path) {
     status = __TRUE;

    if(!p_path) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) set_device_path(): invalid path\n");
          #endif
          goto _end;
     }

     if(!p_device_info) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) set_device_path(): device information not initialized\n");
          #endif
          goto _end;
     }


     p_device_info->p_device_path = (char*)calloc(1, strlen(p_path) + 1);
     if(!p_device_info->p_device_path) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) set_device_path(): out of memory!\n");
          #endif
          goto _end;
     }
     strcpy(p_device_info->p_device_path, p_path);

_end:
     return status;
}

int  get_device_id() {
     return p_device_info->fd_device;
}

bool get_device_info() {
     status = __TRUE;

     if(!p_device_info) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) get_device_info(): device information not initialized!\n");
          #endif
          goto _end;
     }

     if(!open_device(O_RDONLY)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) get_device_info(): unable to open device!\n");
          #endif
          goto _end;
     }

     if(ioctl(p_device_info->fd_device, HDIO_GETGEO, &p_device_info->geometry)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) get_device_info(): unable to get device geometry information!\n");
          #endif
          goto _end;
     }

     if(ioctl(p_device_info->fd_device, BLKGETSIZE, &p_device_info->size)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) get_device_info(): unable to get device size\n");
          #endif
          goto _end;
     }

_end:
     return status; 
}

void print_device_info() {
     #ifdef DEBUG
          fprintf(stderr, "(INFO) print_device_info(): path         : %s\n", p_device_info->p_device_path);
          fprintf(stderr, "(INFO) print_device_info(): cylinders    : %d\n" , p_device_info->geometry.cylinders);
          fprintf(stderr, "(INFO) print_device_info(): heads        : %d\n" , p_device_info->geometry.heads);
          fprintf(stderr, "(INFO) print_device_info(): sectors      : %d\n" , p_device_info->geometry.sectors);
          fprintf(stderr, "(INFO) print_device_info(): size         : %ld\n", p_device_info->size);
     #endif
}

bool zero_device() {
     status = __TRUE;

     if(!p_device_info) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) zero_device(): device information not initialized!\n");
          #endif
          goto _end;
     }

     if(!open_device(O_WRONLY)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) zero_device(): unable to open device!\n");
          #endif
          goto _end;
     }
     
     uint8_t zero_data[DEFAULT_SECTOR_SIZE];
     memset(zero_data, 0, sizeof(zero_data));
     for(unsigned long i = 0; i < p_device_info->size; i++) {
          bytes_written = write(p_device_info->fd_device, zero_data, DEFAULT_SECTOR_SIZE);
          if(bytes_written != DEFAULT_SECTOR_SIZE) {
               status = __FALSE;
               #ifdef DEBUG
                    fprintf(stderr, "(ERROR) zero_device(): unable to write to device!\n");
               #endif
               goto _end; 
          }
     } 

_end:
     close_device(p_device_info->fd_device);
     return status;
}
