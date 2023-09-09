/* search.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

static unsigned char curr;


struct foundit *
boyer_moore_search (unsigned char *substr, unsigned long offs,
                    unsigned long len, unsigned long buflen)
/* Uses the Boyer Moore Search algorithm by building up a table */
{
   int j = 0;
   unsigned char *s = NULL;
   unsigned char *buf = NULL;
   unsigned char c = '\0';
   unsigned long i = 0;
   unsigned long l_table[256];
   static struct foundit found;

   found.offset = 0x00;
   found.flag = 0;
   if ((len <= 0) || (buflen <= 0))
      return &found;

   buf = malloc (len);
   if (!buf)
      return &found;

   memset (l_table, 0, 256 * sizeof (int));
   i = 0;
   s = substr + len - 1;
   while (i < len)
   {
      if (!l_table[(int) *s])
      {
         if (i != 0)
            l_table[(int) *s] = i;
         else
            l_table[(int) *s] = -1;
      }
      else if (l_table[(int) *s] == -1)
         l_table[(int) *s] = i;
      s--;
      i++;
   }
   for (i = 0; i < 256; i++)
   {
      if (l_table[i] == -1)
         l_table[i] = len;
      else if (!l_table[i])
         l_table[i] = len;
   }
   s = substr;
   i = 0;
   curr = '\0';
   while (i < buflen - len + 1)
   {
      for (j = 0; j < len; j++)
         *(buf + j) = filebuffer (offs + i + j);
      if ((c = bytecmp (s, buf, len)) != 0)
      {
         if (i == buflen - len + 1)
         {
            free (buf);
            return &found;
         }
         i += l_table[(unsigned int) curr];
      }
      else
      {
         found.offset = i;
         found.flag = 1;
         free (buf);
         return &found;
      }
   }
   free (buf);
   return &found;
}



unsigned char
bytecmp (const unsigned char *s1, const unsigned char *s2, long n)
/* searched string backwards for Boyer-Moore search */
{
   s1 += n - 1;
   s2 += n - 1;
   curr = *s2;
   while (n-- > 0)
   {
      if (*s1 != *s2)
      {
         return 1;
      }
      s1--;
      s2--;
   }
   if (n < 0)
      return 0;
   return 1;
}
