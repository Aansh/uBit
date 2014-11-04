A utility program that allows a USB drive to be formatted to FAT32 File System and make it bootable.





Features:

     
      * This is an initial draft of the bootloader that can fit in USB drives of different sizes.

     
      * Just prints the message "Welcome to u B i t - USB boot Intializer" onto the screen in protected mode






How to run:


1. Insert the usb drive into your system


2. make a clear note of the usb path


3. goto src directory, where you can find the Makefile


4. search for USB_PATH keyword(exact)


5. replace the string /dev/sdb with your device path


6. after saving just run the below command
   make -f Makefile build


7. restart your machine and boot from USB device to see the result.



cheers :)



Dependencies:

1. Operating System: Linux

2. Need GNU BIN Utilities to compile the code
