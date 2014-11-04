/*****************

 Author   : Ashakiran Bhatter
 Date     : 11/03/2014(MM/DD/YYYY), 03-Nov-2014(DD-MON-YYYY)
 Version  : 0.0.1

 This file is part of the uBit project, and is made available under
 the terms of the GNU General Public License version 2.

*****************/
#ifndef __FAT32_H
#define __FAT32_H 0x01

/* boot sector details              */
#define FAT32_BOOTSTRAP_CODE       420

/* boot parameter header            */
#define BS_JMPBOOT                   3
#define BS_OEMNAME                   8

/* boot parameter block             */
#define BPB_BYTESPERSEC              2
#define BPB_SECPERCLUS               1
#define BPB_RSVDSECCNT               2
#define BPB_NUMFATS                  1
#define BPB_ROOTENTCNT               2
#define BPB_TOTSEC16                 2
#define BPB_MEDIA                    1
#define BPB_FATSZ16                  2
#define BPB_SECPERTRK                2
#define BPB_NUMHEADS                 2
#define BPB_HIDDSEC                  4
#define BPB_TOTSEC32                 4

/* extended boot parameter block    */
#define BPB_FATSZ32                  4
#define BPB_EXTFLAGS                 2
#define BPB_FSVER                    2
#define BPB_ROOTCLUS                 4
#define BPB_FSINFO                   2
#define BPB_BKBOOTSEC                2
#define BPB_RESERVED                12

/* boot parameter trailer           */
#define BS_DRVNUM                    1
#define BS_RESERVED                  1
#define BS_BOOTSIG                   1
#define BS_VOLID                     4
#define BS_VOLLAB                   11
#define BS_FILSYSTYPE                8

/* root directory details            */
#define ROOT_DIR_NAME                11
#define ROOT_ENTRY_SIZE              32
#define ROOT_DIR_END               0x00
#define ROOT_DIR_KANJI             0x05
#define ROOT_DIR_ENTRY_FREE        0xe5

/* fat tables                        */
#define MAX_FATS                      2

#define DEFAULT_BOOT_LOCATION         0
#define DEFAULT_BOOT_BKP_LOCATION     6
#define DEFAULT_CLUSTER_SIZE          4 
#define DEFAULT_OEM_NAME     "MSDOS5.0"
#define DEFAULT_BOOT_LOCATION         0
#define DEFAULT_SECTOR_SIZE         512
#define DEFAULT_FS_INFO_SECTOR        1
#define DEFAULT_BK_BOOT_SECTOR        6
#define DEFAULT_NUM_FATS              2
#define DEFAULT_ROOT_ENT_CNT          0
#define DEFAULT_TOT_SEC16             0
#define DEFAULT_MEDIA              0xf8
#define DEFAULT_FAT_SZ16              0
#define DEFAULT_HIDD_SEC              0
#define DEFAULT_EXT_FLAGS             0
#define DEFAULT_FS_VER                0
#define DEFAULT_ROOT_CLUS             2
#define DEFAULT_DRV_NUM            0x80
#define DEFAULT_RESERVED1             1
#define DEFAULT_BOOT_SIG           0x29
#define DEFAULT_VOL_ID       0x30b42ab1
#define DEFAULT_VOL_LAB   "uBit       "
#define DEFAULT_FIL_SYS_TYPE "FAT32   "
#define DEFAULT_MAGIC_WORD       0xaa55

