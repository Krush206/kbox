/* undo.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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


void
pushUndo (int type, unsigned long offset,
          unsigned char old, unsigned char new)
   /* Push the latest change on the undo stack */
{
   if (!UndoStack.s)
   {
      assert (!UndoStack.base);
      UndoStack.base = malloc (sizeof (struct Change));
      if (!UndoStack.base)
         /* quietly make undo disabled */
         return;
      UndoStack.base->type = type;
      UndoStack.base->offset = offset;
      UndoStack.base->new = new;
      UndoStack.base->old = old;
      UndoStack.base->p = NULL;
      UndoStack.top = UndoStack.base;
   }   
   else
   {
      assert (UndoStack.base && UndoStack.top);
      UndoStack.top->p = malloc (sizeof (struct Change));
      if (!UndoStack.top->p)
         /* quietly make undo disabled */
         return;
      UndoStack.top = UndoStack.top->p;
      UndoStack.top->type = type;
      UndoStack.top->offset = offset;
      UndoStack.top->new = new;
      UndoStack.top->old = old;
      UndoStack.top->p = NULL;
   }

   UndoStack.s++;
}


void
undoLastChange ()
/* This is not reentrant.  If you somehow hit Control-U and resize the 
 * screen extremely quickly, It may try to undo a portion of the screen
 * that is no longer viewable.  Ignoring SIGWINCH would fix this.
 */
{
   int bold = 0;

   if (!UndoStack.s)
   {
      do_beep ();
      refresh ();
      return;
   }
   else
   {
      unsigned long top = 0;
      unsigned long bottom = 0;

      assert (UndoStack.base && UndoStack.top);

      top = cursor_y - MAIN_TOP_LINE;
      top = (offset - (top * M_0x10)) & M_0xFFFFFFF0;

      bottom = BOTTOM_LINE - cursor_y + 1;
      bottom = (offset + (bottom * M_0x10)) & M_0xFFFFFFF0;

      if ((UndoStack.top->offset >= top) && (UndoStack.top->offset < bottom))
         /* Last changed position is currently viewable on the screen */
      {
         int t = 0;
         cursor_x = mappos (UndoStack.top->offset & M_0x0F);

         t = ((UndoStack.top->offset & M_0xFFFFFFF0)
               - (offset & M_0xFFFFFFF0));
         if (t > 0)
            cursor_y += t >> 4;
         else if (t < 0)
            cursor_y -= (-t) >> 4;

         offset = UndoStack.top->offset;
         switch (UndoStack.top->type)
         {
            case INSERT:
               deleteByte (1);
               break;

            case DELETE:
               insertByte (UndoStack.top->old, 1);
               break;

            case CHANGE:
            {
               struct Change *p = UndoStack.base;
               struct Change *q = NULL;
                  /* We need to see if there are other changes
                   * at this offset, so we can add that older value
                   * as the new current one in the hash.
                   */
               while (p != UndoStack.top)
               {
                  if (p->offset == offset)
                     q = p;
                  p = p->p;
               }
               filebuff_modify (offset, UndoStack.top->old);
                  /* If the older change to this offset exists, add
                   * it as the current one in the cache.
                   */
               if (q && (q != UndoStack.top))
                  insert_hash_entry (offset, q->new);
               else
                  delete_hash_entry (offset);
               break;
            }
         }

         bold = hash_lookup (offset, NULL);
         if (!bold)
            wattrset (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                                : A_BOLD);
         wmove (Globals.wmain, cursor_y - 1, cursor_x);
         wprintw (Globals.wmain, "%02lX", filebuffer (offset));

         wmove (Globals.wmain, cursor_y - 1,
                               COLS - M_0x10 + mapcur (cursor_x));
         if (Globals.charset == ASCII_CHAR_SET)
         {
            if (isprintable (filebuffer (offset)))
               wprintw (Globals.wmain, "%c", filebuffer (offset));
            else
               wprintw (Globals.wmain, ".");
         }
         else /* EBCDIC */
         {
            wprintw (Globals.wmain, "%c", EBCDIC[filebuffer (offset)]);
         }

         if (!bold)
         {
            wattroff (Globals.wmain, color_term ? COLOR_PAIR(2) | A_BOLD
                                                : A_BOLD);
            if (color_term)
               wattrset (Globals.wmain, COLOR_PAIR(1));
         }

         if (Globals.tabb)
            move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
         else
            move (cursor_y, cursor_x);
         printStatusWindow ();
         wrefresh (Globals.wmain);
         refresh ();
      }
      else
         /* must move the screen so the last changed position is viewable */
      {
         offset = UndoStack.top->offset;
         cursor_y = MAIN_TOP_LINE;
         cursor_x = mappos (offset & M_0x0F);

         if (Globals.tabb)
            move (cursor_y, COLS - M_0x10 + mapcur (cursor_x));
         else
            move (cursor_y, cursor_x);
            /* we need to move the whole screen to see the change, unlike
             * above where the change is on the current viewable screen.
             * We don't undo though, we let the user see that position of
             * the file, and the next ^U will do that actual undo using
             * the previous section of code.
             */
         redraw (); 
         return;
      }
   }

   if (UndoStack.s == 1)
   {
      assert (UndoStack.base == UndoStack.top);
      free (UndoStack.base);
      UndoStack.base = UndoStack.top = NULL;
      UndoStack.s = 0;
   }
   else
   {
      struct Change *p = UndoStack.top;

      assert (UndoStack.base != UndoStack.top);

      UndoStack.top = UndoStack.base;
      while (UndoStack.top->p != p)
         UndoStack.top = UndoStack.top->p;

      free (UndoStack.top->p);
      UndoStack.top->p = NULL;
      UndoStack.s--;
   }
}
