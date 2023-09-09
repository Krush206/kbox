/* calc.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

   /* Used only for redraw function after Control-C */
struct calcEntryBox *bbox;
struct calcEntryBox *obox;
struct calcEntryBox *dbox;
struct calcEntryBox *hbox;
int calc_mode = DEC_BOX;


void
binary_calculator ()
{
   wchar_t in = 0;
   struct calcEntryBox binbox = { "01", { '\0' }, 0,
                                  3, 1, { (int) 0 } };

   struct calcEntryBox octbox = { "01234567", { '\0' }, 0,
                                  5, 1, { 0L } };

   struct calcEntryBox decbox = { "0123456789", { '\0' }, 0,
                                  7, 1, { (unsigned int) 0 } };

   struct calcEntryBox hexbox = { "0123456789AaBbCcDdEeFf", { '\0' }, 0,
                                  9, 1, { 0UL } };


   calc_mode = DEC_BOX;

   bbox = &binbox;
   obox = &octbox;
   dbox = &decbox;
   hbox = &hexbox;

   werase (Globals.wmain);
   werase (Globals.wstatus);
   werase (Globals.whelp);

   wmove (Globals.wmain, 1, (COLS / 2) - (strlen ("Binary Calculator") / 2));
   wprintw (Globals.wmain, "Binary Calculator");

   calcStatWindow ();
   wmove (Globals.whelp, 0, 0);
   wprintw (Globals.whelp, "^X/^G  Return   ^C  Exit   J  Down   K  Up");
   calcDrawBoxes (&binbox, &octbox, &decbox, &hexbox, calc_mode);
   wrefresh (Globals.wstatus);
   wrefresh (Globals.whelp);
   wrefresh (Globals.wmain);
   refresh ();

   while (1)
   {   
      in = getch ();
      switch (in)
      {
         case CONTROL_G:
         case CONTROL_X:
         case ESCAPE_CHARACTER:
            return;

         case CONTROL_L:
            break;         

         case KEY_DOWN:
         case KEY_UP:
         case 'j':
         case 'J':
         case 'k':
         case 'K':
            calcArrowKey (&binbox, &octbox, &decbox, &hexbox, in, &calc_mode);
            break;

         default:
            calcNumberKey (&binbox, &octbox, &decbox, &hexbox, in, calc_mode);
            break;
      }
      calcStatWindow ();
      werase (Globals.whelp);
      wmove (Globals.whelp, 0, 0);
      wprintw (Globals.whelp, "^X/^G  Return   ^C  Exit   J  Down   K  Up");
      calcDrawBoxes (&binbox, &octbox, &decbox, &hexbox, calc_mode);
      wrefresh (Globals.wstatus);
      wrefresh (Globals.whelp);
      wrefresh (Globals.wmain);
      refresh ();
   }
}


void
calcDrawBoxes (struct calcEntryBox *binbox, struct calcEntryBox *octbox,
               struct calcEntryBox *decbox, struct calcEntryBox *hexbox,
               int mode)
{
      wmove (Globals.wmain, binbox->pos_y, binbox->pos_x);
      wprintw (Globals.wmain, "Base  2");
      wmove (Globals.wmain, octbox->pos_y, octbox->pos_x);
      wprintw (Globals.wmain, "Base  8");
      wmove (Globals.wmain, decbox->pos_y, decbox->pos_x);
      wprintw (Globals.wmain, "Base 10");
      wmove (Globals.wmain, hexbox->pos_y, decbox->pos_x);
      wprintw (Globals.wmain, "Base 16");

      calcBinaryString (binbox);
      drawEntryBox (Globals.wmain, binbox, binbox->pos_y, 10, BOX_LEN);
      if (mode == BIN_BOX)
      {
         wattrset (Globals.wmain, A_BOLD);
         wprintw  (Globals.wmain, " <==");
         wattroff (Globals.wmain, A_BOLD);
      }
      else
         wprintw (Globals.wmain, "    ");

      calcOctalString (octbox);
      drawEntryBox (Globals.wmain, octbox, octbox->pos_y, 10, BOX_LEN);
      if (mode == OCT_BOX)
      {
         wattrset (Globals.wmain, A_BOLD);
         wprintw  (Globals.wmain, " <==");
         wattroff (Globals.wmain, A_BOLD);
      }
      else
         wprintw (Globals.wmain, "    ");

      calcDecimalString (decbox);
      drawEntryBox (Globals.wmain, decbox, decbox->pos_y, 10, BOX_LEN);
      if (mode == DEC_BOX)
      {
         wattrset (Globals.wmain, A_BOLD);
         wprintw  (Globals.wmain, " <==");
         wattroff (Globals.wmain, A_BOLD);
      }
      else
         wprintw (Globals.wmain, "    ");

      calcHexString (hexbox);
      drawEntryBox (Globals.wmain, hexbox, hexbox->pos_y, 10, BOX_LEN);
      if (mode == HEX_BOX)
      {
         wattrset (Globals.wmain, A_BOLD);
         wprintw  (Globals.wmain, " <==");
         wattroff (Globals.wmain, A_BOLD);
      }
      else
         wprintw (Globals.wmain, "    ");
}


