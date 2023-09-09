/* filebuf.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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


void
initbuffer (FILE *filep, unsigned long front, unsigned long end)
{
   Globals.buf_front = front;
   Globals.buf_end   = end;
   Globals.fp        = filep;
}


unsigned char
filebuffer (unsigned long offs)
/* This is used for all accesses to the current file being viewed or
 * edited.  All access to filebuf is done indirectly since this way
 * we can buffer the file in small pieces to save memory if needed.
 * The buffer very well may be just the size of the entire file.
 */
{
   unsigned long temp = 0;

      /* If the requested section of the file is already in memory, 
       * just return that byte
       */
   if ((offs >= Globals.buf_front) && (offs <= Globals.buf_end))
      return *(filebuf + offs - Globals.buf_front);

      /* Else we load a new section of the file into memory.  We will
       * take the position asked for and buffer before and after it
       */
   temp = offs - (Globals.buffsize / 2);
      /* check for unsigned underflow: lower bound > upper bound */
   if (temp > offs)
      temp = 0;
   Globals.buf_front = temp;

   Globals.buf_end   = Globals.buf_front + Globals.buffsize - 1;

      /* check for unsigned overflow: upper bound < lower bound */
   if (Globals.buf_end < Globals.buf_front)
      Globals.buf_end = Globals.buffsize;


   memset (filebuf, 0x00, Globals.buffsize);

   filebuffer_new_buf ();

   return *(filebuf + offs - Globals.buf_front);
}


unsigned long
filebuffer_new_buf ()
{

   int i = 0;
   unsigned long filled = 0;
   int next_read = 0;
   int result = 0;
   unsigned char strbuf[DEFAULT_READ_BUFFER];
   unsigned char c = '\0';

   result = fseek (Globals.fp, Globals.buf_front, SEEK_SET);
   filled = 0;

   next_read = ((filled + DEFAULT_READ_BUFFER) > Globals.buffsize) ?
                 DEFAULT_READ_BUFFER - filled
               : DEFAULT_READ_BUFFER;

   memset (strbuf, 0, DEFAULT_READ_BUFFER);
   while ((result = fread (strbuf, 1, next_read, Globals.fp)) > 0)
   {
      if (ferror (Globals.fp))
         die_horribly (NULL, Globals.filename);

      memcpy (filebuf + i, strbuf, result);
      i += result;
      if (i > Globals.filesize + 1)
         die_horribly ("file has changed, restart", NULL);

      filled += result;
      if (filled < Globals.buffsize)
      {
         next_read = ((filled + DEFAULT_READ_BUFFER) > Globals.buffsize) ?
                       DEFAULT_READ_BUFFER - filled
                     : DEFAULT_READ_BUFFER;
      }
      if ((next_read <= 0) || (filled >= Globals.buffsize))
         break;
      memset (strbuf, 0, 2048);
   }
      /* Since we may be reading a portion of the file from disk that
       * may have been edited, we need to add those changes back into
       * the current filebuf.
       */
   if (Globals.modified != READ_ONLY && !Globals.fullsize)
   {
      for (i = 0; i < filled; i++)
      {
         if (!hash_lookup (Globals.buf_front + i, &c))
            *(filebuf + i) = c;
      }
   }
   return filled;
}
