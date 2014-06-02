/*
 *  Copyright (C) 2006 Ludovic Jacomme (ludovic.jacomme@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "global.h"
#include "psp_sdl.h"
#include "psp_kbd.h"
#include "psp_menu.h"
#include "psp_fmgr.h"
#include "psp_run.h"
#include "psp_menu_kbd.h"
#include "psp_menu_set.h"
#include "psp_menu_help.h"
#include "gp2x_psp.h"

extern SDL_Surface *back_surface;

# define MENU_SCREENSHOT   0
#if !defined(GCW0_MODE)
# define MENU_VOLUME       MENU_SCREENSHOT + 1
# define MENU_REWIND_K7    MENU_VOLUME + 1
#else
# define MENU_REWIND_K7    MENU_SCREENSHOT + 1
#endif

# define MENU_COMMAND      MENU_REWIND_K7 + 1

# define MENU_LOAD_SLOT    MENU_COMMAND + 1
# define MENU_SAVE_SLOT    MENU_LOAD_SLOT + 1
# define MENU_DEL_SLOT     MENU_SAVE_SLOT + 1

# define MENU_HELP         MENU_DEL_SLOT + 1

# define MENU_LOAD_DISK0   MENU_HELP + 1
# define MENU_LOAD_DISK1   MENU_LOAD_DISK0 + 1
# define MENU_LOAD_K7     MENU_LOAD_DISK1 + 1
# define MENU_LOAD_STATE  MENU_LOAD_K7 + 1

# define MENU_KEYBOARD    MENU_LOAD_STATE + 1
# define MENU_SETTINGS    MENU_KEYBOARD + 1

# define MENU_SOFT_RESET  MENU_SETTINGS + 1

# define MENU_BACK        MENU_SOFT_RESET + 1
# define MENU_EXIT        MENU_BACK + 1

# define MAX_MENU_ITEM (MENU_EXIT + 1)

  static menu_item_t menu_list[] =
  {
    "Save Screenshot :",
#if !defined(GCW0_MODE)
    "Volume          :",
#endif
    "Rewind K7       :",
    "Command         : ",

    "Load Slot",
    "Save Slot",
    "Delete Slot",

    "Help",

    "Load Disk 0",
    "Load Disk 1",
    "Load K7",
    "Load State",

    "Keyboard",
    "Settings",

    "Soft Reset",

    "Back to TO7-70",
    "Exit"
  };

  static int cur_menu_id = MENU_LOAD_K7;
  static int cur_slot    = 0;

  static int cur_command = 0;

#define MAX_COMMAND 4

  static char* command_name[MAX_COMMAND] = {
    "RUN", "LOAD", "LOADM", "DIR"
  };

void
psp_menu_display_save_name()
{
  char buffer[128];
  int Length;

  snprintf(buffer, 30, "Game: %s", THOM.thom_save_name);
  Length = strlen(buffer);
  psp_sdl_back2_print(300 - (6*Length), 5, buffer, PSP_MENU_TEXT2_COLOR);
}

void
string_fill_with_space(char *buffer, int size)
{
  int length = strlen(buffer);
  int index;

  for (index = length; index < size; index++) {
    buffer[index] = ' ';
  }
  buffer[size] = 0;
}

static void
psp_display_screen_menu(void)
{
  char buffer[64];
  int menu_id = 0;
  int slot_id = 0;
  int color   = 0;
  int x       = 0;
  int y       = 0;
  int y_step  = 0;

  psp_sdl_blit_background();

  x      = 10;
  y      =  5;
  y_step = 10;

  for (menu_id = 0; menu_id < MAX_MENU_ITEM; menu_id++) {
    color = PSP_MENU_TEXT_COLOR;
    if (cur_menu_id == menu_id) color = PSP_MENU_SEL_COLOR;
    else
    if (menu_id == MENU_EXIT) color = PSP_MENU_WARNING_COLOR;
    else
    if (menu_id == MENU_HELP) color = PSP_MENU_GREEN_COLOR;

    if (menu_id == MENU_COMMAND) {
      strcpy(buffer, menu_list[menu_id].title);
      strcat(buffer, command_name[cur_command]);
      string_fill_with_space(buffer, 25);
      psp_sdl_back2_print(x, y, buffer, color);
    } else {
      psp_sdl_back2_print(x, y, menu_list[menu_id].title, color);
    }

    if (menu_id == MENU_SCREENSHOT) {
      sprintf(buffer,"%d", THOM.psp_screenshot_id);
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(120, y, buffer, color);
    } else
#if !defined(GCW0_MODE)
    if (menu_id == MENU_VOLUME) {
      sprintf(buffer,"%d", gp2xGetSoundVolume());
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(120, y, buffer, color);
    } else
#endif
    if (menu_id == MENU_REWIND_K7) {
      sprintf(buffer,"%d", to7_GetK7Counter());
      string_fill_with_space(buffer, 6);
      psp_sdl_back2_print(120, y, buffer, color);
      //y += y_step;
    } else
    if (menu_id == MENU_DEL_SLOT) {
      y += y_step;
    } else
    if (menu_id == MENU_COMMAND) {
      y += y_step;
    } else
    if (menu_id == MENU_LOAD_DISK1) {
      //y += y_step;
    } else
    if (menu_id == MENU_SETTINGS) {
      y += y_step;
    } else
    if (menu_id == MENU_SOFT_RESET) {
      //y += y_step;
    } else
    if (menu_id == MENU_LOAD_STATE) {
      y += y_step;
    } else
    if (menu_id == MENU_BACK) {
      y += y_step;
    } else
    if (menu_id == MENU_HELP) {
      y += y_step;
    }

    y += y_step;
  }

  y_step = 10;
#if defined(GCW0_MODE)
  y      = 45;
#else
  y      = 55;
#endif

  for (slot_id = 0; slot_id < THOM_MAX_SAVE_STATE; slot_id++) {
    if (slot_id == cur_slot) color = PSP_MENU_SEL2_COLOR;
    else                     color = PSP_MENU_TEXT_COLOR;

    if (THOM.thom_save_state[slot_id].used) {
      sprintf(buffer, "- used");
    } else {
      sprintf(buffer, "- empty");
    }
    string_fill_with_space(buffer, 32);
    psp_sdl_back2_print(100, y, buffer, color);

    y += y_step;
  }

  if (THOM.thom_save_state[cur_slot].thumb) {
    psp_sdl_blit_thumb(150, 40, THOM.thom_save_state[cur_slot].surface);
  } else {
    psp_sdl_blit_thumb(150, 40, thumb_surface);
  }

  psp_menu_display_save_name();
}

static void
psp_main_menu_cold_reset(void)
{
  /* Reset ! */
  psp_display_screen_menu();
  psp_sdl_back2_print( 90, 145, "Cold Reset TO7-70 !",
                     PSP_MENU_WARNING_COLOR);
  psp_sdl_flip();
  emulator_cold_reset();
  sleep(1);
}

