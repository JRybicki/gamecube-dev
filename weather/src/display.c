#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>

#include "display.h"
#include "sockettest.h"

#include "textures_tpl.h"
#include "textures.h"

GXRModeObj *vmode = NULL;
u32* xfb[2] = {NULL, NULL};

TPLFile imagesTPL;
GXTexObj backgroundTexObj;
GXTlutObj backgroundTlutObj;

uiDrawObj_t* DrawContainer()
{
	uiDrawObj_t *event = calloc(1, sizeof(uiDrawObj_t));
	event->type = EV_CONTAINER;
	return event;
}

void InitTextures() {
	TPL_OpenTPLFromMemory(&imagesTPL, (void *)textures_tpl, textures_tpl_size);
	TPL_GetTextureCI(&imagesTPL, background, &backgroundTexObj, &backgroundTlutObj, GX_TLUT0);
	GX_InitTexObjUserData(&backgroundTexObj, &backgroundTlutObj);
}

void SetVideoMode() {
	vmode = VIDEO_GetPreferredMode(NULL);
	VIDEO_Configure(vmode);

	xfb[0] = (u32*)MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmode));
	xfb[1] = (u32*)MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmode));

	console_init (xfb[0], 20, 64, vmode->fbWidth, vmode->xfbHeight, vmode->fbWidth * 2);

	VIDEO_ClearFrameBuffer (vmode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer (vmode, xfb[1], COLOR_BLACK);

	VIDEO_SetNextFramebuffer(xfb[0]);
	VIDEO_SetBlack (FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync ();
}

void DisplayInit() {

	SetVideoMode();
}
