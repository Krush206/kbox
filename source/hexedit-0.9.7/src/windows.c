/* windows.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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


   /* Brings up Jump to offset window, to relocate to a different part
    * of the file.  Uses one string box and takes input in octal, decimal, 
    * and hex.
    */
void
popupGotoOffset (long filesize)
#define OFFSET_BOX_STRING_BOX 12
#define OFFSET_BOX_HEIGHT     8
#define OFFSET_BOX_WIDTH      50
#define OFFSET_STRING_BUF     11
{
   char *offstr = NULL;
   char offsetstr[OFFSET_STRING_BUF];
   unsigned long off = 0x00;
   WINDOW *wpopup = popupWindow (OFFSET_BOX_WIDTH, OFFSET_BOX_HEIGHT);
   struct sigaction sig_new, sig_old;

/*   signal (SIGINT, SIG_IGN); */
   sig_new.sa_handler = SIG_IGN;
   memset (&sig_new.sa_mask, 0x00, sizeof (sigset_t));
   sig_new.sa_flags = 0;
   sigaction (SIGINT, &sig_new, &sig_old);

   wmove (wpopup, 1, (OFFSET_BOX_WIDTH / 2) - (strlen ("Goto Offset") / 2));
   wprintw (wpopup, "Goto Offset");
   wmove (wpopup, 5, 2);
   wprintw (wpopup, "  Hint: Decimal 255 = Hex 0xFF = Octal 0377");
   wmove (wpopup, 3, (OFFSET_BOX_WIDTH / 2) - 
                     (OFFSET_BOX_STRING_BOX / 2)
                     - (strlen ("Offset:") / 2));
   wprintw (wpopup, "Offset: ");
   memset (offsetstr, 0, OFFSET_STRING_BUF);

   sprintf (offsetstr, "0x");
/* uncomment to have eof offset autofilled in string box
   sprintf (offsetstr, "0x%08lX", filesize);
*/
   werase (Globals.whelp);
   wmove (Globals.whelp, 0, 0);
   wprintw (Globals.whelp, "^G/^X/Escape Cancel   ^U Clear input");
   wrefresh (Globals.whelp);

   offstr = stringBox (wpopup, 3, (OFFSET_BOX_WIDTH / 2), 
                      OFFSET_BOX_STRING_BOX,
                      OFFSET_BOX_STRING_BOX, offsetstr);
   if (!*offstr)
      off = offset;
   else
   {
      errno = 0;
      off = strtoul (offstr, NULL, 0);
      if (errno) /* Possible ERANGE return */
         offset = 0;
      else if (off > filesize)
         offset = filesize;
      else
         offset = off;
      cursor_x = mappos (offset & M_0x0F);
      cursor_y = MAIN_TOP_LINE;
   }
   free (offstr);
   delwin (wpopup);
/*   signal (SIGINT, handleInterrupt); */
   sigaction (SIGINT, &sig_old, NULL);
}


   /* Print the whole line between the box border. */
static void
print_line (WINDOW *win, int cols, int y, char c)
{
   int i = 0;

   wmove (win, y, 1);
   for (i = 0; i < (cols - 2); i++)
      wprintw (win, "%c", c);
}


void
do_outer_help_box (WINDOW *win, int y, int lines)
{
   box (win, 0, 0);

   wmove (win, 0, (COLS / 2) - (strlen ("Help Menu") / 2) - 2);
   wprintw (win, "Help Menu");

   if (y > 0)
   {
      wmove (win, 0, COLS - strlen ("(more)") - 1);
      wprintw (win, "(more)");
   }
   if ((y + lines) < (Globals.help_msg_count + 1))
   {
      wmove (win, MAIN_BOTTOM_LINE, COLS - strlen ("(more)") - 1);
      wprintw (win, "(more)");
   }

}


/*
initialize the help at the start of the program so that it's dynamic
so we can have the right help when you start up, then it will be easy.

*/


