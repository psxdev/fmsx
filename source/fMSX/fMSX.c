/** fMSX: portable MSX emulator ******************************/
/**                                                         **/
/**                          fMSX.c                         **/
/**                                                         **/
/** This file contains generic main() procedure statrting   **/
/** the emulation.                                          **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1994-2017                 **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/

#include "MSX.h"
#include "Help.h"
#include "EMULib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static const char *Options[]=
{ 
  "verbose","skip","pal","ntsc","help",
  "printer","serial","diska","diskb","tape","font","logsnd","state",
  "ram","vram","rom","auto","noauto","msx1","msx2","msx2+","joy",
  "home","simbdos","wd1793","sound","nosound","trap","sync","nosync",
  "scale","static","nostatic","vsync","480","200",
  0
};

extern const char *Title;/* Program title                       */
extern int   UseSound;   /* Sound mode                          */
extern int   UseZoom;    /* Zoom factor (#ifdef UNIX)           */
extern int   UseEffects; /* EFF_* bits, ORed (UNIX/MAEMO/MSDOS) */
extern int   UseStatic;  /* Use static colors (#ifdef MSDOS)    */
extern int   FullScreen; /* Use 640x480 screen (#ifdef MSDOS)   */
extern int   SyncFreq;   /* Sync scr updates (UNIX/MAEMO/MSDOS) */

/** Zero-terminated arrays of disk names for each drive ******/
extern const char *Disks[2][MAXDISKS+1];

/** main() ***************************************************/
/** This is a main() function used in Unix and MSDOS ports. **/
/** It parses command line arguments, sets emulation        **/
/** parameters, and passes control to the emulation itself. **/
/*************************************************************/
int fmsxInit()
{
  int CartCount,TypeCount;
  int JoyCount,DiskCount[2];
  int N,J;




  Verbose=1;
  SETJOYTYPE(0,1&0x03);

  /* Clear everything */
  CartCount=TypeCount=JoyCount=0;
  DiskCount[0]=DiskCount[1]=0;

  /* Default disk images */
  Disks[0][1]=Disks[1][1]=0;
  Disks[0][0]=DSKName[0];
  Disks[1][0]=DSKName[1];



  /* Terminate disk lists and set initial disk names */
  if(DiskCount[0]) { Disks[0][DiskCount[0]]=0;DSKName[0]=Disks[0][0]; }
  if(DiskCount[1]) { Disks[1][DiskCount[1]]=0;DSKName[1]=Disks[1][0]; }

  /* Start fMSX! */
  if(!InitMachine()) return(1);  
  StartMSX(Mode,32,4);
  TrashMSX();
  TrashMachine();
  return(0);
}
