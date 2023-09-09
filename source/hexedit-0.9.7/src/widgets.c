/* widgets.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

#define S_BOX_COLOR  (COLOR_PAIR(4) | A_BOLD)


WINDOW *
popupWindow (int width, int height)
/* Generic blank popup window with a border.  Comes out centered.
 */
{
   int winy = 0, winx = 0;
   WINDOW *wpopup = NULL;

   if (LINES < height)
      height = LINES;
   if (COLS < width)
      width = COLS;

   winy = (LINES / 2) - (height / 2);
   winx = (COLS / 2) - (width / 2);
   wpopup = newwin (height, width, winy, winx);
   if (wpopup == NULL)
   {
      fprintf (stderr, "Cannot open up popup window\n");
      return NULL;
   }
   scrollok (wpopup, FALSE);
   box (wpopup, 0, 0);
   return wpopup;
}


char *
stringBox (WINDOW *win, int y, int x, int len, int max, char *sample)
/* Creates a entry box to type in a string and returns it.
 * You are responsible for freeing the returned pointer.
 */
{
   char *str = NULL;
   int done = 0;
   int i = 0;
   int n = 0;
   int oldx = 0;
   int left = 0;
   int right = max - 1;
   int samplelen = strlen (sample ? sample : "");
   wchar_t in = 0;

   wmove (win, y, x);
   wattrset (win, color_term ? S_BOX_COLOR : A_REVERSE);
   str = malloc (len + 2);
   if (!str)
   {
/*      fprintf (stderr, "%s - StringBox\n", NOT_ENOUGH_MEMORY); */
      popup_Error (NOT_ENOUGH_MEMORY, 0);
      return NULL;
   }
   *(str + len + 1) = '\0';
   memset (str, ' ', len + 1);
   strncpy (str, sample ? sample : "", samplelen);
   for (i = 0; i < max; i++)
   {
      if (*(str + i) && (i < max))
      {
         wprintw (win, "%c", *(str + i));
      }
      else
         wprintw (win, " ");
   }
   i = samplelen;
   if (i >= max)
      i = max - 1;
   wmove (win, y, x + i);
   wrefresh (win);

   while (!done && ((in = getch ()) != '\n'))
   {
      switch (in)
      {
         case CONTROL_C:
         case CONTROL_G:
         case CONTROL_X:
         case ESCAPE_CHARACTER:
            *str = '\0';
            done = 1;
            break;
         case KEY_LEFT:
            if (i > 0)
            {
               if (win->_curx > x)
               /* not at left hand side */
               {
                  i--;
                  wmove (win, y, win->_curx - 1);
               }
               else if (i > 0)
               /* left hand side */
               {
                  i--;
                  if (left > 0)
                  {
                     left--;
                     right--;
                  }
                  wmove (win, y, x);
                  for (n = left; n <= right; n++)
                  {
                     if ((n < len) && *(str + n))
                        wprintw (win, "%c", *(str + n));
                     else
                        wprintw (win, " ");
                  }
                  wmove (win, y, x);
               }

            }
            break;
         case KEY_RIGHT:
            if (win->_curx < (x + max - 1))
            /* not at right hand side */
            {
               i++;
               wmove (win, y, win->_curx + 1);
            }
            else if (i < len - 1)
            /* right hand side */
            {
               i++;
               if (right < len - 1)
               {
                  left++;
                  right++;
               }
               wmove (win, y, x);
               for (n = left; n <= right; n++)
               {
                  if (*(str + n))
                     wprintw (win, "%c", *(str + n));
                  else
                     wprintw (win, " ");
               }
               wmove (win, y, x + max - 1);
            }
            break;
         case BACKSPACE:
         case CONTROL_H:
         case KEY_BACKSPACE:
         case KEY_DC:
#ifdef __PDCURSES__
            case CTL_BKSP:
#endif
            if (i > 0)
            {
               if (win->_curx > x)
               /* not at left hand side */
               {
                  i--;
                  for (n = strlen (str); n < len; n++)
                     *(str + n) = ' ';
                  for (n = i; n < len; n++)
                     *(str + n) = *(str + n + 1);
                  *(str + n) = ' ';
                  oldx = win->_curx;
                  wmove (win, y, x);
                  for (n = left; n <= right; n++)
                  {
                     if (*(str + n))
                        wprintw (win, "%c", *(str + n));
                     else
                        wprintw (win, " ");
                  }
                  wmove (win, y, oldx - 1);
               }
               else
               /* at left hand side, not begining of buffer */
               {
                  i--;
                  for (n = i; n < len - 1; n++)
                     *(str + n) = *(str + n + 1);
                  if (left > 0)
                  {
                     left--;
                     right--;
                  }
                  wmove (win, y, x);
                  for (n = left; n <= right; n++)
                  {
                     if (*(str + n))
                        wprintw (win, "%c", *(str + n));
                     else
                        wprintw (win, " ");
                  }
                  wmove (win, y, x);
               }
            }
            else
               do_beep ();
            break;
         case SPACEBAR:
            oldx = win->_curx;
            if ((win->_curx < x + max - 1)
             && (i < len - 1) && (*(str + len - 1) <= ' '))
            {
               for (n = len - 1; n > i; n--)
               {
                  *(str + n) = *(str + n - 1);
               }
               *(str + i) = ' ';
               *(str + len) = ' ';
               i++;
               wmove (win, y, x);
               for (n = left; n <= right; n++)
               {
                  if (*(str + n))
                     wprintw (win, "%c", *(str + n));
                  else
                     wprintw (win, " ");
               }
               wmove (win, y, oldx + 1);
            }
            else if ((win->_curx == x + max - 1)
             && (i < len - 1) && (*(str + len - 1) <= ' '))
            {
               for (n = len - 1; n > i; n--)
               {
                  *(str + n) = *(str + n - 1);
               }
               *(str + i) = ' ';
               *(str + len) = ' ';
               i++;
               if (right < len - 1)
               {
                  left++;
                  right++;
               }
               wmove (win, y, x);
               for (n = left; n <= right; n++)
               {
                  if (*(str + n))
                     wprintw (win, "%c", *(str + n));
                  else
                     wprintw (win, " ");
               }
               wmove (win, y, x + max - 1);      
            }
            else if ((win->_curx == x + max - 1)
             && (i == len - 1))
            {
               *(str + len - 1) = ' ';
               wmove (win, y, x + max - 1);
               wprintw (win, " ");
               wmove (win, y, x + max - 1);
               do_beep ();
            }
            else
               do_beep ();
            break;
         
            /* erase whole line and bring cursor back to begining */
         case CONTROL_U:
            memset (str, 0x00, len);

            wmove (win, y, x);
            for (i = 0; i < max; i++)
               wprintw (win, " ");
            i = 0;
            left = 0;
            right = max - 1;
            wmove (win, y, x);
            wrefresh (win);
            break;

         default: /* normal input */
            if (isprintable (in))
            {
               if (win->_curx != (x + max - 1))
               /* not at right hand side */
               {
                  *(str + i) = in;
                  i++;
                  oldx = win->_curx;
                  wmove (win, y, x);
                  for (n = left; n <= right; n++)
                  {
                     if (*(str + n))
                        wprintw (win, "%c", *(str + n));
                     else
                        wprintw (win, " ");
                  }
                  wmove (win, y, oldx + 1);
               }
               else if (i < len - 1)
               /* right hand side, not end of buffer */
               {
                  *(str + i) = in;
                  i++;
                  if (right < len - 1)
                  {
                     left++;
                     right++;
                  }
                  wmove (win, y, x);
                  for (n = left; n <= right; n++)
                  {
                     if (*(str + n))
                        wprintw (win, "%c", *(str + n));
                     else
                        wprintw (win, " ");
                  }
                  wmove (win, y, x + max - 1);                  
               }
               else if (i == len - 1)
               /* right hand side, end of buffer */
               {
                  *(str + i) = in;
                  do_beep ();
                  wmove (win, y, x);
                  for (n = left; n <= right; n++)
                  {
                     if (*(str + n))
                        wprintw (win, "%c", *(str + n));
                     else
                        wprintw (win, " ");
                  }
                  wmove (win, y, x + max - 1);                  
               }
            }
      }
      wrefresh (win);
   }
   return str;
}


