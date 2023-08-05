#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>

#include "display.h"
#include "sockettest.h"

void Initialise();

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	Initialise();

	printf ("\Entered main\n");
	printf("Configuring network ...\n");

	while(1) {

		VIDEO_WaitVSync();
		PAD_ScanPads();

		int buttonsDown = PAD_ButtonsDown(0);
		
		if (buttonsDown & PAD_BUTTON_START) {
			exit(0);
		}

	}

	return 0;
}
//---------------------------------------------------------------------------------
void Initialise() {
//---------------------------------------------------------------------------------

	void *framebuffer;

	VIDEO_Init();
	PAD_Init();
	
	GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
	framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(framebuffer,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(framebuffer);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	BackgroundInit();
}
