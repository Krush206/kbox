/* hexkeys.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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
hexMode (wchar_t in)
{
   switch (in)
   {
      case KEY_UP:
      case KEY_DOWN:
      case KEY_LEFT:
      case KEY_RIGHT:
         handleArrowKeys (in);
         wrefresh (Globals.wmain);
         refresh ();
         break;
      case CONTROL_L:
         redraw ();
         break;
      case CONTROL_F:
      case CONTROL_V:
      case KEY_NPAGE:
      case KEY_C3:
         pagedown ();
         wrefresh (Globals.wmain);
         refresh ();
         break;
      case CONTROL_B:
      case CONTROL_Y:
      case KEY_PPAGE:
      case KEY_A3:
#ifdef __PDCURSES__
         case ALT_V:
#endif
         pageup ();
         wrefresh (Globals.wmain);
         refresh ();
         break;
      case CONTROL_T:
         popupGotoOffset (Globals.filesize);
         redraw ();
         break;
      case CONTROL_G:
      case KEY_F(1):
         popupHelp ();
         redraw ();
         break;
      case CONTROL_O:
      case KEY_F(2):
         popupFileSave ();
         redraw ();
         break;
      case CONTROL_P:
         Globals.spacing ^= 1;
         cursor_x = cursor_ascii_to_ebcdic (cursor_x);
         redraw ();
         break;
      case CONTROL_R:
         Globals.charset ^= 1;
         redraw ();
         break;
      case CONTROL_U:
         if (Globals.modified != READ_ONLY)
            undoLastChange ();
         break;
      case CONTROL_X:
      case KEY_F(3):
         if (Globals.modified == MODIFIED)
            exitSave (popupFileSave ());
         else
            exitProgram ();
         break;
      case CONTROL_A:
      case KEY_IC:
         if (Globals.modified != READ_ONLY && Globals.fullsize)
            insertByte (0x00, 0);
         break;
      case CONTROL_D:
      case KEY_DC:
         if (Globals.modified != READ_ONLY && Globals.fullsize)
            deleteByte (0);
         break;
      case KEY_END:
         endKey ();
         break;
      case KEY_HOME:
         homeKey ();
         break;
      case TAB:
         tabcursor (&Globals.tabb);
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
      case CONTROL_SLASH:
      {
         int mode = Globals.mode;
         Globals.mode = CALCULATOR_MODE;
         binary_calculator ();
         Globals.mode = mode;
         redraw ();
         break;
      }
      case KEY_F(5):
         if (color_term != -1)
            color_term ^= 1;
         redraw ();
         break;
      case ESCAPE_CHARACTER:
         in = getch ();
         switch (in)
         {
            case ALT_V:
               pageup ();
               wrefresh (Globals.wmain);
               refresh ();
               break;

            case 79:
               in = getch ();
               switch (in)
               {
                  case 'A':
                     in = KEY_UP;
                     break;

                  case 'B':
                     in = KEY_DOWN;
                     break;

                  case 'D':
                     in = KEY_LEFT;
                     break;

                  case 'C':
                     in = KEY_RIGHT;
                     break;

                }
                handleArrowKeys (in);
                wrefresh (Globals.wmain);
                refresh ();
                break;

            case 91:
               in = getch ();
               switch (in)
               {
                  case 49: /* home */
                     homeKey ();
                     break;
                  case 50: /* insert */
                     if (Globals.modified != READ_ONLY && Globals.fullsize)
                        insertByte (0x00, 0);
                     break;
                  case 51: /* delete */
                     if (Globals.modified != READ_ONLY && Globals.fullsize)
                        deleteByte (0);
                     break;
                  case 52: /* end */
                     endKey ();
                     break;
                  case 53: /* page up */
                     pageup ();
                     wrefresh (Globals.wmain);
                     refresh ();
                     break;
                  case 54: /* page down */
                     pagedown ();
                     wrefresh (Globals.wmain);
                     refresh ();
                    break;
               }
               getch ();  /* returns 126 */
               break;
         }
         break;
      default:
            /* Lots of extra keys can be used only in hex data mode.
             */
         if (!Globals.tabb || (Globals.modified == READ_ONLY))
         {
            int break_out = 0;
               /* check vi movement keys (h,j,k,l) */
            switch (in)
            {
               case 'h':      /* left */
               case 'H':
                  in = KEY_LEFT;
                  break_out = 1;
                  break;

               case 'j':      /* down */
               case 'J':
                  in = KEY_DOWN;
                  break_out = 1;
                  break;

               case 'k':      /* up */
               case 'K':
                  in = KEY_UP;
                  break_out = 1;
                  break;

               case 'l':      /* right */
               case 'L':
                  in = KEY_RIGHT;
                  break_out = 1;
                  break;

               case 'v':      /* page down */
               case 'V':
               case ' ':
                  pagedown ();
                  wrefresh (Globals.wmain);
                  refresh ();
                  break_out = 1;
                  break;

               case 'u':      /* page up */
               case 'U':
               case 'y':
               case 'Y':
                  pageup ();
                  wrefresh (Globals.wmain);
                  refresh ();
                  break_out = 1;
                  break;

               case 't':      /* goTo offset */
               case 'T':
                  popupGotoOffset (Globals.filesize);
                  redraw ();
                  break_out = 1;
                  break;

               case 'w':     /* Search */
               case 'W':
                  search (0);
                  redraw ();
                  break_out = 1;
                  break;

               case 'n':
               case 'N':
                  jump_relative_offset (0);
                  break_out = 1;
                  break;

               case '+':
               case '-':
                  jump_relative_offset (in);
                  break_out = 1;
                  break;

            }
            if (break_out)
            {
               handleArrowKeys (in);
               wrefresh (Globals.wmain);
               refresh ();
               break;
            }
         }
         if (Globals.modified != READ_ONLY)
            over_write (in);
         break;
   }
   refresh ();
}