static void
psp_main_menu_soft_reset(void)
{
  /* Reset ! */
  psp_display_screen_menu();
  psp_sdl_back2_print( 90, 145, "Soft Reset TO7-70 !",
                     PSP_MENU_WARNING_COLOR);
  psp_sdl_flip();
  emulator_soft_reset();
  sleep(1);
}

static void
psp_main_menu_cmd_loadm(void)
{
  char *RunName;
  RunName = psp_run_search(THOM.thom_save_name);
  if (RunName != (char *)0 ) {
    psp_kbd_run_command(RunName);
  } else
  if ((THOM.thom_load_mode == THOM_LOAD_DISK_MODE) ||
      (THOM.thom_basic_128 == 0                  )) {
    psp_kbd_run_command("LOADM\"\",,R\n");
  } else {
    psp_kbd_run_command("LOADM\"CASS:\",,R\n");
  }
}

static void
psp_main_menu_cmd_dir(void)
{
  if (THOM.thom_load_mode == THOM_LOAD_DISK_MODE) {
    psp_kbd_run_command("DIR\n");
  }
}

static void
psp_main_menu_cmd_load(void)
{
  char *RunName;
  RunName = psp_run_search(THOM.thom_save_name);
  if (RunName != (char *)0 ) {
    psp_kbd_run_command(RunName);
  } else
  if ((THOM.thom_load_mode == THOM_LOAD_DISK_MODE) ||
      (THOM.thom_basic_128 == 0                  )) {
    psp_kbd_run_command("LOAD\"\",,R\n");
  } else {
    psp_kbd_run_command("LOAD\"CASS:\",,R\n");
  }
}

