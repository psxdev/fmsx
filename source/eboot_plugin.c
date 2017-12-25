/*
 * fMSX orbis launcher   for PlayStation 4 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/psxdev/fmsx
 */
#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <systemservice.h>
#include <logdebug.h>
#include <orbis2d.h>
#include <orbisPad.h>
#include <orbisAudio.h>
#include <modplayer.h>
#include <ps4link.h>
#include <debugnet.h>
#include <orbisFileBrowser.h>
#include "menu.h"

int x=1280/2;
int y=720/2;
int w=1280/64;
int h=1280/64;
int step=10;


int64_t flipArg=0;
int R,G,B;
uint32_t color=0x80ff0000;
int flag=0;
//int closeMenu=0;
Orbis2dConfig *conf;
OrbisPadConfig *confPad;
Orbis2dTexture *icons=NULL;
int col=0;
void finishApp()
{
	orbis2dFinish();
	orbisAudioFinish();
	orbisKeyboardFinish();
	orbisPadFinish();
	ps4LinkFinish();
		
}
void initApp()
{
	int ret;

	ret=ps4LinkInit("192.168.1.3",0x4711,0x4712,0x4712,INFO);
	if(!ret)
	{
		ps4LinkFinish();
		return;
	}
	while(!ps4LinkRequestsIsConnected())
	{
		
	}
	debugNetPrintf(DEBUG,"[PS4LINK] Initialized and connected from pc/mac ready to receive commands\n");
	
	orbisFileBrowserInit("host0:GAMES");
	
	//hide playroom splash
	
	sceSystemServiceHideSplashScreen();
	
	
	ret=orbisPadInit();
	
	if(ret==1)
	{
		
	
		confPad=orbisPadGetConf();
		ret=orbis2dInit();
		if(ret==1)
		{
			conf=orbis2dGetConf();
			flag=1;
			ret=orbisAudioInit();
			if(ret==1)
			{
				ret=orbisAudioInitChannel(ORBISAUDIO_CHANNEL_MAIN,512,48000,ORBISAUDIO_FORMAT_S16_MONO);
				//ret=orbisAudioInitChannel(ORBISAUDIO_CHANNEL_MAIN,1024,48000,ORBISAUDIO_FORMAT_S16_STEREO);
				debugNetPrintf(DEBUG,"orbisKeyboardInit %d\n",orbisKeyboardInit());
				sleep(1);
				debugNetPrintf(DEBUG,"orbisKeyboardInit %d\n",orbisKeyboardOpen());
			
			}
		}
	}
	
}
int main(uint64_t stackbase, uint64_t othervalue) 
{
	int ret;

	initApp();
	
	
	/*Mod_Init(0);
	Mod_Load("host0:zweifeld.mod");
	Mod_Play();*/
    orbisAudioResume(0);
	menuInit();
	
	
	
	
	while(flag)
	{
		fmsxInit();
	}
	
	orbisAudioResume(0);
	//Mod_End();
	//wait for current display buffer
	orbis2dStartDrawing();

	// clear with background (default white) to the current display buffer 
	orbis2dClearBuffer();
					
	//flush and flip
	orbis2dFinishDrawing(flipArg);
			
	//swap buffers
	orbis2dSwapBuffers();
	
	finishApp();

	exit(0);

	return 0;
}