void
popupHelp (void)
#define HELP_BOX_HEIGHT     MAIN_HEIGHT
#define HELP_BOX_WIDTH      COLS
#define STRING_BUF          80
{
      /* Unless you change what you shouldn't, this is fine */
   char s[STRING_BUF];
   int i = 0;
   int y = 0;
   WINDOW *wpopup = popupWindow (HELP_BOX_WIDTH, HELP_BOX_HEIGHT);
   struct sigaction sig_new, sig_old;
   wchar_t in = 0;

/*   signal (SIGINT, SIG_IGN); */
   sig_new.sa_handler = SIG_IGN;
   memset (&sig_new.sa_mask, 0x00, sizeof (sigset_t));
   sig_new.sa_flags = 0;
   sigaction (SIGINT, &sig_new, &sig_old);

   wattrset (wpopup, color_term ? COLOR_PAIR(1) : A_NORMAL);
   mvwin (wpopup, MAIN_TOP_LINE, 0);

   do_outer_help_box (wpopup, 0, LINES - 3);

   for (y = 1; y < MAIN_BOTTOM_LINE; y++)
      print_line (wpopup, HELP_BOX_WIDTH, y, ' ');

   memset (s, 0x00, STRING_BUF);
#ifdef HAVE_SNPRINTF
      snprintf (s, STRING_BUF,
         "[N]Curses Hexedit %s Adam Rogoyski <apoc@laker.net>", VERSION);
#else
      sprintf (s,
        "[N]Curses Hexedit %s Adam Rogoyski <apoc@laker.net>", VERSION);
#endif
   werase (Globals.whelp);
   wmove (Globals.whelp, 0, (COLS / 2) - (strlen (s) / 2));
   wprintw (Globals.whelp, "%s", s);
   wrefresh (Globals.whelp);
            
   y = 0;
   help_print (wpopup, y, LINES - 3);

   wrefresh (wpopup);

   while ((in = getch ()))
   {
      switch (in)
      {
         case '\n':
         case ESCAPE_CHARACTER:
         case CONTROL_C:
         case CONTROL_G:
         case CONTROL_X:
         case 'q':
         case 'Q':
            delwin (wpopup);
            sigaction (SIGINT, &sig_old, NULL);
            return;

         case KEY_DOWN:
         case 'j':
         case 'J':
            if ((y + LINES - 5) < (Globals.help_msg_count - 1))
               y++;
            break;
       
         case KEY_UP:
         case 'k':
         case 'K':
            if (y > 0)
               y--;
            break;

         default:
            continue;
       
      }

      for (i = 1; i < MAIN_BOTTOM_LINE; i++)
         print_line (wpopup, HELP_BOX_WIDTH, i, ' ');

      help_print (wpopup, y, LINES - 3);
      do_outer_help_box (wpopup, y, LINES - 3);

      wrefresh (wpopup);
   }

   delwin (wpopup);

/*   signal (SIGINT, handleInterrupt); */
   sigaction (SIGINT, &sig_old, NULL);
}


   /* Brings up a popup window and prompts with a stringbox for the
    * filename to save as.
    */