static void
psp_main_menu_cmd_run(void)
{
  char *RunName;
  RunName = psp_run_search(THOM.thom_save_name);
  if (RunName != (char *)0 ) {
    psp_kbd_run_command(RunName);
  } else
  if ((THOM.thom_load_mode == THOM_LOAD_DISK_MODE) ||
      (THOM.thom_basic_128 == 0                  )) {
    psp_kbd_run_command("RUN\"\"\n");
  } else {
    psp_kbd_run_command("RUN\"CASS:\"\n");
  }
}

static void
psp_main_menu_cur_command(int step)
{
  if (step > 0) {
    cur_command++;
    if (cur_command >= MAX_COMMAND) cur_command = 0;
  } else
  if (step < 0) {
    cur_command--;
    if (cur_command < 0) cur_command = MAX_COMMAND - 1;
  }
}

static void
psp_main_menu_command(void)
{
  switch (cur_command) {
    case 0: psp_main_menu_cmd_run();    break;
    case 1: psp_main_menu_cmd_load();   break;
    case 2: psp_main_menu_cmd_loadm();  break;
    case 3: psp_main_menu_cmd_dir();    break;
  }
}

static int
psp_main_menu_load(int format, int disk_id)
{
  int ret;

  ret = psp_fmgr_menu(format, disk_id);
  if (ret ==  1) /* load OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "File loaded !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
    return 1;
  }
  else
  if (ret == -1) /* Load Error */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "Can't load file !",
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  return 0;
}

/* used by command keys */
int
psp_main_menu_load_current()
{
  if (THOM.thom_save_state[cur_slot].used) {
    return thom_snapshot_load_slot(cur_slot);
  }
  return -1;
}

int
psp_main_menu_save_current()
{
  return thom_snapshot_save_slot(cur_slot);
}

static int
psp_main_menu_load_slot()
{
  int error;

  if (! THOM.thom_save_state[cur_slot].used) {

    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "Slot is empty !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);

    return;
  }

  error = thom_snapshot_load_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "File loaded !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);

    return 1;
  }
  else
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "Can't load file !",
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  return 0;
}

