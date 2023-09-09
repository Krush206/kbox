/* misc.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

#ifdef HAVE_TERMIO_H
#include <termio.h>
#else
#ifdef HAVE_TERMIOS_H
#include <termios.h>
#else
#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif
#endif
#endif


int
mappos (int pos)
/* This is used for cursor_x.  It has a value of 0-F, depending on what
 * byte you are on in the line, and this returns the corresponding screen
 * cursor position */
{
   if (Globals.spacing)
   {
      switch (pos)
      {
         case 0:
            return 10;
         case 1:
            return 13;
         case 2:
            return 16;
         case 3:
            return 19;
         case 4:
            return 23;
         case 5:
            return 26;
         case 6:
            return 29;
         case 7:
            return 32;
         case 8:
            return 37;
         case 9:
            return 40;
         case 10:
            return 43;
         case 11:
            return 46;
         case 12:
            return 50;
         case 13:
            return 53;
         case 14:
            return 56;
         case 15:
            return 59;
      }
   }
   else 
   {
      switch (pos)
      {
         case 0:
            return 10;
         case 1:
            return 12;
         case 2:
            return 14;
         case 3:
            return 16;
         case 4:
            return 19;
         case 5:
            return 21;
         case 6:
            return 23;
         case 7:
            return 25;
         case 8:
            return 28;
         case 9:
            return 30;
         case 10:
            return 32;
         case 11:
            return 34;
         case 12:
            return 37;
         case 13:
            return 39;
         case 14:
            return 41;
         case 15:
            return 43;
      }
   }
   return 0;
}



int
mapcur (int cur)
/* This takes the cursor position and returns what byte it is on that line
 */
{
   if (Globals.spacing)
   {
      switch (cur)
      {
         case 10:
         case 11:
            return 0;
         case 13:
         case 14:
            return 1;
         case 16:
         case 17:
            return 2;
         case 19:
         case 20:
            return 3;
         case 23:
         case 24:
            return 4;
         case 26:
         case 27:
            return 5;
         case 29:
         case 30:
            return 6;
         case 32:
         case 33:
            return 7;
         case 37:
         case 38:
            return 8;
         case 40:
         case 41:
            return 9;
         case 43:
         case 44:
            return 10;
         case 46:
         case 47:
            return 11;
         case 50:
         case 51:
            return 12;
         case 53:
         case 54:
            return 13;
         case 56:
         case 57:
            return 14;
         case 59:
         case 60:
            return 15;
      }
   }
   else
   {
      switch (cur)
      {
         case 10:
         case 11:
            return 0;
         case 12:
         case 13:
            return 1;
         case 14:
         case 15:
            return 2;
         case 16:
         case 17:
            return 3;
         case 19:
         case 20:
            return 4;
         case 21:
         case 22:
            return 5;
         case 23:
         case 24:
            return 6;
         case 25:
         case 26:
            return 7;
         case 28:
         case 29:
            return 8;
         case 30:
         case 31:
            return 9;
         case 32:
         case 33:
            return 10;
         case 34:
         case 35:
            return 11;
         case 37:
         case 38:
            return 12;
         case 39:
         case 40:
            return 13;
         case 41:
         case 42:
            return 14;
         case 43:
         case 44:
            return 15;
      }
   }
   return 0;
}


