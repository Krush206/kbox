/* file.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

#include "../config.h"

#include <assert.h>

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif

#include <grp.h>
#include <pwd.h>

#ifdef TM_IN_SYS_TIME
#include <sys/time.h>
#endif

#include <time.h>

#include "hexedit.h"

struct FileNames *fp = NULL;     /* used to traverse the list of files */
struct FileNames **pages = NULL; /* node that starts a new page */
int current_page = 0;
int num_pages = 0;

static void exit_p (int);

static void
exit_p (int i)
{
   endwin ();
   exit (EXIT_SUCCESS);
}

char *
fileSelect ()
{
   char *p = NULL;
   char *ret = NULL;
   char dir[PATH_MAX + 1];
   char previous_dir[PATH_MAX + 1];
   char trunc_file[PATH_MAX + 1];
   wchar_t in = 0;
   int i = 0;
   int result = 0;
   struct FileNames *front = NULL;
   struct stat filestats;
   DIR *d = NULL;
   struct sigaction sig_a, sig_old;

   sig_a.sa_handler = exit_p;
   memset (&sig_a.sa_mask, 0x00, sizeof (sigset_t));
   sig_a.sa_flags = 0;
   sigaction (SIGINT, &sig_a, &sig_old);

   Globals.mode = FILE_MODE;

   memset (dir, 0x00, PATH_MAX + 1);
   memset (previous_dir, 0x00, PATH_MAX + 1);

   getcwd (previous_dir, PATH_MAX);

   while (1)
   {
      int num = 0;
      int rerr = 0;
      int derr = 0;

      if (*dir)
         strcpy (previous_dir, dir);
      memset (dir, 0x00, PATH_MAX + 1);
      p = getcwd (dir, PATH_MAX);
      if (!p)
         die_horribly (NULL, "getcwd");



      front = fp = getDirectory (dir);
      if (!fp)
      {
         result = errno;
         front = fp = getDirectory (previous_dir);
         if (!fp)
         {
            errno = result;
            die_horribly (NULL, previous_dir);
         }
         strcpy (dir, previous_dir);
      }
      while (fp)
      {
         fp = fp->p;
         num++;
      }
      num_pages = ((num - 1) / (LINES - 2)) + 1;

      pages = malloc (sizeof (struct FileNames *) * num_pages);
      memset (pages, 0x00, num_pages * sizeof (struct FileNames *));
      *pages = front;

      fp = front;
      for (i = 1; i < num_pages; i++)
      {
         int j = 0;
         for (j = 0; (j < (LINES - 2)) && fp; j++)
            fp = fp->p;
         *(pages + i) = fp;
      }
      fp = front;

      current_page = 0;
      cursor_y = MAIN_TOP_LINE;
      werase (Globals.wstatus);
      werase (Globals.wmain);
      werase (Globals.whelp);
      printPage (*pages);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wmove (Globals.wmain, 0, NAME_POS);
      wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      wprintw (Globals.wmain, "%s", trunc_file);
      wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                          : A_BOLD);
      statWindow (fp->filename);
      helpWindow ("^C ^X  Exit   ^M  Select File   k  Up   j  Down");
      wrefresh (Globals.wstatus);
      wrefresh (Globals.wmain);
      wrefresh (Globals.whelp);
      move (cursor_y, NAME_POS);
      refresh ();

      while ((in = getch ()) != '\n')
      {
         memset (trunc_file, 0x00, PATH_MAX + 1);
         switch (in)
         {
            case CONTROL_C:
            case CONTROL_X:
               exit_p (0);

            case KEY_UP:
            case 'k':
               file_key_up (&current_page);
               break;

            case KEY_DOWN:
            case 'j':
               file_key_down (&current_page, num_pages);
               break;

            case CONTROL_L:
               file_redraw (&current_page);
               break;

            case KEY_F(5):
               if (color_term != -1)
                  color_term ^= 1;
               file_redraw (&current_page);
               break;

            case CONTROL_V:
            case CONTROL_F:
            case KEY_NPAGE:
            case KEY_C3:
            case ' ':
               file_pagedown (&current_page, num_pages);
               break;

            case CONTROL_Y:
            case CONTROL_B:
            case KEY_PPAGE:
            case KEY_A3:
            case '-':
#ifdef __PDCURSES__
               case ALT_V:
#endif
               /* Page up */
               file_pageup (&current_page);
               break;

            case KEY_HOME:
               file_home (&current_page);
               break;                     
 
            case KEY_END:
               file_end (&current_page, num_pages);
               break;

            case ESCAPE_CHARACTER:
               in = getch ();

               switch (in)
               {
                  case 79:
                     in = getch ();
                     switch (in)
                     {
                        case 'A':
                           file_key_up (&current_page);
                           break;

                        case 'B':
                           file_key_down (&current_page, num_pages);
                           break;
                     }
                     break;

                  case 91:
                     in = getch ();
                     switch (in)
                     {
                        case 49: /* home */
                           file_home (&current_page);
                           break;                     
 
                        case 52: /* end */
                           file_end (&current_page, num_pages);
                           break;

                         case 53: /* page up */
                            file_pageup (&current_page);
                            break;

                         case 54: /* page down */
                            file_pagedown (&current_page, num_pages);
                            break;
                      }
                   getch ();  /* returns 126 */
               }
               break;

            default:
               break;
         }
      }

      stat (fp->filename, &filestats);
      if (!S_ISDIR (filestats.st_mode))
         break;

      errno = 0;
      d = opendir (fp->filename);
      derr = errno;
      closedir (d);
      errno = 0;
      result = chdir (fp->filename);
      rerr = errno;
      if (result || !d)
      {
#define FILE_BOX_WIDTH  50
#define FILE_BOX_HEIGHT 6
         WINDOW *wpopup = popupWindow (FILE_BOX_WIDTH, FILE_BOX_HEIGHT);
         char pdir[PATH_MAX + 1];

         errno = derr ? derr : rerr; 
         werase (wpopup);
         box (wpopup, 0, 0);
         wmove (wpopup, 1, (FILE_BOX_WIDTH / 2)
                 - (strlen ("Cannot Change Directory") / 2));
         wprintw (wpopup, "Cannot Change Direcotry");
         wmove (wpopup, FILE_BOX_HEIGHT - 3, (FILE_BOX_WIDTH / 2) -
                (strlen (strerror (errno)) + strlen ("Reason: ")) / 2);
         wprintw (wpopup, "Reason: %s", strerror (errno));
         wrefresh (wpopup);
         getch ();
         memset (pdir, 0x00, PATH_MAX + 1);
         if (*previous_dir != '/' && *previous_dir != '.')
            strcpy (pdir, "../");
         strcat (pdir, previous_dir);
         result = chdir (pdir);
         if (result)
            die_horribly ("Cannot change back to previous directory", pdir);
      }
      else
      {
         memset (dir, 0x00, PATH_MAX + 1);
         strcpy (dir, fp->filename);
      }


      free (pages);
      while (front)
      {
         fp = front->p;
         free (front->filename);
         free (front);
         front = fp;
      }        

   }

      /* This needs to be freed when editing a new file. */
   ret = malloc (strlen (fp->filename) + 1);
   if (!ret)
      die_horribly (NOT_ENOUGH_MEMORY, NULL);

   strcpy (ret, fp->filename);

   while (front)
   {
      fp = front->p;
      free (front->filename);
      free (front);
      front = fp;
   }
   if (pages)
      free (pages);

