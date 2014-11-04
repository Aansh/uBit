/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#ifndef __GDT__H
     #define __GDT__H 0x01

_gdt_start:

_gdt_null:
     .int 0x00000000
     .int 0x00000000

_gdt_code:
     .word 0xffff
     .word 0x0000
     .byte 0x00
     .byte 0x9a
     .byte 0xcf
     .byte 0x00

_gdt_data:
     .word 0xffff
     .word 0x0000
     .byte 0x00
     .byte 0x92
     .byte 0xcf
     .byte 0x00

_gdt_end:

_gdt_descriptor:
     .word _gdt_end - _gdt_start -1
     .int  _gdt_start

#endif