int
cursor_ascii_to_ebcdic (int cur)
{
   if (!Globals.spacing)
   {
      switch (cur)
      {
         case 10:
            return 10;
         case 11:
            return 11;
         case 13:
            return 12;
         case 14:
            return 13;
         case 16:
            return 14;
         case 17:
            return 15;
         case 19:
            return 16;
         case 20:
            return 17;
         case 23:
            return 19;
         case 24:
            return 20;
         case 26:
            return 21;
         case 27:
            return 22;
         case 29:
            return 23;
         case 30:
            return 24;
         case 32:
            return 25;
         case 33:
            return 26;
         case 37:
            return 28;
         case 38:
            return 29;
         case 40:
            return 30;
         case 41:
            return 31;
         case 43:
            return 32;
         case 44:
            return 33;
         case 46:
            return 34;
         case 47:
            return 35;
         case 50:
            return 37;
         case 51:
            return 38;
         case 53:
            return 39;
         case 54:
            return 40;
         case 56:
            return 41;
         case 57:
            return 42;
         case 59:
            return 43;
         case 60:
            return 44;
      }
   }
   else
   {
      switch (cur)
      {
         case 10:
            return 10;
         case 11:
            return 11;
         case 12:
            return 13;
         case 13:
            return 14;
         case 14:
            return 16;
         case 15:
            return 17;
         case 16:
            return 19;
         case 17:
            return 20;
         case 19:
            return 23;
         case 20:
            return 24;
         case 21:
            return 26;
         case 22:
            return 27;
         case 23:
            return 29;
         case 24:
            return 30;
         case 25:
            return 32;
         case 26:
            return 33;
         case 28:
            return 37;
         case 29:
            return 38;
         case 30:
            return 40;
         case 31:
            return 41;
         case 32:
            return 43;
         case 33:
            return 44;
         case 34:
            return 46;
         case 35:
            return 47;
         case 37:
            return 50;
         case 38:
            return 51;
         case 39:
            return 53;
         case 40:
            return 54;
         case 41:
            return 56;
         case 42:
            return 57;
         case 43:
            return 59;
         case 44:
            return 60;
      }
   }
   return 0;
}



void
usage (char *prog)
{
   printf ("[N]Curses Hexedit %s by Adam Rogoyski <apoc@laker.net>\n" \
     "Copyright (C) 1998, 1999 Adam Rogoyski\n" \
     "This is free software; see the source for copying conditions.\n" \
     "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A\n" \
     "PARTICULAR PURPOSE.\n\n" \
     "Usage: %s [options] [file]\n" \
     "Options:\n" \
     "  -h, --help              Print this message and exit.\n"
     "  -8, --highbit           Print high order 8-bit text.\n"
     "  -a, --alltext           Print all text characters.\n"
     "  -b, --buffer            Buffer the entire file in memory.\n"
     "                            Much faster and enables insert/delete.\n"
#if  defined (__linux__) || defined (__OpenBSD__)
     "  -d, --disk              Edit a fixed disk, i.e. /dev/hda (Read-only)\n"
     "  -f, --force             Force editing of disk.\n"
     "                            Needed to write to disks.\n"
#endif
     "  -n, --nocolor           Force Gray scale, no colors.\n"
     "  -q, --quiet             Quiet Mode, No annoying beeping\n"
     "  -r, --readonly          Do not modifying of the file.\n"
     "  -v, --version           Print the version number and exit.\n",
     VERSION, prog);
}


void
die_horribly (const char * const message, const char * const pmsg)
   /* clear the screen reset the screen, and optionally print a message
    * why hexedit terminated to standard error.  Possibly print perror ().
    */
{
   erase ();
   refresh ();
   endwin ();

   if (message)
      fprintf (stderr, "%s\n", message);

   if (pmsg)
      perror (pmsg);

   exit (EXIT_FAILURE);
}


