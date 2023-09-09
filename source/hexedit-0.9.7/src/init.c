/* init.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

#if defined(HAVE_GETOPT_H) && defined(HAVE_GETOPT_LONG)
#include <getopt.h>
#else
#include "../gnu/getopt.h"
#include "../gnu/getopt.c"
#include "../gnu/getopt1.c"
#endif

#ifdef __linux__
#include <linux/hdreg.h>
#endif

#ifdef __OpenBSD__
#include <fcntl.h>
#include <util.h>
#include <sys/disklabel.h>
#endif

extern char **environ;


   /* This is called once at the start of the program.  Handles HEXEDIT
    * Environment variable, command line arguments, sets up signal
    * handlers, Global Variables, setup windows, possibly use file widget,  
    * and load the file.
    */
void
init (int argc, char **argv)
{
   int i = 0;
   FILE *fp = NULL;
   void *p;

   int option_index = 0;
   struct option long_options[] = 
   {
      { "help",     0, 0, 0 },
      { "highbit",  0, 0, 0 },
      { "alltext",  0, 0, 0 },
      { "readonly", 0, 0, 0 },
      { "version",  0, 0, 0 },
      { "disk",     0, 0, 0 },
      { "buffer",   0, 0, 0 },
      { "quiet",    0, 0, 0 },
      { "force",    0, 0, 0 },
      { "nocolor",  0, 0, 0 },
      {  NULL,      0, 0, 0 }
   };

   Globals.modified   = 0;
   Globals.filesize   = 0;
   Globals.print_mode = REGULAR_PRINT;
   Globals.buffsize   = DEFAULT_BUFFER_SIZE;
   Globals.fixed_disk = 0;
   Globals.fullsize   = 0;
   Globals.buf_front  = 0;
   Globals.buf_end    = 0;
   Globals.beeping    = 1;
   Globals.charset    = ASCII_CHAR_SET;
   Globals.spacing    = 1;
   Globals.tabb       = 0;

   newlines = NULL;
   filebuf  = NULL;

      /* process environemnt variable first, then command line options */

   i = 0;
   while (*environ)
   {
      if (!strncasecmp (*environ, "HEXEDIT", 7))
      {
         while (**environ)
         {
            switch (**environ)
            {
               case '8':
                  Globals.print_mode = HIGH_ASCII_PRINT;
                  break;

               case 'a':
               case 'A':
                  Globals.print_mode = ALL_PRINT;
                  break;

               case 'b':
               case 'B':
                  Globals.buffsize = 0;
                  Globals.fullsize = 1;
                  break;

               case 'f':
               case 'F':
                  Globals.modified = 0;
                  break;

               case 'r':
               case 'R':
                  Globals.modified = READ_ONLY;
                  break;

               case 'd':
               case 'D':
                  Globals.fixed_disk = 1;
                  Globals.modified = READ_ONLY; /* need force as well */
                  break;

               case 'q':
               case 'Q':
                  Globals.beeping = 0;
                  break;

               case 'n':
               case 'N':
                  color_term = -1; /* don't use color terminal if availible */
                  break;
            }
            ++*environ;
         }
         break;
      }
      environ++;
      i++;
   }
   environ -= i;

   while ((i = getopt_long (argc, argv, "8abdfhnqrv", long_options,
              &option_index)) != EOF)
   {
      switch (i)
      {
         case 0:
            switch (option_index)
            {
               case 0:
                  usage (*argv);
                  exit (EXIT_SUCCESS);

               case 1:
                  Globals.print_mode = HIGH_ASCII_PRINT;
                  break;

               case 2:
                  Globals.print_mode = ALL_PRINT;
                  break;

               case 3:
                  Globals.modified = READ_ONLY;
                  break;

               case 4:
                  printf ("%s\n", VERSION);
                  exit (EXIT_SUCCESS);

               case 5:
                  Globals.fixed_disk = 1;
                  Globals.modified = READ_ONLY; /* need force as well */
                  break;

               case 6:
                  Globals.buffsize = 0;
                  Globals.fullsize = 1;
                  break;

               case 7:
                  Globals.beeping = 0;
                  break;

               case 8:
                  Globals.modified = 0;
                  break;

               case 9:
                  color_term = -1;
                  break;
            }
            break;

         case '8':
            Globals.print_mode = HIGH_ASCII_PRINT;
            break;

         case 'a':
            Globals.print_mode = ALL_PRINT;
            break;

         case 'b':
            Globals.buffsize = 0;
            Globals.fullsize = 1;
            break;

         case 'd':
            Globals.fixed_disk = 1;
            Globals.modified = READ_ONLY; /* need force as well*/
            break;

         case 'f':
            Globals.modified = 0;
            break;

         case 'h':
            usage (*argv);
            exit (EXIT_SUCCESS);

         case 'n':
            color_term = -1;
            break;

         case 'q':
            Globals.beeping = 0;
            break;

         case 'r':
            Globals.modified = READ_ONLY;
            break;

         case 'v':
            printf ("%s\n", VERSION);
            exit (EXIT_SUCCESS);
      }
   }

   help_initialize ();

   i = 0;
   p = initscr ();
   if (p == NULL)
   {
      fprintf (stderr, "Cannot initialize screen - curses!\n");
      exit (EXIT_FAILURE);
   }

   if (color_term != -1)
   {
      start_color ();
      if ((color_term = has_colors ()))
      {
         color_term = 1;
         init_pair (1, COLOR_WHITE, COLOR_BLUE);
         init_pair (2, COLOR_GREEN, COLOR_BLUE);
         init_pair (3, COLOR_BLUE, COLOR_BLACK);
         init_pair (4, COLOR_YELLOW, COLOR_CYAN);
      }
   }
   else
      color_term = 0;
   keypad (stdscr, TRUE);
   scrollok (stdscr, FALSE);
   cbreak ();
   noecho ();
   refresh ();

   Globals.wstatus = newwin (1, COLS, 0, 0);
   if (Globals.wstatus == NULL)
      die_horribly ("Cannot open status window - curses!", NULL);
   wattrset (Globals.wstatus, color_term ? COLOR_PAIR(4) | A_BOLD
                                         : A_REVERSE);
   scrollok (Globals.wstatus, FALSE);

   Globals.wmain = newwin (MAIN_HEIGHT, COLS, MAIN_TOP_LINE, 0);
   if (Globals.wmain == NULL)
      die_horribly ("Cannot open main window - curses!", NULL);

   scrollok (Globals.wmain, FALSE);

   Globals.whelp = newwin (1, COLS, LINES - 1, 0);
   if (Globals.whelp == NULL)
      die_horribly ("Cannot open help window - curses!", NULL);
   wattrset (Globals.whelp, color_term ? COLOR_PAIR(3) | A_BOLD : A_NORMAL); 

   scrollok (Globals.whelp, FALSE);

#ifdef __PDCURSES__
   leaveok(Globals.wmain, FALSE);  /* Stupid macro */
#else
   leaveok (Globals.wmain, FALSE);
#endif


#ifndef __PDCURSES__
/*   signal (SIGWINCH, handleSigwinch); */
   {
      struct sigaction sig_a;
      sig_a.sa_handler = handleSigwinch;
      memset (&sig_a.sa_mask, 0x00, sizeof (sigset_t));
      sig_a.sa_flags = 0;
      sigaction (SIGWINCH, &sig_a, NULL);
   }
   
#endif

   if (optind < argc)
   {
      int l = 0;

      fp = fopen (*(argv + optind), "rb");
      if (!fp)
         popup_Error (*(argv + optind), 1);
      else
      {
         l = strlen (*(argv + optind)) + 1;
         Globals.filename = malloc (l);
         if (!Globals.filename)
            die_horribly (NOT_ENOUGH_MEMORY, NULL);
         strncpy ((char *) Globals.filename, *(argv + optind), l);
      }
   }

   load_new_file (&fp);


   for (i = 0; i < COLS; i++)
      waddch (Globals.wstatus, ' ');
   printStatusWindow ();
   printHelpWindow ();

   drawdump (offset);
   wrefresh (Globals.wmain);
   cursor_y = MAIN_TOP_LINE;
   cursor_x = 10;
   move (cursor_y, cursor_x);
   refresh ();

}



