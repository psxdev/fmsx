/*
 * browser for PlayStation 4 
 * Copyright (C) 2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/fmsx
 */

#include <orbisPad.h>
#include <orbis2d.h>
#include <orbisFileBrowser.h>
#include <orbisXbmFont.h>
#include "menu.h"
#include "browser.h"
#include "MSX.h"

int notSelected=1;
int comeBack=0;
extern int64_t flipArg;
int extension;
int slot=0;
int drive=0;
char path[256];
void browserUpdateController()
{
	int ret;
	unsigned int buttons=0;
	OrbisFileBrowserListEntry *currentEntry;
	OrbisFileBrowserListEntry *entry;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		if(orbisPadGetButtonPressed(ORBISPAD_UP))
		{
			debugNetPrintf(DEBUG,"Up pressed\n");
			orbisFileBrowserEntryUp();
			currentEntry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos()+orbisFileBrowserGetRelPos());
			if(currentEntry!=NULL)
			{
				debugNetPrintf(INFO,"current entry %s\n",currentEntry->name);
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			orbisFileBrowserEntryDown();
			currentEntry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos()+orbisFileBrowserGetRelPos());
			if(currentEntry!=NULL)
			{
				debugNetPrintf(INFO,"current entry %s\n",currentEntry->name);
			}
		}						
		if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
		{
			debugNetPrintf(DEBUG,"Circle pressed come back to menu\n");
			comeBack=1;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			debugNetPrintf(DEBUG,"Cross pressed\n");
			notSelected=0;
			entry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos()+orbisFileBrowserGetRelPos());
			if(entry!=NULL)
			{
				switch(entry->type)
				{
					case FILE_TYPE_GAME_ROM:
						if(extension==FILE_TYPE_GAME_ROM)
						{
							sprintf(path,"host0:GAMES/%s",entry->name);
							LoadCart(path,slot,MAP_GUESS);
						}
						else
						{
							debugNetPrintf(INFO,"wrong extension choose the right one\n");
						}
						break;
					case FILE_TYPE_GAME_DSK:
						if(extension==FILE_TYPE_GAME_DSK)
						{
						
							sprintf(path,"host0:GAMES/%s",entry->name);
							LoadFileDrive(path,drive);
						}
						else
						{
							debugNetPrintf(INFO,"wrong extension choose the right one\n");
						}
						break;
					case FILE_TYPE_CAS:
						if(extension==FILE_TYPE_CAS)
						{
							sprintf(path,"host0:GAMES/%s",entry->name);
							ChangeTape(path);
						}
						else
						{
							debugNetPrintf(INFO,"wrong extension choose the right one\n");
						}
						break;
					default:
						comeBack=1;
						break;
				}
			}
		}	
	}
}
void browserDraw()
{
	int i;
	OrbisFileBrowserListEntry *entry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos());
	if(entry!=NULL)
	{
		for(i=0;i<MAX_ENTRIES;i++)
		{
			
			if(entry!=NULL)
			{
				//debugNetPrintf(DEBUG,("%s %d\n",entry->name,entry->type);
				if(i==orbisFileBrowserGetRelPos())
				{
					print_text(180,200+i*(FONT_H+5),"*");
					
				}
				print_text(200,200+i*(FONT_H+5),entry->name);
				entry=entry->next;				
			}
		}
	}
	
	
}
void showBrowser(int m)
{
	uint32_t f1 = 0x800000AA,f2 = 0x80FF0066;
	update_gradient(&f1, &f2);
	debugNetPrintf(INFO,"in showBrowser %d\n",m);
	//orbisFileBrowserListRefresh();
	switch(m)
	{
		case MENU_MEGAROM1:
			extension=FILE_TYPE_GAME_ROM;
			slot=0;
			break;
		case MENU_MEGAROM2:
			extension=FILE_TYPE_GAME_ROM;
			slot=1;
			break;
		case MENU_DISKA:
			extension=FILE_TYPE_GAME_DSK;
			drive=0;
			break;
		case MENU_DISKB:
			extension=FILE_TYPE_GAME_DSK;
			drive=1;
			break;
		case MENU_TAPE:
			extension=FILE_TYPE_CAS;
			break;
		default:
			break;
	}
	while(notSelected==1 && comeBack==0)
	{
		browserUpdateController();
		//wait for current display buffer
		orbis2dStartDrawing();

		// clear with background (default white) to the current display buffer 
		orbis2dClearBuffer(0);
			
		//default red is here press X to random color
		//orbis2dDrawRectColor(x,w,y,h,color);
		browserDraw();	
		//flush and flip
		orbis2dFinishDrawing(flipArg);
			
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;
	}
	notSelected=1;
	comeBack=0;
}