/*   signal (SIGINT, sigp); */
   sigaction (SIGINT, &sig_old, NULL);
   return ret;
}


void
statWindow (const char * const filename)
{
   int i = 0;
   int l = 0;

   wattrset (Globals.wstatus, color_term ? COLOR_PAIR(4) | A_BOLD
                                         : A_REVERSE);
   wmove (Globals.wstatus, 0, 0);
   wprintw (Globals.wstatus, "File: %s\n", filename);

   l = strlen (filename);
   wmove (Globals.wstatus, 0, l + 6);
   for (i = 0; i < (COLS - l - 6); i++)
      wprintw (Globals.wstatus, " ");

}


void
helpWindow (const char * const filename)
{
   wattrset (Globals.whelp, color_term ? COLOR_PAIR(3) | A_BOLD
                                         : A_NORMAL);
   wmove (Globals.whelp, 0, 0);
   wprintw (Globals.whelp, "%s", filename);
}


struct FileNames *
getDirectory (const char * const dir)
{
   DIR *d = NULL;
   int i = 0;
   int num = 0;
   struct dirent *dp = NULL;
   struct FileNames *fnp = NULL;
   struct FileNames *front = NULL;

   d = opendir (dir);
   if (!d)
   {
      return NULL;
   }

   while ((dp = readdir (d)))
   {
      if (!fnp)
      {
         front = fnp = malloc (sizeof (struct FileNames));
         if (!fnp)
            die_horribly (NOT_ENOUGH_MEMORY, NULL);
      }
      else
      {
         fnp->p = malloc (sizeof (struct FileNames));
         if (!fnp->p)
            die_horribly (NOT_ENOUGH_MEMORY, NULL);

         fnp = fnp->p;
      }
      fnp->filename = malloc (strlen (dp->d_name) + 1);
      if (!fnp->filename)
         die_horribly (NOT_ENOUGH_MEMORY, NULL);

      strcpy (fnp->filename, dp->d_name);
      fnp->p = NULL;
      num++;
   }

   if (num > 1)
      /* Split the list up in half for merge sort. */
   {
      fnp = front;
      for (i = 0; i < (num / 2); i++)
      {
         assert (fnp && fnp->p);
         if (i == ((num / 2) - 1))
         {
            struct FileNames *p = fnp;
            fnp = fnp->p;
            p->p = NULL;
         }
         else
            fnp = fnp->p;
      }
      front = merge (msort (front), msort (fnp));
   }

   closedir (d);

   return front;
}