/* Used to go to the file selection widget continously until a new file
 * is choses or the user exits.  Returns the name of the new file and
 * sets fp to the new file opened file pointer.  Need to eventually free
 * this returned value.  This is called outside init.c.  
 */

char *
select_new_file (FILE **fp)
#define FILE_ERR_WIDTH  50
#define FILE_ERR_HEIGHT 6
{
   char *p = NULL;

   while (!*fp)
   {
         /* need to eventually free () returned valued */
      p = fileSelect ();

      *fp = fopen (p, "rb");
      if (!*fp)
      {
         popup_Error ("Cannot Open File", 1);
         free (p);
      }
      else
         break;
   }
   return p;
}


   /* Generic popup error box.  XXX I should use this in places where it
    * looks like I cut and pasted this. XXX
    */
void
popup_Error (const char * const msg, int i)
{
         WINDOW *wpopup = popupWindow (FILE_ERR_WIDTH, FILE_ERR_HEIGHT);

         werase (wpopup);
         box (wpopup, 0, 0);
         wmove (wpopup, 1, (FILE_ERR_WIDTH / 2)
                 - (strlen (msg) / 2));
         wprintw (wpopup, (char *) msg);
         wmove (wpopup, FILE_ERR_HEIGHT - 3, (FILE_ERR_WIDTH / 2) -
                (strlen (strerror (errno)) + strlen ("Reason: ")) / 2);
         wprintw (wpopup, "Reason: %s", 
                  i ? strerror (errno) : "Not applicable");
         wrefresh (wpopup);
         getch ();
}



   /* This is called outside init.c.  Deals with loading the file. */
