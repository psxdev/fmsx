/** fMSX: portable MSX emulator ******************************/
/**                                                         **/
/**                         Orbis.c                         **/
/**                                                         **/
/** This file contains Orbis-dependent subroutines and      **/
/** drivers. It includes screen drivers via CommonMux.h.    **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1994-2017                 **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/** Copyright (c) Antonio Jose Ramos Marquez 2017			**/
/*************************************************************/
#include "MSX.h"
#include "EMULib.h"
#include "Sound.h"

#include <string.h>
#include <stdio.h>
#include <orbisPad.h>
#include <orbisKeyboard.h>
#include <orbis2d.h>
#include <debugnet.h>

#define WIDTH       272                   /* Buffer width    */
#define HEIGHT      228                   /* Buffer height   */
extern int vmode;
extern flipArg;
int counter=0;
extern OrbisPadConfig *confPad;
extern menuClosed;
/* Press/Release keys in the background KeyState */
#define XKBD_SET(K) XKeyState[Keys[K][0]]&=~Keys[K][1]
#define XKBD_RES(K) XKeyState[Keys[K][0]]|=Keys[K][1]
static volatile unsigned int JoyState = 0; /* Joystick state */
static volatile unsigned int LastKey  = 0; /* Last key prsd  */
static volatile unsigned int KeyModes = 0; /* SHIFT/CTRL/ALT */

/* Combination of EFF_* bits */
int UseEffects  = EFF_SCALE|EFF_SAVECPU;

int InMenu;                /* 1: In MenuMSX(), ignore keys   */
int UseZoom     = 1;       /* Zoom factor (1=no zoom)        */
int UseSound    = 44100;   /* Audio sampling frequency (Hz)  */
int SyncFreq    = 60;      /* Sync frequency (0=sync off)    */
int FastForward;           /* Fast-forwarded UPeriod backup  */
int SndSwitch;             /* Mask of enabled sound channels */
int SndVolume;             /* Master volume for audio        */
int OldScrMode;            /* fMSX "ScrMode" variable storage*/

const char *Title     = "fMSX 5.0";       /* Program version */

Image NormScreen;          /* Main screen image              */
Image WideScreen;          /* Wide screen image              */
static pixel *WBuf;        /* From Wide.h                    */
static pixel *XBuf;        /* From Common.h                  */
static unsigned int XPal[80];
static unsigned int BPal[256];
static unsigned int XPal0;
typedef struct OrbisImage
{
	unsigned int *pixels;
	int width;
	int height;
}OrbisImage;

OrbisImage Screen;
	
const char *Disks[2][MAXDISKS+1];         /* Disk names      */
volatile byte XKeyState[20]; /* Temporary KeyState array     */

void HandleKeys(unsigned int Key);
void PutImage(void);
void myPutImage(void);
void newKeyboard(void);

/** CommonMux.h **********************************************/
/** Display drivers for all possible screen depths.         **/
/*************************************************************/
#include "CommonMux.h"

/** InitMachine() ********************************************/
/** Allocate resources needed by machine-dependent code.    **/
/*************************************************************/
int InitMachine(void)
{
  int J,I;

  /* Initialize variables */
  UseZoom         = UseZoom<1? 1:UseZoom>5? 5:UseZoom;
  InMenu          = 0;
  FastForward     = 0;
  OldScrMode      = 0;
  NormScreen.Data = 0;
  WideScreen.Data = 0;

  /* Initialize system resources */
  InitOrbis(Title,WIDTH,HEIGHT);

  /* Set visual effects */
  SetEffects(UseEffects);

  /* Create main image buffer */
    if(!NewImage(&NormScreen,WIDTH,HEIGHT)) { TrashOrbis();return(0); }
	 XBuf = NormScreen.Data;
	 debugNetPrintf(3,"NormScreen %X\n",&NormScreen);
	 debugNetPrintf(3,"NormScreen Data %X\n",&NormScreen.Data);
 
#ifndef NARROW
  /* Create wide image buffer */
  if(!NewImage(&WideScreen,512,HEIGHT)) { TrashOrbis();return(0); }
  WBuf = WideScreen.Data;
#endif

  /* Set correct screen drivers */
  //if(!SetScreenDepth(NormScreen.D)) { TrashUnix();return(0); }
  SetScreenDepth(32);
  /* Initialize video to main image */
  SetVideo(&NormScreen,0,0,WIDTH,HEIGHT);


  

  /* Set all colors to black */
   for(J=0;J<80;J++) SetColor(J,0,0,0);

  /* Create SCREEN8 palette (GGGRRRBB) */
   for(J=0;J<256;J++)
	   BPal[J]=OrbisGetColor(((J>>2)&0x07)*255/7,((J>>5)&0x07)*255/7,(J&0x03)*255/3);

  /* Initialize temporary keyboard array */
  memset((void *)XKeyState,0xFF,sizeof(XKeyState));

  /* Attach keyboard handler */
  SetKeyHandler(HandleKeys);
  //UseSound=0;
  /* Initialize sound */
  InitSound(UseSound,150);
  SndSwitch=(1<<MAXCHANNELS)-1;
  SndVolume=255/MAXCHANNELS;  
  SetChannels(SndVolume,SndSwitch);

  
  /* Done */
  return(1);
}

