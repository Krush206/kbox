/* hash.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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


/* This file contains a simple hash for looking up changes we may have
 * made to specific offsets in the file.  When we are editing a large
 * file that cannot fit in memory, when we reread a portion of the file
 * from disk, it will be the original, and not the changed, so we need
 * to lookup changes and add them to the current buffer in memory.
 * The Hash function is just a mod 1009 into the hash table of 0-1008
 * entries.  Collisions are handled with just a list of entries for
 * that index in the hash.  The key/pair is offset/byte-value.
 */

#include "hexedit.h"


   /* All the books say this should usually be a prime number */
#define HASH_TABLE_SIZE   1009

   /* The really complex and thought out hash function.  */
#define HASH(x)    (x % HASH_TABLE_SIZE)


   /* list of chained collisions */
struct hash_list
{
   unsigned long offset;
   unsigned char value;
   struct hash_list *next;
};

struct hash_table
{
   struct hash_list *chain;
};

   /* hash table */
static struct hash_table *table = NULL;



void
init_hash ()
   /* set each location empty by setting *next chain to -1 */
{
   int i = 0;

   if (table)
   {
      struct hash_list *p = NULL;

      for (i = 0; i < HASH_TABLE_SIZE; i++)
      {
         while (table[i].chain)
         {
            p = table[i].chain->next;
            free (table[i].chain);
            table[i].chain = p;
         }
      }
      
      free (table);
   }

   table = malloc (HASH_TABLE_SIZE * sizeof (struct hash_table));
   if (!table)
      die_horribly ("Not enough memory to create hash table\n", NULL);

   for (i = 0; i < HASH_TABLE_SIZE; i++)
   {
      table[i].chain = NULL;
   }
}


void
clear_hash ()
{
   int i = 0;
   struct hash_list *p = NULL;

   for (i = 0; i < HASH_TABLE_SIZE; i++)
   {
      while (table[i].chain)
      {
         p = table[i].chain;
         table[i].chain = table[i].chain->next;
         free (p);
      }
   }
}


void
insert_hash_entry (unsigned long offset, unsigned char value)
{
   int i = HASH(offset);

   if (!table[i].chain)
      /* empty location in table */
   {
      table[i].chain = malloc (sizeof (struct hash_list));
      if (!table[i].chain)
         die_horribly (NOT_ENOUGH_MEMORY, NULL);

      table[i].chain->offset = offset;
      table[i].chain->value  = value;
      table[i].chain->next   = NULL;
   }
   else
      /* Has at least one element in table.  Insert the new collision
       * into the front of the chain.
       */
   {
      struct hash_list *p = table[i].chain;

         /* First, make sure entry isn't already here, if it is, we 
          * change it to the new value.
          */
      while (p)
      {
         if (p->offset == offset)
         {
            p->value = value;
            break;
         }
         p = p->next;
      }
      if (p)
         return;       /* already in hash */

      p = malloc (sizeof (struct hash_list));
      if (!p)
         die_horribly (NOT_ENOUGH_MEMORY, NULL);

         /* build the new entry */
      p->offset = offset;
      p->value  = value;
      p->next   = table[i].chain;

         /* now put entry in the front of the list */
      table[i].chain = p;
   }
}


void
delete_hash_entry (unsigned long offset)
{
   int i = HASH(offset);
   struct hash_list *p = table[i].chain;

      /* hash entry not here */
   if (!p)
      return;

      /* one or more at this index */
   else
   {
      struct hash_list *last = NULL;
      while (p)
      {
         if (p->offset == offset)
            break;
         last = p;
         p = p->next;
      }
         /* if we found it delete it */
      if (p)
      {
            /* if item isn't first element */
         if (last)
         {
            last->next = p->next;   /* skip over p */
            free (p);               /* then delete it */
         }
         else
         {
            table[i].chain = p->next;  /* skip over p */
            free (p);                   /* then delete it */
         }
      }
   }
}


int
hash_lookup (unsigned long offset, unsigned char *c)
   /* return -1 on lookup failure.  Store value in *c if successful */
{
   int i = HASH(offset);
   struct hash_list *p = table[i].chain;
   
   while (p)
   {
      if (p->offset == offset)
         break;
      p = p->next;
   }

   if (p)
   {
      if (c)
         *c = p->value;
      return 0;
   }

   return -1;
}


#ifdef DEBUG_HASH
void
print_hash_table ()
{
   int i = 0;

   for (i = 0; i < HASH_TABLE_SIZE; i++)
   {
      fprintf (stderr, "%d: ", i);
         /* empty entry */
      if (!table[i].chain)
         fprintf (stderr, "empty\n");
      else
      {
         struct hash_list *p = table[i].chain;
         while (p)
         {
            fprintf (stderr, "(%ld,%d) ", p->offset, p->value);
            p = p->next;
         }
         fprintf (stderr, "\n");
      }
   }
}
#endif


void
commit_changes_in_hash (FILE *fp)
   /* cycle through the hash and write back all the entries, and clear
    * the hash while we're at it.
    */
{
   int i = 0;
   int result = 0;
   struct hash_list *p = NULL;

   for (i = 0; i < HASH_TABLE_SIZE; i++)
   {
      p = table[i].chain;
      while (table[i].chain)
      {
         result = fseek (fp, table[i].chain->offset, SEEK_SET);
         if (result)
            die_horribly ("Cannot write new file correctly", "fseek");

         result = fwrite (&table[i].chain->value, 1, 1, fp);
         if (!result && ferror (fp))
            die_horribly ("File writing error", "ferror");
         else if (!result && feof (fp))
            die_horribly ("End of File Reached?", "feof");

         p = table[i].chain->next;
         free (table[i].chain);
         table[i].chain = p;
      }
   }
}
