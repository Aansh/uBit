/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#ifndef __BOOL__H
     #define __BOOL__H 0x01

     #ifndef __STDINT__H
          #define __STDINT__H 0x01
          #include <stdint.h>
     #endif

     typedef uint8_t bool;

     #ifndef __STATUS
          #define __STATUS 0x01
          extern bool status;
     #endif

     #ifndef __FALSE
          #define __FALSE 0x00
     #endif

     #ifndef __TRUE
          #define __TRUE 0x01
     #endif

#endif