/* required structures */
struct __fat_boot_header {
     /*******************************************************************
     * Jump instruction to boot code. This field has two allowed forms: *
     *                                                                  *
     * jmpBoot[0] = 0xEB, jmpBoot[1] = 0x??, jmpBoot[2] = 0x90          *
     * and                                                              *
     * jmpBoot[0] = 0xE9, jmpBoot[1] = 0x??, jmpBoot[2] = 0x??          *
     *                                                                  *
     * 0x?? indicates that any 8-bit value is allowed in that byte.     *
     * What this forms is a three-byte Intel x86 unconditional branch   *
     * (jump) instruction that jumps to the start of the operating      *
     * system bootstrap code. This code typically occupies the rest     *
     * of sector 0 of the volume following the BPB and possibly other   *
     * sectors. Either of these forms is acceptable.                    *
     * JmpBoot[0] = 0xEB is the more frequently used format.            *
     *******************************************************************/
     char jmp_boot[BS_JMPBOOT];
     /*******************************************************************
     * “MSWIN4.1” There are many misconceptions about this field.       *
     * It is only a name string. Microsoft operating systems don’t      *
     * pay any attention to this field. Some FAT drivers do. This       *
     * is the reason that the indicated string, “MSWIN4.1”, is the      *
     * recommended setting, because it is the setting least likely      *
     * to cause compatibility problems. If you want to put something    *
     * else in here, that is your option, but the result may be that    *
     * some FAT drivers might not recognize the volume.                 *
     * Typically this is some indication of what system formatted the   *
     *******************************************************************/
     char oem_name[BS_OEMNAME];
} __attribute__((packed));

