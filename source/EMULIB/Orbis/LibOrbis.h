/** EMULib Emulation Library *********************************/
/**                                                         **/
/**                        LibOrbis.h                       **/
/**                                                         **/
/** This file contains Orbis-dependent definitions and      **/
/** declarations for the emulation library.                 **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1996-2016                 **/
/** Copyright (C) Akop Karapetyan 2007                      **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/** Copyright (c) Antonio Jose Ramos Marquez 2017			**/
/*************************************************************/
#ifndef LIBORBIS_H
#define LIBORBIS_H


#ifdef __cplusplus
extern "C" {
#endif

#define SND_CHANNELS    16     /* Number of sound channels   */
#define SND_BITS        8
#define SND_BUFSIZE     256 //(1<<SND_BITS)

/** PIXEL() **************************************************/
/** Unix may use multiple pixel formats.                    **/
/*************************************************************/
#if defined(BPP32) || defined(BPP24)
#define PIXEL(R,G,B)  (pixel)(((int)R<<16)|((int)G<<8)|B)
#define PIXEL2MONO(P) (((P>>16)&0xFF)+((P>>8)&0xFF)+(P&0xFF))/3)
#define RMASK 0xFF0000
#define GMASK 0x00FF00
#define BMASK 0x0000FF

#elif defined(BPP16)
#define PIXEL(R,G,B)  (pixel)(((31*(R)/255)<<11)|((63*(G)/255)<<5)|(31*(B)/255))
	//#define RGB(r,g,b)   (((((b)>>3)&0x1F)<<10)|((((g)>>3)&0x1F)<<5)|(((r)>>3)&0x1F)|0x8000)
#define PIXEL2MONO(P) (522*(((P)&31)+(((P)>>5)&63)+(((P)>>11)&31))>>8)
#define RMASK 0xF800
#define GMASK 0x07E0
#define BMASK 0x001F

#elif defined(BPP8)
#define PIXEL(R,G,B)  (pixel)(((7*(R)/255)<<5)|((7*(G)/255)<<2)|(3*(B)/255))
#define PIXEL2MONO(P) (3264*((((P)<<1)&7)+(((P)>>2)&7)+(((P)>>5)&7))>>8)
#define RMASK 0xE0
#define GMASK 0x1C
#define BMASK 0x03
#endif

int  ARGC;
char **ARGV;

/** InitUnix() ***********************************************/
/** Initialize Unix/X11 resources and set initial window.   **/
/** title and dimensions.                                   **/
/*************************************************************/
int InitOrbis(const char *Title,int Width,int Height);

/** TrashUnix() **********************************************/
/** Free resources allocated in InitUnix()                  **/
/*************************************************************/
void TrashOrbis(void);

/** InitAudio() **********************************************/
/** Initialize sound. Returns rate (Hz) on success, else 0. **/
/** Rate=0 to skip initialization (will be silent).         **/
/*************************************************************/
unsigned int InitAudio(unsigned int Rate,unsigned int Latency);

/** StopSound() **********************************************/
/** Temporarily suspend sound.                              **/
/*************************************************************/
void StopSound(void);

/** StopSound() **********************************************/
/** Reset sound chips.                                      **/
/*************************************************************/
void ResetSound(void);

/** ResumeSound() ********************************************/
/** Resume sound after StopSound().                         **/
/*************************************************************/
void ResumeSound(void);

/** TrashAudio() *********************************************/
/** Free resources allocated by InitAudio().                **/
/*************************************************************/
void TrashAudio(void);

/** PauseAudio() *********************************************/
/** Pause/resume audio playback.                            **/
/*************************************************************/
int PauseAudio(int Switch);

/** X11Window() **********************************************/
/** Open a window of a given size with a given title.       **/
/*************************************************************/
//Window X11Window(const char *Title,int Width,int Height);

/** X11GetColor **********************************************/
/** Get pixel for the current screen depth based on the RGB **/
/** values.                                                 **/
/*************************************************************/
unsigned int OrbisGetColor(unsigned char R,unsigned char G,unsigned char B);

#if defined(FMSX) && defined(ALTSOUND)
void WritePSG(int R,int V);
void WriteSNG(int R,int V);
void Write2212(int R,int V);
void WriteOPLL(int R,int V);
void WriteAUDIO(int R,int V);
int  ReadAUDIO(int R);
int  ReadPSG(int R);
#endif

#ifdef __cplusplus
}
#endif
#endif /* LIBORBIS_H */