void
handleInterrupt (int i)
#define QUIT_BOX_WIDTH  60
#define QUIT_BOX_HEIGHT 6
#define YES_VALUE       1
#define NO_VALUE        2
#define NEW_FILE_VALUE  3
#define FIRST_VALUE     YES_VALUE
#define LAST_VALUE      NEW_FILE_VALUE
{
   int done = 0;
   int j = 0;
   int value = YES_VALUE;
   wchar_t in = 0;
   static int semph = 0;
   WINDOW *win = popupWindow (QUIT_BOX_WIDTH, QUIT_BOX_HEIGHT);

/*   signal (SIGINT, SIG_IGN); */

   if (semph)
      return;
   semph = 1;

   box (win, 0, 0);
   if (Globals.modified == READ_ONLY)
   {
      wmove (win, 2,
             QUIT_BOX_WIDTH / 2 - strlen ("   Really Quit?        ") / 2);
      wprintw (win, "   Really Quit?        ");
   }
   else
   {
      wmove (win, 2,
             QUIT_BOX_WIDTH / 2 - strlen ("Quit without Saving?") / 2);
      wprintw (win, "Quit without Saving?");
   }
   wmove (win, 4, QUIT_BOX_WIDTH / 2
          - (strlen (" Yes       No       New File ") / 2));
   wattrset (win, A_REVERSE);
   wprintw (win, " Yes ");
   wattroff (win, A_REVERSE);

   wprintw (win, "      No");

   wprintw (win, "       New File");
   wrefresh (win);

   wmove (Globals.whelp, 0, 0);
   wprintw (Globals.whelp, "^G/^X/Escape Cancel   Y Yes   N No   F New File");
   for (j = strlen ("^G/^X/Escape Cancel   Y Yes   N No   F New File");
        j < COLS; j++)
      wprintw (Globals.whelp, " ");
   wrefresh (Globals.whelp);

   value = YES_VALUE;
   while (!done && (in = getch ()) != '\n')
   {
      switch (in)
      {
         case TAB:
         case KEY_RIGHT:
            (value == LAST_VALUE) ? value = FIRST_VALUE : value++;
            break;

         case KEY_LEFT:
            (value == FIRST_VALUE) ? value = LAST_VALUE : value--;
            break;

         case 'n':
         case 'N':
         case CONTROL_G:
         case CONTROL_X:
         case ESCAPE_CHARACTER:
            value = NO_VALUE;
            done = 1;
            break;
        
         case 'y':
         case 'Y':
            value = YES_VALUE;
            done = 1;
            break;
        
         case 'f':
         case 'F':
            value = NEW_FILE_VALUE;
            done = 1;
            break;
      }

      if (value == YES_VALUE)
      {
         wmove (win, 4, QUIT_BOX_WIDTH / 2
                - (strlen (" Yes       No       New File ") / 2));
         wattrset (win, A_REVERSE);
         wprintw (win, " Yes ");
         wattroff (win, A_REVERSE);
         wprintw (win, "      No       New File ");
      }
      else if (value == NO_VALUE)
      {
         wmove (win, 4, QUIT_BOX_WIDTH / 2
                - (strlen (" Yes       No       New File ") / 2));
         wattroff (win, A_REVERSE);
         wprintw (win, " Yes      ");
         wattrset (win, A_REVERSE);
         wprintw (win, " No ");
         wattroff (win, A_REVERSE);
         wprintw (win, "      New File ");
      }
      else if (value == NEW_FILE_VALUE)
      {
         wmove (win, 4, QUIT_BOX_WIDTH / 2
                - (strlen (" Yes       No       New File ") / 2));
         wattroff (win, A_REVERSE);
         wprintw (win, " Yes       No      ");
         wattrset (win, A_REVERSE);
         wprintw (win, " New File ");
      }
      wrefresh (win);
   }   

   if (value == YES_VALUE)
      exitProgram ();
   else if (value == NEW_FILE_VALUE)
   {
      FILE *fp = NULL;

      load_new_file (&fp);
      cursor_y = MAIN_TOP_LINE;
      cursor_x = 10;
      offset = 0x00;
      Globals.modified = 0;
      Globals.tabb = 0;
   }
   redraw ();
   semph = 0;
/*   signal (SIGINT, handleInterrupt); */
}