struct ret_string *
hex_string_box (WINDOW *win, int searchlen, int boxwidth)
{
   wchar_t in = 0;
   int newbyte = 1;
   int i = 0;
   int move = 0;
   int width = (searchlen * 3) - 1;
   int boxleft = (boxwidth / 2) - (width / 2);
   static struct ret_string rstr = { 0, NULL };

   rstr.len = 0;
   if (rstr.str)
   {
      rstr.len = 0;
      free (rstr.str);
      rstr.str = NULL;
   }
   rstr.str = malloc (searchlen);
   if (!rstr.str)
   {
      wmove (win, 4, boxleft);
      wprintw (win, (char *) NOT_ENOUGH_MEMORY);
      getch ();
      return NULL;
   }
   memset (rstr.str, 0, searchlen);
   wmove (win, 3, boxleft);
   wprintw (win, "text: ");
   wattrset (win, color_term ? S_BOX_COLOR : A_REVERSE);
   wmove (win, 6, boxleft);
   for (i = 0; i < width + 3; i++)
      wprintw (win, " ");
   wattroff (win, color_term ? S_BOX_COLOR : A_REVERSE);
   wmove (win, 6, boxleft);
   wrefresh (win);
   while ((in = getch ()) != '\n')
   {
      if (isHexChar (in))
      {
         if (rstr.len >= 16)
         {
            do_beep ();
            wrefresh (win);
            continue;
         }
         *(rstr.str + rstr.len) |= newbyte ?
                           getHexValue (in) << 4:
                           getHexValue (in);
         move = 0;
         if (rstr.len > 0)
            move = rstr.len + (2 * rstr.len);
         if (!newbyte)
            move += 1;
         wmove (win, 3, boxleft + strlen ("text:  ") + rstr.len);
         if (Globals.charset == ASCII_CHAR_SET)
         {
            if (isprintable (*(rstr.str + rstr.len)))
               wprintw (win, "%c", *(rstr.str + rstr.len));
            else
               wprintw (win, ".");
         }
         else
            wprintw (win, "%c", EBCDIC[*(rstr.str + rstr.len)]);
         wmove (win, 6, boxleft + move);
         wattrset (win, color_term ? S_BOX_COLOR : A_REVERSE);
         wprintw (win, "%c", isupper (in) ? in : (toupper (in)));
/* isupper (in) ? in : toupper (in)); */
         if (newbyte)
            wprintw (win, "0");
         wattroff (win, color_term ? S_BOX_COLOR : A_REVERSE);
         if (newbyte)
         {
            wmove (win, 6, boxleft + move + 1);
            newbyte = 0;
         }
         else
         {
            wmove (win, 6, boxleft + move + 2);
            newbyte = 1;
            rstr.len++;
         }
         wrefresh (win);
      }
      else
         switch (in)
         {
            case BACKSPACE:
            case CONTROL_H:
            case KEY_BACKSPACE:
            case KEY_DC:
#ifdef __PDCURSES__
               case CTL_BKSP:
#endif
               if (newbyte)
               {
                  if (rstr.len <= 0)
                  {
                     do_beep ();
                     wrefresh (win);
                     continue;
                  }
                  newbyte = 0;
                  rstr.len--;
                  *(rstr.str + rstr.len) &= M_0xF0;
                  move = rstr.len + (2 * rstr.len) + 1;
                  wmove (win, 3, boxleft + strlen ("text:  ") + rstr.len + 1);
                  wprintw (win, " ");
                  wmove (win, 6, boxleft + move + 2);
                  wattrset (win, color_term ? S_BOX_COLOR : A_REVERSE);
                  wprintw (win, "  ");
                  wmove (win, 6, boxleft + move);
                  wprintw (win, "0");
                  wattroff (win, color_term ? S_BOX_COLOR : A_REVERSE);
                  wmove (win, 6, boxleft + move);
                  wmove (win, 3, boxleft + strlen ("text:  ") + rstr.len);
                  if (isprintable (*(rstr.str + rstr.len)))
                     wprintw (win, "%c", *(rstr.str + rstr.len));
                  else
                     wprintw (win, ".");
                  wmove (win, 6, boxleft + move);
                  wrefresh (win);
               }
               else
               {
                  newbyte = 1;
                  *(rstr.str + rstr.len) = 0x00;
                  if (rstr.len > 0)
                     move = rstr.len + (2 * rstr.len);
                  else
                     move = 0;
                  wmove (win, 3, boxleft + strlen ("text:  ") + rstr.len);
                  if (isprintable (*(rstr.str + rstr.len)))
                     wprintw (win, "%c", *(rstr.str + rstr.len));
                  else
                     wprintw (win, ".");
                  wmove (win, 6, boxleft + move);
                  wattrset (win, color_term ? S_BOX_COLOR : A_REVERSE);
                  wprintw (win, "0");
                  wattroff (win, color_term ? S_BOX_COLOR : A_REVERSE);
                  wmove (win, 6, boxleft + move);
                  wrefresh (win);
               }
               break;

            case CONTROL_C:
            case CONTROL_G:
            case CONTROL_X:
            case ESCAPE_CHARACTER:
               return NULL;
         }
   }
   return &rstr;
}
