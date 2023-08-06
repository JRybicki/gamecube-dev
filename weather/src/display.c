#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>

#include "display.h"
#include "sockettest.h"

GXRModeObj *vmode = NULL;
u32* xfb = NULL;

void InitTextures() {
	
}

void DisplayInit() {
	vmode = VIDEO_GetPreferredMode(NULL);
	VIDEO_Configure(vmode);

	xfb = (u32*)MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmode));
	console_init (xfb, 20, 64, vmode->fbWidth, vmode->xfbHeight, vmode->fbWidth * 2);

	VIDEO_ClearFrameBuffer (vmode, xfb, COLOR_BLACK);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack (FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync ();
	
/* 	GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
	framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(framebuffer,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(framebuffer);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync(); */
}