#ifndef __PDCURSES__
void
handleSigwinch (int i)
{
   char *tty = NULL;
   int fd = 0;
   int result = 0;
   struct winsize win;

   tty = ttyname (0);
   if (!tty)
      return;
   fd = open (tty, O_RDWR);
   if (fd == -1)
   {
      perror (tty);
      return;
   }
   result = ioctl (fd, TIOCGWINSZ, &win);
   if (result == -1)
   {
      perror ("ioctl TIOCGWINSZ");
      return;
   }
   delwin (Globals.wmain);
   delwin (Globals.wstatus);
   delwin (Globals.whelp);
   endwin ();
   COLS = win.ws_col;
   LINES = win.ws_row;
   initscr ();
   if (stdscr == NULL)
      die_horribly ("Cannot initialize screen - curses!\n", NULL);
   keypad (stdscr, TRUE);
   scrollok (stdscr, FALSE);
   cbreak ();
   noecho ();
   refresh ();
   Globals.wmain = newwin (MAIN_HEIGHT, COLS, MAIN_TOP_LINE, 0);
   if (Globals.wmain == NULL)
      die_horribly ("Cannot open larger main window - curses!\n", NULL);
   scrollok (Globals.wmain, FALSE);
#ifdef __PDCURSES__
   leaveok(Globals.wmain, FALSE);  /* Stupid macro */
#else
      leaveok (Globals.wmain, FALSE);
#endif
   Globals.wstatus = newwin (1, COLS, 0, 0);
   if (Globals.wstatus == NULL)
   {
      fprintf (stderr, "Cannot open status window - curses!\n");
      exit (EXIT_FAILURE);
   }
   scrollok (Globals.wstatus, FALSE);
   wattrset (Globals.wstatus, color_term ? COLOR_PAIR(3) | A_BOLD
                                         : A_REVERSE);
   Globals.whelp = newwin (1, COLS, LINES - 1, 0);
   if (Globals.whelp == NULL)
      die_horribly ("Cannot open help window - curses!\n", NULL);
   scrollok (Globals.whelp, FALSE);
   if (newlines)
      free (newlines);
   newlines = malloc (BOTTOM_LINE * sizeof (int));
   if (!newlines)
      die_horribly (NOT_ENOUGH_MEMORY, NULL);

   Globals.wmain->_cury = cursor_y - 1;
   Globals.wmain->_curx = cursor_x;
   stdscr->_cury = cursor_y;
   stdscr->_curx = cursor_x;
   if (cursor_y >= BOTTOM_LINE)
      cursor_y = BOTTOM_LINE;
   if (Globals.mode == FILE_MODE)
   {
      extern struct FileNames *fp;
      extern struct FileNames **pages;
      extern int current_page;
      extern int num_pages;
      char trunc_file[PATH_MAX + 1];
      struct FileNames *p = *pages;
      struct FileNames *pfront = *pages;
      int i = 0;
      int j = 0;
      int n = 0;

      if (p)
      {
         while (p->p)
         {
            p = p->p;
            n++;
         }
      }
      if (pages)
         free (pages);
      num_pages = ((n - 1) / (LINES - 2)) + 1;
      pages = malloc (sizeof (struct FileNames *) * num_pages);
      if (!pages)
         die_horribly (NOT_ENOUGH_MEMORY, NULL);

      memset (pages, 0x00, num_pages * sizeof (struct FileNames *));
      *pages = pfront;
      p = pfront;
      for (i = 1; i < num_pages; i++)
      {
         for (j = 0; (j < (LINES - 2)) && p; j++)
            p = p->p;
         *(pages + i) = p;
      }

      i = 0;
      p = pfront;
      while (p != fp)
      {
         if (p == *(pages + i + 1))
            i++;
         p = p->p;
         
      }
      if (p == *(pages + i + 1))
         i++;
      current_page = i;
      cursor_y = MAIN_TOP_LINE;
      p = *(pages + current_page);
      while (p != fp)
      {
         p = p->p;
         cursor_y++;
      }
      refresh ();
      wrefresh (Globals.wmain);
      wrefresh (Globals.wstatus);
      wrefresh (Globals.whelp);
      printPage (*(pages + current_page));
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove    (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
      wattrset (Globals.wmain, A_BOLD);
      wprintw  (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, A_BOLD);

      statWindow (fp->filename);
      helpWindow ("^C ^X  Exit Program   ^M  Select File");
      move (cursor_y, NAME_POS);
      wrefresh (Globals.wmain);
      wrefresh (Globals.wstatus);
      wrefresh (Globals.whelp);
   }
   else
   {
      redraw ();
      redraw ();
   }
   refresh ();
/*   signal (SIGWINCH, handleSigwinch); */
}
#endif


void
exitProgram (void)
{
   int i = 0;
   struct Change *cp = NULL;

   move (LINES - 1, 0);
   wattroff (Globals.wmain, A_REVERSE);
   for (i = 0; i < COLS; i++)
      wprintw (stdscr, " ");
   wrefresh (stdscr);
   werase (Globals.whelp);
   wrefresh (Globals.whelp);
   delwin (Globals.wstatus);
   delwin (Globals.wmain);
   delwin (Globals.whelp);
   delwin (stdscr);
   free (filebuf);
   free (newlines);

   while (UndoStack.base)
   {
      cp = UndoStack.base->p;
      free (UndoStack.base);
      UndoStack.base = cp;
   }
   endwin ();
   exit (EXIT_SUCCESS);
}


void
exitSave (int saved)
{
   if (saved)
      exitProgram ();
   redraw ();
}


char *
chompWhiteSpace (char *str)
{
   int i = 0;
   i = 0;
   while (*(str + i))
   {
      if ((*(str + i) == ' ') || (*(str + i) == '\r')
         || (*(str + i) == '\t') || (*(str + i) == '\n'))
      {
         *(str + i) = 0;
         break;
      }
      else
         i++;
   }
   return str;
}


int
isHexChar (wchar_t c)
{
   switch (c)
   {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
         return 1;
      default:
        return 0;
   }
   return 0;
}


int
getHexValue (wchar_t c)
{
   switch (c)
   {
      case '0':
         return 0x00;
      case '1':
         return 0x01;
      case '2':
         return 0x02;
      case '3':
         return 0x03;
      case '4':
         return 0x04;
      case '5':
         return 0x05;
      case '6':
         return 0x06;
      case '7':
         return 0x07;
      case '8':
         return 0x08;
      case '9':
         return 0x09;
      case 'a':
      case 'A':
         return 0x0A;
      case 'b':
      case 'B':
         return 0x0B;
      case 'c':
      case 'C':
         return 0x0C;
      case 'd':
      case 'D':
         return 0x0D;
      case 'e':
      case 'E':
         return 0x0E;
      case 'f':
      case 'F':
         return 0x0F;
   }
   return 0;
}

char
getAsciiValue (wchar_t c)
{
   switch (c)
   {
      case 0:
         return '0';
      case 1:
         return '1';
      case 2:
         return '2';
      case 3:
         return '3';
      case 4:
         return '4';
      case 5:
         return '5';
      case 6:
         return '6';
      case 7:
         return '7';
      case 8:
         return '8';
      case 9:
         return '9';
      case 10:
         return 'A';
      case 11:
         return 'B';
      case 12:
         return 'C';
      case 13:
         return 'D';
      case 14:
         return 'E';
      case 15:
         return 'F';
   }
   return '\0';
}


void
switchModes (void)
{
   if (Globals.mode == HEX_MODE)
      Globals.mode = ASCII_MODE;
   else
      Globals.mode = HEX_MODE;
   redraw ();
}


int
isprintable (int c)
{
   if (Globals.charset == EBCDIC_CHAR_SET)
      return 1;

   switch (Globals.print_mode)
   {
      case REGULAR_PRINT:
#ifdef HAVE_ISPRINT
         return isprint (c);
#else
         return ((c > 32) && (c < 127));
#endif

      case HIGH_ASCII_PRINT:
#ifdef HAVE_ISPRINT
         return (isprint (c) || ((c > 127)));
#else
         return ((c > 32) && (c <= 255) && (c != 127));
#endif

      case ALL_PRINT:
         return (c > 0);

   }      
   return 0;   
}


void
do_beep ()
{
   if (Globals.beeping)
      beep ();
}
