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

// clang-format off

// Standard lib includes
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vita sdk includes
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/power.h>

#include "graphics.h"

// single header ini lib
// i really dont wanna compile libiniparser
#define INI_IMPLEMENTATION
#include "ini.h"

// macros //
#define printf psvDebugScreenPrintf



// !@-- ENTRY POINT --@! //
int main() {
  // Menu variables
  int selected = 0;
  int lastSelected = -1;
  bool actionTriggered;

  // basic initializers
  psvDebugScreenInit();
  psvDebugScreenClear(COLOR_BLACK);

  // Creating a Struct for pad data
  SceCtrlData pad;

  // Menu items
  const char *menuOptions[3] = {
  	"Install Plugin\n", 
  	"Reboot System\n",
  	"Exit...\n"
  };

  int numOptions = sizeof(menuOptions) / sizeof(menuOptions[0]);

  while (true) {
    sceCtrlPeekBufferPositive(0, &pad, 1);

    if (pad.buttons & SCE_CTRL_DOWN) {
      selected = (selected + 1) % numOptions;
      actionTriggered = false;
      sceKernelDelayThread(160000);
    }
    if (pad.buttons & SCE_CTRL_UP) {
      selected = (selected - 1 + numOptions) % numOptions;
      actionTriggered = false;
      sceKernelDelayThread(160000);
    }

    // Force exit when select is pressed
    // mainly for debug stuff rnnn
    if (pad.buttons & SCE_CTRL_SELECT) {
      break;
    }

    // only print when we really need to print
    if (selected != lastSelected) {
      psvDebugScreenClear(COLOR_BLACK);
      psvDebugScreenSetFgColor(COLOR_WHITE);
      printf("Kernel Plugin Installer\n");
      printf("By AM7999\n\n");
      // Print every option
      for (int i = 0; i < numOptions; i++) {
        if (i == selected)
          printf(" > %s\n", menuOptions[i]);
        else
          printf("   %s\n", menuOptions[i]);
      }
      lastSelected = selected;
    }

    // Checking what option was pressed
    if ((pad.buttons & SCE_CTRL_CROSS) && !actionTriggered) {
      actionTriggered = true;
      if (selected == 0) {
        //ini_t ini = ini_parse("ux0:data/Plugin-Installer/plugin.ini", NULL);
        //char *plugin = ini_as_str(ini_get(ini_get_table(&ini, INI_ROOT), "plugin"), false);

        printf("Installing Plugin: <pluginName>\n"); // TODO: pull from a text file or ini? for name

        // dest and source paths
        char *srcPath = "ux0:app/KPLUG0001/kern/kernel_plugin.skprx";
        char *dstPath = "ur0:tai/kernel_plugin.skprx";

        // actually opening the files
        FILE *file = fopen(srcPath, "rb");
        FILE *copy = fopen(dstPath, "wb");

        // error handling
        if(file == NULL || copy == NULL) {
        	psvDebugScreenSetFgColor(COLOR_RED);
        	printf("Failed to open source or destination file.\n");
        	psvDebugScreenSetFgColor(COLOR_WHITE);
        	printf("Press X to continue...\n");

        	if(file) fclose(file);
        	if(copy) fclose(copy);

        	// Waiting for the user to press X
        	while(true) {
        		sceCtrlPeekBufferPositive(0, &pad, 1);
        		if(pad.buttons & SCE_CTRL_CROSS) break;
        		sceKernelDelayThread(10000);
        	}
        	continue;
        }
        
        // copying the file to copy
        char buf[1024];
        size_t bytes;
        while((bytes = fread(buf, 1, sizeof(buf), file)) > 0) {
        	fwrite(buf, 1, bytes, copy);
        }
        fclose(file);
        fclose(copy);

        // creating the config struct
        // actually goint to ur0 to grab the config for tai
        FILE *config = fopen("ur0:tai/config.txt", "a");

        if(config == NULL) {
        	psvDebugScreenSetFgColor(COLOR_RED);
        	printf("Failed to open config file: ur0:tai/config.txt\n");
        	psvDebugScreenSetFgColor(COLOR_WHITE);
        	printf("Press X to continue...\n");
        	while(true) {
        		sceCtrlPeekBufferPositive(0, &pad, 1);
        		if(pad.buttons & SCE_CTRL_CROSS) break;
        		sceKernelDelayThread(10000);
        	}
        	continue;
        }

        char buff[1024];
        size_t bytes2;
        while((bytes2 = fread(buf, 1, sizeof(buf), config)) > 0) {
        	fwrite(buff, 1, bytes2, config);
        }
        // sending the string "ur0:tai/kernel_plugin.skprx" to the buffer
        // snprintf(buff, sizeof(buff), "#kern_example\n*KERNEL\nur0:tai/kernel_plugin.skprx\n#---------------\n");
        // writing to the file
        // fputs(buff, config);
        // close to free memory
        fclose(config);

        // Change the foreground color to green so text prints green
        psvDebugScreenSetFgColor(COLOR_GREEN);
        printf("Plugin Installed successfully!\n");
        // Resetting the colors to print everything else normally
        psvDebugScreenSetFgColor(COLOR_WHITE);
        printf("Press X to continue...\n");
        while(true) {
        	sceCtrlPeekBufferPositive(0, &pad, 1);
        	if(pad.buttons & SCE_CTRL_CROSS) break;
        	sceKernelDelayThread(10000);
        }
      }
      if(selected == 1)
      	return scePowerRequestColdReset();
      if(selected == 2)
      	break;
    }
  }
  sceKernelExitProcess(0);
  return 0;
}
