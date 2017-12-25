/** EMULib Emulation Library *********************************/
/**                                                         **/
/**                          SndOrbis.c                     **/
/**                                                         **/
/** This file contains standard sound generation routines   **/
/** for Playstation Vita console                            **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1996-2008                 **/
/**               Akop Karapetyan 2008                      **/
/**               Vincent van Dam 2002                      **/
/**               Antonio Jose Ramos Marquez 2015-2017      **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/

#include <string.h>

#if defined(FMSX) && defined(ALTSOUND)
#include "fmopl.h"
#include "emu2149.h"
#include "emu2212.h"
#include "emu2413.h"
#endif

#include <orbisAudio.h>
#include "Sound.h"
#include <debugnet.h>
static void MixAudio(short *buffer, unsigned int length);
static void AudioCallback(OrbisAudioSample *buf, unsigned int samples, void *userdata);

#if defined(FMSX) && defined(ALTSOUND)
#define MSX_CLK 3579545

static OPLL   *opll;
static FM_OPL *fm_opl;
static PSG    *psg;
static SCC    *scc;

int Use2413 = 1;     /* MSX-MUSIC emulation (1=enable)  */
int Use8950 = 1;     /* MSX-AUDIO emulation (1=enable)  */

float FactorPSG  = 3.00;  /* Volume percentage of PSG        */
float FactorSCC  = 3.00;  /* Volume percentage of SCC        */
float Factor2413 = 3.00;  /* Volume percentage of MSX-MUSIC  */
float Factor8950 = 2.25;  /* Volume percentage of MSX-AUDIO  */
#else
static sample SndData[SND_BUFSIZE];
static int MixBuffer[SND_BUFSIZE];
static int WPtr = 0;
#endif

static int SndRate     = 0;  /* Audio sampling rate          */

/** StopSound() **********************************************/
/** Temporarily suspend sound.                              **/
/*************************************************************/
void StopSound(void) { orbisAudioPause(0); }

/** ResumeSound() ********************************************/
/** Resume sound after StopSound().                         **/
/*************************************************************/
void ResumeSound(void) { orbisAudioResume(0); }

/** InitAudio() **********************************************/
/** Initialize sound. Returns rate (Hz) on success, else 0. **/
/** Rate=0 to skip initialization (will be silent).         **/
/*************************************************************/
unsigned int InitAudio(unsigned int Rate,unsigned int Latency)
{
  TrashAudio();

  orbisAudioSetCallback(0, AudioCallback, 0);

  /* Only 44100 supported */
  if(Rate != 44100) return(SndRate = 0);

#if defined(FMSX) && defined(ALTSOUND)
  /* MSX-MUSIC emulation */
  debugNetPrintf(3,"AQUISSSSSSSS \n");
  if (Use2413)
  {
    OPLL_init(MSX_CLK,Rate);
    opll=OPLL_new();
    OPLL_reset(opll);
    OPLL_reset_patch(opll,0);
  }

  /* MSX-AUDIO emulation */
  if (Use8950)
  {
    fm_opl=OPLCreate(OPL_TYPE_Y8950,MSX_CLK,Rate,256);
    OPLResetChip(fm_opl);
  }

  /* PSG/SCC emulation */
  PSG_init(MSX_CLK,Rate);
  psg=PSG_new();
  PSG_reset(psg);
  PSG_setVolumeMode(psg,2);

  SCC_init(MSX_CLK,Rate);
  scc=SCC_new();
  SCC_reset(scc);
#else
  WPtr=0;
  memset(SndData,0,sizeof(SndData));
  memset(MixBuffer,0,sizeof(MixBuffer));
#endif

  /* Done */
  return(SndRate = Rate);
}

/** ResetSound() *********************************************/
/** Reset sound chips.                                      **/
/*************************************************************/
void ResetSound()
{
#if defined(FMSX) && defined(ALTSOUND)
  PSG_reset(psg);
  SCC_reset(scc);
  if (Use2413) OPLL_reset(opll);
  if (Use8950) OPLResetChip(fm_opl);
#endif
}