int
popupFileSave (void)
#define FILE_SAVE_STRING_BOX      50
#define FILE_SAVE_STRING_BOX_MAX  PATH_MAX
#define FILE_SAVE_HEIGHT          6
#define FILE_SAVE_WIDTH           70
{
   int result = 0;
   int saved = 0;
   char *filename = NULL;
   FILE *fp = NULL;
   WINDOW *wpopup = popupWindow (FILE_SAVE_WIDTH, FILE_SAVE_HEIGHT);
   struct sigaction sig_new, sig_old;
  
/*   signal (SIGINT, SIG_IGN); */
   sig_new.sa_handler = SIG_IGN;
   memset (&sig_new.sa_mask, 0x00, sizeof (sigset_t));
   sig_new.sa_flags = 0;
   sigaction (SIGINT, &sig_new, &sig_old);

   wmove (wpopup, 1, (FILE_SAVE_WIDTH / 2) - (strlen ("Save File") / 2));
   wprintw (wpopup, "Save File");
   wmove (wpopup, 3, 2);
   wprintw (wpopup, "Filename: ");
   werase (Globals.whelp);
   wmove (Globals.whelp, 0, 0);
   wprintw (Globals.whelp, "^G/^X/Escape Cancel");
   wrefresh (Globals.whelp);
   filename = stringBox (wpopup, 3, 2 + strlen ("Filename: "), 
                      FILE_SAVE_STRING_BOX_MAX,
                      FILE_SAVE_STRING_BOX, (char *) Globals.filename);
   chompWhiteSpace (filename);

   if (!filename);
   else if (!*filename)
   {
      free (filename);
      filename = NULL;
   }
   else
   {
      if (!strcmp (Globals.filename, filename))
      {
         fclose (Globals.fp);
         fp = fopen (filename, "r+b");
         Globals.fp = fp;
      }
      else
         fp = fopen (filename, "wb");
      if (!fp)
      {
         wattroff (wpopup, A_REVERSE);
         werase (wpopup);
         box (wpopup, 0, 0);
         wmove (wpopup, 1, (FILE_SAVE_WIDTH / 2)
                 - (strlen ("Cannot Save File") / 2));
         wprintw (wpopup, "Cannot Save File");
         wmove (wpopup, FILE_SAVE_HEIGHT - 3, (FILE_SAVE_WIDTH / 2) - 
                (strlen (strerror (errno)) + strlen ("Reason: ")) / 2);
         wprintw (wpopup, "Reason: %s", strerror (errno));
         wrefresh (wpopup);
         getch ();
         if (!strcmp (Globals.filename, filename))
         {
            Globals.fp = fopen (Globals.filename, "rb");
            if (!Globals.fp)
               die_horribly ("Cannot save to original file, nor reopen it",
                              NULL);
         }
         if (filename)
            free (filename);
         filename = NULL;
      }
      else
      {
         int buf = 0;
         unsigned long i = 0;
         int len = Globals.filesize + 1;
         
            /* If it's all in memory just write it out to the file */
         if (Globals.fullsize)
         {
            while (len > 0)
            {
               if (len > DEFAULT_READ_BUFFER)
                  buf = DEFAULT_READ_BUFFER;
               else
                  buf = len;
               result = fwrite (filebuf + i, 1, buf, fp);
               if (result > 0)
                  len -= result;
               else
                  len = 0;
               i += result;
            }
            ftruncate (fileno (fp), Globals.filesize);
         }
            /* If not, we read it from the old file, make the neccessary
             * changes, and then write it to the new file (possibly the
             * same file.
             */
         else
         {
            /* Two situations:
             * 1.  If we are writing to the same file, we just add the
             *     changes (fast).
             * 2.  If we are writing to a new file, we need to re-read
             *     the entire old file and make the changes as we go (slow).
             */
            if (!strcmp (Globals.filename, filename) || Globals.fixed_disk)
               /* same file, just add the changes */
            {
                  /* cycle through the hash and write all the changes */
               commit_changes_in_hash (fp);
            }
            else
               /* different file, re-read whole old one and add changes */
            {
               unsigned char buffer[DEFAULT_READ_BUFFER];
               unsigned char c = '\0';
               int pos = 0;
               int result2 = 0;
               int tst = 0;

               memset (buffer, 0x00, DEFAULT_READ_BUFFER);
               while (len > 0)
               {
                  if (len > DEFAULT_READ_BUFFER)
                     buf = DEFAULT_READ_BUFFER;
                  else
                     buf = len;
               
                  result = fseek (Globals.fp, pos, SEEK_SET);
                  if (result < 0)
                     die_horribly ("Cannot re-read original file",
                                   Globals.filename);

                  result = fread (buffer, 1, buf, Globals.fp);
                  if (ferror (Globals.fp))
                     die_horribly ("Cannot re-read original file",
                                   Globals.filename);
                     /* Make edited changes to file */
                  for (i = pos; i < (pos + result); i++)
                  {
                     if (!hash_lookup (i, &c))
                        buffer[i - pos] = c;
                  }

               
                  tst = fseek (fp, pos, SEEK_SET);
                  if (tst < 0)
                     die_horribly ("Cannot read file",
                                    filename);

                  result2 = fwrite (buffer, 1, result, fp);
                  if (result2 != result)
                     die_horribly ("Cannot write new file correctly", "fwrite");
                  if (result > 0)
                     len -= result;
                  else
                     len = 0;
                  pos += result;
               }
               free ((char *) Globals.filename);
               Globals.filename = filename;
               filename = NULL;
            }
         }
         fflush (fp);
         result = 0;
         if (fp != Globals.fp)
            result = fclose (fp);
         else
            result = fflush (fp);
         if (result == -1)
         {
            wattroff (wpopup, A_REVERSE);
            werase (wpopup);
            box (wpopup, 0, 0);
            wmove (wpopup, 1, (FILE_SAVE_WIDTH / 2)
                    - (strlen ("Cannot Save File") / 2));
            wprintw (wpopup, "bCannot Save File");
            wmove (wpopup, FILE_SAVE_HEIGHT - 3, (FILE_SAVE_WIDTH / 2) - 
                   (strlen (strerror (errno)) + strlen ("Reason: ")) / 2);
            wprintw (wpopup, "Reason: %s", strerror (errno));
            wrefresh (wpopup);
            if (filename)
               free (filename);
            filename = NULL;
            getch ();
         }
         else
         {
            saved = 1;
            Globals.modified = 0;
         }
      }
      if (filename)
      {
         free ((char *) Globals.filename);
         Globals.filename = filename;
         filename = NULL;
      }
   }
   if (filename)
      free (filename);
   delwin (wpopup);
/*   signal (SIGINT, handleInterrupt); */
   sigaction (SIGINT, &sig_old, NULL);
   return saved;
}


   /* XXX This is really ugly, this needs to be redone eventually.  I 
    * want to add search/replace type things, but it's too ugly, needs
    * to be totaly redone, I don't feel like figuring out how it works. 
    * I must have been up really late.
    */
