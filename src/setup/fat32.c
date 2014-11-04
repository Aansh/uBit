/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#include "reqs.h"
#include "bool.h"
#include "env.h"
#include "device.h"
#include "fat32.h"

const char* p_partition_file = "partition.h";
const char* p_bpb_file = "boot_parameter_block.h";

struct __boot_record*     p_boot_record = NULL;
struct __fat_root_entry*  p_root_entry  = NULL;
struct __fat_root_info*   p_root_info   = NULL;
struct __fat_info*        p_fat_info    = NULL;

bool init_bpb() {
     status = __TRUE;

     if(!open_device(O_RDONLY)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): unable to open device '%s'!\n", p_device_info->p_device_path);
          #endif
          goto _end;
     }

     p_boot_record = (struct __boot_record*)calloc(1, sizeof(struct __boot_record));
     if(!p_boot_record) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): out of memory!\n");
          #endif
          goto _end;
     }

     off_t seek_status = lseek(p_device_info->fd_device, DEFAULT_SECTOR_SIZE * DEFAULT_BOOT_LOCATION, SEEK_CUR);
     if(seek_status == INVALID_HANDLE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): unable to seek to boot location!\n");
          #endif
          goto _end;
     }

     bytes_read = read(p_device_info->fd_device, &p_boot_record->_boot_header, sizeof(struct __fat_boot_header));
     if(bytes_read != sizeof(struct __fat_boot_header)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): unable to read boot header!\n");
          #endif
          goto _end;
     }

     bytes_read = read(p_device_info->fd_device, &p_boot_record->_boot_parameter_block, sizeof(struct __fat_boot_parameter_block));
     if(bytes_read != sizeof(struct __fat_boot_parameter_block)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): unable to read boot parameter block!\n");
          #endif
          goto _end;
     }

     bytes_read = read(p_device_info->fd_device, &p_boot_record->_extended_boot_parameter_block, sizeof(struct __fat_extended_boot_parameter_block));
     if(bytes_read != sizeof(struct __fat_extended_boot_parameter_block)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): unable to read extended boot parameter block!\n");
          #endif
          goto _end;
     }

     seek_status = lseek(p_device_info->fd_device, DEFAULT_BOOT_LOCATION * (DEFAULT_SECTOR_SIZE - 2), SEEK_CUR);
     if(seek_status == INVALID_HANDLE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): unable to seek to boot trailer!\n");
          #endif
          goto _end;
     }

     bytes_read = read(p_device_info->fd_device, &p_boot_record->_boot_trailer, sizeof(struct __fat_boot_trailer));
     if(bytes_read != sizeof(struct __fat_boot_trailer)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) init_bpb(): unable to read boot trailer!\n");
          #endif
          goto _end;
     }

     strncpy(p_boot_record->_boot_header.oem_name, "uBit    ", BS_OEMNAME);
_end:
     return status;     
}

