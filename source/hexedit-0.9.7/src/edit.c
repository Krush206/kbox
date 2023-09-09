/* edit.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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


#include <assert.h>
#include "hexedit.h"

/* The routines in this file are very messy and ugly and not much thought
 * went into thinking how to minimize the amount of code, I only did it
 * real fast, making every case I could think of a seperate code block,
 * so this may be someplace someone may want to improve, especially since
 * all the hard coded screen positions make it hard to add new viewing
 * modes.
 */


/* Table for translating an ASCII character into an EBCDIC char.  Index into 
 * the array with an ASCII char value, at that index is the EBCDIC char value
 * of that character.                                                        
 */
const unsigned char ASCII_to_EBCDIC[] = {
0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,
0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,0x2e,
0x40,0x5a,0x7f,0x7b,0x5b,0x6c,0x50,0x7d,0x4d,0x5d,0x5c,0x4e,0x6b,0x60,0x00,0x61,
0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0x7a,0x5e,0x4c,0x7e,0x6e,0x6f,
0x7c,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,
0xd7,0xd8,0xd9,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xad,0xe0,0xbd,0x5f,0x6d,
0x79,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x91,0x92,0x93,0x94,0x95,0x96,
0x97,0x98,0x99,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0x8b,0x4f,0x9b,0xa1,0x2e
};



void
insertByte (unsigned char c, int in_undo)
{
   unsigned long off = 0;
   if (extrabuf <= 0)
   /* allocate extra chunks to use for inserting when full */
   {
      extrabuf = EXTRA_BUF;
      filebuf = realloc (filebuf, Globals.filesize + 1 + extrabuf);
      if (!filebuf)
         die_horribly ("\nNot enough Memory! file not saved", NULL);

      memset (filebuf + Globals.filesize, 0, extrabuf);
   }
   memmove (filebuf + offset + 1, filebuf + offset, Globals.filesize -
                                                    offset + 1);
   if (!in_undo)
      pushUndo (INSERT, offset, filebuffer (offset), c);

   *(filebuf + offset) = c;
   clear_hash ();
   off = offset & M_0xFFFFFFF0;
   off -= (cursor_y - MAIN_TOP_LINE) * 0x10;
   Globals.filesize++;
   Globals.buffsize++;
   Globals.buf_end++;
   extrabuf--;
   Globals.modified = MODIFIED;

   printStatusWindow ();
   drawdump (off);
   wmove (Globals.wmain, cursor_y, cursor_x);
   wrefresh (Globals.wmain);
   refresh ();
}


void
deleteByte (int in_undo)
{
   unsigned long off = 0;
   if (Globals.filesize <= 0x00)
   {
      off = 0;
      if (*filebuf)
         pushUndo (CHANGE, 0x00, filebuffer (0x00), filebuffer (0x00));
      *filebuf = 0;
      cursor_x = 10;
      Globals.filesize = 0x00;
      Globals.buf_front = 0;
      Globals.buf_end   = 0;
      do_beep ();
      printStatusWindow ();
      drawdump (off);
      if (Globals.tabb)
         move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
      else
         move (cursor_y, cursor_x);
      wrefresh (Globals.wmain);
      refresh ();
      return;
   }

   if (offset < Globals.filesize)
   {
      if (!in_undo)
         pushUndo (DELETE, offset, filebuffer (offset), filebuffer (offset));
      memmove (filebuf + offset, filebuf + offset + 1,
               Globals.filesize - offset);
   }
   else if (offset == Globals.filesize)
   {
      if (!in_undo)
         pushUndo (DELETE, offset, filebuffer (offset), filebuffer (offset));
      *(filebuf + offset) = 0;
      if (!mapcur (cursor_x))
      {
         if (Globals.spacing)
            cursor_x = 59;
         else
            cursor_x = 43;
         if (cursor_y > MAIN_TOP_LINE)
            cursor_y--;
      }
      else
         cursor_x = mappos (mapcur (cursor_x) - 1);
      offset--;
   }
   else /* this had better not ever happen */
   {
      offset = Globals.filesize;
      cursor_x = mappos (offset & M_0x0F);
      do_beep ();
      die_horribly ("Died a horrible, horrible death.  Mail maintainer", NULL);
      return;
   }

   *(filebuf + Globals.filesize) = 0;
   clear_hash ();
   Globals.filesize--;
   Globals.buffsize--;
   Globals.buf_end--;
   off = offset & M_0xFFFFFFF0;
   off -= (cursor_y - MAIN_TOP_LINE) * 0x10;
   if (off < 0x00)
      off = 0;
   extrabuf++;
   Globals.modified = MODIFIED;
   printStatusWindow ();
   drawdump (off);
   if (Globals.tabb)
      move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
   else
      move (cursor_y, cursor_x);
   wrefresh (Globals.wmain);
   refresh ();
}