/** TrashMachine() *******************************************/
/** Deallocate all resources taken by InitMachine().        **/
/*************************************************************/
void TrashMachine(void)
{
 
#ifndef NARROW
	 FreeImage(&WideScreen);
#endif
	  FreeImage(&NormScreen);
  TrashSound();
  TrashOrbis();
}

/** PutImage() ***********************************************/
/** Put an image on the screen.                             **/
/*************************************************************/
void PutImage(void)
{
	int ret;
#ifndef NARROW
  /* If screen mode changed... */
  if(ScrMode!=OldScrMode)
  {
    /* Switch to the new screen mode */
    OldScrMode=ScrMode;
    /* Depending on the new screen width... */
    if((ScrMode==6)||((ScrMode==7)&&!ModeYJK)||(ScrMode==MAXSCREEN+1))
	{
      SetVideo(&WideScreen,0,0,512,HEIGHT);
	  vmode=1;
    }
    else
	{
      SetVideo(&NormScreen,0,0,WIDTH,HEIGHT);
	  vmode=0;
	}  
  }
#endif

  
  /* Show display buffer */
  ret=ShowVideo();
  if(ret==0)
  {
    /* Initialize temporary keyboard array */
    memset((void *)XKeyState,0xFF,sizeof(XKeyState));
  	orbisAudioResume(0);
  	
  }
 
}

void myPutImage(void)
{
	
   // if (LastScrMode!=ScrMode) ResetView();
	
	orbis2dStartDrawing();
	orbis2dClearBuffer();
	orbis2dPutImage(VideoImg->Data, 0, 0, VideoW, VideoH);
	
	
	//flush and flip
	orbis2dFinishDrawing(flipArg);
			
	//swap buffers
	orbis2dSwapBuffers();
	flipArg++;
}


/** PlayAllSound() *******************************************/
/** Render and play given number of microseconds of sound.  **/
/*************************************************************/
void PlayAllSound(int uSec)
{
  /* @@@ Twice the argument to avoid skipping */
  RenderAndPlayAudio(2*uSec*UseSound/1000000);
}