/** TrashAudio() *********************************************/
/** Free resources allocated by InitAudio().                **/
/*************************************************************/
void TrashAudio(void)
{
  if (!SndRate) return;
  SndRate = 0;

  /* Shutdown wave audio */
  StopSound();

#if defined(FMSX) && defined(ALTSOUND)
  /* clean up MSXMUSIC */
  if (Use2413)
  {
    OPLL_close();
    OPLL_delete(opll);
  }

  /* clean up MSXAUDIO */
  if (Use8950) OPLDestroy(fm_opl);

  /* clean up PSG/SCC */
  PSG_delete(psg);
  SCC_delete(scc);
#endif
}

/** AudioCallback() ******************************************/
/** Called by the system to render sound                    **/
/*************************************************************/
static void AudioCallback(OrbisAudioSample *buf, unsigned int samples, void *userdata)
{
  MixAudio((short*)buf, samples);
}

/** AudioCallback() ******************************************/
/** Writes sound to the output buffer,                      **/
/** mixing as necessary                                     **/
/*************************************************************/
static void MixAudio(short *buffer, unsigned int length)
{
  register int J;

#if defined(FMSX) && defined(ALTSOUND)
  register int R;
  register INT16 P,O,A,S;

  /* Mix sound */
  for(J=0;J<length;J++)
  {
    P=PSG_calc(psg);
    S=SCC_calc(scc);
    O=Use2413? OPLL_calc(opll): 0;
    A=Use8950? Y8950UpdateOne(fm_opl): 0;
    R=P*FactorPSG+O*Factor2413+A*Factor8950+S*FactorSCC;

    /* Write to output buffer */
    *buffer++ = (R>32767)?32767:(R<-32768)?-32768:R;
  }
#else
  /* Mix sound */
  memset(MixBuffer,0,sizeof(MixBuffer));
  RenderAudio(MixBuffer,length);
  PlayAudio(MixBuffer,length);

  /* Write to output buffer */
  for(J=0;J<length;J++)
    *buffer++ = SndData[J];
#endif
}

/** GetFreeAudio() *******************************************/
/** Get the amount of free samples in the audio buffer.     **/
/*************************************************************/
unsigned int GetFreeAudio(void)
{
#if defined(FMSX) && defined(ALTSOUND)
  return(0); /* Not used; should not be called */
#else
  return(!SndRate?0:SND_BUFSIZE-WPtr);
#endif
}

/** WriteAudio() *********************************************/
/** Write up to a given number of samples to audio buffer.  **/
/** Returns the number of samples written.                  **/
/*************************************************************/
unsigned int WriteAudio(sample *Data,unsigned int Length)
{
#if defined(FMSX) && defined(ALTSOUND)
  return(0); /* Not used; should not be called */
#else
  /* Require audio to be initialized      */
  /* and buffer to have enough free space */
  if(!SndRate||WPtr+Length>SND_BUFSIZE) return(0);

  /* Copy sample data and increment buffer position */
  memcpy(SndData+WPtr*sizeof(sample),Data,Length*sizeof(sample));
  if ((WPtr+=Length)>=SND_BUFSIZE) WPtr=0;
  
  /* Return number of samples copied */
  return(Length);
#endif
}

#if defined(FMSX) && defined(ALTSOUND)
/* wrapper functions to actual sound emulation */
void WriteOPLL (int R,int V) { OPLL_writeReg(opll,R,V); }
void WriteAUDIO(int R,int V) { OPLWrite(fm_opl,R,V); }
void Write2212 (int R,int V) { SCC_write(scc,R,V); }
void WritePSG  (int R,int V) { PSG_writeReg(psg,R,V); }
int  ReadAUDIO (int R)       { return OPLRead(fm_opl,R); }
int  ReadPSG   (int R)       { return PSG_readReg(psg,R); }
#endif