void
search (int findnext)
#define SEARCH_BOX_WIDTH  60
#define SEARCH_BOX_HEIGHT 10
#define SEARCH_STRING_BOX      50
#define SEARCH_STRING_BOX_MAX  50
{
   char *options[] = { "Find Next",
                      "search for text string",
                      "search for Hex bytes" };
   static unsigned char *laststr = NULL;
   static int first = 1;
   static int last_str_len = 0;
   static unsigned char *s = NULL;
   int done = 0;
   int selection = 0;
   wchar_t in = 0;
   WINDOW *wpopup = popupWindow (SEARCH_BOX_WIDTH, SEARCH_BOX_HEIGHT);
   struct sigaction sig_new, sig_old;

   if (findnext && !laststr)
      return;

/*   signal (SIGINT, SIG_IGN); */
   sig_new.sa_handler = SIG_IGN;
   memset (&sig_new.sa_mask, 0x00, sizeof (sigset_t));
   sig_new.sa_flags = 0;
   sigaction (SIGINT, &sig_new, &sig_old);


   werase (Globals.whelp);
   wmove (Globals.whelp, 0, 0);
   wprintw (Globals.whelp,
       "^G/^X/Escape Cancel   F/N Find Next   A Text Search   H Hex Search");
   wrefresh (Globals.whelp);

   wmove (wpopup, 1, (SEARCH_BOX_WIDTH / 2) - (strlen ("Search") / 2));
   wprintw (wpopup, "Search");
   if (first)
      selection = 1;
   if (!first)
      wattrset (wpopup, A_REVERSE);
   wmove (wpopup, 3, (SEARCH_BOX_WIDTH / 2) - (strlen (options[0]) / 2));
   wprintw (wpopup, "%s", options[0]);      
   if (!first)
      wattroff (wpopup, A_REVERSE);
   if (first)
      wattrset (wpopup, A_REVERSE);
   wmove (wpopup, 5, (SEARCH_BOX_WIDTH / 2) - (strlen (options[1]) / 2));
   wprintw (wpopup, "%s", options[1]);
   if (first)
      wattroff (wpopup, A_REVERSE);
   wmove (wpopup, 7, (SEARCH_BOX_WIDTH / 2) - (strlen (options[2]) / 2));
   wprintw (wpopup, "%s", options[2]);
   if (findnext)
   {
      selection = 0;
      done = 1;
   }
   else
      wrefresh (wpopup);
   while (!done && ((in = getch()) != '\n'))
   {
      switch (in)
      {
         case 'f':
         case 'F':
         case 'n':
         case 'N':
            first = 0;
            done = 1;
            selection = 0;
            break;
         case 'a':
         case 'A':
            first = 0;
            done = 1;
            selection = 1;
            break;
         case 'h':
         case 'H':
            first = 0;
            done = 1;
            selection = 2;
            break;
         case CONTROL_C:
         case CONTROL_G:
         case CONTROL_X:
         case ESCAPE_CHARACTER:
            done = -1;
            break;
         case KEY_UP:
         case KEY_DOWN:
         case 'j':
         case 'J':
         case 'k':
         case 'K':
            if (toupper (in) == 'J')
                in = KEY_DOWN;
            else if (toupper (in) == 'K')
                in = KEY_UP;
            if ((in == KEY_UP) && (selection == 0))
               selection = 2;
            else if (in == KEY_UP)
               selection--;
            if ((in == KEY_DOWN) && (selection == 2))
               selection = 0;
            else if (in == KEY_DOWN)
               selection++;
            if (first && (selection == 0) && (in == KEY_UP))
               selection = 2;
            else if (first && (selection == 0) && (in == KEY_DOWN))
               selection = 1;
            if (selection == 0)
               wattrset (wpopup, A_REVERSE);
            wmove (wpopup, 3, (SEARCH_BOX_WIDTH / 2) - (strlen (options[0]) / 2));
            wprintw (wpopup, "%s", options[0]);      
            if (selection == 0)
               wattroff (wpopup, A_REVERSE);
            if (selection == 1)
                wattrset (wpopup, A_REVERSE);
            wmove (wpopup, 5, (SEARCH_BOX_WIDTH / 2) - (strlen (options[1]) / 2));
            wprintw (wpopup, "%s", options[1]);
            if (selection == 1)
                wattroff (wpopup, A_REVERSE);
            if (selection == 2)
               wattrset (wpopup, A_REVERSE);
            wmove (wpopup, 7, (SEARCH_BOX_WIDTH / 2) - (strlen (options[2]) / 2));
            wprintw (wpopup, "%s", options[2]);
            if (selection == 2)
               wattroff (wpopup, A_REVERSE);
            wrefresh (wpopup);
            break;
      }
   }
   if (done == -1)
   {
/*      signal (SIGINT, handleInterrupt); */
      sigaction (SIGINT, &sig_old, NULL);
      return;
   }
   done = 0;
   wmove (wpopup, 3, (SEARCH_BOX_WIDTH / 2) - (strlen (options[0]) / 2));
   wprintw (wpopup, "%*c", strlen (options[0]), ' ');
   wmove (wpopup, 5, (SEARCH_BOX_WIDTH / 2) - (strlen (options[1]) / 2));
   wprintw (wpopup, "%*c", strlen (options[1]), ' ');
   wmove (wpopup, 7, (SEARCH_BOX_WIDTH / 2) - (strlen (options[2]) / 2));
   wprintw (wpopup, "%*c", strlen (options[2]), ' ');
   if (selection == 0)
   {
      struct foundit *spot = NULL;
      if (offset < Globals.filesize - 1)
         spot = boyer_moore_search (laststr, offset + 1,
                              last_str_len,
                              Globals.filesize - offset);
      if (spot && spot->flag)
      {
         offset += spot->offset + 1; /* started search one character
                                       ahead of offset */
         cursor_y = 1;
         cursor_x = mappos (offset & M_0x0F);
      }
      else
      {
         int i = 0;
         wattroff (wpopup, A_REVERSE);
         wmove (wpopup, (SEARCH_BOX_HEIGHT / 2) + 1, 1);
         for (i = 2; i < SEARCH_BOX_WIDTH; i++)
            wprintw (wpopup, " ");
         wmove (wpopup, (SEARCH_BOX_HEIGHT / 2) + 1, (SEARCH_BOX_WIDTH / 2)
                - (strlen ("** String not found **") / 2));
         wprintw (wpopup, "** String not found **");
         wrefresh (wpopup);
         getch ();
      }
   }
   else if (selection == 1)
   {
      unsigned char *str = NULL;
      unsigned char *strb = NULL;
      if (laststr)
      {
         free (laststr);
         laststr = NULL;
      }
      wmove (wpopup, 1, (SEARCH_BOX_WIDTH / 2) - (strlen ("Text String")
                        / 2));
      wprintw (wpopup, "Text String");
      str = strb = (unsigned char *)
                      stringBox (wpopup, (SEARCH_BOX_HEIGHT / 2) + 1,
                      (SEARCH_BOX_WIDTH - SEARCH_STRING_BOX) / 2,
                         SEARCH_STRING_BOX_MAX,
                         SEARCH_STRING_BOX, NULL);
      s = (unsigned char *) str;
      laststr = s;
      for (str = s + SEARCH_STRING_BOX_MAX - 1; str != (s - 1); str--)
      {
         if (*str != ' ')
         {
            *(str + 1) = '\0';
            done = 1;
            break;
         }
      }
      if (Globals.charset == EBCDIC_CHAR_SET)
      {
         unsigned char *p = s;
         while (*p)
         {
            *p = findEBCDIC (*p);
            p++;
         }
      }
      if (done)
      {
         struct foundit *spot = NULL;
         if (offset < Globals.filesize - 1)
            spot = boyer_moore_search (s, offset + 1,
                                 last_str_len = strlen ((char *) s),
                                 Globals.filesize - offset);
         if (spot && spot->flag)
         {
            offset += spot->offset + 1; /* started search one character
                                          ahead of offset */
            cursor_y = 1;
            cursor_x = mappos (offset & M_0x0F);
        }
        else
        {
           int i = 0;
           wattroff (wpopup, A_REVERSE);
           wmove (wpopup, (SEARCH_BOX_HEIGHT / 2) + 1, 1);
           for (i = 2; i < SEARCH_BOX_WIDTH; i++)
              wprintw (wpopup, " ");
           wmove (wpopup, (SEARCH_BOX_HEIGHT / 2) + 1, (SEARCH_BOX_WIDTH / 2)
                  - (strlen ("** String not found **") / 2));
           wprintw (wpopup, "** String not found **");
           wrefresh (wpopup);
           getch ();
        }
      }
   }
   else if (selection == 2)
   {
#define BYTE_SEARCH_LENGTH  16
      struct ret_string *retr;
      wmove (wpopup, 1, (SEARCH_BOX_WIDTH / 2) - (strlen ("Byte Search")
                        / 2));
      wprintw (wpopup, "Byte Search");
      retr = hex_string_box (wpopup, BYTE_SEARCH_LENGTH, SEARCH_BOX_WIDTH);
      if (retr)
      {
         struct foundit *spot = NULL;
         if (offset < Globals.filesize - 1)
            spot = boyer_moore_search (retr->str, offset + 1,
                                 last_str_len = retr->len,
                                 Globals.filesize - offset);
         if (spot && spot->flag)
         {
            offset += spot->offset + 1; /* started search one character
                                          ahead of offset */
            cursor_y = 1;
            cursor_x = mappos (offset & M_0x0F);
            if (laststr)
               free (laststr);
            laststr = malloc (retr->len + 1);
            memset (laststr, 0x00, retr->len + 1);
            memcpy (laststr, retr->str, retr->len);
            free (retr->str);
            retr->str = NULL;
            retr->len = 0;
         }
         else
         {
            int i = 0;
            wattroff (wpopup, A_REVERSE);
            wmove (wpopup, (SEARCH_BOX_HEIGHT / 2) + 1, 1);
            for (i = 2; i < SEARCH_BOX_WIDTH; i++)
               wprintw (wpopup, " ");
            wmove (wpopup, (SEARCH_BOX_HEIGHT / 2) + 1, (SEARCH_BOX_WIDTH / 2)
                   - (strlen ("** String not found **") / 2));
            wprintw (wpopup, "** String not found **");
            wrefresh (wpopup);
            getch ();
         }
      }
   }
   first = 0;
/*   signal (SIGINT, handleInterrupt); */
   sigaction (SIGINT, &sig_old, NULL);
   delwin (wpopup);
}


   /* Brings up a prompt in the help window and prompts for the relative
    * offset to jump by.
    */
