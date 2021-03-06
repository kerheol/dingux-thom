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
 *                      l'�mulateur Thomson TO7-70
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
 *  Module     : linux/display.h
 *  Version    : 1.5.5
 *  Cr�� par   : Eric Botcazou juillet 1999
 *  Modifi� par: Eric Botcazou 03/11/2003
 *
 *  Module d'interface avec le serveur X.
 */


#ifndef LINUX_DISPLAY_H
#define LINUX_DISPLAY_H

#define PROG_NAME  "thom"
#define PROG_CLASS "EmuTO" 

extern int mouse_x, mouse_y;

#define SCREEN_WIDTH  (TO7_WINDOW_W)
#define SCREEN_HEIGHT (TO7_WINDOW_H)
#define BORDER_SIZE   (TO7_BORDER_W)
#define WINDOW_WIDTH  (SCREEN_WIDTH+2*BORDER_SIZE)
#define WINDOW_HEIGHT (SCREEN_HEIGHT+2*BORDER_SIZE)

extern ushort to7_vram[TO7_SCREEN_W * TO7_SCREEN_H];
extern void InitDisplay(void);
extern void InitWindow(int, char *[], int, int, int);
extern void HandleEvents(void);

#endif