bool write_bpb_to_file() {
     status = __TRUE;

     if(p_boot_record) {

          FILE* fout = fopen(p_bpb_file, "w");
          if(fout == NULL) {
               status = __FALSE;
               #ifdef DEBUG
                    fprintf(stderr, "(ERROR) write_bpb_to_file(): unable to create boot parameter block!\n");
               #endif
               goto _end;
          }

          char str_info[50];
          /* boot header */
          sprintf(str_info, "%-13s: %-10s\"%s\"\n", "oem_name", ".ascii", p_boot_record->_boot_header.oem_name);
          fputs(str_info, fout);
          /* boot parameter block */
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "byts_per_sec", ".word", p_boot_record->_boot_parameter_block.byts_per_sec);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%02x\n", "sec_per_clus", ".byte" , p_boot_record->_boot_parameter_block.sec_per_clus);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "rsvd_sec_cnt", ".word" , p_boot_record->_boot_parameter_block.rsvd_sec_cnt);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%02x\n", "num_fats", ".byte" , p_boot_record->_boot_parameter_block.num_fats);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "root_ent_cnt", ".word" , p_boot_record->_boot_parameter_block.root_ent_cnt);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "tot_sec16", ".word" , p_boot_record->_boot_parameter_block.tot_sec16);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%02x\n", "media", ".byte" , p_boot_record->_boot_parameter_block.media);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "fat_sz16", ".word" , p_boot_record->_boot_parameter_block.fat_sz16);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "sec_per_trk", ".word" , p_boot_record->_boot_parameter_block.sec_per_trk);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "num_heads", ".word" , p_boot_record->_boot_parameter_block.num_heads);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%08x\n", "hidd_sec", ".int"  , p_boot_record->_boot_parameter_block.hidd_sec);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%08x\n", "tot_sec32", ".int"  , p_boot_record->_boot_parameter_block.tot_sec32);
          fputs(str_info, fout);

          /* extended boot parameter block */
          sprintf(str_info, "%-13s: %-10s0x%08x\n", "fat_sz32", ".int"  , p_boot_record->_extended_boot_parameter_block.fat_sz32);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "ext_flags", ".word" , p_boot_record->_extended_boot_parameter_block.ext_flags);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "fs_ver", ".word" , p_boot_record->_extended_boot_parameter_block.fs_ver);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%08x\n", "root_clus", ".int"  , p_boot_record->_extended_boot_parameter_block.root_clus);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "fs_info", ".word" , p_boot_record->_extended_boot_parameter_block.fs_info);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%04x\n", "bk_boot_sec", ".word" , p_boot_record->_extended_boot_parameter_block.bk_boot_sec);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s"        , "reserved", ".byte");
          fputs(str_info, fout);
          for(uint8_t i = 0; i < BPB_RESERVED; i++) {
               sprintf(str_info, "0x%02x", '\0');
               fputs(str_info, fout);
               if(i < BPB_RESERVED - 1) {
                    sprintf(str_info, ",");
                    fputs(str_info, fout);
               }
          }
          sprintf(str_info, "\n");
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%02x\n", "drv_num", ".byte" , p_boot_record->_extended_boot_parameter_block.drv_num);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%02x\n", "reserved1", ".byte" , p_boot_record->_extended_boot_parameter_block.reserved1);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%02x\n", "ext_boot_sig", ".byte" , p_boot_record->_extended_boot_parameter_block.boot_sig);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s0x%08x\n", "vol_id", ".int"  , p_boot_record->_extended_boot_parameter_block.vol_id);
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s", "vol_lab", ".ascii");
          fputs(str_info, fout);
          sprintf(str_info, "\"");
          fputs(str_info, fout);
          for(uint8_t i = 0; i < BS_VOLLAB; i++) {
               sprintf(str_info, "%c", p_boot_record->_extended_boot_parameter_block.vol_lab[i]);
               fputs(str_info, fout);
          } 
          sprintf(str_info, "\"\n");
          fputs(str_info, fout);
          sprintf(str_info, "%-13s: %-10s", "fil_sys_type", ".ascii");
          fputs(str_info, fout);
          sprintf(str_info, "\"");
          fputs(str_info, fout);
          for(uint8_t i = 0; i < BS_FILSYSTYPE; i++) {
               sprintf(str_info, "%c", p_boot_record->_extended_boot_parameter_block.fil_sys_type[i]);
               fputs(str_info, fout);
          }
          sprintf(str_info, "\"\n");
          fputs(str_info, fout);

          if(fout != NULL) {
               fclose(fout);
          }                  
     } else {
          status = __FALSE;
     }

_end:
     return status;
}

bool write_bpb_info() {
     status = __TRUE;

     if(!init_bpb()) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to initialize boot parameter block!\n");
          #endif
          goto _end;
     }     

     if(!write_bpb_to_file()) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to write boot parameter block!\n");
          #endif
          goto _end;
     }

_end:
     return status;
}

bool write_boot_info() {
     status = __TRUE;

     if(!open_device(O_WRONLY)) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to open device!\n");
          #endif
          goto _end;
     }

     p_boot_record = (struct __boot_record*)calloc(1, sizeof(struct __boot_record));
     if(!p_boot_record) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): out of memory!\n");
          #endif
          goto _end;
     }

     int fd_boot_slave1 = open("..//bin//boot0", O_RDONLY);
     if(fd_boot_slave1 == INVALID_HANDLE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to open boot loader!\n");
          #endif
          goto _end;
     }

     bytes_read = read(fd_boot_slave1, p_boot_record, DEFAULT_SECTOR_SIZE);
     if(bytes_read != DEFAULT_SECTOR_SIZE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to read boot record!\n");
          #endif
          goto _end;
     }
     close_device(fd_boot_slave1);


     off_t seek_status = lseek(p_device_info->fd_device, (DEFAULT_SECTOR_SIZE * DEFAULT_BOOT_LOCATION)
                                                         , SEEK_CUR);
     if(seek_status == INVALID_HANDLE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to seek to boot location!\n");
          #endif
          goto _end;
     }

     bytes_written = write(p_device_info->fd_device, p_boot_record, DEFAULT_SECTOR_SIZE);
     if(bytes_written != DEFAULT_SECTOR_SIZE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to write boot info!\n");
          #endif
          goto _end;
     }

     seek_status = lseek(p_device_info->fd_device, (DEFAULT_SECTOR_SIZE * DEFAULT_BOOT_BKP_LOCATION)
                                                         , SEEK_SET);
     if(seek_status == INVALID_HANDLE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to seek to boot record backup location!\n");
          #endif
          goto _end;
     }

     bytes_written = write(p_device_info->fd_device, p_boot_record, DEFAULT_SECTOR_SIZE);
     if(bytes_written != DEFAULT_SECTOR_SIZE) {
          status = __FALSE;
          #ifdef DEBUG
               fprintf(stderr, "(ERROR) write_bpb_info(): unable to write boot info to backup boot record backup location!\n");
          #endif
          goto _end;
     }



_end:
     if(p_boot_record) {
          free(p_boot_record);
          p_boot_record = NULL;
     }
     close_device(p_device_info->fd_device);

     return status;
}
