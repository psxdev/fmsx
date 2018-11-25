/** EMULib Emulation Library *********************************/
/**                                                         **/
/**                        LibOrbis.c                       **/
/**                                                         **/
/** This file contains Orbis-dependent implementation       **/
/** parts of the emulation library.                         **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1996-2016                 **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/** Copyright (c) Antonio Jose Ramos Marquez 2017			**/
/*************************************************************/
#include "EMULib.h"
#include "Sound.h"

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

extern int64_t flipArg;
extern Image NormScreen;          /* Main screen image              */
extern Image WideScreen;          /* Wide screen image              */
extern int displayNumber;
int vmode;

#define FPS_COLOR PIXEL(255,0,255)

extern int MasterSwitch; /* Switches to turn channels on/off */
extern int MasterVolume; /* Master volume                    */

static volatile int TimerReady = 0;   /* 1: Sync timer ready */
static volatile unsigned int JoyState = 0; /* Joystick state */
static volatile unsigned int LastKey  = 0; /* Last key prsd  */
static volatile unsigned int KeyModes = 0; /* SHIFT/CTRL/ALT */

static int Effects    = EFF_SCALE|EFF_SAVECPU; /* EFF_* bits */
static int TimerON    = 0; /* 1: sync timer is running       */
//static Display *Dsp   = 0; /* X11 display                    */
//static Screen *Scr    = 0; /* X11 screen                     */
//static Window Wnd     = 0; /* X11 window                     */
//static Colormap CMap;      /* X11 color map                  */
static Image OutImg;       /* Scaled output image buffer     */
static const char *AppTitle; /* Current window title         */
static int XSize,YSize;    /* Current window dimensions      */

static int FrameCount;      /* Frame counter for EFF_SHOWFPS */
static int FrameRate;       /* Last frame rate value         */
//static struct timeval TimeStamp; /* Last timestamp           */



/** InitUnix() ***********************************************/
/** Initialize Unix/X11 resources and set initial window    **/
/** title and dimensions.                                   **/
/*************************************************************/
int InitOrbis(const char *Title,int Width,int Height)
{
  /* Initialize variables */
 // AppTitle    = Title;
  XSize       = Width;
  YSize       = Height;
  TimerON     = 0;
  TimerReady  = 0;
  JoyState    = 0;
  LastKey     = 0;
  KeyModes    = 0;
 // Wnd         = 0;
 // Dsp         = 0;
 // Scr         = 0;
 // CMap        = 0;
  FrameCount  = 0;
  FrameRate   = 0;

  NewImage(&OutImg,1280,720);
 
  return(1);
}

/** TrashUnix() **********************************************/
/** Free resources allocated in InitUnix()                  **/
/*************************************************************/
void TrashOrbis(void)
{
  
  /* Shut down audio */
  TrashAudio();
  
  
}

/** ShowVideo() **********************************************/
/** Show "active" image at the actual screen or window.     **/
/*************************************************************/
int ShowVideo(void)
{
	int ret;
	if(displayNumber==0)
	{
		if(OutImg.Data)
    	ScaleImage(&OutImg,VideoImg,VideoX,VideoY,VideoW,VideoH);
	
		orbis2dStartDrawing();
		orbis2dClearBuffer(0);
		if(OutImg.Data)
		orbis2dPutImage(OutImg.Data, 0, 0, 1280, 720);
	
	
		//flush and flip
		orbis2dFinishDrawing(flipArg);
			
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;
		ret=1;
	}
	else
	{
		showBrowser();
		ret=0;
	}
	
	return ret;
}

/** GetJoystick() ********************************************/
/** Get the state of joypad buttons (1="pressed"). Refer to **/
/** the BTN_* #defines for the button mappings.             **/
/*************************************************************/
unsigned int GetJoystick(void)
{
 
 
  /* Return current joystick state */
  return(JoyState|KeyModes);
}

/** GetMouse() ***********************************************/
/** Get mouse position and button states in the following   **/
/** format: RMB.LMB.Y[29-16].X[15-0].                       **/
/*************************************************************/
unsigned int GetMouse(void)
{
  int X,Y,J,Mask;

 
  
    /* Translate mouse position */
    X-= ((XSize-VideoW)>>1);
    Y-= ((YSize-VideoH)>>1);
    X = X<0? 0:X>=XSize? XSize-1:X;
    Y = Y<0? 0:Y>=YSize? YSize-1:Y;
  
	return 0;
  
}

/** GetKey() *************************************************/
/** Get currently pressed key or 0 if none pressed. Returns **/
/** CON_* definitions for arrows and special keys.          **/
/*************************************************************/
unsigned int GetKey(void)
{
  unsigned int J;

  J=LastKey;
  LastKey=0;
  return(J);
}

