/* filekeys.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

#define NAME_POS 57   /* Magic number of where filename always starts */

extern struct FileNames *fp;
extern struct FileNames **pages;  /* node that starts a new page */


void
file_key_up (int *current_page)
{
   char trunc_file[PATH_MAX + 1];

   if (cursor_y <= MAIN_TOP_LINE)
   {
      if (*current_page <= 0)
      /*
         do_beep ();
      */
         ;
      else
      {
	 cursor_y = BOTTOM_LINE;
	 memset (trunc_file, 0x00, PATH_MAX + 1);
	 fp = *(pages + --*current_page);
	 while (fp->p != *(pages + *current_page + 1))
	    fp = fp->p;
	 werase (Globals.wstatus);
	 werase (Globals.wmain);
	 printPage (*(pages + *current_page));
	 statWindow (fp->filename);
	 wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
		NAME_POS);
	 strncpy (trunc_file, fp->filename, COLS - NAME_POS);
	 wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
	 wprintw (Globals.wmain, "%s", trunc_file);
	 wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
	 wrefresh (Globals.wstatus);
	 wrefresh (Globals.wmain);
	 move (cursor_y, NAME_POS);
      }
   }
   else
   {
      struct FileNames *tp = *(pages + *current_page);
      while (tp->p != fp)
	 tp = tp->p;
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
	     NAME_POS);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);
      wprintw (Globals.wmain, "%s", trunc_file);
      cursor_y--;
      fp = tp;
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
	     NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      statWindow (fp->filename);
      wrefresh (Globals.wstatus);
      wrefresh (Globals.wmain);
      move (cursor_y, NAME_POS);
   }
   refresh ();
}


void
file_key_down (int *current_page, int n)
{
   char trunc_file[PATH_MAX + 1];

   if (cursor_y >= BOTTOM_LINE)
   {
      if ((*current_page < (n - 1)) && (n > 1))
      {
	 werase (Globals.wstatus);
	 werase (Globals.wmain);
	 printPage (*(pages + ++*current_page));
	 fp = *(pages + *current_page);
	 wmove (Globals.wmain, 0, NAME_POS);
	 wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
	 strncpy (trunc_file, fp->filename, COLS - NAME_POS);
	 wprintw (Globals.wmain, "%s", trunc_file);
	 wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
	 statWindow (fp->filename);
	 cursor_y = MAIN_TOP_LINE;
	 move (cursor_y, NAME_POS);
	 wrefresh (Globals.wstatus);
	 wrefresh (Globals.wmain);
      }
      else
      /*
         do_beep ();
      */
         ;
   }
   else
   {
      if (!fp->p)
      /*
         do_beep ();
      */
         ;
      else
      {
	 wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
		NAME_POS);
	 strncpy (trunc_file, fp->filename, COLS - NAME_POS);
         wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);
	 wprintw (Globals.wmain, "%s", trunc_file);
	 cursor_y++;
	 fp = fp->p;
	 memset (trunc_file, 0x00, PATH_MAX + 1);
	 strncpy (trunc_file, fp->filename, COLS - NAME_POS);
	 wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
		NAME_POS);
	 wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
	 wprintw (Globals.wmain, "%s", trunc_file);
	 wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                             : A_BOLD);
	 statWindow (fp->filename);
	 wrefresh (Globals.wstatus);
	 wrefresh (Globals.wmain);
	 move (cursor_y, NAME_POS);
      }
   }
   refresh ();
}


void
file_redraw (int *current_page)
{
   char trunc_file[PATH_MAX + 1];

   werase (Globals.wstatus);
   wrefresh (Globals.wstatus);
   werase (Globals.wmain);
   wrefresh (Globals.wmain);
   werase (Globals.whelp);
   wrefresh (Globals.whelp);
   printPage (*(pages + *current_page));
   wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
   wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                       : A_BOLD);
   strncpy (trunc_file, fp->filename, COLS - NAME_POS);
   wprintw (Globals.wmain, "%s", trunc_file);
   wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                       : A_BOLD);
   statWindow (fp->filename);
   helpWindow ("^C ^X  Exit   ^M  Select File");
   wrefresh (Globals.wstatus);
   wrefresh (Globals.wmain);
   wrefresh (Globals.whelp);
   wmove (Globals.wmain, 0, NAME_POS);
   move (cursor_y, NAME_POS);
   refresh ();
}