struct __fat_boot_parameter_block {
     /*******************************************************************
     * Count of bytes per sector. This value may take on only the       *
     * following values: 512, 1024, 2048 or 4096. If maximum            *
     * compatibility with old implementations is desired, only the      *
     * value 512 should be used. There is a lot of FAT code in the      *
     * world that is basically “hard wired” to 512 bytes per sector and *
     * doesn’t bother to check this field to make sure it is 512.       *
     * Microsoft operating systems will properly support 1024, 2048,    *
     * and 4096.                                                        *
     *                                                                  *
     * Note: Do not misinterpret these statements about maximum         *
     * compatibility. If the media being recorded has a physical sector *
     * size N, you must use N and this must still be less than or equal *
     * to 4096. Maximum compatibility is achieved by only using media   *
     * with specific sector sizes.                                      *
     *******************************************************************/
     uint16_t byts_per_sec;
     /*******************************************************************
     * Number of sectors per allocation unit. This value must be a      *
     * power of 2 that is greater than 0. The legal values are 1, 2, 4, *
     * 8, 16, 32, 64, and 128. Note however, that a value should never  *
     * be used that results in a “bytes per cluster” value              *
     * (BPB_BytsPerSec * BPB_SecPerClus) greater than 32K (32 * 1024).  *
     * There is a misconception that values greater than this are OK.   *
     * Values that cause a cluster size greater than 32K bytes do not   *
     * work properly; do not try to define one. Some versions of some   *
     * systems allow 64K bytes per cluster value. Many application      *
     * setup programs will not work correctly on such a FAT volume.     *
     *******************************************************************/
     uint8_t sec_per_clus;
     /*******************************************************************
     * Number of reserved sectors in the Reserved region of the volume  *
     * starting at the first sector of the volume. This field must not  *
     * be 0. For FAT12 and FAT16 volumes, this value should never be    *
     * anything other than 1. For FAT32 volumes, this value is          *
     * typically 32. There is a lot of FAT code in the world            *
     * “hard wired” to 1 reserved sector for FAT12 and FAT16 volumes    *
     * and that doesn’t bother to check this field to make sure it is 1.*
     *  Microsoft operating systems will properly support any non-zero  *
     * value in this field.                                             *
     *******************************************************************/
     uint16_t rsvd_sec_cnt;
     /*******************************************************************
     * The count of FAT data structures on the volume. This field       *
     * should always contain the value 2 for any FAT volume of any      *
     * type. Although any value greater than or equal to 1 is perfectly *
     * valid, many software programs and a few operating systems’ FAT   *
     * file system drivers may not function properly if the value is    *
     * something other than 2. All Microsoft file system drivers will   *
     * support a value other than 2, but it is still highly recommended *
     * that no value other than 2 be used in this field.                *
     *                                                                  *
     * The reason the standard value for this field is 2 is to provide  *
     * redun-dancy for the FAT data structure so that if a sector goes  *
     * bad in one of the FATs, that data is not lost because it is      *
     * duplicated in the other FAT. On non-disk-based media, such as    *
     * FLASH memory cards, where such redundancy is a useless feature,  *
     * a value of 1 may be used to save the space that a second copy of *
     * the FAT uses, but some FAT file system drivers might not         *
     * recognize such a volume properly.                                *
     *******************************************************************/
     uint8_t num_fats;
     /*******************************************************************
     * For FAT12 and FAT16 volumes, this field contains the count of    *
     * 32-byte directory entries in the root directory. For FAT32       *
     * volumes, this field must be set to 0. For FAT12 and FAT16        *
     * volumes, this value should always specify a count that when      *
     * multiplied by 32 results in an even multiple of BPB_BytsPerSec.  *
     * For maximum compatibility, FAT16 volumes should use the value    *
     * 512.                                                             *
     *******************************************************************/
     uint16_t root_ent_cnt;
     /*******************************************************************
     * This field is the old 16-bit total count of sectors on the       *
     * volume. This count includes the count of all sectors in all four *
     * regions of the volume. This field can be 0; if it is 0, then     *
     * BPB_TotSec32 must be non-zero. For FAT32 volumes, this field     *
     * must be 0. For FAT12 and FAT16 volumes, this field contains the  *
     * sector count, and BPB_TotSec32 is 0 if the total sector count    *
     * “fits” (is less than 0x10000).                                   *
     *******************************************************************/
     uint16_t tot_sec16;
     /*******************************************************************
     * 0xF8 is the standard value for “fixed” (non-removable) media.    *
     * For removable media, 0xF0 is frequently used. The legal values   *
     * for this field are 0xF0, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD,     *
     * 0xFE, and 0xFF. The only other important point is that whatever  *
     * value is put in here must also be put in the low byte of the     *
     * FAT[0] entry. This dates back to the old MS-DOS 1.x media        *
     * determination noted earlier and is no longer usually used for    *
     * anything.                                                        *
     *******************************************************************/
     uint8_t media;
     /*******************************************************************
     * This field is the FAT12/FAT16 16-bit count of sectors occupied   *
     * by ONE FAT. On FAT32 volumes this field must be 0, and           *
     * BPB_FATSz32 contains the FAT size count.                         *
     *******************************************************************/
     uint16_t fat_sz16;
     /*******************************************************************
     * Sectors per track for interrupt 0x13. This field is only         *
     * relevant for media that have a geometry (volume is broken down   *
     * into tracks by multiple heads and cylinders) and are visible on  *
     * interrupt 0x13. This field contains the “sectors per track”      *
     * geometry value.                                                  *
     *******************************************************************/
     uint16_t sec_per_trk;
     /*******************************************************************
     * Number of heads for interrupt 0x13. This field is relevant as    *
     * discussed earlier for BPB_SecPerTrk. This field contains the one *
     * based “count of heads”. For example, on a 1.44 MB 3.5-inch       *
     * floppy drive this value is 2.                                    *
     *******************************************************************/
     uint16_t num_heads;
     /*******************************************************************
     * Count of hidden sectors preceding the partition that contains    *
     * this FAT volume. This field is generally only relevant for media *
     * visible on interrupt 0x13. This field should always be zero on   *
     * media that are not partitioned. Exactly what value is            *
     * appropriate is operating system specific.                        *
     *******************************************************************/
     uint32_t hidd_sec;
     /*******************************************************************
     * This field is the new 32-bit total count of sectors on the       *
     * volume. This count includes the count of all sectors in all four * 
     * regions of the volume. This field can be 0; if it is 0, then     *
     * BPB_TotSec16 must be non-zero. For FAT32 volumes, this field     *
     * must be non-zero. For FAT12/FAT16 volumes, this field contains   *
     * the sector count if BPB_TotSec16 is 0 (count is greater than or  *
     * equal to 0x10000).                                               *
     *******************************************************************/
     uint32_t tot_sec32;
} __attribute__((packed));

