#/*****************
#
# Author   : Ashakiran Bhatter
# Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
# Version  : 0.0.1
#
# This file is part of the uBit project, and is made available under
# the terms of the GNU General Public License version 2.
#
#*****************/
.SILENT:
ROOT_DIR                 =    ..
SRC_DIR                  =    $(ROOT_DIR)/src
LOG_DIR                  =    $(ROOT_DIR)/log
ARC_DIR                  =    $(ROOT_DIR)/arc
BIN_DIR                  =    $(ROOT_DIR)/bin
ISO_DIR                  =    $(ROOT_DIR)/iso
INC_DIR                  =    $(SRC_DIR)/inc
BOOT_DIR                 =    $(SRC_DIR)/boot
KERNEL_DIR               =    $(SRC_DIR)/kernel
SETUP_DIR                =    $(SRC_DIR)/setup
BOOT_SEGMENT             =    0x7c00
SECTOR_SIZE              =    0x0200
FILE_FORMAT              =    binary
HD_SRC_EXT               =    h
AS_SRC_EXT               =    S
AS_OBJ_EXT               =    o
CS_SRC_EXT               =    c
CS_OBJ_EXT               =    o
CCOMPILER                =    gcc
COMPFLAGS                =    -g -D=DEBUG -c -Wall -Werror -std=c99 -I$(INC_DIR)
#COMPFLAGS               =    -c -Wall -Werror -std=c99 -I$(INC_DIR)
LINKER                   =    ld
LINKFLAGS                =    -Ttext=$(BOOT_SEGMENT) --oformat=$(FILE_FORMAT)
COPYCOMM                 =    dd
COPYFLAGS                =    bs=$(SECTOR_SIZE) count=1
COMM_DEL                 =    rm
DEL_FLAGS                =    -rf
BPB_HEADER               =    "boot_parameter_block".$(HD_SRC_EXT)
USB_PATH                 =    /dev/sdb
WRITE_BPB_INFO           =    1 
WRITE_BOOT_INFO          =    2 

header:
	@clear	
	@echo "***********************************************************************"
	@echo "*                                                                     *"
	@echo "*                Name   : Makefile                                    *"
	@echo "*                Date   : 11/03/2014                                  *"
	@echo "*                Author : Ashakiran Bhatter                           *"
	@echo "*                Version: 0.0.1                                       *"
	@echo "*                Usage  : make -f Makefile build                      *"
	@echo "*                                                                     *"
	@echo "***********************************************************************"

