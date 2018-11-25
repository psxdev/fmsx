

fMSX for PlayStation 4
======================
 
 What does this do?
===================
 
  fMSX is an emulator made by Marat Fayzullin. It is not open source and don't let commercial use of it but it is easy to port to many devices. So respect his license condition. 
  
  You can get all information about it at:
  
  https://fms.komkon.org/fMSX/
  
  
  
  What do you need to compile it?
==================

 1) You will need ps4sdk fork branch firmware505 and liborbis from my own repository

  Follow steps in readme
  ```
  git clone https://github.com/orbisdev/liborbis
  
  ```
  
  
 2) Compile fmsx
  
  
  ```
  git clone https://github.com/psxdev/fmsx
  cd fmsx
  make
  ```
  copy bin/msxorbis.elf to ps4sh/bin folder or where you are executing ps4sh binary

 3) You will need ps4sh because all is loaded from host using IV0002-NPXS29041_00-MSXORBIS00000000.pkg look for it or make your own using code from https://gist.github.com/psxdev/a7cf37729986039a2283c00aa454ebb8
 	
  If you have a folder where ps4sh binary is located, please create there:
  ```
  mkdir system
  mkdir GAMES
  ```
  
  Put your msx system rom and  CARTS.SHA and png files from system directoty from this repository on your new system folder
  ```
  -rw-r--r--@ 1 bigboss  staff   16384 24 oct 23:16 DISK.ROM
  -rw-r--r--@ 1 bigboss  staff  131072 22 oct 00:45 KANJI.ROM
  -rw-r--r--@ 1 bigboss  staff   32768 15 oct 00:32 MSX.ROM
  -rw-r--r--@ 1 bigboss  staff   32768 23 dic 13:30 MSX2.ROM
  -rw-r--r--@ 1 bigboss  staff   16384 23 dic 13:30 MSX2EXT.ROM
  -rw-r--r--@ 1 bigboss  staff   32768 15 oct 00:32 MSX2P.ROM
  -rw-r--r--@ 1 bigboss  staff   16384 15 oct 00:32 MSX2PEXT.ROM
  -rw-r--r--  1 bigboss  staff   33152 25 nov 18:38 CARTS.SHA
  -rw-r--r--@ 1 bigboss  staff   48679 25 nov 18:25 browser.png
  -rw-r--r--@ 1 bigboss  staff  142695 25 nov 18:25 credits.png
  -rw-r--r--@ 1 bigboss  staff    1932 25 nov 18:59 file_icon.png
  -rw-r--r--@ 1 bigboss  staff     199 25 nov 18:59 folder_icon.png
  -rw-r--r--@ 1 bigboss  staff   69966 25 nov 18:27 settings.png
  ```
  Put your .ROM .DSK .CAS games on GAMES
  
  
  
 4) Before execute msxorbis.elf you need ps4sh running on directory where system and GAMES folders are created. Load msxorbis pkg and you will see logs in your ps4sh tool
 
 After you see msxorbis splash screen execute connect command on ps4sh
  
 ```
 bigmini:bin bigboss$ ./ps4sh
 ps4sh version 1.0
 /Users/bigboss/.ps4shrc: No such file or directory

 log: [HOST][INFO]: [PS4SH] Ready
 log: [PS4][INFO]: debugnet initialized
 log: [PS4][INFO]: Copyright (C) 2010,2016 Antonio Jose Ramos Marquez aka bigboss @psxdev
 log: [PS4][INFO]: ready to have a lot of fun...
 ps4sh> connect
 log: [HOST][INFO]: [PS4SH] Connecting to fio ps4link ip 192.168.1.17
 log: [HOST][INFO]: [PS4SH] PlayStation is listening at 192.168.1.17
 log: [HOST][DEBUG]: [PS4SH] Opening system/fmsxtexture.png flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return 7
 log: [HOST][DEBUG]: [PS4SH] read 8 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 13 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 4 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 9 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 4 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 310 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 4 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 17974 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 4 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 32 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 4 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 8192 bytes of file descritor 7
 log: [HOST][DEBUG]: [PS4SH] read 3708 bytes of file descritor 7
 log: [PS4][INFO]: Allocating 16kB for empty space...
 log: [PS4][INFO]: Reset MSX with mode 30015, rampages 32 , vrampages 4
 log: [HOST][DEBUG]: [PS4SH] Opening system/MSX2.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return 7
 log: [PS4][INFO]:   Opening MSX2.ROM...[PS4][INFO]: Opening rom file host0:system/MSX2.ROM return 7
 log: [HOST][DEBUG]: [PS4SH] read 32768 bytes of file descritor 7
 log: [PS4][INFO]: OK
 log: [HOST][DEBUG]: [PS4SH] Opening system/MSX2EXT.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return 7
 log: [PS4][INFO]:   Opening MSX2EXT.ROM...[PS4][INFO]: Opening rom file host0:system/MSX2EXT.ROM return 7
 log: [HOST][DEBUG]: [PS4SH] read 16384 bytes of file descritor 7
 log: [PS4][INFO]: OK
 log: [PS4][INFO]:   Patching BIOS: [PS4][INFO]: 00E1..[PS4][INFO]: 00E4..[PS4][INFO]: 00E7..[PS4][INFO]: 00EA..[PS4][INFO]: 00ED..[PS4][INFO]: 00F0..[PS4][INFO]: 00F3..[PS4][INFO]: OK
 log: [HOST][DEBUG]: [PS4SH] Opening system/DISK.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return 7
 log: [PS4][INFO]:   Opening DISK.ROM...[PS4][INFO]: Opening rom file host0:system/DISK.ROM return 7
 log: [HOST][DEBUG]: [PS4SH] read 16384 bytes of file descritor 7
 log: [PS4][INFO]: OK
 log: [PS4][INFO]: Allocating 512kB for RAM...[PS4][INFO]: OK
 log: [PS4][INFO]: Allocating 128kB for VRAM...[PS4][INFO]: OK
 log: [PS4][INFO]: jaaaaaarrarr 7
 log: [HOST][DEBUG]: [PS4SH] Opening DEFAULT.FNT flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Loading host0:DEFAULT.FNT font...[PS4][INFO]: FAILED
 log: [HOST][DEBUG]: [PS4SH] Opening system/CMOS.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Loading optional ROMs: [PS4][INFO]: Opening rom file host0:system/CMOS.ROM return -1
 log: [HOST][DEBUG]: [PS4SH] Opening system/KANJI.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return 7
 log: [PS4][INFO]: Opening rom file host0:system/KANJI.ROM return 7
 log: [HOST][DEBUG]: [PS4SH] Opening system/RS232.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: KANJI.ROM..[PS4][INFO]: Opening rom file host0:system/RS232.ROM return -1
 log: [PS4][INFO]: OK
 log: [PS4][INFO]: In LoadCart name host0:system/PAINTER.ROM slot 2 type 0
 log: [HOST][DEBUG]: [PS4SH] Opening system/PAINTER.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Open cart file host0:system/PAINTER.ROM return -1
 log: [PS4][INFO]: In LoadCart name host0:system/FMPAC.ROM slot 2 type 7
 log: [HOST][DEBUG]: [PS4SH] Opening system/FMPAC.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Open cart file host0:system/FMPAC.ROM return -1
 log: [PS4][INFO]: In LoadCart name host0:system/GMASTER2.ROM slot 2 type 6
 log: [HOST][DEBUG]: [PS4SH] Opening system/GMASTER2.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Open cart file host0:system/GMASTER2.ROM return -1
 log: [PS4][INFO]: In LoadCart name host0:system/GMASTER.ROM slot 2 type 0
 log: [HOST][DEBUG]: [PS4SH] Opening system/GMASTER.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Open cart file host0:system/GMASTER.ROM return -1
 log: [PS4][INFO]: In LoadCart name host0:CARTA.ROM slot 0 type 65536
 log: [HOST][DEBUG]: [PS4SH] Opening CARTA.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Open cart file host0:CARTA.ROM return -1
 log: [PS4][INFO]: In LoadCart name host0:CARTB.ROM slot 1 type 131072
 log: [HOST][DEBUG]: [PS4SH] Opening CARTB.ROM flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Open cart file host0:CARTB.ROM return -1
 log: [PS4][INFO]: Redirecting printer output to STDOUT...OK
 log: [HOST][DEBUG]: [PS4SH] Opening DRIVEA.DSK flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [HOST][DEBUG]: [PS4SH] Opening DRIVEB.DSK flags 0
 log: [HOST][DEBUG]: [PS4SH] Open return -1
 log: [PS4][INFO]: Initializing VDP, FDC, PSG, OPLL, SCC, and CPU...
 log: [PS4][INFO]:   Attached normal joystick to joystick port A
 log: [PS4][INFO]:   Attached nothing to joystick port B
 log: [PS4][INFO]:   228 CPU cycles per HBlank
 log: [PS4][INFO]:   71364 CPU cycles per VBlank
 log: [PS4][INFO]:   313 scanlines
 log: [PS4][INFO]: RUNNING ROM CODE...
 ```
 
 5) ready to have a lot of fun :P 
 
 MSX2 is loaded by default, if you has bluetooth keyboard you can write your basic programs :p
 
 Press L2+R2 to go to menu where you can browser in your pc/mac folders to choose a ROM or DSK or CAS press X 
 
 ![alt text](https://raw.github.com/psxdev/fmsx/master/system/browser.png "msxorbis settings")
 
 You can go to settings pressing triangle 
 
 You can choose MSX MSX2 and MSX2+ and PAL or NTSC mode, pressing X 
 
 ![alt text](https://raw.github.com/psxdev/fmsx/master/system/settings.png "msxorbis settings")
 
 Red power button let you reset the msx system with new settings.
 
 6) I wait that this can help to developers really interested in homebrew if you want piracy this is not for you. Respect the licenses from fMSX part please.


  Change log
===========================
 - 12/10/2018 msxorbis code for 5th MSX RU Sevilla 2018
 - 25/12/2017 Initial release
 - 23/06/2018 5.05 support
 

  Credits
===========================
  
  ![alt text](https://raw.github.com/psxdev/fmsx/master/system/credits.png "msxorbis credits")
  
  Special thanks goes to:
  
 