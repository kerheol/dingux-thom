/*
 *    TTTTTTTTTTTTTT  HH          HH  OOOOOOOOOOOOOO  MM          MM
 *    TTTTTTTTTTTTTT  HH          HH  OOOOOOOOOOOOOO  MMM        MMM
 *          TT        HH          HH  OO          OO  MMMM      MMMM 
 *          TT        HH          HH  OO          OO  MM MM    MM MM
 *          TT        HH          HH  OO          OO  MM  MM  MM  MM 
 *          TT        HHHHHHHHHHHHHH  OO          OO  MM   MMMM   MM
 *          TT        HHHHHHHHHHHHHH  OO          OO  MM    MM    MM
 *          TT        HH          HH  OO          OO  MM          MM
 *          TT        HH          HH  OO          OO  MM          MM
 *          TT        HH          HH  OO          OO  MM          MM
 *          TT        HH          HH  OOOOOOOOOOOOOO  MM          MM
 *          TT        HH          HH  OOOOOOOOOOOOOO  MM          MM 
 *
 *                      l'émulateur Thomson TO7-70
 *
 *  Copyright (C) 1996 Sylvain Huet, 1999-2003 Eric Botcazou.
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  Module     : linux/graphic.c
 *  Version    : 1.5.5
 *  Créé par   : Eric Botcazou juillet 1999
 *  Modifié par: Eric Botcazou 03/11/2003
 *
 *  Gestion de l'affichage du TO7-70.
 */


#ifndef SCAN_DEPEND

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#endif
#include <SDL.h>

#include "psp_sdl.h"
#include "global.h"
#include "to7.h"
#include "display.h"


int psp_screenshot_mode;
# if 0 //LUDO:
int need_border_refresh = 0;
static int border_color;
# endif

#define CELL_COLS  40
#define CELL_ROWS  25

static int xcolor[TO7_NCOLORS+2];

# if 0 //LUDO:
/* RefreshBorder:
 *  Met à jour la couleur du pourtour de l'écran.
 */
void RefreshBorder(void)
{
  memset(to7_vram, 0x0, sizeof(to7_vram));
  need_border_refresh=0;
}
# endif

/* SetBorderColor:
 *  Fixe la couleur du pourtour de l'écran.
 */
void 
to7_SetBorderColor(int color)
{
# if 0 //LUDO:
    if (border_color != color)
    {
        border_color=color;
        need_border_refresh=1;
    }
# endif
}



/* DrawGPL:
 *  Affiche un Groupe Point Ligne (un octet de VRAM).
 */
void 
to7_DrawGPL(int addr, int pt, int col)
{
    register int i;
    unsigned int x,y,c1,c2;

    c1=((col>>3)&7)+(((~col)&0x40)>>3);
    c2=(col&7)+(((~col)&0x80)>>4);

    x=(addr%CELL_COLS)*8;
    y=addr/CELL_COLS;

    ushort *vram = &to7_vram[x + (y * TO7_SCREEN_W)];
    for (i=0; i<8; i++) {
      *vram++ = xcolor[pt&(0x80>>i) ? c1 : c2];
    }
}


#define LED_SIZE 6

/* SetDiskLed:
 *  Allume/éteint la led du lecteur de disquettes.
 */
void 
to7_SetDiskLed(int led_on)
{
# if 0  //LUDO:
  if (led_on) {
    psp_sdl_draw_rectangle( WINDOW_WIDTH-LED_SIZE  , 0, LED_SIZE-1, LED_SIZE-1,
           xcolor[TO7_NCOLORS], 0);
    psp_sdl_fill_rectangle( WINDOW_WIDTH-LED_SIZE+1, 1, LED_SIZE-2, LED_SIZE-2,
           xcolor[TO7_NCOLORS+1], 0);
  } else {
    need_border_refresh = 1;
  }
# endif
}

void 
to7_SetNoCapsLed(int led_on)
{
# if 0 //LUDO: TO_BE_DONE !
# endif
}

