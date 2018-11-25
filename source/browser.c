/*
 * browser for PlayStation 4 
 * Copyright (C) 2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/fmsx
 */

#include <orbisPad.h>
#include <orbis2d.h>
#include <orbisFileBrowser.h>
#include <orbisXbmFont.h>
#include <ps4link.h>
#include "menu.h"
#include "browser.h"
#include "MSX.h"

int notSelected=1;
int comeBack=0;
extern int64_t flipArg;
int displayNumber=0;
int extension;
int slot=0;
int drive=0;
char path[256];
int posy=2;
int posx=0;

extern Orbis2dTexture *browserTexture;
extern Orbis2dTexture *folderTexture;
extern Orbis2dTexture *fileTexture;


int flagfolder=0;
OrbisFileBrowserListEntry *currentEntry;

void browserUpdateController()
{
	int ret;
	unsigned int buttons=0;
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
				debugNetPrintf(INFO,"current entry %s\n",currentEntry->dir->name);
			}
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN))
		{
			
			debugNetPrintf(DEBUG,"Down pressed\n");
			debugNetPrintf(DEBUG," before entry down level=%d base=%d rel=%d\n",orbisFileBrowserGetDirLevel(),orbisFileBrowserGetBasePos(),orbisFileBrowserGetRelPos());
			
			orbisFileBrowserEntryDown();
			debugNetPrintf(DEBUG," after entry down level=%d base=%d rel=%d\n",orbisFileBrowserGetDirLevel(),orbisFileBrowserGetBasePos(),orbisFileBrowserGetRelPos());
			
			currentEntry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos()+orbisFileBrowserGetRelPos());
			if(currentEntry!=NULL)
			{
				debugNetPrintf(INFO,"current entry %s\n",currentEntry->dir->name);
			}
			else
			{
				debugNetPrintf(INFO,"current entry chunga\n");
				
			}
		}						
		if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
		{
			debugNetPrintf(DEBUG,"Circle pressed come back to msx\n");
			comeBack=1;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			debugNetPrintf(DEBUG,"Triangle pressed posy=%d\n",++posy);
			showSettings();
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed posx=%d\n",++posx);
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			debugNetPrintf(DEBUG,"Cross pressed\n");
			notSelected=0;
			debugNetPrintf(DEBUG,"cross level=%d base=%d rel=%d\n",orbisFileBrowserGetDirLevel(),orbisFileBrowserGetBasePos(),orbisFileBrowserGetRelPos());
			
			//entry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos()+orbisFileBrowserGetRelPos());					
			currentEntry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos()+orbisFileBrowserGetRelPos());

			if(currentEntry!=NULL)
			{
				
				debugNetPrintf(INFO,"cross current entry %s customtype=%d\n",currentEntry->dir->name,currentEntry->dir->customtype);
				
				switch(currentEntry->dir->customtype)
				{	
					case FILE_TYPE_FOLDER:
						notSelected=1;
						//if(strcmp(currentEntry->dir->name, ".") == 0)
						//{
						//	flagfolder=1;
						//}
						//else
						//{
							//if(strcmp(currentEntry->dir->name, ".")!=0)
							//{
								debugNetPrintf(DEBUG,"cross selected folder level=%d base=%d rel=%d\n",orbisFileBrowserGetDirLevel(),orbisFileBrowserGetBasePos(),orbisFileBrowserGetRelPos());
								
								//orbisFileBrowserDirLevelUp(currentEntry->dir->name);
								//debugNetPrintf(DEBUG,"cross selected folder level=%d base=%d rel=%d\n",orbisFileBrowserGetDirLevel(),orbisFileBrowserGetBasePos(),orbisFileBrowserGetRelPos());
								flagfolder=1;
								//}
						//}
						break;
					case FILE_TYPE_GAME_ROM:
						
						//if(extension==FILE_TYPE_GAME_ROM)
						//{
							sprintf(path,"%s/%s",orbisFileBrowserGetListPath(),currentEntry->dir->name);
							debugNetPrintf(DEBUG,"cross selected entry game %s\n",path);
							
							LoadCart(path,slot,MAP_GUESS);
							//}
						//else
						//{
						//	debugNetPrintf(INFO,"wrong extension choose the right one\n");
							//}
						break;
					case FILE_TYPE_GAME_DSK:
						//if(extension==FILE_TYPE_GAME_DSK)
						//{
						
							sprintf(path,"%s/%s",orbisFileBrowserGetListPath(),currentEntry->dir->name);
							LoadFileDrive(path,drive);
							//}
						//else
						//{
						//	debugNetPrintf(INFO,"wrong extension choose the right one\n");
							//}
						break;
					case FILE_TYPE_CAS:
					//	if(extension==FILE_TYPE_CAS)
					//	{
							sprintf(path,"%s/%s",orbisFileBrowserGetListPath(),currentEntry->dir->name);
							ChangeTape(path);
					//	}
					//	else
					//	{
					//		debugNetPrintf(INFO,"wrong extension choose the right one\n");
					//	}
						break;
					default:
						comeBack=1;
						break;
				}
			}
		}	
	}
}
void getSizeString(char string[16], uint64_t size) 
{
	double double_size = (double)size;

	int i = 0;
	static char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
	while (double_size >= 1024.0) {
		double_size /= 1024.0;
		i++;
	}

	snprintf(string, 16, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}

void browserDraw()
{
	int i=0;
	uint32_t f1 ,f2;
	char dateString[20]; 
	char sizeString[16];
	if(flagfolder==1 && currentEntry)
	{
		if(strcmp(currentEntry->dir->name,".")!=0)
		{
			debugNetPrintf(DEBUG,"go to new directory %s\n",currentEntry->dir->name);
			//char rootpath[256];
			//sprintf(rootpath,"%s/%s",orbisFileBrowserGetListPath(),currentEntry->dir->name);
			//debugNetPrintf(DEBUG,"go to new directory %s\n",rootpath);
			if(strcmp(currentEntry->dir->name,"..")!=0)
			{
			orbisFileBrowserDirLevelUp(currentEntry->dir->name);
			}
			else
			{
				orbisFileBrowserDirLevelDown();
			}
			debugNetPrintf(DEBUG,"after orbisFileBrowserDirLevelUp\n");
		}
		else
		{
			orbisFileBrowserListRefresh();
		}
		flagfolder=0;
		
	}
	OrbisFileBrowserListEntry *entry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos());
	
	
		
		while(entry && i<MAX_ENTRIES)
		{
			
			if(entry->dir->customtype==FILE_TYPE_FOLDER)
			{
				if(folderTexture)
				{
					orbis2dDrawTexture(folderTexture,30,90+i*20);	
				}
				sprintf(sizeString,"%s","FOLDER");
				
			}
			else
			{
				if(fileTexture)
				{
					orbis2dDrawTexture(fileTexture,30,90+i*20+2);	
				}
				getSizeString(sizeString,entry->dir->size);
			}
			//debugNetPrintf(DEBUG,("%s %d\n",entry->name,entry->type);
			if(i==orbisFileBrowserGetRelPos())
			{
				
				f1 = 0xFF24FFBD;
				f2 = 0xFF24FFBD;
				update_gradient(&f1, &f2);
								
			}
			else
			{
				f1 = 0x80FFFFFF;
				f2 = 0x80FFFFFF;
				update_gradient(&f1, &f2);
			}
			//print_text(50+posx,90+i*20+posy,entry->dir->name);
			print_text(60,90+i*20+1,entry->dir->name);
		
			sprintf(dateString,"%02d/%02d/%04d %02d:%02d %s",
			entry->dir->mtime.day,
			entry->dir->mtime.month,
			entry->dir->mtime.year,
			entry->dir->mtime.hour>12?entry->dir->mtime.hour-12:entry->dir->mtime.hour,
			entry->dir->mtime.minute,
			entry->dir->mtime.hour>=12? "PM" : "AM");	
			print_text(960+8,90+i*20+1,dateString);
			print_text(960-8-16*strlen(sizeString)-posx,90+i*20+1,sizeString);
			
				
			entry=entry->next;	
			i++;			
			
		}
		f1 = 0xFF24FFBD,f2 = 0xFF24FFBD;
		update_gradient(&f1, &f2);
		print_text(30,52,orbisFileBrowserGetListPath());
	
}
void showBrowser()
{
	orbisAudioPause(0);
	
	uint32_t f1 = 0x80FFFFFF,f2 = 0x80FFFFFF;
	update_gradient(&f1, &f2);
	//debugNetPrintf(INFO,"in showBrowser %d\n",m);
	//orbisFileBrowserListRefresh();
	/*switch(m)
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
	}*/
	while(notSelected==1 && comeBack==0 && displayNumber!=0)
	{
		browserUpdateController();
		//wait for current display buffer
		orbis2dStartDrawing();

		// clear with background (default white) to the current display buffer 
		orbis2dClearBuffer(0);
		
		if(browserTexture)
		{
			orbis2dDrawTexture(browserTexture,0,0);	
		}
		//default red is here press X to random color
		//orbis2dDrawRectColor(x,w,y,h,color);
		if(displayNumber==1)
		browserDraw();	
		//flush and flip
		orbis2dFinishDrawing(flipArg);
			
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;
	}
	notSelected=1;
	comeBack=0;
	//msx go go
	displayNumber=0;
}