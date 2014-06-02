THOM_VERSION	= 1.1.0
TARGET			= dingux-thom

CROSS_COMPILE	= /opt/gcw0-toolchain/usr/bin/mipsel-linux-
CC				= $(CROSS_COMPILE)gcc
CXX				= $(CROSS_COMPILE)g++
STRIP			= $(CROSS_COMPILE)strip

SYSROOT		= $(shell $(CC) --print-sysroot)
SDL_CFLAGS	= $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
SDL_LIBS	= $(shell $(SYSROOT)/usr/bin/sdl-config --libs)

ifdef V
	CMD =
	SUM = @\#
else
	CMD = @
	SUM = @echo
endif

INCLUDES	= -I.
CFLAGS		= $(SDL_CFLAGS) $(INCLUDES) -DGCW0_MODE -g -O3 -Wall -fstrength-reduce -flto -s -DSOUND_SUPPORT -DNO_STDIO_REDIRECT -DTHOM_VERSION=\"$(THOM_VERSION)\"
CXXFLAGS	= $(CFLAGS) -fno-exceptions -fno-rtti
LDFLAGS		= $(CXXFLAGS) -lpthread -lz -lpng -lm -lgcc -lSDL_image -lSDL_ttf $(SDL_LIBS)

OBJS = \
	gp2x_psp.o \
	cpudingux.o \
	disk.o \
	error.o \
	errors.o \
	hardware.o \
	k7.o \
	keyboard.o \
	mouse.o \
	psp_danzeff.o \
	psp_fmgr.o \
	psp_run.o \
	psp_font.o \
	psp_kbd.o \
	psp_main.o \
	psp_menu.o \
	psp_menu_kbd.o \
	psp_menu_set.o \
	psp_menu_help.o \
	psp_sdl.o \
	to7.o \
	udisplay.o \
	ugraphic.o \
	main.o \
	usound.o \
	global.o \
	mc68xx/dasm6809.o \
	mc68xx/mc6809.o \
	mc68xx/mc6821.o \
	mc68xx/mc6846.o \

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(SUM) "LD $@"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(SUM) "CC $@"
	$(CMD)$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(SUM) "CXX $@"
	$(CMD)$(CXX) $(CFLAGS) -c $< -o $@

clean :
	$(SUM) "CLEAN"
	$(CMD)rm -f $(OBJS) $(TARGET)



#
# THOM port on GP2X
#
# Copyright (C) 2009 Ludovic Jacomme (ludovic.jacomme@gmail.com)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# THOM_VERSION=1.1.0

# TARGET = dingux-thom
# SDL_CONFIG = $(DINGUXDEV)/bin/sdl-config


# CC=/usr/local/dinguxdev/usr/bin/mipsel-linux-gcc
# STRIP=/usr/local/dinguxdev/usr/bin/mipsel-linux-strip

# DEFAULT_CFLAGS = $(shell $(SDL_CONFIG) --cflags)

# MORE_CFLAGS = -I. -I$(DINGUXDEV)/usr/include \
# -DENABLE_THREADS -mips32 -msoft-float -O3 -fstrength-reduce -fthread-jumps -fexpensive-optimizations \
# -fomit-frame-pointer -frename-registers -pipe -G 0 -D_GNU_SOURCE=1 -D_REENTRANT -DIS_LITTLE_ENDIAN -D_DEBUG \
# -DMPU_JZ4740 -ffast-math -msoft-float \
# -DUNIX -DBPP16 -DLSB_FIRST -DSOUND  -DNO_STDIO_REDIRECT  -DTHOM_VERSION=\"$(THOM_VERSION)\" \
# -DDINGUX_MODE

# CFLAGS = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
# LDFLAGS = -static -s

# LIBS += -L/opt/mipsel-linux-uclibc/usr/lib \
# /opt/mipsel-linux-uclibc/usr/lib/libSDL_image.a \
# /opt/mipsel-linux-uclibc/usr/lib/libSDL.a \
# -lpng -lz -lm -lpthread -ldl

# .c.o:
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(TARGET): $(OBJS)
# 	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET) && $(STRIP) $(TARGET)

# install: $(TARGET)
# 	cp $< /media/dingux/local/emulators/dingux-thom/

# clean:
# 	rm -f $(OBJS) $(TARGET)

# ctags:
# 	ctags *[ch]