void
drawEntryBox (WINDOW *win, struct calcEntryBox *box, int y, int x, int len)
{
   int i = 0;

   wmove (win, y, x);
   wattrset (win, A_REVERSE);

   for (i = 0; i < len; i++)
      if (box->s[BOX_LEN - 1 - i])
         wprintw (win, "%c", box->s[BOX_LEN - 1 - i]);
      else
         wprintw (win, " ");

   wattroff (win, A_REVERSE);
}


void
calcStatWindow ()
{
   int i = 0;
   werase (Globals.wstatus);

   wmove (Globals.wstatus, 0, 0);
   wprintw (Globals.wstatus, "Integer: %d bits     Long: %d bits",
            sizeof (int) * 8, sizeof (long) * 8);
   
   for (i = 0; i < (COLS - 34); i++)
      wprintw (Globals.wstatus, " ");
}


void
calcBinaryString (struct calcEntryBox *binbox)
{
   int i = 0;
   int v = binbox->value.i;

   memset (binbox->s, 0x00, BOX_LEN + 1);
   for (i = 0; i < sizeof (int) * 8; i++)
   {
      binbox->s[i] = getAsciiValue (v & 0x01);
      v = (unsigned int) v >> 1;
   }
}


void
calcOctalString (struct calcEntryBox *octbox)
{
   char s[BOX_LEN + 1];
   int i = 0;
   int v = octbox->value.i;

   memset (octbox->s, 0x00, BOX_LEN + 1);
   memset (s, 0x00, BOX_LEN + 1);
   for (i = 0; i < sizeof (int) * 8; i++)
   {
#ifdef HAVE_SNPRINTF
        snprintf (s, BOX_LEN, "%o", octbox->value.i);
#else
        sprintf (s, "%o", octbox->value.i);
#endif
      v = v >> 1;
   }
   v = 0;
   for (i = BOX_LEN - 1; i >= 0; i--)
      /* swap order of the characters so it prints out right */
   {
      if (!s[i])
         continue;
      octbox->s[v++] = s[i];
   }
}


void
calcDecimalString (struct calcEntryBox *decbox)
{
   char s[BOX_LEN + 1];
   int i = 0;
   int v = decbox->value.i;

   memset (decbox->s, 0x00, BOX_LEN + 1);
   memset (s, 0x00, BOX_LEN + 1);
   for (i = 0; i < sizeof (int) * 8; i++)
   {
#ifdef HAVE_SNPRINTF
        snprintf (s, BOX_LEN, "%d", decbox->value.i);
#else
        sprintf (s, "%d", decbox->value.i);
#endif
      v = v >> 1;
   }
   v = 0;
   for (i = BOX_LEN - 1; i >= 0; i--)
      /* swap order of the characters so it prints out right */
   {
      if (!s[i])
         continue;
      decbox->s[v++] = s[i];
   }
}


void
calcHexString (struct calcEntryBox *hexbox)
{
   char s[BOX_LEN + 1];
   int i = 0;
   int v = hexbox->value.i;

   memset (hexbox->s, 0x00, BOX_LEN + 1);
   memset (s, 0x00, BOX_LEN + 1);
   for (i = 0; i < sizeof (int) * 8; i++)
   {
#ifdef HAVE_SNPRINTF
        snprintf (s, BOX_LEN, "%X", hexbox->value.i);
#else
        sprintf (s, "%X", hexbox->value.i);
#endif
      v = v >> 1;
   }
   v = 0;
   for (i = BOX_LEN - 1; i >= 0; i--)
      /* swap order of the characters so it prints out right */
   {
      if (!s[i])
         continue;
      hexbox->s[v++] = s[i];
   }
}