clean_init:
	@echo "\t(INFO ) cleaning in progress................please wait"
	$(COMM_DEL) $(DEL_FLAGS) $(LOG_DIR)/*
	$(COMM_DEL) $(DEL_FLAGS) $(BIN_DIR)/*
	$(COMM_DEL) $(DEL_FLAGS) $(ISO_DIR)/*
	$(COMM_DEL) $(DEL_FLAGS) $(SRC_DIR)/$(BPB_HEADER)
	$(COMM_DEL) $(DEL_FLAGS) $(INC_DIR)/$(BPB_HEADER)
	@echo "\t                                                   done"

reqs:
	@echo "\t(INFO ) building module reqs................please wait"
	$(CCOMPILER) $(COMPFLAGS) $(SETUP_DIR)/$@.$(CS_SRC_EXT) -o $(BIN_DIR)/$@.$(CS_OBJ_EXT)
	@echo "\t                                                   done"

bool:
	@echo "\t(INFO ) building module bool................please wait"
	$(CCOMPILER) $(COMPFLAGS) $(SETUP_DIR)/$@.$(CS_SRC_EXT) -o $(BIN_DIR)/$@.$(CS_OBJ_EXT)
	@echo "\t                                                   done"

user:
	@echo "\t(INFO ) building module user................please wait"
	$(CCOMPILER) $(COMPFLAGS) $(SETUP_DIR)/$@.$(CS_SRC_EXT) -o $(BIN_DIR)/$@.$(CS_OBJ_EXT)
	@echo "\t                                                   done"

env:
	@echo "\t(INFO ) building module env.................please wait"
	$(CCOMPILER) $(COMPFLAGS) $(SETUP_DIR)/$@.$(CS_SRC_EXT) -o $(BIN_DIR)/$@.$(CS_OBJ_EXT)
	@echo "\t                                                   done"

fat32:
	@echo "\t(INFO ) building module fat32...............please wait"
	$(CCOMPILER) $(COMPFLAGS) $(SETUP_DIR)/$@.$(CS_SRC_EXT) -o $(BIN_DIR)/$@.$(CS_OBJ_EXT)
	@echo "\t                                                   done"

device:
	@echo "\t(INFO ) building module device..............please wait"
	$(CCOMPILER) $(COMPFLAGS) $(SETUP_DIR)/$@.$(CS_SRC_EXT) -o $(BIN_DIR)/$@.$(CS_OBJ_EXT)
	@echo "\t                                                   done"

usb:
	@echo "\t(INFO ) building module usb.................please wait"
	$(CCOMPILER) $(COMPFLAGS) $(SETUP_DIR)/$@.$(CS_SRC_EXT) -o $(BIN_DIR)/$@.$(CS_OBJ_EXT)
	@echo "\t                                                   done"

uBit:
	@echo "\t(INFO ) building module uBit................please wait"
	$(CCOMPILER) $(BIN_DIR)/reqs.o $(BIN_DIR)/user.o $(BIN_DIR)/env.o $(BIN_DIR)/bool.o $(BIN_DIR)/device.o $(BIN_DIR)/fat32.o $(BIN_DIR)/usb.o -o $(BIN_DIR)/$@
	@echo "\t                                                   done"

write_fat_info:
	@echo "\t(INFO ) building file system................please wait"
	@mkdosfs -I -F 32 $(USB_PATH) 1>/dev/null
	@echo "\t                                                   done"

write_bpb_info:
	@echo "\t(INFO ) building bpb info...................please wait"
	$(BIN_DIR)/uBit $(USB_PATH) $(WRITE_BPB_INFO)
	@cp $(SRC_DIR)/$(BPB_HEADER) $(INC_DIR)
	@echo "\t                                                   done"

boot0:
	@echo "\t(INFO ) building boot loader................please wait"
	$(CCOMPILER) $(COMPFLAGS) $(BOOT_DIR)/$@.$(AS_SRC_EXT) -o $(BIN_DIR)/$@.$(AS_OBJ_EXT)
	$(LINKER) $(LINKFLAGS) $(BIN_DIR)/$@.$(AS_OBJ_EXT) -o $(BIN_DIR)/$@
	@echo "\t                                                   done"

write_boot_info:
	@echo "\t(INFO ) writing boot info...................please wait"
	$(BIN_DIR)/uBit $(USB_PATH) $(WRITE_BOOT_INFO)
	@echo "\t                                                   done"

clean_exit:
	@echo "\t(INFO ) cleaning in progress................please wait"
	$(COMM_DEL) $(DEL_FLAGS) $(BIN_DIR)/*.$(AS_OBJ_EXT)
	$(COMM_DEL) $(DEL_FLAGS) $(BIN_DIR)/*.$(CS_OBJ_EXT)
	$(COMM_DEL) $(DEL_FLAGS) $(SRC_DIR)/$(BPB_HEADER)
	$(COMM_DEL) $(DEL_FLAGS) $(INC_DIR)/$(BPB_HEADER)
	@echo "\t                                                   done"

status:
	@echo "\t(INFO ) Congrats!!!                                    "
	@echo "\t(INFO ) Now, go ahead and restart your machine         "
	@echo "\t(INFO ) Remeber to boot from your USB drive            "
	@echo "\t(INFO ) cheers :)                                      "
	

build:
	@make header
	@make clean_init
	@make reqs
	@make bool
	@make env
	@make fat32
	@make device
	@make user
	@make usb
	@make uBit
	@make write_fat_info
	@make write_bpb_info
	@make boot0 
	@make write_boot_info
	@make clean_exit
	@make status
