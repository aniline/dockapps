/* -*- Mode: C; fill-column: 79 -*- *******************************************
*******************************************************************************
 pclock -- a simple analog clock program for the X Window System
 Copyright (C) 1998 Alexander Kourakos
 Time-stamp: <1998-05-28 20:48:08 awk@oxygene.vnet.net>

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

 Author: Alexander Kourakos <Alexander@Kourakos.com>
    Web: http://www.kourakos.com/~awk/pclock/
*******************************************************************************
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>

#include <libdockapp/dockapp.h>

#include "PClock.h"
#include "Defaults.h"
#include "Version.h"

/*****************************************************************************/

options option;
static char def_HAND_COLOR[STRING_LENGTH] = HAND_COLOR;
static char def_SECOND_HAND_COLOR[STRING_LENGTH] = SECOND_HAND_COLOR;
static char *def_background_pixmap = "";
/*****************************************************************************/

static void SetOptions(int, char *[]);

static DAProgramOption DAPoptions[] =
{
    {"-B", "--background", "use the given pixmap as the clock background",
     DOString, False, { .string = &(option.background_pixmap) } },
    {"-H", "--hands-color", "draw the hour and minute hands (and the second"
     "hand, if -S is not also specified) in the specified color.",
     DOString, False, {&(option.hand_color)} },
    {NULL, "--hands-width", "draw the hour and minute hands with the specified width.",
     DONatural, False, {&(option.hand_width)} },
    {NULL, "--hour-hand-length", "draw the hour hand with the specified length.",
     DONatural, False, {&(option.hour_hand_length)} },
    {NULL, "--minute-hand-length", "\n\t\t\t\tdraw the minute hand with the specified length.",
     DONatural, False, {&(option.minute_hand_length)} },
    {"-S", "--second-hand-color", "\n\t\t\t\tdraw the second hand in the specified color.",
     DOString, False, {&(option.second_hand_color)} },
    {NULL, "--second-hand-length", "\n\t\t\t\tdraw the second hand with the specified length.",
     DONatural, False, {&(option.second_hand_length)} },
    {NULL, "--second-hand-width", "\n\t\t\t\tdraw the second hand with the specified width.",
     DONatural, False, {&(option.second_hand_width)} },
    {"-s", "--second-hand", "display the second hand",
     DONone, False, {&(option.second_hand_width)} },
};

/*****************************************************************************/

int
main(int argc, char *argv[])
{
  DACallbacks eventCallbacks = {
      GraphicsClose,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      UpdateClock,
  };

  /* DAParseArguments() sets up the internal libdockapp context. */
  SetOptions(argc, argv);

  DASetExpectedVersion(20050716);
  DAInitialize("", NAME, 64, 64, argc, argv);
  GraphicsInit(argc, argv);

  DASetCallbacks(&eventCallbacks);
  DASetTimeout(PERIOD);
  DAShow();
  DAEventLoop();

  return EXIT_SUCCESS;
}

/*****************************************************************************/

static void SetOptions(int ac, char *av[])
{
  option.under_windowmaker = UNDER_WINDOWMAKER;
  option.hand_width = HAND_WIDTH;
  option.second_hand_width = SECOND_HAND_WIDTH;
  option.hour_hand_length = HOUR_HAND_LENGTH;
  option.minute_hand_length = MINUTE_HAND_LENGTH;
  option.second_hand_length = SECOND_HAND_LENGTH;

  /* Default values for string 'options' */
  option.hand_color = def_HAND_COLOR;
  option.second_hand_color = def_SECOND_HAND_COLOR;
  option.background_pixmap = def_background_pixmap;

  DAParseArguments(ac, av, DAPoptions,
                   sizeof(DAPoptions)/sizeof(DAProgramOption),
                   "Author: Alexander Kourakos <Alexander@Kourakos.com>\n"
                   "   Web: http://www.kourakos.com/~awk/pclock/\n",
                   "This is " NAME " " VERSION "\nCopyright (C) 1999-2000 Alexander Kourakos\n");
  option.show_seconds = DAPoptions[8].used ? !SHOW_SECONDS : SHOW_SECONDS;
}

/******************************************************************************
*******************************************************************************
                                  END OF FILE
*******************************************************************************
******************************************************************************/
