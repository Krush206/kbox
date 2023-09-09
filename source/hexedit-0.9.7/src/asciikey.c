/* asciikeys.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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


void
asciiMode (wchar_t in)
{
   switch (in)
   {
      case KEY_UP:
      case KEY_DOWN:
      case KEY_LEFT:
      case KEY_RIGHT:
         break;
      case CONTROL_L:
         redraw ();
         break;
      case CONTROL_F:
      case CONTROL_V:
      case KEY_NPAGE:
      case KEY_C3:
         break;
      case CONTROL_B:
      case CONTROL_Y:
      case KEY_PPAGE:
      case KEY_A3:
#ifdef __PDCURSES__
         case ALT_V:
#endif
         break;
      case CONTROL_T:
         popupGotoOffset (Globals.filesize);
         redraw ();
         break;
      case CONTROL_G:
         popupHelp ();
         redraw ();
         break;
      case CONTROL_O:
         popupFileSave ();
         redraw ();
         break;
      case CONTROL_R:
         Globals.charset ^= 1;
         redraw ();
         break;
      case CONTROL_X:
         if (Globals.modified == MODIFIED)
            exitSave (popupFileSave ());
         else
            exitProgram ();
         break;
      case CONTROL_A:
      case KEY_IC:
         break;
      case CONTROL_D:
      case KEY_DC:
         break;
      case KEY_END:
         break;
      case KEY_HOME:
         break;
      case TAB:
         break;
      case CONTROL_E:
         switchModes ();
         break;
      case CONTROL_W:
         search (0);
         redraw ();
         break;
      case CONTROL_N:
         search (1);
         redraw ();
         break;
      case ESCAPE_CHARACTER:
         in = getch ();
         switch (in)
         {
            case ALT_V:
               break;
            case 91:
               in = getch ();
               switch (in)
               {
                  case 49: /* home */
                     break;
                  case 50: /* insert */
                     break;
                  case 51: /* delete */
                     break;
                  case 52: /* end */
                     break;
                  case 53: /* page up */
                     break;
                  case 54: /* page down */
                    break;
               }
               /* getch (); */ /* returns 126 */
               break;
         }
         break;
      default:
         break;
   }
}
