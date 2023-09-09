/* help.c by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
 * Copyright (C) 1999 Adam Rogoyski
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

   /* we need to modify these two strings to display the correct message,
    * how to insert/delete if enabled, or how to enable it.
    */
#define INSERT_MSG_INDEX  4
#define DELETE_MSG_INDEX  5

#define HELP_MSG_COUNT    21

   /* The help messages used with the online help screen. */
const char *help_msgs[] = {
 "Maneuver            Arror Keys (or h,j,k,l), Home, End",
 "Page Up             Control-y, Control-b, Alt-v, Page Up (^Y) (^B) u y",
 "Page Down           Control-v, Control-f, Page Down (^V) (^F) v space",
 "Bytes <=> text      Tab, Control-i (^I)",
 "Insert/Add Byte     ",
 "Delete Byte         ",
 "goTo Offset         Control-t (^T), t",
 "Offset + Jump       +, -, n (jump again)",
 "Help                Control-g (^G), F1",
 "Redraw Screen       Control-l (^L)",
 "Save                Control-o (^O), F2",
 "Save and Quit       Control-x (^X), F3",
 "Quit/New File       Control-c (^C)",
 "viEw as text        Control-e (^E)",
 "Search              Control-w (^W), w",
 "find Next           Control-n (^N)",
 "Undo                Control-U (^U)",
 "ASCII <=> EBCDIC    Control-R (^R)",
 "Switch spacings     Control-P (^P)",
 "Binary Calculator   Control-/, Control-_ (^/) (^_)",
 "Toggle Color        F5 (requires color capable terminal)",
 NULL
};


void
help_initialize ()
{
   char *temp = NULL;

      /* Take care of Insert message. */
   temp = malloc (1 + strlen (help_msgs[INSERT_MSG_INDEX])
    + strlen (
       Globals.fullsize ? "Control-a, Insert (^A)"
                        : "Disabled.  Use -b, --buffer option to enable"));
   strcpy (temp, help_msgs[INSERT_MSG_INDEX]);
   strcat (temp, 
       Globals.fullsize ? "Control-a, Insert (^A)"
                        : "Disabled.  Use -b, --buffer option to enable");

   help_msgs[INSERT_MSG_INDEX] = temp;
   

      /* Take care of Delete message. */
   temp = malloc (1 + strlen (help_msgs[DELETE_MSG_INDEX])
    + strlen (
       Globals.fullsize ? "Control-d, Delete (^D)"
                        : "Disabled.  Use -b, --buffer option to enable"));
   strcpy (temp, help_msgs[DELETE_MSG_INDEX]);
   strcat (temp, 
       Globals.fullsize ? "Control-d, Delete (^D)"
                        : "Disabled.  Use -b, --buffer option to enable");

   help_msgs[DELETE_MSG_INDEX] = temp;

   Globals.help_msg_count = HELP_MSG_COUNT;
}


void
help_print (WINDOW *win, int y, int lines)
{
   int i = 1;
   while ((i < lines) && help_msgs[y])
   {
      wmove (win, i, 3);
      wprintw (win, "%s", help_msgs[y]);
      i++;
      y++;
   }
}