struct FileNames *
merge (struct FileNames *p1, struct FileNames *p2)
   /* Merge for merge sort of linked list of filenames. */
{
   struct FileNames *files = NULL;
   struct FileNames *front = NULL;

   while (p1 && p2)
   {
      if (!files)
      {
         if (strcmp (p1->filename, p2->filename) < 0)
         {
            files = p1;
            p1 = p1->p;
         }
         else
         {
            files = p2;
            p2 = p2->p;
         }
         front = files;
      }
      else
      {
         if (strcmp (p1->filename, p2->filename) < 0)
         {
            files->p = p1;
            p1 = p1->p;
         }
         else
         {
            files->p = p2;
            p2 = p2->p;
         }
         files = files->p;
      }
      files->p = NULL;
   }
   while (p1)
   {
      files->p = p1;
      files = files->p;
      p1 = p1->p;
   }

   while (p2)
   {
      files->p = p2;
      files = files->p;
      p2 = p2->p;
   }

   files->p = NULL;

   return front;
}


struct FileNames *
msort (struct FileNames *p)
   /* The sort part of the merge sort. */
{
   int i = 0;
   int num = 0;
   struct FileNames *fnp = p;

   if (!p)
      return NULL;
   if (!p->p)
      return p;

   while (fnp)
   {
      fnp = fnp->p;
      num++;
   }

   fnp = p;
   for (i = 0; i < (num / 2); i++)
   {
      assert (fnp && fnp->p);
      if (i == ((num / 2) - 1))
      {
         struct FileNames *tp = fnp;
         fnp = fnp->p;
         tp->p = NULL;
      }
      else
         fnp = fnp->p;
   }
   
   return merge (msort (p), msort (fnp));
}