/* InitGraphic:
 *  Sélectionne le visual, met en place la palette de couleurs et
 *  initialise le mécanisme de bufferisation (dirty rectangles).
 */
void 
InitGraphic(void)
{
    int i;

    /* récupération de la palette du TO7-70 */
    for (i=0; i<TO7_NCOLORS; i++)
    {
        int red, green, blue;
        to7_QueryColor(i, &red, &green, &blue);
        xcolor[i] = psp_sdl_rgb(red, green, blue);
    }

    /* Couleurs de la led du lecteur de disquettes */
    xcolor[TO7_NCOLORS  ] = psp_sdl_rgb(0x00, 0x00, 0x00);
    xcolor[TO7_NCOLORS+1] = psp_sdl_rgb(0x00, 0xff, 0x00);
}

static inline void
to7_display_screen_normal(void)
{
# if 0 //LUDO: ORIG
  ushort* src_vram = to7_vram;
  ushort* dst_vram = psp_sdl_get_vram_addr(0,20);
  
  int y;
  for (y = 0; y < TO7_SCREEN_H; y++) {
    memcpy( dst_vram, src_vram, TO7_SCREEN_W * sizeof(ushort));
    dst_vram += PSP_LINE_SIZE;
    src_vram += TO7_SCREEN_W;
  }
# else
  uint* src_vram = (uint *)to7_vram;
  uint* dst_vram = (uint *)psp_sdl_get_vram_addr(0,20);
  
  int y;
  int x;
  for (y = 0; y < TO7_SCREEN_H; y++) {
    for (x = 0; x < (TO7_SCREEN_W >> 1); x++) {
      dst_vram[x] = src_vram[x];
    }
    dst_vram += (PSP_LINE_SIZE >> 1);
    src_vram += (TO7_SCREEN_W  >> 1);
  }
# endif
}

void
to7_synchronize(void)
{
  static u32 nextclock = 1;
  static u32 next_sec_clock = 0;
  static u32 cur_num_frame = 0;

  u32 curclock = SDL_GetTicks();

  if (THOM.thom_speed_limiter) {
    while (curclock < nextclock) {
     curclock = SDL_GetTicks();
    }
    u32 f_period = 1000 / THOM.thom_speed_limiter;
    nextclock += f_period;
    if (nextclock < curclock) nextclock = curclock + f_period;
  }

  if (THOM.thom_view_fps) {
    cur_num_frame++;
    if (curclock > next_sec_clock) {
      next_sec_clock = curclock + 1000;
      THOM.thom_current_fps = cur_num_frame;
      cur_num_frame = 0;
    }
  }
}

/* RefreshScreen:
 *  Rafraîchit l'écran du TO7-70.
 */
void 
RefreshScreen(void)
{ 
  if (THOM.psp_skip_cur_frame <= 0) {

    THOM.psp_skip_cur_frame = THOM.psp_skip_max_frame;

    /* if (THOM.thom_render_mode == THOM_RENDER_NORMAL) */
    to7_display_screen_normal(); 

    if (psp_kbd_is_danzeff_mode()) {
      danzeff_moveTo(-10, -65);
      danzeff_render();
    }

    if (THOM.thom_view_fps) {
      char buffer[32];
      sprintf(buffer, "%03d %3d", THOM.thom_current_clock, (int)THOM.thom_current_fps );
      psp_sdl_fill_print(0, 0, buffer, 0xffffff, 0 );
    }

    if (THOM.psp_display_lr) {
      psp_kbd_display_active_mapping();
    }
    psp_sdl_flip();
  
    if (psp_screenshot_mode) {
      psp_screenshot_mode--;
      if (psp_screenshot_mode <= 0) {
        psp_sdl_save_screenshot();
        psp_screenshot_mode = 0;
      }
    }

  } else if (THOM.psp_skip_max_frame) {
    THOM.psp_skip_cur_frame--;
  }

  to7_synchronize();
}
