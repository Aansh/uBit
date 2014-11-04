/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#ifndef __BOOT0__H
     #define __BOOT0__H 0x01

#define SECTOR_SIZE    0x0200
#define BOOT_SECTOR    0x0001
#define BOOT_SEGMENT   0x7c00
#define BOOT_SIGNATURE 0xaa55

#define PRINT(x)       \
     pushw x;          \
     call _printString;\
     addw $0x02, %sp;  \

#define INIT_SEGMENTS_16             \
     /* initialize the environment */\
     movb %dl          , bootDrive;  \
     movw %cs          , %ax;        \
     movw %ax          , %ds;        \
     movw %ax          , %es;        \
     movw %ax          , %fs;        \
     movw %ax          , %gs;        \
     /* Main intension is to allocate 4096 bytes or 4(kb)to stack     */\
     /* after the boot sector by following the below steps            */\
     /* 1. set the stack segment address = 0x7c00 + (0x0200 * 1)      */\
     /*    so stack segment address = 0x7e00                          */\
     /* 2. set the stack pointer to 4096 bytes after the boot sector  */\
     /*    i.e., stack pointer = 0x7e00 + 0x1000 = 0x8e00             */\
     /*                                                               */\
     movw $SECTOR_SIZE     , %ax;    \
     movw $BOOT_SECTOR     , %bx;    \
     mulw %bx;                       \
     addw $BOOT_SEGMENT    , %ax;    \
     /* clear the interrupts before setting the stack                  */\
     cli;                            \
     /* stack grows downwards                                          */\
     /* set the lower bound address 0x7e00 to stack segment            */\
     movw %ax    , %ss;              \
     /* set the upper bound address to 4096 bytes as the stack pointer */\
     movw $0x1000, %sp;              \
     sti;                            \

#define INIT_A20_GATE  \
     cli;              \
1:                     \
     inb $0x64, %al;   \
     testb $0x02, %al; \
     jnz 1b;           \
                       \
     movb $0xd1, %al;  \
     outb %al, $0x64;  \
1:                     \
     inb $0x64, %al;   \
     testb $0x02, %al; \
     jnz  1b;          \
                       \
     movb $0xdf, %al;  \
     outb %al, $0x60;  \
                       \
     sti;              \

#define INIT_PROT_MODE      \
     /* install gdt */      \
     cli;                   \
     pusha;                 \
     lgdt _gdt_descriptor;  \
     sti;                   \
     popa;                  \
                            \
     cli;                   \
     mov %cr0, %eax;        \
     or  $0x01, %eax;       \
     mov %eax, %cr0;        \
                            \
     ljmp $(_gdt_code - _gdt_start), $_stage2;\

#define INIT_SEGMENTS_32    \
     .code32;               \
     movw $0x10, %ax;       \
     movw %ax  , %ds;       \
     movw %ax  , %es;       \
     movw %ax  , %fs;       \
     movw %ax  , %gs;       \
     movw %ax  , %ss;       \

#define PRINT_32(x)\
     movl $0xb8000, %edx;\
     movl x, %ebx;\
_print_32_in:\
     movb (%ebx), %al; \
     cmpb $0x00 , %al; \
     je _print_32_out; \
     movb $0x0f , %ah; \
     movl %eax  , (%edx);\
     addl $0x01 , %ebx;\
     addl $0x02 , %edx;\
     jmp _print_32_in; \
_print_32_out:\

#endif
