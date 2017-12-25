/*
 * menu fMSX for PlayStation 4 
 * Copyright (C) 2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/fmsx
 */
#include "menu.h"
#include "MSX.h"
#include "browser.h"
#include <orbisPad.h>
#include <orbis2d.h>
#include <logdebug.h>

int menuLevel=MENU_ABOUT;
int menuStatus=MENU_NOT_SELECTED;
int menuContextStatus=MENU_CONTEXT_CLOSED;
int menuMsx=MSX_MSX2;
int menuVideo=MSX_PAL;
int msxmode;
int menuClosed=1;
int menuExit=0;
extern int64_t flipArg;
extern int flag;

extern Orbis2dTexture *icons;
void menuAction()
{
	switch(menuGetStatus())
	{
		
		case MENU_ABOUT_SELECTED:
		debugNetPrintf(INFO,"based on work from Marat Fayzullin original author of this incredible emulator https://fms.komkon.org/fMSX/\n");			
		menuStatus=MENU_NOT_SELECTED;
			break;
		case MENU_MSX1_SELECTED:
		case MENU_MSX2_SELECTED:
		case MENU_MSX2PLUS_SELECTED:
		case MENU_VIDEO_NTSC_SELECTED:
		case MENU_VIDEO_PAL_SELECTED:
			menuStatus=MENU_NOT_SELECTED;
			msxmode=menuMsx|menuVideo|MSX_GUESSA|MSX_GUESSB;
			debugNetPrintf(INFO,"Changing Mode %x to %x \n",Mode,msxmode);
			
			menuClosed=0;
			break;
		case MENU_RESET_SELECTED:
			menuStatus=MENU_NOT_SELECTED;
			debugNetPrintf(INFO,"Mode %x %x %x \n",msxmode,msxmode&MSX_MODEL,msxmode&MSX_VIDEO);
			ResetMSX(msxmode,32,8);
			break;
		case MENU_MEGAROM1_SELECTED:
			menuStatus=MENU_NOT_SELECTED;
			menuClosed=0;
			showBrowser(MENU_MEGAROM1);
			menuClosed=1;
			//LoadCart("host0:ROM/GRADIUS2.ROM",0,MAP_GUESS);
			break;
		case MENU_MEGAROM2_SELECTED:
			menuStatus=MENU_NOT_SELECTED;
			showBrowser(MENU_MEGAROM2);
			//LoadCart("host0:ROM/SOLIDE12.ROM",0,MAP_GUESS);
			break;
		case MENU_DISKA_SELECTED:
			menuStatus=MENU_NOT_SELECTED;
			showBrowser(MENU_DISKA);
			//LoadFileDrive("host0:DSK/ABADIA.DSK",0);
			break;
		case MENU_DISKB_SELECTED:
			menuStatus=MENU_NOT_SELECTED;
			showBrowser(MENU_DISKB);
			//LoadFileDrive("host0:DSK/DRIVEB.DSK",1);
			break;
		case MENU_TAPE_SELECTED:
			menuStatus=MENU_NOT_SELECTED;
			showBrowser(MENU_TAPE);
			//ChangeTape("host0:CAS/DEFAULT.CAS");
			break;
		default:
			menuStatus=MENU_NOT_SELECTED;
			break;			
	}
	
}
void updateController()
{
	int ret;
	unsigned int buttons=0;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R2))
		{
			sys_log("Combo L2R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R1) )
		{
			sys_log("Combo L1R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1|ORBISPAD_R2) || orbisPadGetButtonHold(ORBISPAD_L1|ORBISPAD_R2))
		{
			sys_log("Combo L1R2 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L1|ORBISPAD_R2);
			orbisPadSetCurrentButtonsPressed(buttons);
			
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2|ORBISPAD_R1) || orbisPadGetButtonHold(ORBISPAD_L2|ORBISPAD_R1) )
		{
			sys_log("Combo L2R1 pressed\n");
			buttons=orbisPadGetCurrentButtonsPressed();
			buttons&= ~(ORBISPAD_L2|ORBISPAD_R1);
			orbisPadSetCurrentButtonsPressed(buttons);
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_UP))
		{
			sys_log("Up pressed\n");
			menuLevelUp();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN))
		{
			sys_log("Down pressed\n");
			menuLevelDown();
		}						
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT))
		{
			sys_log("Right pressed\n");
			menuLevelRight();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT))
		{
			sys_log("Left pressed\n");
			menuLevelLeft();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			sys_log("Triangle pressed exit\n");
			
			flag=0;
				
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
		{
			sys_log("Circle pressed come back to MSX\n");
			menuClosed=1;
			menuExit=1;
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			sys_log("Cross pressed\n");
			menuStatusChange();
			menuClosed=1;
			//menuSetContextStatus(MENU_CONTEXT_OPENING);
			//debugNetPrintf(3,"Context menu OPENING\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			sys_log("Square pressed\n");
			//orbisAudioPause(0);				
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L1))
		{
			sys_log("L1 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_L2))
		{
			sys_log("L2 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R1))
		{
			sys_log("R1 pressed\n");
			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_R2))
		{
			sys_log("R2 pressed\n");
			
		}	
	}
}
int menuGetStatus()
{
	return menuStatus;
}
void menuSetStatus(int status)
{
	if(status>=0)
	{
		menuStatus=status;
	}	
}
int menuGetLevel()
{
	return menuLevel;
}
void menuSetLevel(int level)
{
	if(level>=0)
	{
		menuLevel=level;
	}	
}
int menuGetContextStatus()
{
	return menuContextStatus;
}
void menuSetContextStatus(int status)
{
	if(status>=0)
	{
		menuContextStatus=status;
	}
}
void menuStatusChange()
{
	if(menuGetStatus()==MENU_NOT_SELECTED)
	{
		switch(menuGetLevel())
		{
			case MENU_ABOUT:
				menuSetStatus(MENU_ABOUT_SELECTED);
				menuClosed=1;
				break;
			case MENU_MSX1:
				menuSetStatus(MENU_MSX1_SELECTED);
				menuMsx=MSX_MSX1;
				break;
			case MENU_MSX2:
				menuSetStatus(MENU_MSX2_SELECTED);
				menuMsx=MSX_MSX2;
				break;
			case MENU_MSX2PLUS:
				menuSetStatus(MENU_MSX2PLUS_SELECTED);
				menuMsx=MSX_MSX2P;
				break;
			case MENU_VIDEO_NTSC:
				menuSetStatus(MENU_VIDEO_NTSC_SELECTED);
				menuVideo=MSX_NTSC;
				break;
			case MENU_VIDEO_PAL:
				menuSetStatus(MENU_VIDEO_PAL_SELECTED);
				menuVideo=MSX_PAL;
				break;
			case MENU_RESET:
				menuSetStatus(MENU_RESET_SELECTED);
				menuClosed=1;
				break;
			case MENU_MEGAROM1:
				menuSetStatus(MENU_MEGAROM1_SELECTED);
				menuClosed=1;
				break;
			case MENU_MEGAROM2:
				menuSetStatus(MENU_MEGAROM2_SELECTED);
				menuClosed=1;
				break;
			case MENU_DISKA:
				menuSetStatus(MENU_DISKA_SELECTED);
				menuClosed=1;
				break;
			case MENU_DISKB:
				menuSetStatus(MENU_DISKB_SELECTED);
				menuClosed=1;	
				break;
			case MENU_TAPE:
				menuSetStatus(MENU_TAPE_SELECTED);
				menuClosed=1;
				break;
			default:
				break;
		}
	}
}
void menuLevelUp()
{
	switch(menuGetLevel())
	{
		case MENU_ABOUT:
			menuSetLevel(MENU_RESET);
			break;
		case MENU_MSX1:
		case MENU_MSX2:
		case MENU_MSX2PLUS:
			menuSetLevel(MENU_ABOUT);
			break;
		case MENU_VIDEO_NTSC:
		case MENU_VIDEO_PAL:
			menuSetLevel(MENU_MSX1);
			break;
		case MENU_RESET:
		case MENU_MEGAROM1:
		case MENU_MEGAROM2:
		case MENU_DISKA:
		case MENU_DISKB:
		case MENU_TAPE:
			menuSetLevel(MENU_VIDEO_NTSC);
			break;
		default:
			menuSetLevel(MENU_ABOUT);
			break;
	}
}
void menuLevelDown()
{
	switch(menuGetLevel())
	{
		case MENU_ABOUT:
			menuSetLevel(MENU_MSX1);
			break;
		case MENU_MSX1:
		case MENU_MSX2:
		case MENU_MSX2PLUS:
			menuSetLevel(MENU_VIDEO_NTSC);
			break;
		case MENU_VIDEO_NTSC:
		case MENU_VIDEO_PAL:
			menuSetLevel(MENU_RESET);
			break;
		case MENU_RESET:
		case MENU_MEGAROM1:
		case MENU_MEGAROM2:
		case MENU_DISKA:
		case MENU_DISKB:
		case MENU_TAPE:
			menuSetLevel(MENU_ABOUT);
			break;
		default:
			menuSetLevel(MENU_ABOUT);
			break;
	}
}
void menuLevelRight()
{
	switch(menuGetLevel())
	{
		case MENU_ABOUT:
			break;
		case MENU_MSX1:
			menuSetLevel(MENU_MSX2);
			break;
		case MENU_MSX2:
			menuSetLevel(MENU_MSX2PLUS);
			break;
		case MENU_MSX2PLUS:
			menuSetLevel(MENU_MSX1);
			break;
		case MENU_VIDEO_NTSC:
			menuSetLevel(MENU_VIDEO_PAL);
			break;
		case MENU_VIDEO_PAL:
			menuSetLevel(MENU_VIDEO_NTSC);
			break;
		case MENU_RESET:
			menuSetLevel(MENU_MEGAROM1);
			break;
		case MENU_MEGAROM1:
			menuSetLevel(MENU_MEGAROM2);
			break;
		case MENU_MEGAROM2:
			menuSetLevel(MENU_DISKA);
			break;
		case MENU_DISKA:
			menuSetLevel(MENU_DISKB);
			break;
		case MENU_DISKB:
			menuSetLevel(MENU_TAPE);
			break;
		case MENU_TAPE:
			menuSetLevel(MENU_RESET);
			break;
		default:
			menuSetLevel(MENU_ABOUT);
			break;
	}
}
void menuLevelLeft()
{
	switch(menuGetLevel())
	{
		case MENU_ABOUT:
			break;
		case MENU_MSX1:
			menuSetLevel(MENU_MSX2PLUS);
			break;
		case MENU_MSX2:
			menuSetLevel(MENU_MSX1);
			break;
		case MENU_MSX2PLUS:
			menuSetLevel(MENU_MSX2);
			break;
		case MENU_VIDEO_NTSC:
			menuSetLevel(MENU_VIDEO_PAL);
			break;
		case MENU_VIDEO_PAL:
			menuSetLevel(MENU_VIDEO_NTSC);
			break;
		case MENU_RESET:
			menuSetLevel(MENU_TAPE);
			break;
		case MENU_MEGAROM1:
			menuSetLevel(MENU_RESET);
			break;
		case MENU_MEGAROM2:
			menuSetLevel(MENU_MEGAROM1);
			break;
		case MENU_DISKA:
			menuSetLevel(MENU_MEGAROM2);
			break;
		case MENU_DISKB:
			menuSetLevel(MENU_DISKA);
			break;
		case MENU_TAPE:
			menuSetLevel(MENU_DISKB);
			break;
		default:
			menuSetLevel(MENU_ABOUT);
			break;
	}
}
void menuFinish()
{
	orbis2dDestroyTexture(icons);
}
void menuDrawIcons()
{
	if(icons)
	{
		orbis2dDrawTexture(icons,0,0);	
	}
}
void menuDrawIconsMaskSelected()
{
	int x,y,w,h;
	//draw selected in menuMsx
	switch(menuMsx)
	{
		case MSX_MSX1:
			x=ICON_MSX1_MASK_X;
			y=ICON_MSX1_MASK_Y;
			w=ICON_MSX1_MASK_W;
			h=ICON_MSX1_MASK_H;
			break;
		case MSX_MSX2:
			x=ICON_MSX2_MASK_X;
			y=ICON_MSX2_MASK_Y;
			w=ICON_MSX2_MASK_W;
			h=ICON_MSX2_MASK_H;
			break;
		case MSX_MSX2P:
			x=ICON_MSX2PLUS_MASK_X;
			y=ICON_MSX2PLUS_MASK_Y;
			w=ICON_MSX2PLUS_MASK_W;
			h=ICON_MSX2PLUS_MASK_H;
			break;
		default:
			x=ICON_MSX2_MASK_X;
			y=ICON_MSX2_MASK_Y;
			w=ICON_MSX2_MASK_W;
			h=ICON_MSX2_MASK_H;
			break;
	}
	orbis2dDrawRectColor(x,w,y,2,RED_COLOR);
	orbis2dDrawRectColor(x,w,y+h-2,2,RED_COLOR);
	orbis2dDrawRectColor(x,2,y,h,RED_COLOR);
	orbis2dDrawRectColor(x+w-2,2,y,h,RED_COLOR);
	//draw selected in menuVideo
	switch(menuVideo)
	{
		case MSX_NTSC:
			x=ICON_VIDEO_NTSC_MASK_X;
			y=ICON_VIDEO_NTSC_MASK_Y;
			w=ICON_VIDEO_NTSC_MASK_W;
			h=ICON_VIDEO_NTSC_MASK_H;
			break;
		case MSX_PAL:
			x=ICON_VIDEO_PAL_MASK_X;
			y=ICON_VIDEO_PAL_MASK_Y;
			w=ICON_VIDEO_PAL_MASK_W;
			h=ICON_VIDEO_PAL_MASK_H;
			break;
		default:
			x=ICON_VIDEO_PAL_MASK_X;
			y=ICON_VIDEO_PAL_MASK_Y;
			w=ICON_VIDEO_PAL_MASK_W;
			h=ICON_VIDEO_PAL_MASK_H;
			break;
	}
	orbis2dDrawRectColor(x,w,y,2,RED_COLOR);
	orbis2dDrawRectColor(x,w,y+h-2,2,RED_COLOR);
	orbis2dDrawRectColor(x,2,y,h,RED_COLOR);
	orbis2dDrawRectColor(x+w-2,2,y,h,RED_COLOR);	
}
void menuDrawIconsMaskSelect()
{
	
	int x,y,w,h;
	
		switch(menuLevel)
		{
			case MENU_ABOUT:
				x=ICON_ABOUT_MASK_X;
				y=ICON_ABOUT_MASK_Y;
				w=ICON_ABOUT_MASK_W;
				h=ICON_ABOUT_MASK_H;
				break;
			case MENU_MSX1:
				x=ICON_MSX1_MASK_X;
				y=ICON_MSX1_MASK_Y;
				w=ICON_MSX1_MASK_W;
				h=ICON_MSX1_MASK_H;
				break;
			case MENU_MSX2:
				x=ICON_MSX2_MASK_X;
				y=ICON_MSX2_MASK_Y;
				w=ICON_MSX2_MASK_W;
				h=ICON_MSX2_MASK_H;
				break;
			case MENU_MSX2PLUS:
				x=ICON_MSX2PLUS_MASK_X;
				y=ICON_MSX2PLUS_MASK_Y;
				w=ICON_MSX2PLUS_MASK_W;
				h=ICON_MSX2PLUS_MASK_H;
				break;
			case MENU_VIDEO_NTSC:
				x=ICON_VIDEO_NTSC_MASK_X;
				y=ICON_VIDEO_NTSC_MASK_Y;
				w=ICON_VIDEO_NTSC_MASK_W;
				h=ICON_VIDEO_NTSC_MASK_H;
				break;
			case MENU_VIDEO_PAL:
				x=ICON_VIDEO_PAL_MASK_X;
				y=ICON_VIDEO_PAL_MASK_Y;
				w=ICON_VIDEO_PAL_MASK_W;
				h=ICON_VIDEO_PAL_MASK_H;
				break;
			case MENU_RESET:
				x=ICON_RESET_MASK_X;
				y=ICON_RESET_MASK_Y;
				w=ICON_RESET_MASK_W;
				h=ICON_RESET_MASK_H;
				break;
			case MENU_MEGAROM1:
				x=ICON_MEGAROM1_MASK_X;
				y=ICON_MEGAROM1_MASK_Y;
				w=ICON_MEGAROM1_MASK_W;
				h=ICON_MEGAROM1_MASK_H;
				break;
			case MENU_MEGAROM2:
				x=ICON_MEGAROM2_MASK_X;
				y=ICON_MEGAROM2_MASK_Y;
				w=ICON_MEGAROM2_MASK_W;
				h=ICON_MEGAROM2_MASK_H;
				break;
			case MENU_DISKA:
				x=ICON_DISKA_MASK_X;
				y=ICON_DISKA_MASK_Y;
				w=ICON_DISKA_MASK_W;
				h=ICON_DISKA_MASK_H;
				break;
			case MENU_DISKB:
				x=ICON_DISKB_MASK_X;
				y=ICON_DISKB_MASK_Y;
				w=ICON_DISKB_MASK_W;
				h=ICON_DISKB_MASK_H;
				break;
			case MENU_TAPE:
				x=ICON_TAPE_MASK_X;
				y=ICON_TAPE_MASK_Y;
				w=ICON_TAPE_MASK_W;
				h=ICON_TAPE_MASK_H;
				break;	
			default:
				x=ICON_ABOUT_MASK_X;
				y=ICON_ABOUT_MASK_Y;
				w=ICON_ABOUT_MASK_W;
				h=ICON_ABOUT_MASK_H;
				menuStatus=MENU_ABOUT;
				break;
		}
		
		orbis2dDrawRectColor(x,w,y,2,YELLOW_COLOR);
		orbis2dDrawRectColor(x,w,y+h-2,2,YELLOW_COLOR);
		orbis2dDrawRectColor(x,2,y,h,YELLOW_COLOR);
		orbis2dDrawRectColor(x+w-2,2,y,h,YELLOW_COLOR);
		
		
		
		
}


int menuInit()
{
	int ret=0;
	icons=orbis2dLoadPngFromHost(ICONS_FILE_PATH);
	if(icons)
	{
		
	}
	else
	{
		ret=-1;
		debugNetPrintf(ERROR,"Problem loading Icon image file from %s \n",ICONS_FILE_PATH);
		
	}
	menuLevel=0;
	return ret;
 
}
void menuDraw()
{
	//draw icons texture
	menuDrawIcons();
	//draw red square for selected icons
	menuDrawIconsMaskSelected();
	//draw yellow square to current icon
	menuDrawIconsMaskSelect();
	
}
void menuOpen()
{
	orbisAudioPause(0);
	
	while(menuClosed==0)
	{
		updateController();
		
		//wait for current display buffer
		orbis2dStartDrawing();

		// clear with background (default white) to the current display buffer 
		orbis2dClearBuffer();
			
		//default red is here press X to random color
		//orbis2dDrawRectColor(x,w,y,h,color);
		menuDraw();	
		//flush and flip
		orbis2dFinishDrawing(flipArg);
			
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;
		if(menuClosed==1)
		{
			if(menuExit!=1)
			{
				menuAction();
			}
			menuExit=0;
		}
	}
	
}

