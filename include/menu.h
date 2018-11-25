#ifndef _MENU_H_
#define _MENU_H_

#include <debugnet.h>

#define RED_COLOR 0x80ff0000
#define YELLOW_COLOR 0xFF24FFBD 
#define SETTINGS_FILE_PATH "host0:system/settings.png"
#define CREDITS_FILE_PATH "host0:system/credits.png"

#define ICON_ABOUT_MASK_X 499
#define ICON_ABOUT_MASK_Y 220
#define ICON_ABOUT_MASK_W 305
#define ICON_ABOUT_MASK_H 167

#define ICON_MSX1_MASK_X 501
#define ICON_MSX1_MASK_Y 388
#define ICON_MSX1_MASK_W 56
#define ICON_MSX1_MASK_H 23

#define ICON_MSX2_MASK_X 571
#define ICON_MSX2_MASK_Y 388
#define ICON_MSX2_MASK_W 69
#define ICON_MSX2_MASK_H 23

#define ICON_MSX2PLUS_MASK_X 650
#define ICON_MSX2PLUS_MASK_Y 388
#define ICON_MSX2PLUS_MASK_W 84
#define ICON_MSX2PLUS_MASK_H 23

#define ICON_VIDEO_NTSC_MASK_X 653
#define ICON_VIDEO_NTSC_MASK_Y 421
#define ICON_VIDEO_NTSC_MASK_W 44
#define ICON_VIDEO_NTSC_MASK_H 16

#define ICON_VIDEO_PAL_MASK_X 606
#define ICON_VIDEO_PAL_MASK_Y 421
#define ICON_VIDEO_PAL_MASK_W 34
#define ICON_VIDEO_PAL_MASK_H 16
 
#define ICON_RESET_MASK_X 612
#define ICON_RESET_MASK_Y 447
#define ICON_RESET_MASK_W 66
#define ICON_RESET_MASK_H 71





enum MENU_LEVEL {
	MENU_ABOUT,
	MENU_MSX1,
	MENU_MSX2,
	MENU_MSX2PLUS,
	MENU_VIDEO_NTSC,
	MENU_VIDEO_PAL,
	MENU_RESET,
	MENU_MEGAROM1,
	MENU_MEGAROM2,
	MENU_DISKA,
	MENU_DISKB,
	MENU_TAPE,
};

enum MENU_SELECT_STATUS {
	MENU_NOT_SELECTED,
	MENU_ABOUT_SELECTED,
	MENU_MSX1_SELECTED,
	MENU_MSX2_SELECTED,
	MENU_MSX2PLUS_SELECTED,
	MENU_VIDEO_NTSC_SELECTED,
	MENU_VIDEO_PAL_SELECTED,
	MENU_RESET_SELECTED,
	MENU_MEGAROM1_SELECTED,
	MENU_MEGAROM2_SELECTED,
	MENU_DISKA_SELECTED,
	MENU_DISKB_SELECTED,
	MENU_TAPE_SELECTED,
};

enum MENU_CONTEXT_STATUS{
	MENU_CONTEXT_CLOSED,
	MENU_CONTEXT_CLOSING,
	MENU_CONTEXT_OPENED,
	MENU_CONTEXT_OPENING,
};
int menuGetStatus();
int menuGetLevel();
int menuGetContextStatus();
float menuGetContextHeight();
void menuSetStatus(int status);
void menuSetLevel(int level);
void menuSetContextStatus(int status);
void menuSetContextHeight(float height);
//DRAW MENUS TEXTURES
void menuDrawIcons();
void menuDrawIconsMaskSelect();

//PUBLIC
int menuInit();
void menuFinish();
void menuLevelUp();
void menuLevelDown();
void menuLevelLeft();
void menuLevelRight();
void menuDraw();
void menuStatusChange();
void showCredits();
void showSettings();

#endif