struct __fat_extended_boot_parameter_block {
     /*******************************************************************
     * This field is only defined for FAT32 media and does not exist on *
     * FAT12 and FAT16 media. This field is the FAT32 32-bit count of   *
     * sectors occupied by ONE FAT. BPB_FATSz16 must be 0.              *
     *******************************************************************/
     uint32_t fat_sz32;
     /*******************************************************************
     * "This field is only defined for FAT32 media and does not exist   *
     * on FAT12 and FAT16 media.                                        *
     * Bits 0-3 -- Zero-based number of active FAT. Only valid if       *
     * mirroring is disabled.                                           *
     * Bits 4-6 -- Reserved.                                            *
     * Bit      7 -- 0 means the FAT is mirrored at runtime into all    *
     * FATs.                                                            *
     * -- 1 means only one FAT is active; it is the one referenced in   *
     * bits 0-3.                                                        *
     * Bits 8-15  -- Reserved.                                          *
     *******************************************************************/
     uint16_t ext_flags;
     /*******************************************************************
     * This field is only defined for FAT32 media and does not exist on *
     * FAT12 and FAT16 media. High byte is major revision number. Low   *
     * byte is minor revision number. This is the version number of the *
     * FAT32 volume. This supports the ability to extend the FAT32      *
     * media type in the future without worrying about old FAT32        *
     * drivers mounting the volume. This document defines the version   * 
     * to 0:0.  If this field is non-zero, back-level Windows versions  *
     * will not mount the volume.                                       *
     * NOTE:  Disk utilities should respect this field and not operate  *
     * on volumes with a higher major or minor version number than that *
     * for which they were designed. FAT32 file system drivers must     *
     * check this field and not mount the volume if it does not contain *
     * a version number that was defined at the time the driver was     *
     * written.                                                         *
     *******************************************************************/
     uint16_t fs_ver;
     /*******************************************************************
     * This field is only defined for FAT32 media and does not exist on *
     * FAT12 and FAT16 media. This is set to the cluster number of the  *
     * first cluster of the root directory, usually 2 but not required  *
     * to be 2.                                                         *
     * NOTE:  Disk utilities that change the location of the root       *
     * directory should make every effort to place the first cluster of *
     * the root directory in the first non-bad cluster on the drive     *
     * (i.e., in cluster 2, unless it’s marked bad). This is specified  *
     * so that disk repair utilities can easily find the root directory *
     * if this field accidentally gets zeroed.                          *
     *******************************************************************/
     uint32_t root_clus;
     /*******************************************************************
     * This field is only defined for FAT32 media and does not exist   *
     * on FAT12 and FAT16 media. Sector number of FSINFO structure in   *
     * the reserved area of the FAT32 volume. Usually 1.                *
     * NOTE: There will be a copy of the FSINFO structure in            *
     * BackupBoot, but only the copy pointed to by this field will be   *
     * kept up to date (i.e., both the primary and backup boot record   *
     * will point to the same FSINFO sector).                           *
     *******************************************************************/
     uint16_t fs_info;
     /*******************************************************************
     * This field is only defined for FAT32 media and does not exist on *
     * FAT12 and FAT16 media. If non-zero, indicates the sector number  *
     * in the reserved area of the volume of a copy of the boot record. *
     * Usually 6. No value other than 6 is recommended.                 *
     *******************************************************************/
     uint16_t bk_boot_sec;
     /*******************************************************************
     * This field is only defined for FAT32 media and does not exist on *
     * FAT12 and FAT16 media. Reserved for future expansion. Code that  *
     * formats FAT32 volumes should always set all of the bytes of this *
     * field to 0.                                                      *
     *******************************************************************/
     char reserved[BPB_RESERVED];
     /*******************************************************************
     * Int 0x13 drive number (e.g. 0x80). This field supports MS-DOS    *
     * bootstrap and is set to the INT 0x13 drive number of the media   *
     * (0x00 for floppy disks, 0x80 for hard disks).                    *
     * NOTE: This field is actually operating system specific.          *
     * This field has the same definition as it does for FAT12 and      *
     * FAT16 media. The only difference for FAT32 media is that the     *
     * field is at a different offset in the boot sector.               *
     *******************************************************************/
     uint8_t drv_num;
     /*******************************************************************
     * Reserved (used by Windows NT). Code that formats FAT volumes     *
     * should always set this byte to 0.                                *
     * This field has the same definition as it does for FAT12 and      *
     * FAT16 media. The only difference for FAT32 media is that the     *
     * field is at a different offset in the boot sector.               *
     *******************************************************************/
     uint8_t reserved1;
     /*******************************************************************
     * Extended boot signature (0x29). This is a signature byte that    *
     * indicates that the following three fields in the boot sector are *
     * present.                                                         *
     * This field has the same definition as it does for FAT12 and      *
     * FAT16 media. The only difference for FAT32 media is that the     *
     * field is at a different offset in the boot sector.               *
     *******************************************************************/
     uint8_t boot_sig;
     /*******************************************************************
     * Volume serial number. This field, together with BS_VolLab,       *
     * supports volume tracking on removable media. These values allow  *
     * FAT file system drivers to detect that the wrong disk is         *
     * inserted in a removable drive. This ID is usually generated by   *
     * simply combining the current date and time into a 32-bit value.  *
     * This field has the same definition as it does for FAT12 and      *
     * FAT16 media. The only difference for FAT32 media is that the     *
     * field is at a different offset in the boot sector.               *
     *******************************************************************/
     uint32_t vol_id;
     /*******************************************************************
     * Volume label. This field matches the 11-byte volume label        *
     * recorded in the root directory.                                  *
     * NOTE: FAT file system drivers should make sure that they update  *
     * this field when the volume label file in the root directory has  *
     * its name changed or created. The setting for this field when     *
     * there is no volume label is the string “NO NAME    ”.            *
     * This field has the same definition as it does for FAT12 and      *
     * FAT16 media. The only difference for FAT32 media is that the     *
     * field is at a different offset in the boot sector.               *
     *******************************************************************/
     char vol_lab[BS_VOLLAB];
     /*******************************************************************
     * One of the strings “FAT12   ”, “FAT16   ”, or “FAT     ”.        *
     * NOTE: Many people think that the string in this field has        *
     * something to do with the determination of what type of           *
     * FAT—FAT12, FAT16, or FAT32—that the volume has. This is not      *
     * true. You will note from its name that this field is not         *
     * actually part of the BPB. This string is informational only and  *
     * is not used by Microsoft file system drivers to determine FAT    *
     * type because it is frequently not set correctly or is not        *
     * present. See the FAT Type Determination section of this          *
     * document. This string should be set based on the FAT type        *
     * though, because some non-Microsoft FAT file system drivers do    *
     * look at it.                                                      *
     * This field has the same definition as it does for FAT12 and      *
     * FAT16 media. The only difference for FAT32 media is that the     *
     * field is at a different offset in the boot sector.               *
     *******************************************************************/
     char fil_sys_type[BS_FILSYSTYPE];
} __attribute__((packed));

struct __fat_boot_trailer {
     /*******************************************************************
     * Value 0xAA55. This trail signature is used to validate that      *
     * this is in fact a boot sector or not.                            *
     * The value at 510'th byte should be 0x55 and the value at the      *
     * 511'th byte should be 0xaa.                                      *
     *******************************************************************/
     uint16_t boot_signature;
} __attribute__((packed));

struct __boot_record {
     struct __fat_boot_header _boot_header;
     struct __fat_boot_parameter_block _boot_parameter_block;
     struct __fat_extended_boot_parameter_block _extended_boot_parameter_block;
     uint8_t bootstrap_code[FAT32_BOOTSTRAP_CODE];
     struct __fat_boot_trailer _boot_trailer;
} __attribute__((packed));

extern struct __boot_record*     p_boot_record;
extern const char* p_out_file;

bool init_bpb();
bool write_bpb_to_file();
bool write_bpb_info();
bool write_boot_info();

#endif /* !__FAT32_H */
