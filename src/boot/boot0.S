/*****************
 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1
 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.
*****************/
.org 0x0000
.code16
.text
.globl _start;
_start:
     jmp _stage1
     nop
     #include <boot_parameter_block.h>
     #include <boot0.h>
     #include <gdt.h>

_stage1:
     INIT_SEGMENTS_16
     INIT_A20_GATE
     INIT_PROT_MODE
_stage2:
     INIT_SEGMENTS_32
     PRINT_32($msgWelcome)
     
/* infinite loop */
_freeze:
     jmp _freeze

/* abnormal termination of program */
_abort:
     jmp _freeze

     /* user-defined variables */
     bootDrive   : .byte 0x00
     msgWelcome  : .asciz "Welcome to uB i t - USB boot Initializer"
     msgAbort    : .asciz "* * * F A T A L  E R R O R * * *"

     /* traverse 510 bytes from beginning */
     . = _start + 0x01fe

     /* append boot signature             */
     .word BOOT_SIGNATURE
