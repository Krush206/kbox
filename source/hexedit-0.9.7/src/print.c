/* print.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

static long findNewline   (unsigned long);
static void drawAscii     (unsigned long off);
static void drawAsciiDump (unsigned long, unsigned long *);

const unsigned char EBCDIC[] = {
'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
' ', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '<', '(', '+', '|',
'&', '.', '.', '.', '.', '.', '.', '.', '.', '.', '!', '$', '*', ')', ';', '^',
'-', '/', '.', '.', '.', '.', '.', '.', '.', '.', '|', ',', '%', '_', '>', '?',
'.', '.', '.', '.', '.', '.', '.', '.', '.', '`', ':', '#', '@', '\'', '=','"',
'.', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', '.', '{', '.', '.', '.', '.',
'.', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', '.', '}', '.', '.', '.', '.',
'.', '~', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', '.', '.', '[', '.', '.',
'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', ']', '.', '.',
'{', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', '.', '.', '.', '.', '.', '.',
'}', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', '.', '.', '.', '.', '.', '.',
'\\', '.', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.', '.', '.', '.', '.','.',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '.', '.', '.', '.', '.',
};



void
drawdump (unsigned long off)
{
   int i = 0;
   int pos = 0;
   int space = 0;
   int cur_y = 0;
   int bold = 0;
   long upto = (off & M_0xFFFFFFF0) + ((MAIN_HEIGHT) * M_0x10);

   if (color_term)
      wattron (Globals.wmain, COLOR_PAIR(1));
   werase (Globals.wmain);
   off &= M_0xFFFFFFF0;

   while ((off < upto) && (off < Globals.filesize + 1))
   {
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);
      if (pos == 0)
         wprintw (Globals.wmain, "%08X  ", off);
      if (pos == 4 || pos == 12)
      {
         space++;
      }
      else if (pos == 8)
      {
         if (Globals.spacing)
            space = 3;
         else
            space = 2;
      }
      bold = hash_lookup (off, NULL);
      if (!bold)
         wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
      if (Globals.spacing)
      {
         wmove (Globals.wmain, cur_y, (pos * 3) + 10 + space);
         wprintw (Globals.wmain, "%02lX ", filebuffer (off));
         if ((pos == 3) || (pos == 11))
            wprintw (Globals.wmain, " ");
         else if (pos == 7)
            wprintw (Globals.wmain, "  ");
      }
      else
      {
         wmove (Globals.wmain, cur_y, (pos * 2) + 10 + space);
         wprintw (Globals.wmain, "%02lX", filebuffer (off));
         if ((pos == 3) || (pos == 7) || (pos == 11))
            wprintw (Globals.wmain, " ");
      }      
      wmove (Globals.wmain, cur_y, COLS - M_0x10 + pos);
      if (Globals.charset == ASCII_CHAR_SET)
      {
         if (isprintable (filebuffer (off)))
            wprintw (Globals.wmain, "%c", filebuffer (off));
         else
            wprintw (Globals.wmain, ".");
      }
      else /* EBCDIC */
      {
         wprintw (Globals.wmain, "%c", EBCDIC[filebuffer (off)]);
      }
      if (!bold)
         wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
      if (pos == 15)
      {
         if (cur_y == BOTTOM_LINE)
            break;

         if (Globals.spacing)
            i = (pos * 3) + 10 + space + 2;
         else
            i = (pos * 2) + 10 + space + 2;
         wmove (Globals.wmain, cur_y, i);
         for (; i < (COLS - 16); i++)
            wprintw (Globals.wmain, " ");

         cur_y++;

         pos = 0;
         space = 0;
         wmove (Globals.wmain, cur_y, 0);
      }
      else
         pos++;
      off++;
   }
   position = pos;

   wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);

   if (color_term)
   {
      /* If we finished the file and there's screen real estate not
       * written to, we fill it in with the default color.
       */
      /* First fill in the rest of the line. */
      if ((off == (Globals.filesize + 1))
      && (((off - 1) & M_0x0F) < M_0x0F))
      {
         i = mappos (pos);
         wmove (Globals.wmain, cur_y, i);
         for (; i < (COLS - 16); i++)      /* left side of screen. */
            wprintw (Globals.wmain, " ");

         i = COLS - M_0x10 + pos;
         wmove (Globals.wmain, cur_y, i);
         for (; i < COLS; i++)             /* right side of screen. */
            wprintw (Globals.wmain, " ");
      }

         /* All empty lines below get the default color */
      if (!(off & M_0x0F))
         cur_y--;
      while (++cur_y < BOTTOM_LINE)
      {
         wmove (Globals.wmain, cur_y, 0);
         for (i = 0; i < COLS; i++)
            wprintw (Globals.wmain, " ");
      }
   }
}