/** Joystick() ***********************************************/
/** Query positions of two joystick connected to ports 0/1. **/
/** Returns 0.0.B2.A2.R2.L2.D2.U2.0.0.B1.A1.R1.L1.D1.U1.    **/
/*************************************************************/
unsigned int Joystick(void)
{
	
	
	return(JoyState);
	
}
void Keyboard(void)
{
	int ret;
    JoyState=0;
	unsigned int buttons;
	ret=orbisPadUpdate();
	if(ret==0)
	{
	if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R2))
	{
		debugNetPrintf(3,"combo L2R2\n");
		buttons=orbisPadGetCurrentButtonsPressed();
		buttons&= ~(ORBISPAD_L2|ORBISPAD_R2);
		orbisPadSetCurrentButtonsPressed(buttons);
		//LoadCart("host0:ROM/GRADIUS2.ROM",0,MAP_GUESS);
		//ResetMSX(Mode,32,4);
		menuClosed=0;
		//LoadFile("host0:ROM/ABADIA.DSK");
	}
	if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R1))
	{
		debugNetPrintf(3,"combo L1R1\n");
		buttons=orbisPadGetCurrentButtonsPressed();
		buttons&= ~(ORBISPAD_L1|ORBISPAD_R1);
		orbisPadSetCurrentButtonsPressed(buttons);
		LoadCart("host0:ROM/GRADIUS2.ROM",0,MAP_KONAMI4);
		//LoadFile("host0:ROM/ABADIA32.DSK");
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
	{
		//JoyState|=KBD_LEFT;LastKey=CON_LEFT;
		debugNetPrintf(INFO,"press left\n");
		
		XKBD_SET(KBD_LEFT);
	}
	if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
	{
		//JoyState|=KBD_RIGHT;LastKey=CON_RIGHT;
		debugNetPrintf(INFO,"press right\n");
		
		XKBD_SET(KBD_RIGHT);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_UP) || orbisPadGetButtonHold(ORBISPAD_UP))
	{
		//JoyState|=KBD_UP;LastKey=CON_UP;
		debugNetPrintf(INFO,"press up\n");
		
		XKBD_SET(KBD_UP);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
	{
		//JoyState|=KBD_DOWN;LastKey=CON_DOWN;
		debugNetPrintf(INFO,"press down\n");
		
		XKBD_SET(KBD_DOWN);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
	{
		//JoyState|=0x31;
		debugNetPrintf(INFO,"press cross\n");
		XKBD_SET(KBD_SPACE);
		
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
	{
		//JoyState|=0x4D;
		debugNetPrintf(INFO,"press circle\n");
		
		XKBD_SET(0x4D);
		
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press triangle\n");
		//XKBD_SET(KBD_F4);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_L2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press l2\n");
		XKBD_SET(KBD_F3);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_R2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press r2\n");
		XKBD_SET(KBD_F4);
		//XKBD_SET(KBD_CONTROL);
		
		
	}
	
	if(orbisPadGetButtonPressed(ORBISPAD_R1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press r1\n");
		XKBD_SET(KBD_F2);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_L1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press L1\n");
		XKBD_SET(KBD_F1);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
	{
		//JoyState|=BTN_FIRER;
		debugNetPrintf(3,"press square\n");
		XKBD_SET(KBD_F5);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_LEFT))
	{
		//JoyState|=~BTN_LEFT;
		debugNetPrintf(INFO,"release left\n");
		
		XKBD_RES(KBD_LEFT);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_RIGHT))
	{
		//JoyState|=~BTN_RIGHT;
		debugNetPrintf(INFO,"release right\n");
		
		XKBD_RES(KBD_RIGHT);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_UP))
	{
		//JoyState|=~BTN_UP;
		debugNetPrintf(INFO,"release up\n");
		
		XKBD_RES(KBD_UP);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_DOWN))
	{
		//JoyState|=~BTN_DOWN;
		debugNetPrintf(INFO,"release down\n");
		
		XKBD_RES(KBD_DOWN);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_CROSS))
	{
	//	JoyState|=~BTN_FIREA;
		debugNetPrintf(INFO,"release cross\n");
		
		XKBD_RES(KBD_SPACE);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_CIRCLE))
	{
		//JoyState|=~BTN_FIREB;
		debugNetPrintf(INFO,"release circle\n");
		
		XKBD_RES(0x4D);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_TRIANGLE))
	{
	//	JoyState|=~BTN_FIREL;
		debugNetPrintf(INFO,"release triangle\n");
		
		//XKBD_RES(KBD_F4);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_SQUARE))
	{
		//JoyState|=~BTN_FIRER;
		debugNetPrintf(INFO,"release square\n");
		
		XKBD_RES(KBD_F5);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_L2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"release l2\n");
		XKBD_RES(KBD_F3);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_R2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"release triangle\n");
		XKBD_RES(KBD_F4);
		//XKBD_RES(KBD_CONTROL);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_R1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"release r1\n");
		XKBD_RES(KBD_F2);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_L1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(3,"release l1\n");
		XKBD_RES(KBD_F1);
		
	}
 
	
	}
	orbisKeyboardUpdate();
	uint8_t key_shift=orbisKeyboardGetMsxKeyShift();
	if(key_shift)
	{
		XKBD_SET(KBD_SHIFT);
	}
	else
	{
		XKBD_RES(KBD_SHIFT);
	}
	uint8_t key_ctrl=orbisKeyboardGetMsxKeyCtrl();
	if(key_ctrl)
	{
		XKBD_SET(KBD_CONTROL);
	}
	else
	{
		XKBD_RES(KBD_CONTROL);
	}
	uint8_t key=orbisKeyboardGetMsxKey();
	if(orbisKeyboardGetMsxKeyStatus()==1)
	{
		XKBD_SET(key);
	}
	else
	{
		XKBD_RES(key);
	}
	
    memcpy((void *)KeyState,(const void *)XKeyState,sizeof(KeyState));
}
/** Keyboard() ***********************************************/
/** Modify keyboard matrix.                                 **/
/*************************************************************/
void newKeyboard(void)
{
	int ret;unsigned int buttons;
  unsigned int J,I;
  JoyState=0;
  ret=orbisPadUpdate();
  if(ret==0)
  {
	
	if(orbisPadGetButtonPressed(ORBISPAD_LEFT) || orbisPadGetButtonHold(ORBISPAD_LEFT))
	{
		//JoyState|=KBD_LEFT;LastKey=CON_LEFT;
		debugNetPrintf(INFO,"press left\n");
		
		XKBD_SET(KBD_LEFT);
	}
	if(orbisPadGetButtonPressed(ORBISPAD_RIGHT) || orbisPadGetButtonHold(ORBISPAD_RIGHT))
	{
		//JoyState|=KBD_RIGHT;LastKey=CON_RIGHT;
		debugNetPrintf(INFO,"press right\n");
		
		XKBD_SET(KBD_RIGHT);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_UP) || orbisPadGetButtonHold(ORBISPAD_UP))
	{
		//JoyState|=KBD_UP;LastKey=CON_UP;
		debugNetPrintf(INFO,"press up\n");
		
		XKBD_SET(KBD_UP);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_DOWN) || orbisPadGetButtonHold(ORBISPAD_DOWN))
	{
		//JoyState|=KBD_DOWN;LastKey=CON_DOWN;
		debugNetPrintf(INFO,"press down\n");
		
		XKBD_SET(KBD_DOWN);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
	{
		//JoyState|=0x31;
		debugNetPrintf(INFO,"press cross\n");
		XKBD_SET(KBD_SPACE);
		
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
	{
		//JoyState|=0x4D;
		debugNetPrintf(INFO,"press circle\n");
		
		XKBD_SET(0x4D);
		
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press triangle\n");
		//XKBD_SET(KBD_F4);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_L2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press l2\n");
		XKBD_SET(KBD_F3);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_R2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press r2\n");
		XKBD_SET(KBD_F4);
		//XKBD_SET(KBD_CONTROL);
		
		
	}
	
	if(orbisPadGetButtonPressed(ORBISPAD_R1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press r1\n");
		XKBD_SET(KBD_F2);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_L1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"press L1\n");
		XKBD_SET(KBD_F1);
		
	}
	if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
	{
		//JoyState|=BTN_FIRER;
		debugNetPrintf(3,"press square\n");
		XKBD_SET(KBD_F5);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_LEFT))
	{
		//JoyState|=~BTN_LEFT;
		debugNetPrintf(INFO,"release left\n");
		
		XKBD_RES(KBD_LEFT);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_RIGHT))
	{
		//JoyState|=~BTN_RIGHT;
		debugNetPrintf(INFO,"release right\n");
		
		XKBD_RES(KBD_RIGHT);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_UP))
	{
		//JoyState|=~BTN_UP;
		debugNetPrintf(INFO,"release up\n");
		
		XKBD_RES(KBD_UP);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_DOWN))
	{
		//JoyState|=~BTN_DOWN;
		debugNetPrintf(INFO,"release down\n");
		
		XKBD_RES(KBD_DOWN);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_CROSS))
	{
	//	JoyState|=~BTN_FIREA;
		debugNetPrintf(INFO,"release cross\n");
		
		XKBD_RES(KBD_SPACE);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_CIRCLE))
	{
		//JoyState|=~BTN_FIREB;
		debugNetPrintf(INFO,"release circle\n");
		
		XKBD_RES(0x4D);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_TRIANGLE))
	{
	//	JoyState|=~BTN_FIREL;
		debugNetPrintf(INFO,"release triangle\n");
		
		//XKBD_RES(KBD_F4);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_SQUARE))
	{
		//JoyState|=~BTN_FIRER;
		debugNetPrintf(INFO,"release square\n");
		
		XKBD_RES(KBD_F5);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_L2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"release l2\n");
		XKBD_RES(KBD_F3);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_R2))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"release triangle\n");
		XKBD_RES(KBD_F4);
		//XKBD_RES(KBD_CONTROL);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_R1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(INFO,"release r1\n");
		XKBD_RES(KBD_F2);
		
	}
	if(orbisPadGetButtonReleased(ORBISPAD_L1))
	{
		//JoyState|=BTN_FIREL;
		debugNetPrintf(3,"release l1\n");
		XKBD_RES(KBD_F1);
		
	}
 }


  
  //*(unsigned int *)&XKeyState[sizeof(XKeyState)-sizeof(int)]=J;
  memcpy((void *)KeyState,(const void *)XKeyState,sizeof(KeyState));
  
  

 
  /* Done */
  //return(I);
 
}

/** Mouse() **************************************************/
/** Query coordinates of a mouse connected to port N.       **/
/** Returns F2.F1.Y.Y.Y.Y.Y.Y.Y.Y.X.X.X.X.X.X.X.X.          **/
/*************************************************************/
unsigned int Mouse(byte N)
{
  unsigned int J;
  int X,Y;

  return(J);
}

/** SetColor() ***********************************************/
/** Set color N to (R,G,B).                                 **/
/*************************************************************/
void SetColor(byte N,byte R,byte G,byte B)
{
  if(N) XPal[N]=OrbisGetColor(R,G,B); else XPal0=OrbisGetColor(R,G,B);
}

/** HandleKeys() *********************************************/
/** Keyboard handler.                                       **/
/*************************************************************/
void HandleKeys(unsigned int Key)
{
  /* When in MenuMSX() or ConDebug(), ignore keypresses */
  if(InMenu) return;
  
  Key&=CON_KEYCODE;
  if(Key<128) XKBD_SET(Key);

  
}
