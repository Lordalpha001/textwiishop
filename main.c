#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

static u32 *xfb;
static GXRModeObj *rmode;

void Initialise() {
    VIDEO_Init();
    PAD_Init();
    WPAD_Init(); 
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) {
        VIDEO_WaitVSync();
    }
}

void DrawOptions(int selectedOption) {
    printf("\x1b[2J"); 
    printf("\x1b[0;0H");
	
	printf("idkShop Channel");
	
	printf("\n");


    printf("Test1");
    if (selectedOption == 0) {
        printf("  <-"); 
    }
    printf("\n");

    printf("Test2");
    if (selectedOption == 1) {
        printf("  <-"); 
    }
    printf("\n");

    VIDEO_Flush();
}

int main() {
    Initialise();
    
    int selectedOption = 0;
    int exitApp = 0;

    DrawOptions(selectedOption); 

    while (!exitApp) {
        WPAD_ScanPads(); 
        u32 pressed = WPAD_ButtonsDown(0);
        
        if (pressed & WPAD_BUTTON_DOWN) {
            selectedOption++;
            if (selectedOption > 1) {
                selectedOption = 1; 
            }
            DrawOptions(selectedOption); 
        }

        if (pressed & WPAD_BUTTON_UP) {
            selectedOption--;
            if (selectedOption < 0) {
                selectedOption = 0; 
            }
            DrawOptions(selectedOption); 
        }

        if (pressed & WPAD_BUTTON_A) {
            if (selectedOption == 0) {
                printf("Pressed Test1...\n");
            } else if (selectedOption == 1) {
                printf("Pressed Test2...\n");
            }
            exitApp = 1; 
        }

        VIDEO_WaitVSync(); 
    }

    printf("Exiting WII Shoip...\n");

    return 0;
}