long
findNewline (unsigned long off)
/* finds the first newline before the current offset.  This is used for
 * drawing the control-e ascii/text screen
 */
{
   while (off > 0x00)
   {
      if (filebuffer (off) == '\n')
      {
         off++;
         break;
      }
      off--;
   }
   return off;
}


void
drawAsciiDump (unsigned long off, unsigned long *offs)
{
   int move = 0;
   int i = 0;
   int bold = 0;

   while (1)
   {
      if (*offs > Globals.filesize)
         break;
#ifdef __NCURSES_H     /* i don't know why this works */
      if (Globals.wmain->_cury == MAIN_BOTTOM_LINE)
#else
      if (Globals.wmain->_cury == MAIN_HEIGHT)
#endif
         break;
      if (Globals.wmain->_curx == COLS - 2)
      {
         move = 1;
      }
      if (Globals.wmain->_curx == 0)
         *(newlines + i++) = *offs;
      bold = hash_lookup (*offs, NULL);
      if (!bold)
         wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
      if (Globals.charset == ASCII_CHAR_SET)
      {
         if (filebuffer (*offs) == '\n')
         {
            int cury = Globals.wmain->_cury;
            int i = Globals.wmain->_curx;
            for (; i < COLS - 2; i++)
               wprintw (Globals.wmain, " ");
            wprintw (Globals.wmain, ".");
            wmove (Globals.wmain, cury + 1, 0);
         }
         else if (filebuffer (*offs) == ' ')
         {
            wprintw (Globals.wmain, " ");
         } 
         else if (isprintable (filebuffer (*offs)))
            wprintw (Globals.wmain, "%c", filebuffer (*offs));
         else
            wprintw (Globals.wmain, ".");
      }
      else /* EBCDIC */
      {
         if (filebuffer (*offs) == EBCDIC['\n'])
         {
            int cury = Globals.wmain->_cury;
            wprintw (Globals.wmain, ".");
            wmove (Globals.wmain, cury + 1, 0);
         }
         else if (filebuffer (*offs) == EBCDIC[' '])
         {
            wprintw (Globals.wmain, " ");
         } 
         else
            wprintw (Globals.wmain, "%c", EBCDIC[filebuffer (*offs)]);
      }
     
      if (!bold)
      {
         wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
         if (color_term)
            wattrset (Globals.wmain, COLOR_PAIR(1));
      }
      (*offs)++;
      if (move)
      {
         wmove (Globals.wmain, Globals.wmain->_cury + 1, 0);
         move = 0;
         continue;
      }
      if (*offs > Globals.filesize)
         break;
   }
   *(newlines + i) = *offs + 1;
}