static void
psp_main_menu_save_slot()
{
  int error;
  error = thom_snapshot_save_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "File saved !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  else
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "Can't save file !",
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_main_menu_del_slot()
{
  int error;

  if (! THOM.thom_save_state[cur_slot].used) {

    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "Slot is empty !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);

    return;
  }

  error = thom_snapshot_del_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "File deleted !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  else
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 145, "Can't delete file !",
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_main_menu_rewind_k7()
{
  int error;

  error = thom_rewind_k7();

  psp_display_screen_menu();
  psp_sdl_back2_print( 90, 145, "K7 rewinded !",
                     PSP_MENU_NOTE_COLOR);
  psp_sdl_flip();
  sleep(1);
}

static void
psp_main_menu_cur_slot(int step)
{
  if (step == 1) {
    cur_slot++; if (cur_slot >= THOM_MAX_SAVE_STATE) cur_slot = 0;
  } else if (step == -1) {
    cur_slot--; if (cur_slot < 0) cur_slot = THOM_MAX_SAVE_STATE - 1;
  }
}

static void
psp_main_menu_screenshot(void)
{
  psp_screenshot_mode = 10;
}

static void
psp_main_menu_volume(int step)
{
  if (step == 1) {
    gp2xIncreaseVolume();
  } else if (step == -1) {
    gp2xDecreaseVolume();
  }
}

int
psp_main_menu_exit(void)
{
  gp2xCtrlData c;

  psp_display_screen_menu();
  psp_sdl_back2_print( 90, 145, "press B to confirm !", PSP_MENU_WARNING_COLOR);
  psp_sdl_flip();

  psp_kbd_wait_no_button();

  do
  {
    gp2xCtrlReadBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;

    if (c.Buttons & GP2X_CTRL_CROSS) {
      psp_sdl_clear_screen(0);
      psp_sdl_flip();
      psp_sdl_clear_screen(0);
      psp_sdl_flip();
      psp_sdl_exit(0);
    }

  } while (c.Buttons == 0);

  psp_kbd_wait_no_button();

  return 0;
}

int
psp_main_menu(void)
{
  gp2xCtrlData c;
  long        new_pad;
  long        old_pad;
  int         last_time;
  int         end_menu;

  thom_audio_pause();

  psp_kbd_wait_no_button();

  old_pad   = 0;
  last_time = 0;
  end_menu  = 0;


  while (! end_menu)
  {
    psp_display_screen_menu();
    psp_sdl_flip();

    while (1)
    {
      gp2xCtrlReadBufferPositive(&c, 1);
      c.Buttons &= PSP_ALL_BUTTON_MASK;

      if (c.Buttons) break;
    }

    new_pad = c.Buttons;

    if ((old_pad != new_pad) || ((c.TimeStamp - last_time) > PSP_MENU_MIN_TIME)) {
      last_time = c.TimeStamp;
      old_pad = new_pad;

    } else continue;

    if ((c.Buttons & GP2X_CTRL_RTRIGGER) == GP2X_CTRL_RTRIGGER) {
      psp_main_menu_cold_reset();
      end_menu = 1;
    } else
    if ((new_pad == GP2X_CTRL_LEFT ) ||
        (new_pad == GP2X_CTRL_RIGHT) ||
        (new_pad == GP2X_CTRL_CROSS) ||
        (new_pad == GP2X_CTRL_CIRCLE))
    {
      int step = 0;

      if (new_pad & GP2X_CTRL_RIGHT) {
        step = 1;
      } else
      if (new_pad & GP2X_CTRL_LEFT) {
        step = -1;
      }

      switch (cur_menu_id )
      {
        case MENU_LOAD_SLOT : if (step) psp_main_menu_cur_slot(step);
                              else if (psp_main_menu_load_slot()) end_menu = 1;
        break;
        case MENU_SAVE_SLOT : if (step) psp_main_menu_cur_slot(step);
                              else      psp_main_menu_save_slot();
        break;
        case MENU_DEL_SLOT  : if (step) psp_main_menu_cur_slot(step);
                              else      psp_main_menu_del_slot();
        break;

        case MENU_LOAD_K7    : if (psp_main_menu_load(FMGR_FORMAT_K7, 0)) end_menu = 1;
                               old_pad = new_pad = 0;
        break;

        case MENU_LOAD_DISK0 : if (psp_main_menu_load(FMGR_FORMAT_SAP, 0)) end_menu = 1;
                               old_pad = new_pad = 0;
        break;
        case MENU_LOAD_DISK1 : if (psp_main_menu_load(FMGR_FORMAT_SAP, 1)) end_menu = 1;
                               old_pad = new_pad = 0;
        break;
        case MENU_LOAD_STATE : if (psp_main_menu_load(FMGR_FORMAT_STATE, 0)) end_menu = 1;
                               old_pad = new_pad = 0;
        break;
        case MENU_COMMAND   : if (step) psp_main_menu_cur_command(step);
                              else {
                                 psp_main_menu_command();
                                 end_menu = 1;
                              }
        break;

        case MENU_KEYBOARD   : psp_keyboard_menu();
                               old_pad = new_pad = 0;
        break;

        case MENU_SETTINGS   : psp_settings_menu();
                               old_pad = new_pad = 0;
        break;

        case MENU_REWIND_K7  : psp_main_menu_rewind_k7();
                               old_pad = new_pad = 0;
        break;
        case MENU_SCREENSHOT : psp_main_menu_screenshot();
                               end_menu = 1;
        break;
#if !defined(GCW0_MODE)
        case MENU_VOLUME     : psp_main_menu_volume(step);
                               old_pad = new_pad = 0;
        break;
#endif
        case MENU_SOFT_RESET : psp_main_menu_soft_reset();
                               end_menu = 1;
        break;

        case MENU_BACK      : end_menu = 1;
        break;

        case MENU_EXIT      : psp_main_menu_exit();
        break;

        case MENU_HELP      : psp_help_menu();
                              old_pad = new_pad = 0;
        break;
      }

    } else
    if(new_pad & GP2X_CTRL_UP) {

      if (cur_menu_id > 0) cur_menu_id--;
      else                 cur_menu_id = MAX_MENU_ITEM-1;

    } else
    if(new_pad & GP2X_CTRL_DOWN) {

      if (cur_menu_id < (MAX_MENU_ITEM-1)) cur_menu_id++;
      else                                 cur_menu_id = 0;

    } else
    if(new_pad & GP2X_CTRL_SQUARE) {
      /* Cancel */
      end_menu = -1;
    } else
    if(new_pad & GP2X_CTRL_SELECT) {
      /* Back to THOM */
      end_menu = 1;
    }
  }

  psp_kbd_wait_no_button();

  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR );
  psp_sdl_flip();
  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR );
  psp_sdl_flip();

  thom_audio_resume();

  return 1;
}