void
printPage (const struct FileNames *fp)
{
   int i = 0;
   int result = 0;
   char *bits[] = { "---", "--x", "-w-", "-wx",
                    "r--", "r-x", "rw-", "rwx" };
   char trunc_file[PATH_MAX + 1];
   struct stat filestat;


   if (color_term)
      wattrset (Globals.wmain, COLOR_PAIR(1));

   for (i = 0; (i < (LINES - 2)) && fp; i++)
   {
      char modebits[11];
      char s1[11];
      char s2[11];
      struct passwd *pw = NULL;
      struct group  *gw = NULL;
      struct tm *tp = NULL;

      memset (s1, 0x00, 11);
      memset (s2, 0x00, 11);
      result = stat (fp->filename, &filestat);
      wmove (Globals.wmain, i, 0);
      if (!result)
         /* do the ls thingy with the file mode */
      {
         memset (modebits, 0x00, 11);
         if (S_ISDIR (filestat.st_mode))
            modebits[0] = 'd';
         else if (S_ISCHR (filestat.st_mode))
            modebits[0] = 'c';
         else if (S_ISBLK (filestat.st_mode))
            modebits[0] = 'b';
         else if (S_ISFIFO (filestat.st_mode))
            modebits[0] = 'p';
#ifndef __PDCURSES__
         else if (S_ISSOCK (filestat.st_mode))
            modebits[0] = 's';
         else if (S_ISLNK (filestat.st_mode))
            modebits[0] = 'l';
#endif
         else
            modebits[0] = '-';
         strcat (modebits, bits[(filestat.st_mode & 0700) >> 6]);
         strcat (modebits, bits[(filestat.st_mode & 0070) >> 3]);
         strcat (modebits, bits[filestat.st_mode & 0007]);
         if (filestat.st_mode & 01000)
         {
            if (filestat.st_mode & 0001)
               modebits[9] = 't';
            else
               modebits[9] = 'T';
         }
         if (filestat.st_mode & 02000)
         {
            if (filestat.st_mode & 0010)
               modebits[6] = 's';
            else
               modebits[6] = 'S';
         }
         if (filestat.st_mode & 01000)
         {
            if (filestat.st_mode & 0100)
               modebits[3] = 's';
            else
               modebits[3] = 'S';
         }
         tp = localtime (&filestat.st_mtime);

         pw = getpwuid (filestat.st_uid);
         gw = getgrgid (filestat.st_gid);

#ifdef HAVE_SNPRINTF

         if (!pw)
            snprintf (s1, 11, "%d", (int) filestat.st_uid);
         else
            snprintf (s1, 11, "%s", pw->pw_name);

         if (!gw)
            snprintf (s2, 11, "%d", (int) filestat.st_gid);
         else
            snprintf (s2, 11, "%s", gw->gr_name);

#else

         if (!pw)
            sprintf (s1, "%d", (int) filestat.st_uid);
         else
            sprintf (s1, "%s", pw->pw_name);

         if (!gw)
            sprintf (s2, "%d", (int) filestat.st_gid);
         else
            sprintf (s2, "%s", gw->gr_name);

#endif         

         wprintw (Globals.wmain,
   /* why does it seem like on OpenBSD this is broken? */
#if defined(__OpenBSD__) || defined(BROKEN)
            "%s %-8s %-8s %10ld %1d\b2d-%02d-%02d %02d:%02d ",
#else
            "%s %-8s %-8s %10ld %d-%02d-%02d %02d:%02d ",
#endif
            modebits,
            s1,
            s2,
            filestat.st_size,
            1900 + tp->tm_year, 1 + tp->tm_mon, tp->tm_mday,
            tp->tm_hour, tp->tm_min);
      }
         /* stat failed, print enough spaces to align file anyways */
      else
      {
         int j = 0;
         for (j = 0; j < 57; j++)
            wprintw (Globals.wmain, " ");
      }

      memset (trunc_file, 0x00, PATH_MAX + 1);
      strncpy (trunc_file, fp->filename, COLS - NAME_POS);
      wprintw (Globals.wmain, "%s", trunc_file);
      fp = fp->p;

      for (result = Globals.wmain->_curx; result < COLS; result++)
         wprintw (Globals.wmain, " ");
   }

      /* Fill in rest of the screen with default color. */
   for (; i < (LINES - 2); i++)
      for (result = 0; result < COLS; result++)
         wprintw (Globals.wmain, " ");
}
