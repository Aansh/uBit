/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#ifndef DEVICE__H
     #define DEVICE__H 0x01

     #include "reqs.h"
     #include "bool.h"

     #define INVALID_HANDLE -1

     extern size_t bytes_read;
     extern size_t bytes_written;

     struct s_device_info {
          char*    p_device_path;
          int      fd_device;
          struct   hd_geometry geometry;
          unsigned long size;
     };
     extern struct s_device_info* p_device_info;

     bool set_device_path(const char* p_path);
     uint8_t open_device(int open_mode);
     bool get_device_info();
     bool zero_device();
     void close_device(int device_id);
     int  get_device_id();
     void print_device_info();

#endif
