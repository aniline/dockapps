/*
 *  dockapp.h
 *
 *  Copyright (C) 2003 Draghicioiu Mihai <misuceldestept@go.ro>
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
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 */

#ifndef __DOCKAPP_H__
#define __DOCKAPP_H__

#include <X11/X.h>
#include <X11/Xlib.h>

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 64

extern Display *display;
extern int      screen;
extern Window   window, iconwindow;
extern Pixmap   on, off, numbers, buffer;
extern GC       gc;

void make_window(void);
void process_events(void);
void update_window(void);
void free_stuff(void);

#endif /* __DOCKAPP_H__ */