void
load_new_file (FILE **fp)
{
   int i = 0;
   int result = 0;
   unsigned long filled = 0;
   int next_read = 0;
   struct stat filestat;
   unsigned char strbuf[DEFAULT_READ_BUFFER];

   Globals.filesize = 0;

   while (1)
   {
      if (!*fp)
      {
         if (Globals.filename)
            free ((char *) Globals.filename);
         Globals.filename = select_new_file (fp);
      }

         /* hmm how would this happen?  Better to just keep going. */
      if (!*fp)
/*         die_horribly (NULL, Globals.filename); */
         continue;


      result = stat (Globals.filename, &filestat);
      if (result == -1)
      {
/*         die_horribly (NULL, Globals.filename); */
         popup_Error ("Cannot Read File", 1);
         fclose (*fp);
         *fp = NULL;
         continue;
      }

      if ((filestat.st_size == 0) && !Globals.fixed_disk)
      {
/*         die_horribly ("File is Zero Bytes - Nothing to Edit!", NULL); */
         popup_Error ("File is Zero Bytes, Perhaps Used -d disk mode", 0);
         fclose (*fp);
         *fp = NULL;
         continue;
      }

#ifdef __linux__
   else if (Globals.fixed_disk)
   {
      struct hd_geometry fixed_disk;

      result = ioctl (fileno (*fp), HDIO_GETGEO, &fixed_disk);
      if (result == -1)
      {
/*         die_horribly (NULL, Globals.filename); */
         popup_Error ("Cannot get disk info", 1);
         fclose (*fp);
         *fp = NULL;
         continue;
      }

      Globals.filesize = fixed_disk.heads * fixed_disk.sectors
                       * fixed_disk.cylinders * 512;
      filestat.st_size = DEFAULT_BUFFER_SIZE;
   }

#elif defined (__OpenBSD__)
   else if (Globals.fixed_disk)
   {

      struct disklabel dl;

      result = ioctl (fileno (*fp), DIOCGDINFO, &dl);
      if (result == -1)
      {
/*         die_horribly (NULL, Globals.filename); */
         popup_Error ("Cannot get disk info", 1);
         fclose (*fp);
         *fp = NULL;
         continue;         
      }

      Globals.filesize = dl.d_ntracks * dl.d_nsectors
                       * dl.d_ncylinders * 512;
      filestat.st_size = DEFAULT_BUFFER_SIZE;
   }
#endif

      break;
   }

      /* it would be zero if the -b, --buffer flag was given */
   if (!Globals.buffsize)
      Globals.buffsize = filestat.st_size;
   else
      extrabuf = 0;  /* no extrabuf since delete and insert not allowed */

   if (filebuf)
      free (filebuf);
   filebuf = malloc (Globals.buffsize + 1 + extrabuf);

   if (!filebuf)
      die_horribly (NOT_ENOUGH_MEMORY, NULL);

      /* don't think this is needed any more. */
   memset (filebuf, 0x00, Globals.buffsize + 1 + extrabuf);

   i = 0;
   filled = 0;

   next_read = (DEFAULT_READ_BUFFER > Globals.buffsize) ?
                    Globals.buffsize
                  : DEFAULT_READ_BUFFER;


   memset (strbuf, 0, DEFAULT_READ_BUFFER);
   while ((result = fread (strbuf, 1, next_read, *fp)) > 0)
   {
      if (ferror (*fp))
         die_horribly (NULL, Globals.filename);

      memcpy (filebuf + i, strbuf, result);
      i += result;
      if (i > filestat.st_size)
         die_horribly ("file has changed, restart", NULL);

      filled += result;
      if (filled < Globals.buffsize)
      {
         next_read = ((filled + DEFAULT_READ_BUFFER) > Globals.buffsize) ?
                       Globals.buffsize - filled
                     : DEFAULT_READ_BUFFER;
      }

      if ((next_read <= 0) || (filled >= Globals.buffsize))
         break;
      memset (strbuf, 0, DEFAULT_READ_BUFFER);
   }

   if (!Globals.filesize)
      Globals.filesize = filestat.st_size - 1;

   initbuffer (*fp, 0, filled - 1);

   if (Globals.modified != READ_ONLY)
      Globals.modified = 0;

   {
/*    signal (SIGINT, handleInterrupt); */
      struct sigaction sig_a;
      sig_a.sa_handler = handleInterrupt;
      memset (&sig_a.sa_mask, 0x00, sizeof (sigset_t));
/*      sig_a.sa_flags = SA_NOMASK; */
      sig_a.sa_flags = SA_NODEFER;
      sigaction (SIGINT, &sig_a, NULL);
   }


   if (newlines)
      free (newlines);
   newlines = malloc (BOTTOM_LINE * sizeof (int));
   if (!newlines)
      die_horribly (NOT_ENOUGH_MEMORY, NULL);

      /* if the whole buffer is not read into memory, we need to
       * do hashing of all modifications to keep track of changes.
       * Also, for when not in fullsize mode, to keep bold changes.
       */
   init_hash ();

   if (UndoStack.base)
   {
      struct Change *cp = NULL;
      while (UndoStack.base)
      {
         cp = UndoStack.base->p;
         free (UndoStack.base);
         UndoStack.base = cp;
      }
   }
   UndoStack.s = 0;
   UndoStack.base = NULL;
   UndoStack.top = NULL;

      /* May be comming out of file mode from selection widget. */
   Globals.mode       = HEX_MODE;

}
