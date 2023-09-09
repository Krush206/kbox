#define MAIN_C
/* main.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
 * Copyright (C) 1998, 1999 Adam Rogoyski
 * --- GNU General Public License Disclamer ---
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include "hexedit.h"
#include <assert.h>


int
main (int argc, char **argv)
{
   wchar_t in = 0;

   init (argc, argv);
   while (1)
   {
      in = getch ();
      switch (Globals.mode)
      {
         case HEX_MODE:
            hexMode (in);
            break;

         case ASCII_MODE:
            asciiMode (in);
            break;

         assert (1);  /* This would be a programming error. */
      }
   }
      /* Control never reaches here. */
   exitProgram ();
   return EXIT_SUCCESS;
}