void
drawAscii (unsigned long off)
{
   int i = 0;
   unsigned long offs = 0;
   unsigned long os = 0;
   wmove (Globals.wmain, 0, 0);
   os = offs = findNewline (off);
   memset (newlines, 0, BOTTOM_LINE * sizeof (int));
   drawAsciiDump (off, &offs);
   while (offset >= offs)
   {
      memset (newlines, 0, BOTTOM_LINE * sizeof (int));
      werase (Globals.wmain);
      wmove (Globals.wmain, 0, 0);
      drawAsciiDump (offs, &offs);
      if (offset >= Globals.filesize)
         break;
   }
      acursor_y = 1;
      acursor_x = 0;
      for (i = 0; i < BOTTOM_LINE; i++)
      {
         if (offset < *(newlines + i))
         {
            acursor_y = i;
            if (i > 0)
               acursor_x = offset - *(newlines + i - 1);
            else
               acursor_x = offset - os;
            if (acursor_x >= COLS)
            {
               acursor_y += (int) (acursor_x / COLS);
               acursor_x %= COLS;
            }
            break;
         }
      }
}


void
printStatusWindow (void)
{
   static char *buf = NULL;
   static int first = 0;
   static int length = 0;
   double percent = 0.0;
   if (!first)
   {
      length = COLS - 54; /* 54 is rest of status bar needed for other info */
      buf = malloc (PATH_MAX + 1); /* never gets freed. */
      if (!buf)
         die_horribly (NOT_ENOUGH_MEMORY, NULL);

      memset (buf, 0, PATH_MAX + 1);
      first = 1;
   }
   memset (buf, ' ', PATH_MAX);
   length = COLS - 54;
   if (strlen (Globals.filename) < length)
      strncpy (buf, Globals.filename, strlen (Globals.filename));
   else
      strncpy (buf, Globals.filename, length - 1);
   *(buf + length - 1) = '\0';
   if (Globals.filesize <= 0)
      percent = 0.0;
   else
      percent = ((double) offset / (double) Globals.filesize) * 100;
   wmove (Globals.wstatus, 0, 0);
   wprintw (Globals.wstatus,
           "File: %-s %7s Offset: 0x%08lX / 0x%08lX (%%%02.0f) ",
                  buf, 
                  Globals.charset == ASCII_CHAR_SET ? "ASCII" : "EBCDIC",
                  offset, Globals.filesize, percent);
      wmove (Globals.wstatus, 0, COLS - 1);
   if (Globals.modified == MODIFIED)
      wprintw (Globals.wstatus, "M");
   else if (Globals.modified == READ_ONLY)
      wprintw (Globals.wstatus, "R");
   else
      wprintw (Globals.wstatus, " ");
   wrefresh (Globals.wstatus);
}


