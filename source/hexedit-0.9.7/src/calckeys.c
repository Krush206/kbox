/* calckeys.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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


void
calcNumberKey (struct calcEntryBox *binbox, struct calcEntryBox *octbox,
               struct calcEntryBox *decbox, struct calcEntryBox *hexbox,
               wchar_t in, int mode)
{
   unsigned char *p = NULL;
   int bitoff = 0;

   memset (&bitoff, 0xFF, sizeof (int));
   p = (unsigned char *) &bitoff + (sizeof (int) - 1);
   switch (mode)
   {
      case BIN_BOX:
         switch (in)
         {
            case CONTROL_D:
            case BACKSPACE:
            case CONTROL_H:
            case KEY_BACKSPACE:
            case KEY_DC:
#ifdef __PDCURSES__
               case CTL_BKSP:
#endif
               binbox->value.i = binbox->value.i >> 1;
               *p &= 0x7F;
               binbox->value.i &= bitoff;
               octbox->value.i = decbox->value.i = hexbox->value.i =
                                                       binbox->value.i;
               break;

            default:
            {
               char *p = binbox->tokens;
               while (*p)
               {
                  if (*p == in)
                     break;
                  p++;
               }
               if (!*p)
                  return;
               binbox->value.i = (binbox->value.i << 1) + getHexValue (in);
               octbox->value.i = decbox->value.i = hexbox->value.i =
                                                       binbox->value.i;
            }
         }
         break;

      case OCT_BOX:
         switch (in)
         {
            case CONTROL_D:
            case BACKSPACE:
            case CONTROL_H:
            case KEY_BACKSPACE:
            case KEY_DC:
#ifdef __PDCURSES__
               case CTL_BKSP:
#endif
               octbox->value.i = octbox->value.i >> 3;
               *p = 0x1F;
               octbox->value.i &= bitoff;
               binbox->value.i = decbox->value.i = hexbox->value.i =
                                                       octbox->value.i;
               break;

            default:
            {
               char *p = octbox->tokens;
               while (*p)
               {
                  if (*p == in)
                     break;
                  p++;
               }
               if (!*p)
                  return;
               octbox->value.i = (octbox->value.i << 3) + getHexValue (in);
               binbox->value.i = decbox->value.i = hexbox->value.i =
                                                       octbox->value.i;
            }
         }
         break;
      
      case DEC_BOX:
         switch (in)
         {
            case CONTROL_D:
            case BACKSPACE:
            case CONTROL_H:
            case KEY_BACKSPACE:
            case KEY_DC:
#ifdef __PDCURSES__
               case CTL_BKSP:
#endif
               decbox->value.i /= 10;
               binbox->value.i = octbox->value.i = hexbox->value.i =
                                                     decbox->value.i;
               break;

            default:
            {
               char *p = hexbox->tokens;
               while (*p)
               {
                  if (*p == in)
                     break;
                  p++;
               }
               if (!*p)
                  return;
                  /* this doesn't really do what we want it to do */
               decbox->value.i *= 10;
               decbox->value.i += getHexValue (in);
               binbox->value.i = octbox->value.i = hexbox->value.i =
                                                     decbox->value.i;
               break;
            }
         }
         break;

      case HEX_BOX:
         switch (in)
         {
            case CONTROL_D:
            case BACKSPACE:
            case CONTROL_H:
            case KEY_BACKSPACE:
            case KEY_DC:
#ifdef __PDCURSES__
               case CTL_BKSP:
#endif
               hexbox->value.i = hexbox->value.i >> 4;
               *p &= 0x0F;
               hexbox->value.i &= bitoff;
               binbox->value.i = octbox->value.i = decbox->value.i =
                                                       hexbox->value.i;
               break;

            default:
            {
               char *p = hexbox->tokens;
               while (*p)
               {
                  if (*p == in)
                     break;
                  p++;
               }
               if (!*p)
                  return;
               hexbox->value.i = (hexbox->value.i << 4) + getHexValue (in);
               binbox->value.i = octbox->value.i = decbox->value.i =
                                                       hexbox->value.i;
            }
         }
         break;
   }
}


void
calcArrowKey  (struct calcEntryBox *binbox, struct calcEntryBox *octbox,
               struct calcEntryBox *decbox, struct calcEntryBox *hexbox,
               wchar_t in, int *mode)
{
   switch (*mode)
   {
      case BIN_BOX:
         if ((in == KEY_UP) || (toupper (in) == 'K'))
            *mode = HEX_BOX;
         else if ((in == KEY_DOWN) || (toupper (in) == 'J'))
            *mode = OCT_BOX;
         break;

      case OCT_BOX:
         if ((in == KEY_UP) || (toupper (in) == 'K'))
            *mode = BIN_BOX;
         else if ((in == KEY_DOWN) || (toupper (in) == 'J'))
            *mode = DEC_BOX;
         break;

      case DEC_BOX:
         if ((in == KEY_UP) || (toupper (in) == 'K'))
            *mode = OCT_BOX;
         else if ((in == KEY_DOWN) || (toupper (in) == 'J'))
            *mode = HEX_BOX;
         break;

      case HEX_BOX:
         if ((in == KEY_UP) || (toupper (in) == 'K'))
            *mode = DEC_BOX;
         else if ((in == KEY_DOWN) || (toupper (in) == 'J'))
            *mode = BIN_BOX;
         break;
   }
}
