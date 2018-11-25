#include <orbisPad.h>
#include <orbis2d.h>
#include <ps4link.h>
#include "menu.h"
#include "browser.h"
#include "MSX.h"

extern int displayNumber;
extern int64_t flipArg;
extern Orbis2dTexture *creditsTexture;



void creditsUpdateController()
{
	int ret;
	unsigned int buttons=0;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		
		if(orbisPadGetButtonPressed(ORBISPAD_UP))
		{
			debugNetPrintf(DEBUG,"Up pressed\n");
			displayNumber=2;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_DOWN))
		{
			debugNetPrintf(DEBUG,"Down pressed\n");
			displayNumber=2;
		}						
		if(orbisPadGetButtonPressed(ORBISPAD_RIGHT))
		{
			debugNetPrintf(DEBUG,"Right pressed\n");
			displayNumber=2;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_LEFT))
		{
			debugNetPrintf(DEBUG,"Left pressed\n");
			displayNumber=2;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_TRIANGLE))
		{
			debugNetPrintf(DEBUG,"Triangle pressed exit\n");
			displayNumber=2;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CIRCLE))
		{
			debugNetPrintf(DEBUG,"Circle pressed come back to browser\n");
			displayNumber=2;			
		}
		if(orbisPadGetButtonPressed(ORBISPAD_CROSS))
		{
			debugNetPrintf(DEBUG,"Cross pressed\n");
			displayNumber=2;
		}
		if(orbisPadGetButtonPressed(ORBISPAD_SQUARE))
		{
			debugNetPrintf(DEBUG,"Square pressed\n");
			displayNumber=1;			
		}
	}
}
void showCredits()
{
	displayNumber=3;
	int f1=0x80000000;
	int f2=0x80000000;
	while(displayNumber==3)
	{
		creditsUpdateController();
		
		//wait for current display buffer
		orbis2dStartDrawing();

		// clear with background (default white) to the current display buffer 
		orbis2dClearBuffer(0);
			
		//default red is here press X to random color
		//orbis2dDrawRectColor(x,w,y,h,color);
		//draw credits texture
		if(creditsTexture)
		{
			orbis2dDrawTexture(creditsTexture,0,0);	
		}
		
		//flush and flip
		orbis2dFinishDrawing(flipArg);
			
		//swap buffers
		orbis2dSwapBuffers();
		flipArg++;
		
	}
}