void
file_pagedown (int *current_page, int n)
{
   char trunc_file[PATH_MAX + 1];

   if ((*current_page < (n - 1)) && (n > 1))
   {
      int i = MAIN_TOP_LINE;
      fp = *(pages + ++*current_page);
      for (i = MAIN_TOP_LINE; (i < cursor_y) && fp->p; i++)
      {
         fp = fp->p;
      }
      cursor_y = i;  /* bottom may not exist */
      werase (Globals.wmain);
      printPage (*(pages + *current_page));
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
   }
   else
   {
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);
      wprintw (Globals.wmain, "%s", trunc_file);

      cursor_y = MAIN_TOP_LINE;
      fp = *(pages + *current_page);
      while (fp->p)
      {
         cursor_y++;
         fp = fp->p;
      }
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
   }
   statWindow (fp->filename);
   move (cursor_y, NAME_POS);
   wrefresh (Globals.wstatus);
   wrefresh (Globals.wmain);
   refresh ();
}


void
file_pageup (int *current_page)
{
   char trunc_file[PATH_MAX + 1];

   if (*current_page > 0)
   {
      int i = MAIN_TOP_LINE;
      fp = *(pages + --*current_page);
      for (i = MAIN_TOP_LINE; i < cursor_y; i++)
      {
         fp = fp->p;
      }
      werase (Globals.wmain);
      printPage (*(pages + *current_page));
      statWindow (fp->filename);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
   }
   else
   {
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);
      wprintw (Globals.wmain, "%s", trunc_file);

      cursor_y = MAIN_TOP_LINE;
      fp = *pages;
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE, NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      statWindow (fp->filename);
   }
   move (cursor_y, NAME_POS);
   wrefresh (Globals.wstatus);
   wrefresh (Globals.wmain);
   refresh ();
}


void
file_home (int *current_page)
{
   char trunc_file[PATH_MAX + 1];

   if (*current_page)
   {
      *current_page = 0;
      cursor_y = MAIN_TOP_LINE;
      werase (Globals.wmain);
      werase (Globals.wstatus);
      fp = *pages;
      statWindow (fp->filename);
      printPage (*pages);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
	     NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      move (cursor_y, NAME_POS);
   }
   else
      /* already on first page */
   {
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
	     NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);
      wprintw (Globals.wmain, "%s", trunc_file);
      fp = *pages;
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      cursor_y = MAIN_TOP_LINE;
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
	     NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      werase (Globals.wstatus);
      statWindow (fp->filename);
      move (cursor_y, NAME_POS);
   }
   wrefresh (Globals.wstatus);
   wrefresh (Globals.wmain);
   refresh ();
}


void
file_end (int *current_page, int n)
{
   char trunc_file[PATH_MAX + 1];

   if (*current_page < (n - 1))
   {
      cursor_y = MAIN_TOP_LINE;
      werase (Globals.wmain);
      *current_page = n - 1;
      fp = *(pages + *current_page);
      while (fp->p)
      {
         cursor_y++;
         fp = fp->p;
      }
      printPage (*(pages + *current_page));
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
	     NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
   }
   else
   {
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
	     NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(1) : A_NORMAL);
      wprintw (Globals.wmain, "%s", trunc_file);
      fp = *(pages + *current_page);
      cursor_y = MAIN_TOP_LINE;
      while (fp->p)
      {
         cursor_y++;
         fp = fp->p;
      }
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, cursor_y - MAIN_TOP_LINE,
             NAME_POS);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
   }
   statWindow (fp->filename);
   move (cursor_y, NAME_POS);
   wrefresh (Globals.wmain);
   wrefresh (Globals.wstatus);
   refresh ();
}