/** WaitKey() ************************************************/
/** Wait for a key to be pressed. Returns CON_* definitions **/
/** for arrows and special keys.                            **/
/*************************************************************/
unsigned int WaitKey(void)
{
  unsigned int J;

  /* Swallow current keypress */
  GetKey();
  /* Wait in 100ms increments for a new keypress */
  while(!(J=GetKey())&&VideoImg) usleep(100000);
  /* Return key code */
  return(J);
}

/** WaitKeyOrMouse() *****************************************/
/** Wait for a key or a mouse button to be pressed. Returns **/
/** the same result as GetMouse(). If no mouse buttons      **/
/** reported to be pressed, do GetKey() to fetch a key.     **/
/*************************************************************/
unsigned int WaitKeyOrMouse(void)
{
  unsigned int J;

  /* Swallow current keypress */
  GetKey();
  /* Make sure mouse keys are not pressed */
  while(GetMouse()&MSE_BUTTONS) usleep(100000);
  /* Wait in 100ms increments for a key or mouse click */
  while(!(J=GetKey())&&!(GetMouse()&MSE_BUTTONS)&&VideoImg) usleep(100000);
  /* Place key back into the buffer and return mouse state */
  LastKey=J;
  return(GetMouse());
}


/** NewImage() ***********************************************/
/** Create a new image of the given size. Returns pointer   **/
/** to the image data on success, 0 on failure.             **/
/*************************************************************/
pixel *NewImage(Image *Img,int Width,int Height)
{
  int Depth,J,I;

  /* Set data fields to ther defaults */
  Img->Data    = 0;
  Img->W       = 0;
  Img->H       = 0;
  Img->L       = 0;
  Img->D       = 0;
  Img->Attrs   = 0;
  Img->Cropped = 0;

 
  /* Image depth we are going to use */
  Depth = 32;
  Img->Data=(unsigned int *)malloc(Width*Height*Depth/8);
  if(Img->Data==NULL)
  {
  	Img->Data=mmap(NULL,Width*Height*Depth/8,0x01|0x02,0x1000|0x0002,-1,0);
  	
  }
if(Img->Data!=NULL)
{
 
  /* Done */
 // Img->Data  = (pixel *)Img->XImg->data;
    Img->W     = Width;
    Img->H     = Height;
    Img->L     = Width;
    Img->D     = Depth;
    Img->Attrs = Effects;
}
else
{
	debugNetPrintf(DEBUG,"[MSX]error creating image\n");
}
  return(Img->Data);
}

/** FreeImage() **********************************************/
/** Free previously allocated image.                        **/
/*************************************************************/
void FreeImage(Image *Img)
{
  
	/* Need to initalize library first */
	if(Img->Data==NULL)
	{
		return;
	}


	

	free(Img->Data);
	

	/* Image freed */
	Img->Data = 0;
	Img->W    = 0;
	Img->H    = 0;
	Img->L    = 0;
    Img->Attrs   = 0;
    Img->Cropped = 0;
}

/** CropImage() **********************************************/
/** Create a subimage Dst of the image Src. Returns Dst.    **/
/*************************************************************/
Image *CropImage(Image *Dst,const Image *Src,int X,int Y,int W,int H)
{
  Dst->Data    = (pixel *)((char *)Src->Data+(Src->L*Y+X)*(Src->D>>3));
  Dst->Cropped = 1;
  Dst->W       = W;
  Dst->H       = H;
  Dst->L       = Src->L;
  Dst->D       = Src->D;
 // Dst->XImg    = 0;
  Dst->Attrs   = 0;
  return(Dst);
}

/** SetVideo() ***********************************************/
/** Set part of the image as "active" for display.          **/
/*************************************************************/
void SetVideo(Image *Img,int X,int Y,int W,int H)
{
  /* Save current dimensions */
  int OldW = VideoW;
  int OldH = VideoH;

  /* Call default SetVideo() function */
  GenericSetVideo(Img,X,Y,W,H);

  
}

/** SetEffects() *********************************************/
/** Set visual effects applied to video in ShowVideo().     **/
/*************************************************************/
void SetEffects(unsigned int NewEffects)
{
  /* Set new effects */
  Effects=NewEffects;
}


  
/** X11GetColor **********************************************/
/** Get pixel for the current screen depth based on the RGB **/
/** values.                                                 **/
/*************************************************************/
unsigned int OrbisGetColor(unsigned char R,unsigned char G,unsigned char B)
{
  int J;

  /* If using constant BPP, just return a pixel */
  return(PIXEL(R,G,B));

 
}

