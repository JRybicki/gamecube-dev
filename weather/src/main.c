#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>

#include "display.h"
#include "sockettest.h"

void Initialize() {

	// libogc
	VIDEO_Init();
	PAD_Init();

	// source code
	DisplayInit();
}

int main(int argc, char **argv) {

	Initialize();

	printf ("Entered main\n");
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