void
handleArrowKeys (wchar_t key)
{
   unsigned long off = 0;
   switch (key)
   {
      case KEY_DOWN:
         if (cursor_y < BOTTOM_LINE) /* cursor not at bottom */
         {
            offset += M_0x10;
            if (offset > Globals.filesize)
            {
               if ((offset & M_0x0F) > (Globals.filesize & M_0x0F))
               {
                  offset -= mapcur (cursor_x) - 
                             (Globals.filesize & M_0x0F);
                  cursor_x = mappos (position - 1);
               }
               else
               {
                  offset -= M_0x10;
                  break;
               }
            }
            printStatusWindow ();
            if (Globals.tabb)
               move (++cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (++cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         else if (offset + M_0x10 < Globals.filesize + 1)
         /* cursor at bottom of screen, but data is ready below */
         {
            offset += M_0x10;
            off = offset & M_0xFFFFFFF0;
            off -= M_0x10 * (MAIN_BOTTOM_LINE);
            printStatusWindow ();
            wmove (Globals.wmain, 0, 0);
            drawdump (off);
            wrefresh (Globals.wmain);
            if (Globals.tabb)
               move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         else if ((offset & M_0xFFFFFFF0) < 
                  (Globals.filesize & M_0xFFFFFFF0))
         /* cursor at bottom, and no data directly below */
         {
            offset = Globals.filesize;
            off = offset & M_0xFFFFFFF0;
            off -= M_0x10 * (MAIN_BOTTOM_LINE);
            printStatusWindow ();
            wmove (Globals.wmain, 0, 0);
            drawdump (off);
            wrefresh (Globals.wmain);
            cursor_x = mappos (position - 1);
            if (Globals.tabb)
               move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         break;
      case KEY_UP:
         if ((cursor_y <= MAIN_TOP_LINE) && (offset > M_0x0F))
         {
            wmove (Globals.wmain, 0, 0);
            offset -= M_0x10;
            off = offset;
            if (off > Globals.filesize)
               off = 0;
            if (offset >= 0x00)
            {
               printStatusWindow ();
            }
            else
               offset = 0x00;
            drawdump (off);
            wrefresh (Globals.wmain);
            if (Globals.tabb)
               move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         else if (cursor_y > MAIN_TOP_LINE)
         {
            offset -= M_0x10;
            printStatusWindow ();
            if (Globals.tabb)
               move (--cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (--cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         break;
      case KEY_RIGHT:
      if (Globals.spacing)
      {
         if (
             cursor_x == 10 || cursor_x == 13
          || cursor_x == 16 || cursor_x == 19
          || cursor_x == 23 || cursor_x == 26
          || cursor_x == 29 || cursor_x == 32
          || cursor_x == 37 || cursor_x == 40
          || cursor_x == 43 || cursor_x == 46
          || cursor_x == 50 || cursor_x == 53
          || cursor_x == 56 || cursor_x == 59)
         {
            if (Globals.tabb)
            {
               cursor_x++;
               if ((offset < Globals.filesize) &&
                   (cursor_x == 11 || cursor_x == 14
                || cursor_x == 17 || cursor_x == 20
                || cursor_x == 24 || cursor_x == 27
                || cursor_x == 30 || cursor_x == 33
                || cursor_x == 38 || cursor_x == 41
                || cursor_x == 44 || cursor_x == 47
                || cursor_x == 51 || cursor_x == 54
                || cursor_x == 57))
               {
                  offset++;
                  printStatusWindow ();
                  if (cursor_x == 33)
                     cursor_x += 4;
                  else if (cursor_x == 20 || cursor_x == 47)
                     cursor_x += 3;
                  else
                     cursor_x += 2;
                  move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               }
               else if (cursor_x == 60)
               {
                  if (cursor_y < BOTTOM_LINE)
                  {
                     if (offset >= Globals.filesize)
                        break;
                     offset++;
                     printStatusWindow ();
                     cursor_x = 10;
                     move (++cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
                  else if (offset < Globals.filesize)
                  {
                     offset++;
                     printStatusWindow ();
                     off = offset;
                     off -= M_0x10 * (MAIN_BOTTOM_LINE);
                     wmove (Globals.wmain, 0, 0);
                     drawdump (off);
                     wrefresh (Globals.wmain);         
                     cursor_x = 10;
                     move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
               }
            }
            else
               move (cursor_y, ++cursor_x);
            wrefresh (stdscr);
         }
         else if ((offset < Globals.filesize) &&
            (cursor_x == 11 || cursor_x == 14
          || cursor_x == 17 || cursor_x == 20
          || cursor_x == 24 || cursor_x == 27
          || cursor_x == 30 || cursor_x == 33
          || cursor_x == 38 || cursor_x == 41
          || cursor_x == 44 || cursor_x == 47
          || cursor_x == 51 || cursor_x == 54
          || cursor_x == 57))
         {
            offset++;
            printStatusWindow ();
            cursor_x = mappos (offset & M_0x0F);
            if (Globals.tabb)
               move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         else if (cursor_x == 60)
         {
            if (cursor_y < BOTTOM_LINE)
            {
               if (offset >= Globals.filesize)
                  break;
               offset++;
               printStatusWindow ();
               cursor_x = 10;
               if (Globals.tabb)
                  move (++cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (++cursor_y, cursor_x);
               wrefresh (stdscr);
            }
            else if (offset < Globals.filesize)
            {
               offset++;
               printStatusWindow ();
               off = offset;
               off -= M_0x10 * (MAIN_BOTTOM_LINE);
               wmove (Globals.wmain, 0, 0);
               drawdump (off);
               wrefresh (Globals.wmain);         
               cursor_x = 10;
               if (Globals.tabb)
                  move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (cursor_y, cursor_x);
               wrefresh (stdscr);
            }   
         }
         break;
      }
      else
      {
         if (
             cursor_x == 10 || cursor_x == 12
          || cursor_x == 14 || cursor_x == 16
          || cursor_x == 19 || cursor_x == 21
          || cursor_x == 23 || cursor_x == 25
          || cursor_x == 28 || cursor_x == 30
          || cursor_x == 32 || cursor_x == 34
          || cursor_x == 37 || cursor_x == 39
          || cursor_x == 41 || cursor_x == 43)
         {
            if (Globals.tabb)
            {
               cursor_x++;
               if ((offset < Globals.filesize) &&
                   (cursor_x == 11 || cursor_x == 13
                || cursor_x == 15 || cursor_x == 17
                || cursor_x == 20 || cursor_x == 22
                || cursor_x == 24 || cursor_x == 26
                || cursor_x == 29 || cursor_x == 31
                || cursor_x == 33 || cursor_x == 35
                || cursor_x == 38 || cursor_x == 40
                || cursor_x == 42))
               {
                  offset++;
                  printStatusWindow ();
                  if (cursor_x == 26)
                     cursor_x += 3;
                  else if (cursor_x == 17 || cursor_x == 35)
                     cursor_x += 3;
                  else
                     cursor_x += 2;
                  move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               }
               else if (cursor_x == 33)
               {
                  if (cursor_y < BOTTOM_LINE)
                  {
                     if (offset >= Globals.filesize)
                        break;
                     offset++;
                     printStatusWindow ();
                     cursor_x = 10;
                     move (++cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
                  else if (offset < Globals.filesize)
                  {
                     offset++;
                     printStatusWindow ();
                     off = offset;
                     off -= M_0x10 * (MAIN_BOTTOM_LINE);
                     wmove (Globals.wmain, 0, 0);
                     drawdump (off);
                     wrefresh (Globals.wmain);         
                     cursor_x = 10;
                     move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
               }
            }
            else
               move (cursor_y, ++cursor_x);
            wrefresh (stdscr);
         }
         else if ((offset < Globals.filesize) &&
            (cursor_x == 11 || cursor_x == 13
          || cursor_x == 15 || cursor_x == 17
          || cursor_x == 20 || cursor_x == 22
          || cursor_x == 24 || cursor_x == 26
          || cursor_x == 29 || cursor_x == 31
          || cursor_x == 33 || cursor_x == 35
          || cursor_x == 38 || cursor_x == 40
          || cursor_x == 42))
         {
            offset++;
            printStatusWindow ();
            cursor_x = mappos (offset & M_0x0F);
            if (Globals.tabb)
               move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         else if (cursor_x == 44)
         {
            if (cursor_y < BOTTOM_LINE)
            {
               if (offset >= Globals.filesize)
                  break;
               offset++;
               printStatusWindow ();
               cursor_x = 10;
               if (Globals.tabb)
                  move (++cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (++cursor_y, cursor_x);
               wrefresh (stdscr);
            }
            else if (offset < Globals.filesize)
            {
               offset++;
               printStatusWindow ();
               off = offset;
               off -= M_0x10 * (MAIN_BOTTOM_LINE);
               wmove (Globals.wmain, 0, 0);
               drawdump (off);
               wrefresh (Globals.wmain);         
               cursor_x = 10;
               if (Globals.tabb)
                  move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (cursor_y, cursor_x);
               wrefresh (stdscr);
            }   
         }
         break;
      }
      case KEY_LEFT:
      if (Globals.spacing)
      {
         if (cursor_x == 11 || cursor_x == 14
          || cursor_x == 17 || cursor_x == 20
          || cursor_x == 24 || cursor_x == 27
          || cursor_x == 30 || cursor_x == 33
          || cursor_x == 38 || cursor_x == 41
          || cursor_x == 44 || cursor_x == 47
          || cursor_x == 51 || cursor_x == 54
          || cursor_x == 57 || cursor_x == 60)
         {
         
            if (Globals.tabb)
            {
               cursor_x--;
               if (cursor_x == 13
                || cursor_x == 16 || cursor_x == 19
                || cursor_x == 23 || cursor_x == 26
                || cursor_x == 29 || cursor_x == 32
                || cursor_x == 37 || cursor_x == 40
                || cursor_x == 43 || cursor_x == 46
                || cursor_x == 50 || cursor_x == 53
                || cursor_x == 56 || cursor_x == 59)
               {
                  offset--;
                  printStatusWindow ();
                  if (cursor_x == 37)
                     cursor_x -= 4;
                  else if (cursor_x == 23 || cursor_x == 50)
                     cursor_x -= 3;
                  else
                     cursor_x -= 2;
                 move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               }
               else if (cursor_x == 10)
               {
                  if (cursor_y > MAIN_TOP_LINE)
                  {
                     offset--;
                     printStatusWindow ();
                     cursor_x = 60;
                     move (--cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
                  else if ((cursor_y <= MAIN_TOP_LINE) && (offset != 0x00))
                  {
                     offset--;
                     off = offset & M_0xFFFFFFF0;
                     printStatusWindow ();
                     wmove (Globals.wmain, 0, 0);
                     drawdump (off);
                     wrefresh (Globals.wmain);         
                     cursor_x = 60;
                     move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
               }
            }
            else
               move (cursor_y, --cursor_x);
            wrefresh (stdscr);
         }
         else if (cursor_x == 13
          || cursor_x == 16 || cursor_x == 19
          || cursor_x == 23 || cursor_x == 26
          || cursor_x == 29 || cursor_x == 32
          || cursor_x == 37 || cursor_x == 40
          || cursor_x == 43 || cursor_x == 46
          || cursor_x == 50 || cursor_x == 53
          || cursor_x == 56 || cursor_x == 59)
         {
            offset--;
            printStatusWindow ();
            if (cursor_x == 37)
               cursor_x -= 4;
            else if (cursor_x == 23 || cursor_x == 50)
               cursor_x -= 3;
            else
               cursor_x -= 2;
            if (Globals.tabb)
               move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         else if (cursor_x == 10)
         {
            if (cursor_y > MAIN_TOP_LINE)
            {
               offset--;
               printStatusWindow ();
               cursor_x = 60;
               if (Globals.tabb)
                  move (--cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (--cursor_y, cursor_x);
               wrefresh (stdscr);
            }
            else if ((cursor_y <= MAIN_TOP_LINE) && (offset != 0x00))
            {
               offset--;
               off = offset & M_0xFFFFFFF0;
               printStatusWindow ();
               wmove (Globals.wmain, 0, 0);
               drawdump (off);
               wrefresh (Globals.wmain);         
               cursor_x = 60;
               if (Globals.tabb)
                  move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (cursor_y, cursor_x);
               wrefresh (stdscr);
            }
         }
      }
      else
      {
         if (cursor_x == 11 || cursor_x == 13
          || cursor_x == 15 || cursor_x == 17
          || cursor_x == 20 || cursor_x == 22
          || cursor_x == 24 || cursor_x == 26
          || cursor_x == 29 || cursor_x == 31
          || cursor_x == 33 || cursor_x == 35
          || cursor_x == 38 || cursor_x == 40
          || cursor_x == 42 || cursor_x == 44)
         {
         
            if (Globals.tabb)
            {
               cursor_x--;
               if (cursor_x == 12
                || cursor_x == 14 || cursor_x == 16
                || cursor_x == 19 || cursor_x == 21
                || cursor_x == 23 || cursor_x == 25
                || cursor_x == 28 || cursor_x == 30
                || cursor_x == 32 || cursor_x == 34
                || cursor_x == 37 || cursor_x == 39
                || cursor_x == 41 || cursor_x == 43)
               {
                  offset--;
                  printStatusWindow ();
                  if (cursor_x == 28)
                     cursor_x -= 2;
                  else if (cursor_x == 19 || cursor_x == 37)
                     cursor_x -= 2;
                  else
                     cursor_x -= 1;
                 move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               }
               else if (cursor_x == 10)
               {
                  if (cursor_y > MAIN_TOP_LINE)
                  {
                     offset--;
                     printStatusWindow ();
                     cursor_x = 44;
                     move (--cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
                  else if ((cursor_y <= MAIN_TOP_LINE) && (offset != 0x00))
                  {
                     offset--;
                     off = offset & M_0xFFFFFFF0;
                     printStatusWindow ();
                     wmove (Globals.wmain, 0, 0);
                     drawdump (off);
                     wrefresh (Globals.wmain);         
                     cursor_x = 44;
                     move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
                  }
               }
            }
            else
               move (cursor_y, --cursor_x);
            wrefresh (stdscr);
         }
         else if (cursor_x == 12
          || cursor_x == 14 || cursor_x == 16
          || cursor_x == 19 || cursor_x == 21
          || cursor_x == 23 || cursor_x == 25
          || cursor_x == 28 || cursor_x == 30
          || cursor_x == 32 || cursor_x == 34
          || cursor_x == 37 || cursor_x == 39
          || cursor_x == 41 || cursor_x == 43)
         {
            offset--;
            printStatusWindow ();
            if (cursor_x == 28)
               cursor_x -= 2;
            else if (cursor_x == 19 || cursor_x == 37)
               cursor_x -= 2;
            else
               cursor_x -= 1;
            if (Globals.tabb)
               move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
            else
               move (cursor_y, cursor_x);
            wrefresh (stdscr);
         }
         else if (cursor_x == 10)
         {
            if (cursor_y > MAIN_TOP_LINE)
            {
               offset--;
               printStatusWindow ();
               cursor_x = 44;
               if (Globals.tabb)
                  move (--cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (--cursor_y, cursor_x);
               wrefresh (stdscr);
            }
            else if ((cursor_y <= MAIN_TOP_LINE) && (offset != 0x00))
            {
               offset--;
               off = offset & M_0xFFFFFFF0;
               printStatusWindow ();
               wmove (Globals.wmain, 0, 0);
               drawdump (off);
               wrefresh (Globals.wmain);         
               cursor_x = 44;
               if (Globals.tabb)
                  move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
               else
                  move (cursor_y, cursor_x);
               wrefresh (stdscr);
            }
         }
      }
         break;
      default:
         break;
   }
}



void
pagedown (void)
{
   unsigned long off = 0;
   if ((Globals.filesize & M_0xFFFFFFF0) < (MAIN_BOTTOM_LINE * M_0x10))
   {
      off = 0;
      offset &= M_0x0F;
      offset += Globals.filesize & M_0xFFFFFFF0;
      if (offset > Globals.filesize)
         offset = Globals.filesize;
      cursor_y = ((Globals.filesize & M_0xFFFFFFF0) >> 4) + 1;
      if (mapcur (cursor_x) > (offset & M_0x0F))
         cursor_x = mappos (offset & M_0x0F);
      printStatusWindow ();
      wmove (Globals.wmain, 0, 0);
      drawdump (off);
      wrefresh (Globals.wmain);
      if (Globals.tabb)
         move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
      else
         move (cursor_y, cursor_x);
      refresh ();
      return;
   }
   if (((offset & M_0xFFFFFFF0) == (Globals.filesize & M_0xFFFFFFF0))
    && (Globals.filesize > ((MAIN_BOTTOM_LINE) * M_0x10)))
   {
      off = (offset & M_0xFFFFFFF0) - ((MAIN_BOTTOM_LINE) * M_0x10);
      if (off > Globals.filesize)
         off = 0;
      printStatusWindow ();
      wmove (Globals.wmain, 0, 0);
      drawdump (off);
      wrefresh (Globals.wmain);
      cursor_y = BOTTOM_LINE;
      if (Globals.tabb)
         move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
      else
         move (cursor_y, cursor_x);
      refresh ();
      return;
   }
   else if ((offset + ((MAIN_BOTTOM_LINE) * M_0x10) 
            > (Globals.filesize & M_0xFFFFFFF0)))
   {
      if (Globals.filesize >= ((MAIN_BOTTOM_LINE) * M_0x10))
         cursor_y = BOTTOM_LINE;
      else
         cursor_y = ((Globals.filesize & M_0xF0) >> 4) + MAIN_BOTTOM_LINE;
         offset = Globals.filesize;
         if (mapcur (cursor_x) < (offset & M_0x0F))
            offset = (offset & M_0xFFFFFFF0) + mapcur (cursor_x);
         else if (mapcur (cursor_x) > (offset & M_0x0F))
            cursor_x = mappos (offset & M_0x0F);
   }
   offset += (MAIN_BOTTOM_LINE) * M_0x10;
   if (offset > Globals.filesize)
   {
      offset = Globals.filesize;
      if (mapcur (cursor_x) < (offset & M_0x0F))
         offset = (offset & M_0xFFFFFFF0) + mapcur (cursor_x);
      else if (mapcur (cursor_x) > (offset & M_0x0F))
         cursor_x = mappos (offset & M_0x0F);
      off = offset & M_0xFFFFFFF0;
      off -= ((MAIN_BOTTOM_LINE) * M_0x10);
      if (off > Globals.filesize)
         off = 0;
      printStatusWindow ();
      wmove (Globals.wmain, 0, 0);
      drawdump (off);
      wrefresh (Globals.wmain);
      if (Globals.tabb)
         move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
      else
         move (cursor_y, cursor_x);
      refresh ();
      return;
   }
   if ((offset - ((MAIN_BOTTOM_LINE) * M_0x10)) < 0)
   {
      off = 0;
      cursor_y = ((offset & M_0xF0) >> 4) + MAIN_BOTTOM_LINE;
   }
   else
      off = offset;
   off &= M_0xFFFFFFF0;
   off -= (cursor_y - MAIN_TOP_LINE) * M_0x10;
   printStatusWindow ();
   wmove (Globals.wmain, 0, 0);
   drawdump (off);
   wrefresh (Globals.wmain);
   if (Globals.tabb)
      move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
   else
      move (cursor_y, cursor_x);
   refresh ();
}


void
pageup (void)
{
   unsigned long off = 0;
   if ((offset & M_0xFFFFFFF0) <= ((MAIN_BOTTOM_LINE) * M_0x10))
   /* paged up with offset 0x00 on the screen (begining of file) */
   {
      offset = 0x00 + mapcur (cursor_x);
      off = 0;
      cursor_y = MAIN_TOP_LINE;
   }
   else
   /* not at begining of file */
   {
      offset -= M_0x10 * (MAIN_BOTTOM_LINE);
      off = offset;
      off -= M_0x10 * (cursor_y - MAIN_TOP_LINE);
      if (off > Globals.filesize)
         off = 0;
      if (offset > Globals.filesize)
         offset = 0;
      if ((offset & M_0xFFFFFFF0) < ((MAIN_BOTTOM_LINE) * M_0x10))
      {
         cursor_y = ((offset & M_0xFFFFFFF0) >> 4) + MAIN_TOP_LINE;
      }
   }
   printStatusWindow ();
   wmove (Globals.wmain, 0, 0);
   drawdump (off);
   wrefresh (Globals.wmain);
   if (Globals.tabb)
      move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
   else
      move (cursor_y, cursor_x);
   refresh ();
}


void
homeKey (void)
{
   if (cursor_x != 10)
   {
      offset &= M_0xFFFFFFF0;
      cursor_x = mappos (0);
      printStatusWindow ();
      if (Globals.tabb)
         move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
      else
         move (cursor_y, cursor_x);
      refresh ();
   }
}


void
endKey (void)
{
   if ((offset & M_0xFFFFFFF0) == (Globals.filesize & M_0xFFFFFFF0))
   {
      cursor_x = mappos (Globals.filesize & M_0x0F) + 1;
      offset = Globals.filesize;
   }
   else if (cursor_x != 60)
   {
      if (Globals.spacing)
         cursor_x = 60;
      else
         cursor_x = 44;
      offset |= M_0x0F;
   }
   printStatusWindow ();
   if (Globals.tabb)
      move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
   else
      move (cursor_y, cursor_x);
   refresh ();
}


void
tabcursor (int *tabb)
/* switch from hex side to ascii side of screen */
{
   *tabb ^= 1;
   if (*tabb)
      move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
   else
      move (cursor_y, cursor_x);
   refresh ();
}
