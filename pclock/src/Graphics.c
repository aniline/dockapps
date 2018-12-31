/* -*- Mode: C; fill-column: 79 -*- *******************************************
*******************************************************************************
 pclock -- a simple analog clock program for the X Window System
 Copyright (C) 1998 Alexander Kourakos
 Time-stamp: <1998-05-28 21:27:58 awk@oxygene.vnet.net>

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc.,
 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 Author: Alexander Kourakos
  Email: Alexander@Kourakos.com
    Web: http://www.kourakos.com/~awk/pclock/
*******************************************************************************
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <libdockapp/dockapp.h>
#include <libdockapp/misc.h>
#include <X11/extensions/shape.h>

#include "PClock.h"

/*****************************************************************************/

/*
 * The name of the XPM should be ``clock_background''.
 */

#include "Default.xpm"

/*****************************************************************************/

static unsigned long hand_pixel, second_hand_pixel;
static DAShapedPixmap *back_pm = NULL, *all_pm = NULL;
static int old_hour = 0, old_minute = 0, old_second = 0;

/*****************************************************************************/

void
GraphicsInit(int ac, char *av[])
{
    int use_internal_pixmap = TRUE;

    if (option.background_pixmap[0] != '\0') {
        back_pm = DAMakeShapedPixmapFromFile(option.background_pixmap);
        if (!back_pm)
        {
            fprintf(stderr, "ERR: trouble loading pixmap\n");
        }
        else {
            use_internal_pixmap = FALSE;
        }
    }

    if (use_internal_pixmap)
    {
        back_pm = DAMakeShapedPixmapFromData(clock_background);
        if (!back_pm)
        {
            fprintf(stderr, "ERR: trouble using built-in pixmap..exiting\n");
            exit(EXIT_FAILURE);
        }
    }

    all_pm = DAMakeShapedPixmap();
    hand_pixel = DAGetColor(option.hand_color);
    second_hand_pixel = DAGetColor(option.second_hand_color);
    XShapeCombineMask(DADisplay, DAWindow, ShapeBounding, 0, 0, back_pm->shape, ShapeSet);
}

/*****************************************************************************/

void
GraphicsClose(void)
{
  DAFreeShapedPixmap(all_pm);
  DAFreeShapedPixmap(back_pm);
}

void
DrawHand(double angle, int length, int width, Pixel *pf)
{
    int cx = SIZE/2, cy=SIZE/2;
    int x = (cx + (int) ((double) length * sin(angle)));
    int y = (cy - (int) ((double) length * cos(angle)));

    XSetLineAttributes(DADisplay, DAGC, width, LineSolid, CapRound, JoinRound);
    XSetForeground(DADisplay, DAGC, *pf);
    XDrawLine(DADisplay, all_pm->pixmap, DAGC, cx, cy, x, y);
}

/*****************************************************************************/

void
UpdateClock(void)
{
  int h = old_hour, m = old_minute, s;
  struct tm *time_struct;
  time_t curtime;
  double angle_h, angle_m, angle;

  curtime = time(NULL);
  time_struct = localtime(&curtime);

  h = time_struct->tm_hour;
  m = time_struct->tm_min;
  s = time_struct->tm_sec;

  /*
   * Even if we aren't showing the second hand, redraw anyway so a lost
   * X connection can be detected relatively quickly.
   */
  if (h == old_hour && m == old_minute && s == old_second)
    return;

  angle_h = (M_PI / 6.0) * (double) h + (M_PI / 360.0) * (double) m;
  angle_m = (M_PI / 30.0) * (double) m;

  DASPCopyArea(back_pm, all_pm, 0, 0, SIZE, SIZE, 0, 0);
  DrawHand(angle_h, option.hour_hand_length, option.hand_width, &hand_pixel);
  DrawHand(angle_m, option.minute_hand_length, option.hand_width, &hand_pixel);

  if (option.show_seconds) {
    angle = (M_PI / 30.0) * (double) s;
    DrawHand(angle, option.second_hand_length, option.second_hand_width, &second_hand_pixel);
  }

  /* (Re) Draw */
  DASPSetPixmap(all_pm);

  old_hour = h;
  old_minute = m;
  old_second = s;
}

/******************************************************************************
*******************************************************************************
                                  END OF FILE
*******************************************************************************
******************************************************************************/
