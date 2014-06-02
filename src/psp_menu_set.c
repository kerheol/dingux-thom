/*
 *  Copyright (C) 2009 Ludovic Jacomme (ludovic.jacomme@gmail.com)
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
#include "psp_menu_kbd.h"
#include "psp_menu_set.h"

extern SDL_Surface *back_surface;

# define MENU_SET_SOUND         0
# define MENU_SET_JOYSTICK      MENU_SET_SOUND + 1
# define MENU_SET_SPEED_LIMIT   MENU_SET_JOYSTICK + 1
# define MENU_SET_SKIP_FPS      MENU_SET_SPEED_LIMIT + 1
# define MENU_SET_VIEW_FPS      MENU_SET_SKIP_FPS + 1
# define MENU_SET_RENDER        MENU_SET_VIEW_FPS + 1
# define MENU_SET_ANALOG        MENU_SET_RENDER + 1
# define MENU_SET_AUTOFIRE      MENU_SET_ANALOG + 1
# define MENU_SET_DISPLAY_LR    MENU_SET_AUTOFIRE + 1
#if !defined(GCW0_MODE)
# define MENU_SET_CLOCK         MENU_SET_DISPLAY_LR + 1
# define MENU_SET_BASIC         MENU_SET_CLOCK + 1
#else
# define MENU_SET_BASIC         MENU_SET_DISPLAY_LR + 1
#endif

# define MENU_SET_LOAD          MENU_SET_BASIC + 1
# define MENU_SET_SAVE          MENU_SET_LOAD + 1
# define MENU_SET_RESET         MENU_SET_SAVE + 1

# define MENU_SET_BACK          MENU_SET_RESET + 1

# define MAX_MENU_SET_ITEM (MENU_SET_BACK + 1)

  static menu_item_t menu_list[] =
  {
    { "Sound enable       :"},
    { "Active Joystick    :"},
    { "Speed limiter      :"},
    { "Skip frame         :"},
    { "Display fps        :"},
    { "Render mode        :"},
    { "Joystick as cursor :"},
    { "Auto fire period   :"},
    { "Display LR led     :"},
#if !defined(GCW0_MODE)
    { "Clock frequency    :"},
#endif
    { "Basic version      :"},
    { "Load settings"       },
    { "Save settings"       },
    { "Reset settings"      },
    { "Back to Menu"        }
  };

  static int cur_menu_id = MENU_SET_LOAD;

  static int thom_snd_enable       = 0;
  static int thom_view_fps         = 0;
  static int thom_speed_limiter    = 50;
  static int thom_render_mode      = 0;
  static int psp_joystick_cursor   = 0;
  static int psp_active_joystick   = 0;
  static int psp_cpu_clock         = 200;
  static int psp_display_lr        = 1;
  static int thom_skip_fps         = 0;
  static int thom_auto_fire_period  = 0;
  static int thom_basic_128        = 1;

static void
psp_settings_menu_reset(void);

static void
psp_display_screen_settings_menu(void)
{
  char buffer[64];
  int menu_id = 0;
  int color   = 0;
  int x       = 0;
  int y       = 0;
  int y_step  = 0;

  psp_sdl_blit_background();

  x      = 10;
  y      =  5;
  y_step = 10;

  for (menu_id = 0; menu_id < MAX_MENU_SET_ITEM; menu_id++) {
    color = PSP_MENU_TEXT_COLOR;
    if (cur_menu_id == menu_id) color = PSP_MENU_SEL_COLOR;

    psp_sdl_back2_print(x, y, menu_list[menu_id].title, color);

    if (menu_id == MENU_SET_SOUND) {
      if (thom_snd_enable) strcpy(buffer,"yes");
      else                 strcpy(buffer,"no ");
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_VIEW_FPS) {
      if (thom_view_fps) strcpy(buffer,"yes");
      else                strcpy(buffer,"no ");
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_ANALOG) {
      if (psp_joystick_cursor) strcpy(buffer,"yes");
      else                     strcpy(buffer,"no ");
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_AUTOFIRE) {
      sprintf(buffer,"%d", thom_auto_fire_period+1);
      string_fill_with_space(buffer, 7);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_DISPLAY_LR) {
      if (psp_display_lr) strcpy(buffer,"yes");
      else                strcpy(buffer,"no ");
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_JOYSTICK) {
      if (psp_active_joystick) strcpy(buffer,"player 2");
      else                     strcpy(buffer,"player 1 ");
      string_fill_with_space(buffer, 10);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_SKIP_FPS) {
      sprintf(buffer,"%d", thom_skip_fps);
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_SPEED_LIMIT) {
      if (thom_speed_limiter == 0)  strcpy(buffer, "no");
      else sprintf(buffer, "%d fps", thom_speed_limiter);
      string_fill_with_space(buffer, 7);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
    if (menu_id == MENU_SET_RENDER) {

      strcpy(buffer, "normal");
      string_fill_with_space(buffer, 13);
      psp_sdl_back2_print(140, y, buffer, color);
   } else
#if !defined(GCW0_MODE)
    if (menu_id == MENU_SET_CLOCK) {
      sprintf(buffer,"%d", psp_cpu_clock);
      string_fill_with_space(buffer, 4);
      psp_sdl_back2_print(140, y, buffer, color);
    } else
#endif
    if (menu_id == MENU_SET_BASIC) {
      if (thom_basic_128) strcpy(buffer, "1.0 128");
      else                strcpy(buffer, "1.0");
      string_fill_with_space(buffer, 10);
      psp_sdl_back2_print(140, y, buffer, color);
      y += y_step;
    } else
    if (menu_id == MENU_SET_RESET) {
      y += y_step;
    }

    y += y_step;
  }

  psp_menu_display_save_name();
}

static void
psp_settings_menu_clock(int step)
{
#if !defined(GCW0_MODE)
  if (step > 0) {
    psp_cpu_clock += 10;
    if (psp_cpu_clock > GP2X_MAX_CLOCK) psp_cpu_clock = GP2X_MAX_CLOCK;
  } else {
    psp_cpu_clock -= 10;
    if (psp_cpu_clock < GP2X_MIN_CLOCK) psp_cpu_clock = GP2X_MIN_CLOCK;
  }
#endif
}

static void
psp_settings_menu_skip_fps(int step)
{
  if (step > 0) {
    if (thom_skip_fps < 25) thom_skip_fps++;
  } else {
    if (thom_skip_fps > 0) thom_skip_fps--;
  }
}

static void
psp_settings_menu_autofire(int step)
{
  if (step > 0) {
    if (thom_auto_fire_period < 19) thom_auto_fire_period++;
  } else {
    if (thom_auto_fire_period >  0) thom_auto_fire_period--;
  }
}

void
psp_settings_menu_limiter(int step)
{
  if (step > 0) {
    if (thom_speed_limiter < 60) thom_speed_limiter++;
    else                          thom_speed_limiter  = 0;
  } else {
    if (thom_speed_limiter >  0) thom_speed_limiter--;
    else                          thom_speed_limiter  = 60;
  }
}

static void
psp_settings_menu_render(int step)
{
  if (step > 0) {
    if (thom_render_mode < THOM_LAST_RENDER) thom_render_mode++;
    else                                     thom_render_mode = 0;
  } else {
    if (thom_render_mode > 0) thom_render_mode--;
    else                      thom_render_mode = THOM_LAST_RENDER;
  }
}

static void
psp_settings_menu_init(void)
{
  thom_snd_enable      = THOM.thom_snd_enable;
  thom_render_mode     = THOM.thom_render_mode;
  thom_speed_limiter   = THOM.thom_speed_limiter;
  thom_skip_fps        = THOM.psp_skip_max_frame;
  thom_view_fps        = THOM.thom_view_fps;
  thom_basic_128       = THOM.thom_basic_128;
  psp_cpu_clock        = THOM.psp_cpu_clock;
  psp_joystick_cursor  = THOM.psp_joystick_cursor;
  psp_active_joystick  = THOM.psp_active_joystick;
  thom_auto_fire_period = THOM.thom_auto_fire_period;
  psp_display_lr       = THOM.psp_display_lr;
}

static void
psp_settings_menu_load(int format)
{
  int ret;

  ret = psp_fmgr_menu(format, 0);
  if (ret ==  1) /* load OK */
  {
    psp_display_screen_settings_menu();
    psp_sdl_back2_print(170, 110, "File loaded !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
    psp_settings_menu_init();
  }
  else
  if (ret == -1) /* Load Error */
  {
    psp_display_screen_settings_menu();
    psp_sdl_back2_print(170, 110, "Can't load file !",
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_settings_menu_validate(void)
{
  if (THOM.thom_basic_128 != thom_basic_128) {
    thom_set_basic_mode(thom_basic_128);
  }

  THOM.thom_snd_enable     = thom_snd_enable;
  THOM.thom_render_mode    = thom_render_mode;
  THOM.psp_cpu_clock       = psp_cpu_clock;
  THOM.thom_speed_limiter  = thom_speed_limiter;

  THOM.psp_joystick_cursor = psp_joystick_cursor;
  THOM.psp_active_joystick = psp_active_joystick;
  THOM.psp_skip_max_frame  = thom_skip_fps;
  THOM.psp_skip_cur_frame  = 0;

  THOM.thom_view_fps       = thom_view_fps;
  THOM.psp_display_lr      = psp_display_lr;
  THOM.thom_auto_fire_period  = thom_auto_fire_period;

  myPowerSetClockFrequency(THOM.psp_cpu_clock);
}

static void
psp_settings_menu_save_config()
{
  int error;

  psp_settings_menu_validate();
  error = thom_save_settings();

  if (! error) /* save OK */
  {
    psp_display_screen_settings_menu();
    psp_sdl_back2_print(170, 110, "File saved !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  else
  {
    psp_display_screen_settings_menu();
    psp_sdl_back2_print(170, 110, "Can't save file !",
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
}

int
psp_settings_menu_exit(void)
{
  gp2xCtrlData c;

  psp_display_screen_settings_menu();
  psp_sdl_back2_print(170, 110, "press X to confirm !", PSP_MENU_WARNING_COLOR);
  psp_sdl_flip();

  psp_kbd_wait_no_button();

  do
  {
    gp2xCtrlReadBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;

    if (c.Buttons & GP2X_CTRL_CROSS) psp_sdl_exit(0);

  } while (c.Buttons == 0);

  psp_kbd_wait_no_button();

  return 0;
}

static void
psp_settings_menu_save()
{
  int error;

  psp_settings_menu_validate();
  error = thom_save_settings();

  if (! error) /* save OK */
  {
    psp_display_screen_settings_menu();
    psp_sdl_back2_print(170, 110, "File saved !",
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  else
  {
    psp_display_screen_settings_menu();
    psp_sdl_back2_print(170, 110, "Can't save file !",
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_settings_menu_reset(void)
{
  psp_display_screen_settings_menu();
  psp_sdl_back2_print(170, 110, "Reset Settings !",
                     PSP_MENU_WARNING_COLOR);
  psp_sdl_flip();
  thom_default_settings();
  psp_settings_menu_init();
  sleep(1);
}

int
psp_settings_menu(void)
{
  gp2xCtrlData c;
  long        new_pad;
  long        old_pad;
  int         last_time;
  int         end_menu;

  psp_kbd_wait_no_button();

  old_pad   = 0;
  last_time = 0;
  end_menu  = 0;

  psp_settings_menu_init();


  while (! end_menu)
  {
    psp_display_screen_settings_menu();
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
      psp_settings_menu_reset();
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
        case MENU_SET_SOUND      : thom_snd_enable = ! thom_snd_enable;
        break;
        case MENU_SET_SPEED_LIMIT : psp_settings_menu_limiter( step );
        break;
        case MENU_SET_VIEW_FPS   : thom_view_fps = ! thom_view_fps;
        break;
        case MENU_SET_DISPLAY_LR : psp_display_lr = ! psp_display_lr;
        break;
        case MENU_SET_AUTOFIRE  : psp_settings_menu_autofire( step );
        break;
        case MENU_SET_SKIP_FPS   : psp_settings_menu_skip_fps( step );
        break;
        case MENU_SET_ANALOG     : psp_joystick_cursor = ! psp_joystick_cursor;
        break;
        case MENU_SET_JOYSTICK   : psp_active_joystick = ! psp_active_joystick;
        break;
        case MENU_SET_RENDER     : psp_settings_menu_render( step );
        break;
        case MENU_SET_BASIC      : thom_basic_128 = ! thom_basic_128;
        break;
#if !defined(GCW0_MODE)
        case MENU_SET_CLOCK      : psp_settings_menu_clock( step );
        break;
#endif
        case MENU_SET_LOAD       : psp_settings_menu_load(FMGR_FORMAT_SET);
                                   old_pad = new_pad = 0;
        break;
        case MENU_SET_SAVE       : psp_settings_menu_save();
                                   old_pad = new_pad = 0;
        break;
        case MENU_SET_RESET      : psp_settings_menu_reset();
        break;

        case MENU_SET_BACK       : end_menu = 1;
        break;
      }

    } else
    if(new_pad & GP2X_CTRL_UP) {

      if (cur_menu_id > 0) cur_menu_id--;
      else                 cur_menu_id = MAX_MENU_SET_ITEM-1;

    } else
    if(new_pad & GP2X_CTRL_DOWN) {

      if (cur_menu_id < (MAX_MENU_SET_ITEM-1)) cur_menu_id++;
      else                                     cur_menu_id = 0;

    } else
    if(new_pad & GP2X_CTRL_SQUARE) {
      /* Cancel */
      end_menu = -1;
    } else
    if((new_pad & GP2X_CTRL_SELECT) == GP2X_CTRL_SELECT) {
      /* Back to THOM */
      end_menu = 1;
    }
  }

  if (end_menu > 0) {
    psp_settings_menu_validate();
  }

  psp_kbd_wait_no_button();

  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR );
  psp_sdl_flip();
  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR );
  psp_sdl_flip();

  return 1;
}