void
jump_relative_offset (wchar_t sign)
   /* pass sign as '+' or '-' to prompt for an offset to jump to relative
    * to current offset.  
    * pass sign as 0 to jump relative to current offset the amount prompted
    * last time we called this routine.
    */
{
   static wchar_t in = '\0';
   static unsigned long offs  = 0x00;
   unsigned long offst = offset;

   if (sign)
   {
      offs = 0x00;
      werase (Globals.whelp);
      wmove (Globals.whelp, 0, 0);
      wprintw (Globals.whelp, "Offset: 0x%08lX %c 0x", offset, sign);
      wrefresh (Globals.whelp);

      while ((in = getch ()) != '\n')
      {
         if (!isHexChar (in))
         {
            switch (in)
            {
               case BACKSPACE:
               case CONTROL_H:
               case KEY_BACKSPACE:
               case KEY_DC:
#ifdef __PDCURSES__
               case CTL_BKSP:
#endif
                  offs >>= 4;
                  break;

               case CONTROL_U:
                  offs = 0x00;
                  break;

               case CONTROL_G:
               case CONTROL_X:
                  printHelpWindow ();
                  wrefresh (Globals.whelp);
                  move (cursor_y, cursor_x);
                  refresh ();
                  return;

               default:
                  continue;
            }
         }
         else
         {
            /* shift left to make room for new hex digit and or it in least
             * significant place
             */
            offs <<= 4;
            offs  |= getHexValue (in);
         }
      
         werase (Globals.whelp);
         wmove (Globals.whelp, 0, 0);
         wprintw (Globals.whelp, "Offset: 0x%08lX %c 0x%lX",
                  offset, sign, offs);
         wrefresh (Globals.whelp);
      }
   }

   if (sign)
      in = sign;

   if (offs)
   {
      if (in == '+')
      {
         offst += offs;
            /* Check for overflow */
         if (offst > Globals.filesize)
         {
            offst = Globals.filesize;
            do_beep ();
         }
      }
      else
      {
         offst -= offs;
            /* Check for unsigned underflow */
         if (offst > Globals.filesize)
         {
            offst = 0x00;
            do_beep ();
         }
      }

   }

   cursor_x = mappos (offst & M_0x0F);
   cursor_y = 1;

   offset = offst;

   printStatusWindow ();
   printHelpWindow ();
   drawdump (offset);
   wrefresh (Globals.wmain);
   wrefresh (Globals.wstatus);
   wrefresh (Globals.whelp);
   move (cursor_y, cursor_x);
   refresh ();
}