void
printHelpWindow (void)
{
   int len = COLS;

      /* Only what can fit on the screen will be displayed */   
   wmove (Globals.whelp, 0, 0);
   if (len > strlen ("^G Help"))
   {
      wprintw (Globals.whelp, "^G");
      wprintw (Globals.whelp, " Help");
      len -= strlen ("^G Help");
   }
   if (len > strlen ("   ^C Exit (No Save)"))
   {
      wprintw (Globals.whelp, "   ^C");
      if (Globals.modified == READ_ONLY)
      {
         wprintw (Globals.whelp, " Exit");
         len -= strlen ("   ^C Exit");         
      }
      else
      {
         wprintw (Globals.whelp, " Exit (No Save)");
         len -= strlen ("   ^C Exit (No Save)");
      }
   }
   if (len > strlen ("   ^T goTo Offset"))
   {
      wprintw (Globals.whelp, "   ^T");
      wprintw (Globals.whelp, " goTo Offset");
      len -= strlen ("   ^T goTo Offset");
   }
   if (len > strlen ("   ^X Exit and Save"))
   {
      wprintw (Globals.whelp, "   ^X");
      if (Globals.modified == READ_ONLY)
      {
         wprintw (Globals.whelp, " Exit");
         len -= strlen ("   ^X Exit");
      }
      else
      {
         wprintw (Globals.whelp, " Exit and Save");
         len -= strlen ("   ^X Exit and Save");
      }
   }
   if (len > strlen ("   ^W Search"))
   {
      wprintw (Globals.whelp, "   ^W");
      wprintw (Globals.whelp, " Search");
      len -= strlen ("   ^W Search");
   }
   if ((Globals.modified != READ_ONLY) && (len > strlen ("   ^U Undo")))
   {
      wprintw (Globals.whelp, "   ^U");
      wprintw (Globals.whelp, " Undo");
      len -= strlen ("   ^U Undo");
   }
   if (len > strlen ("   ^L Redraw"))
   {
      wprintw (Globals.whelp, "   ^L");
      wprintw (Globals.whelp, " Redraw");
      len -= strlen ("   ^L Redraw");
   }
   if (len > strlen ("   ^E Text Mode"))
   {
      wprintw (Globals.whelp, "   ^E");
      wprintw (Globals.whelp, " Text Mode");
      len -= strlen ("   ^E Text Mode");
   }
   if (len > strlen ("   ^R CharSet"))
   {
      wprintw (Globals.whelp, "   ^R");
      wprintw (Globals.whelp, " CharSet");
      len -= strlen ("   ^R CharSet");
   }
   if (len > strlen ("   ^P Spacing"))
   {
      wprintw (Globals.whelp, "   ^P");
      wprintw (Globals.whelp, " Spacing");
      len -= strlen ("   ^P Spacing");
   }
   if (len > strlen ("   ^N Find Next"))
   {
      wprintw (Globals.whelp, "   ^N");
      wprintw (Globals.whelp, " Find Next");
      len -= strlen ("   ^N Find Next");
   }
   if (len > strlen ("   F5 Color"))
   {
      wprintw (Globals.whelp, "   F5");
      wprintw (Globals.whelp, " Color");
      len -= strlen ("   ^F5 Color");
   }
   wrefresh (Globals.whelp);
}


void
redraw ()
{
   int i = 0;
   unsigned long off = offset;

   off -= M_0x10 * (cursor_y - MAIN_TOP_LINE);
   if (off > Globals.filesize)
      off = 0;
   werase (Globals.wstatus);
   werase (Globals.wmain);
   werase (Globals.whelp);
   wrefresh (Globals.wstatus);
   wrefresh (Globals.wmain);
   wrefresh (Globals.whelp);

   wattrset (Globals.whelp, color_term ? COLOR_PAIR(3) | A_BOLD : A_NORMAL);
   wattrset (Globals.wstatus, color_term ? COLOR_PAIR(4) | A_BOLD : A_REVERSE);

   move (0, 0);
   wmove (Globals.wstatus, 0, 0);
   for (i = 0; i < COLS; i++)
      wprintw (Globals.wstatus, " ");

   if (Globals.mode == CALCULATOR_MODE)
   {
      extern struct calcEntryBox *bbox;
      extern struct calcEntryBox *obox;
      extern struct calcEntryBox *dbox;
      extern struct calcEntryBox *hbox;
      extern int calc_mode;

      wmove (Globals.wmain, 1, (COLS / 2) 
             - (strlen ("Binary Calculator") / 2));
      wprintw (Globals.wmain, "Binary Calculator");

      calcStatWindow ();
      wmove (Globals.whelp, 0, 0);
      wprintw (Globals.whelp, "^X/^G  Return   ^C  Exit   J  Down   K  Up");
      calcDrawBoxes (bbox, obox, dbox, hbox, calc_mode);
      wrefresh (Globals.wstatus);
      wrefresh (Globals.whelp);
      wrefresh (Globals.wmain);
      refresh ();
   }
   else
   {
      printStatusWindow ();
      printHelpWindow ();
      wmove (Globals.wmain, 0, 0);
      if (Globals.mode == HEX_MODE)
      {
         drawdump (off);
         wrefresh (Globals.wmain);
         if (Globals.tabb)
            move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
         else
            move (cursor_y, cursor_x);
      }
      else
      {
         drawAscii (off);
         wrefresh (Globals.wmain);
         move (acursor_y, acursor_x);
      }
   }
   refresh ();
}