void
over_write (wchar_t in)
{
   unsigned long off = 0;

   if (!isprintable (in))
      return;
   if (Globals.tabb)
   {
      if ((cursor_y == BOTTOM_LINE) && (mapcur (cursor_x) == 15))
      /* Bottom right corner */
      {
         pushUndo (CHANGE, offset, filebuffer (offset), in);
         if (Globals.charset == ASCII_CHAR_SET)
            filebuff_modify (offset, in);
         else /* EBCDIC */
            filebuff_modify (offset, findEBCDIC (in));
         if (offset < Globals.filesize)
         /* data below, scroll screen */
         {
            offset++;
            cursor_x = mappos (0);
         }
         else
         /* just overwrite character */
         {
            do_beep ();
         }
      }
      else if (offset == Globals.filesize)
      {
         pushUndo (CHANGE, offset, filebuffer (offset), in);
         if (Globals.charset == ASCII_CHAR_SET)
            filebuff_modify (offset, in);
         else /* EBCDIC */
            filebuff_modify (offset, findEBCDIC (in));
         do_beep ();
      }
      else if (mapcur (cursor_x) == 15)
      /* right edge of screen, not bottom corner */
      {
         pushUndo (CHANGE, offset, filebuffer (offset), in);
         if (Globals.charset == ASCII_CHAR_SET)
            filebuff_modify (offset, in);
         else /* EBCDIC */
            filebuff_modify (offset, findEBCDIC (in));
         offset++;
         cursor_x = mappos (0);
         cursor_y++;
      }
      else
      {
         pushUndo (CHANGE, offset, filebuffer (offset), in);
         if (Globals.charset == ASCII_CHAR_SET)
            filebuff_modify (offset, in);
         else /* EBCDIC */
            filebuff_modify (offset, findEBCDIC (in));
         offset++;
         cursor_x = mappos (mapcur (cursor_x) + 1);
      }
      off = offset & M_0xFFFFFFF0;
      off -= (cursor_y - 1) * M_0x10;
      Globals.modified = MODIFIED;
      drawdump (off);
      printStatusWindow ();
      wrefresh (Globals.wmain);
      wrefresh (Globals.wstatus);
      move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
      refresh ();
   }

   else if (isHexChar (in) && Globals.spacing)
   {
      if ((cursor_y == BOTTOM_LINE) && (cursor_x == 60))
      /* Bottom right corner */
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc        = (filebuffer (offset) & M_0xF0) |
                                                   getHexValue (in);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         if (offset < Globals.filesize)
         {
            offset++;
            cursor_x = mappos (0);
         }
         else
         {
            do_beep ();
         }
      }
      else if (offset == Globals.filesize)
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
            unsigned char old_value = filebuffer (offset);
            unsigned char nc = (getHexValue (in) << 4) |
                                   (filebuffer (offset) & M_0x0F);
            filebuff_modify (offset, nc);
            pushUndo (CHANGE, offset, old_value, filebuffer (offset));
            cursor_x++;
         }
         else
         {
            unsigned char old_value = filebuffer (offset);
            unsigned char nc = (filebuffer (offset) & M_0xF0) |
                                                  getHexValue (in);
            filebuff_modify (offset, nc);
            pushUndo (CHANGE, offset, old_value, filebuffer (offset));
            do_beep ();
         }
      }
      else if (cursor_x == 60)
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc = (filebuffer (offset) & M_0xF0) |
                                               getHexValue (in);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         offset++;
         cursor_x = mappos (0);
         cursor_y++;
      }
      else if (
            cursor_x == 10 || cursor_x == 13
         || cursor_x == 16 || cursor_x == 19
         || cursor_x == 23 || cursor_x == 26
         || cursor_x == 29 || cursor_x == 32
         || cursor_x == 37 || cursor_x == 40
         || cursor_x == 43 || cursor_x == 46
         || cursor_x == 50 || cursor_x == 53
         || cursor_x == 56 || cursor_x == 59)
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc = (getHexValue (in) << 4) |
                                (filebuffer (offset) & M_0x0F);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         cursor_x++;
      }
      else
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc = (filebuffer (offset) & M_0xF0) |
                                               getHexValue (in);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         offset++;
         cursor_x = mappos (mapcur (cursor_x) + 1);
      }
      off = offset & M_0xFFFFFFF0;
      off -= (cursor_y - 1) * M_0x10;
      Globals.modified = MODIFIED;
      drawdump (off);
      printStatusWindow ();
      wrefresh (Globals.wmain);
      wrefresh (Globals.wstatus);
      move (cursor_y, cursor_x);
      refresh ();
   }
   else if (isHexChar (in) && !Globals.spacing)
   {
      if ((cursor_y == BOTTOM_LINE) && (cursor_x == 44))
      /* Bottom right corner */
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc = (filebuffer (offset) & M_0xF0) |
                                                   getHexValue (in);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         if (offset < Globals.filesize)
         {
            offset++;
            cursor_x = mappos (0);
         }
         else
         {
            do_beep ();
         }
      }
      else if (offset == Globals.filesize)
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
            unsigned char old_value = filebuffer (offset);
            unsigned char nc = (getHexValue (in) << 4) |
                                   (filebuffer (offset) & M_0x0F);
            filebuff_modify (offset, nc);
            pushUndo (CHANGE, offset, old_value, filebuffer (offset));
            cursor_x++;
         }
         else
         {
            unsigned char old_value = filebuffer (offset);
            unsigned char nc = (filebuffer (offset) & M_0xF0) |
                                                  getHexValue (in);
            filebuff_modify (offset, nc);
            pushUndo (CHANGE, offset, old_value, filebuffer (offset));
            do_beep ();
         }
      }
      else if (cursor_x == 44)
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc = (filebuffer (offset) & M_0xF0) |
                                               getHexValue (in);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         offset++;
         cursor_x = mappos (0);
         cursor_y++;
      }
      else if (
            cursor_x == 10 || cursor_x == 12
         || cursor_x == 14 || cursor_x == 16
         || cursor_x == 19 || cursor_x == 21
         || cursor_x == 23 || cursor_x == 25
         || cursor_x == 28 || cursor_x == 30
         || cursor_x == 32 || cursor_x == 34
         || cursor_x == 37 || cursor_x == 39
         || cursor_x == 41 || cursor_x == 43)
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc = (getHexValue (in) << 4) |
                                (filebuffer (offset) & M_0x0F);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         cursor_x++;
      }
      else
      {
         unsigned char old_value = filebuffer (offset);
         unsigned char nc = (filebuffer (offset) & M_0xF0) |
                                               getHexValue (in);
         filebuff_modify (offset, nc);
         pushUndo (CHANGE, offset, old_value, filebuffer (offset));
         offset++;
         cursor_x = mappos (mapcur (cursor_x) + 1);
      }
      off = offset & M_0xFFFFFFF0;
      off -= (cursor_y - 1) * M_0x10;
      Globals.modified = MODIFIED;
      drawdump (off);
      printStatusWindow ();
      wrefresh (Globals.wmain);
      wrefresh (Globals.wstatus);
      move (cursor_y, cursor_x);
      refresh ();
   }
}


wchar_t
findEBCDIC (wchar_t in)
{
/*
   wchar_t i = 0;
   for (i = 0; i < 256; i++)
   {
      if (in == EBCDIC[i])
         return i;
   }
   do_beep ();
   assert (0);
   return 0;
*/

   return ASCII_to_EBCDIC[in];
}


void
filebuff_modify (unsigned long offs, unsigned char c)
{
      /* If the whole buffer isn't in memory, we need to adjust where
       * our offset is actually in filebuf, and also hash the change.
       * The actual offset in filebuf will be (offset - buf_front),
       * Assuming of course that the place being modified is always
       * in the current buffer.  This should always be true.  So
       * should (offset >= buf_front);
       */
   if (!Globals.fullsize)
   {
      assert (offset >= Globals.buf_front);
      *(filebuf + offset - Globals.buf_front) = c;
      insert_hash_entry (offset, c);
   }
      /* If the whole buffer Is in memory, we just change at that offset
       * in filebuf.
       */
   else
   {
      *(filebuf + offset) = c;
      insert_hash_entry (offset, c);
   }
}
