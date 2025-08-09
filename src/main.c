/*
 	Vita Kernel Plugin Installer
    Copyright (C) 2025  Angel Martin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Includes
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/vshbridge.h>
#include "graphics.h"

// macro //
#define printf psvDebugScreenPrintf
#define clear() printf("\033[2J\033[1;1H");

int main() {
	int selected = 0;
	int lastSelected = -1;
	bool actionTriggered;

	psvDebugScreenInit();
	psvDebugScreenClear(COLOR_BLACK);

	SceCtrlData pad;

	const char* menuOptions[2] = {
		"Install Plugin\n",
		"Exit...\n"
	};

	int numOptions = sizeof(menuOptions) / sizeof(menuOptions[0]);

	while(true) {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        if (pad.buttons & SCE_CTRL_DOWN) {
            selected = (selected + 1) % numOptions;
			actionTriggered = false;
            sceKernelDelayThread(200000); // Delay to prevent overscroll
        }
        if (pad.buttons & SCE_CTRL_UP) {
            selected = (selected - 1 + numOptions) % numOptions;
			actionTriggered = false;
            sceKernelDelayThread(200000);
        }

		if(pad.buttons & SCE_CTRL_SELECT) {
			break;
		}

		// only print when we really need to print
		if(selected != lastSelected) {
			psvDebugScreenClear(COLOR_BLACK);
			printf("Kernel Plugin Installer\n");
			printf("By AM7999\n\n");
			// Print every option
			for (int i = 0; i < numOptions; i++) {
            	if (i == selected) {
                	printf(" > %s\n", menuOptions[i]);
            	} else {
                	printf("   %s\n", menuOptions[i]);
            	}
        	}
			lastSelected = selected;
		}

		// Checking what option was pressed
		if((pad.buttons & SCE_CTRL_CROSS) && !actionTriggered) {
			actionTriggered = true;
			if(selected == 0) {
				printf("hiiii");
				sceKernelDelayThread(10000000);
			}
			if(selected == 1) {
				break;
			}
		}
	}

	sceKernelExitProcess(0);
    return 0;
}