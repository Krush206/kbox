#ifndef HEXEDIT_H
#define HEXEDIT_H

/* hexedit.h by Adam Rogoyski <apoc@laker.net> Temperanc on EFNet irc
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

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <ctype.h>

#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else
#ifdef HAVE_CURSES_H
#include <curses.h>
#endif
#endif

#include <errno.h>

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#ifdef STDC_HEADERS
#include <stdlib.h>
#include <string.h>
#endif

#include <stdio.h>
#include <signal.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif


   /* When we push a change on the Undo Stack, this is what type of change
    * it was.
    */
#define  INSERT            1
#define  DELETE            2
#define  CHANGE            3

   /* Globals.modified takes on 0 if the file is the original (doesn't need
    * to be saved), 1 if it's modified (needs to be saved), and 2 for
    * read-only.
    */
#define  MODIFIED          1
#define  READ_ONLY         2

   /* Globals.print_mode decides what in the text mode part of the screen
    * is printed.  Regular is 7-bit ascii.  High ascii is 8-bit ascii.
    * All print is all characters.
    */
#define  REGULAR_PRINT     1
#define  HIGH_ASCII_PRINT  2
#define  ALL_PRINT         3

   /* These I think are defined in curses as macros but I still use them */
#ifndef  __PDCURSES__
#define  ALT_V             118
#endif
#define  BACKSPACE         127
#define  CONTROL_A         1
#define  CONTROL_B         2
#define  CONTROL_C         3
#define  CONTROL_D         4
#define  CONTROL_E         5
#define  CONTROL_F         6
#define  CONTROL_G         7
#define  CONTROL_H         8
#define  CONTROL_I         9
#define  CONTROL_L         12
#define  CONTROL_N         14
#define  CONTROL_O         15
#define  CONTROL_P         16
#define  CONTROL_R         18
#define  CONTROL_T         20
#define  CONTROL_U         21
#define  CONTROL_V         22
#define  CONTROL_W         23
#define  CONTROL_X         24
#define  CONTROL_Y         25
#define  CONTROL_SLASH     31
#define  ESCAPE_CHARACTER  27
#define  SPACEBAR          32
#define  TAB               9

   /* Prevent me from typing these in wrong */
#define  M_0xFFFFFFF0      0xfffffff0
#define  M_0xF0            0xf0
#define  M_0x10            0x10
#define  M_0x0F            0x0f

   /* Top and bottom lines of the main window are different depending if
    * curses is broken or not.
    */
#define  MAIN_HEIGHT       (LINES - 2)
#define  MAIN_TOP_LINE     1
#define  MAIN_BOTTOM_LINE  (LINES - 3)
#define  BOTTOM_LINE       (LINES - 2)

   /* Malloc extra bytes so we can do a lot of inserts without realloc */
#define  EXTRA_BUF         1024

   /* In file selection widget, the filename starts at column 57. */
#define  NAME_POS          57

   /* Globals.mode, which mode of the editor we are in.  They are File
    * selection widget, normal hex mode, ascii mode (snapshot), and the
    * calculator.
    */
#define  FILE_MODE         0
#define  HEX_MODE          1
#define  ASCII_MODE        2
#define  CALCULATOR_MODE   3

   /* Globals.charset, if we are using ASCII or EBCDIC character set to
    * display text.
    */
#define  ASCII_CHAR_SET    0
#define  EBCDIC_CHAR_SET   1

   /* Buffer size is how much we read of the file at a time from disk and
    * keep in memory (in filebuf).  Read buffer is how much we read from
    * disk with each fread () call.
    * Buffer size must be greater than read buffer, both must be even.
    */
#define  DEFAULT_BUFFER_SIZE  0x4000    /* 16384 */
#define  DEFAULT_READ_BUFFER  0x800     /*  2048 */


   /* Functions prototypes for functions used througout */

   /* Functions in misc.c */
char *       chompWhiteSpace        (char *);
int          cursor_ascii_to_ebcdic (int);
void         die_horribly           (const char * const, const char * const);
void         do_beep                ();
void         exitProgram            (void);
void         exitSave               (int);
int          getHexValue            (wchar_t);
char         getAsciiValue          (wchar_t);
void         handleInterrupt        (int);
#ifndef __PDCURSES__
   void      handleSigwinch         (int);
#endif
int          isHexChar              (wchar_t);
int          isprintable            (int);
int          mappos                 (int);
int          mapcur                 (int);
void         switchModes            (void);
void         usage                  (char *);


   /* Functions in init.c */
void         init (int, char **);
void         load_new_file (FILE **);
void         popup_Error (const char * const, int);
char *       select_new_file (FILE **);


   /* Functions in hexkeys.c */
void         endKey          (void);
void         handleArrowKeys (wchar_t);
void         hexMode         (wchar_t);
void         homeKey         (void);
void         pagedown        (void);
void         pageup          (void);
void         tabcursor       (int *);


   /* Functions in edit.c */
void         deleteByte      (int);
void         filebuff_modify (unsigned long, unsigned char);
wchar_t      findEBCDIC      (wchar_t);
void         insertByte      (unsigned char, int);
void         over_write      (wchar_t);


   /* Functions in filebuf.c */
void           initbuffer         (FILE *, unsigned long, unsigned long);
unsigned char  filebuffer         (unsigned long);
unsigned long  filebuffer_new_buf ();


   /* Functions in asciikey.c */
void           asciiMode (wchar_t);



   /* Functions in windows.c and widgets.c */
struct
ret_string * hex_string_box       (WINDOW *, int, int);
void         jump_relative_offset (wchar_t);
int          popupFileSave        (void);
void         popupGotoOffset      (long);
void         popupHelp            (void);
WINDOW *     popupWindow          (int, int);
void         search               (int);
char *       stringBox            (WINDOW *, int, int, int, int, char *);



   /* Functions in print.c */
void         drawdump (unsigned long);
void         printHelpWindow (void);
void         printStatusWindow (void);
void         redraw (void);


   /* Functions in search.c */
struct
foundit *    boyer_moore_search (unsigned char *, unsigned long,
                                 unsigned long, unsigned long);
unsigned
char         bytecmp (const unsigned char *, const unsigned char *,
                      long);


   /* Functions in undo.c */
void      pushUndo       (int, unsigned long, unsigned char, unsigned char);
void      undoLastChange (void);


   /* Functions in help.c */
void         help_initialize (void);
void         help_print (WINDOW *, int, int);


   /* Functions in file.c and filekeys.c */
char *             fileSelect (void);
void               statWindow    (const char * const);
void               helpWindow    (const char * const);
struct FileNames * getDirectory  (const char * const);
struct FileNames * merge         (struct FileNames *, struct FileNames *);
struct FileNames * msort         (struct FileNames *);
void               printPage     (const struct FileNames *);
void               file_key_up   (int *);
void               file_key_down (int *, int);
void               file_redraw   (int *);
void               file_pagedown (int *, int);
void               file_pageup   (int *);
void               file_home     (int *);
void               file_end      (int *, int);


   /* functions in calc.c */
#define BIN_BOX  1
#define OCT_BOX  2
#define DEC_BOX  3
#define HEX_BOX  4
#define BOX_LEN  64

struct calcEntryBox;
void binary_calculator (void);
void calcStatWindow    (void);
void drawEntryBox      (WINDOW *, struct calcEntryBox *, int, int, int);
void calcDrawBoxes     (struct calcEntryBox *, struct calcEntryBox *,
                        struct calcEntryBox *, struct calcEntryBox *, int);
void calcNumberKey     (struct calcEntryBox *, struct calcEntryBox *,
                        struct calcEntryBox *, struct calcEntryBox *,
                        wchar_t, int);
void calcArrowKey      (struct calcEntryBox *, struct calcEntryBox *,
                        struct calcEntryBox *, struct calcEntryBox *,
                        wchar_t, int *);
void calcBinaryString  (struct calcEntryBox *);
void calcOctalString   (struct calcEntryBox *);
void calcDecimalString (struct calcEntryBox *);
void calcHexString     (struct calcEntryBox *);


   /* Functions in hash.c */
void         clear_hash ();
void         commit_changes_in_hash (FILE *);
void         delete_hash_entry (unsigned long);
void         init_hash (void);
void         insert_hash_entry (unsigned long, unsigned char);
int          hash_lookup (unsigned long, unsigned char *);




#ifdef MAIN_C
unsigned long offset = 0x00;     /* where in the file you are */
int position = 0;                /* where on the line you are */
int cursor_x = 10;               /* cursor tracking in hex mode */
int cursor_y = MAIN_TOP_LINE;
int acursor_x = 0;               /* cursor tracking in ascii mode */
int acursor_y = 0;
int color_term = 0;              /* is a color terminal */
unsigned char *filebuf = NULL;   /* the file in memory */
int extrabuf = EXTRA_BUF;        /* always malloc extra bytes for
                                    inserting text */
int *newlines = NULL;            /* used in ascii mode, where to start lines */

/* constant strings for printing */
const char * const NOT_ENOUGH_MEMORY = "Not enough memory";


#else
extern unsigned long offset;
extern int position;
extern int cursor_x;
extern int cursor_y;
extern int acursor_x;
extern int acursor_y;
extern int color_term;
extern unsigned char *filebuf;
extern int extrabuf;
extern int *newlines;
extern const unsigned char EBCDIC[];
extern const unsigned char ASCII_to_EBCDIC[];
extern const char * const NOT_ENOUGH_MEMORY;
#endif


   /* Global structure, keep most global variables here. */
struct
{
   WINDOW *wmain, *wstatus, *whelp; /* three windows used throughout. */
   unsigned long filesize;          /* size of the file buffer. */
   FILE *fp;                        /* File we are editing. */
   const char *filename;            /* Name of file we are editing. */
   int mode;                        /* what mode the program is in, such as
                                     * file widget, hex, ascii, calculator. */
   int modified;                    /* if the buffer is modified/read-only. */
   int tabb;                        /* are we editing hex or text side. */
   int charset;                     /* ascii or ebcdic character set. */
   int spacing;                     /* byte or word like spacing layout. */
   int print_mode;                  /* print 7-bit text, 8-bit text, or
                                     * all text. */
   unsigned int buffsize;           /* size of buffer read into memory. */
   int fullsize;                    /* is buffsize holding the full buffer. */
   int fixed_disk;                  /* file is a fixed disk. */
   unsigned long buf_front;         /* current *filebuf in memory is the.  */
   unsigned long buf_end;           /* chunk from offset buf_front through. */
                                    /* buf end. */
   int beeping;                     /* Allow beeping or not. */
   int help_msg_count;              /* Number of messages in help menu. */
} Globals;


struct foundit
   /* used for returning from searching */
{
   int flag;
   unsigned long offset;
};


struct ret_string
   /* used for returning from hexstring box */
{
   int len;
   unsigned char *str;
};


struct Change
   /* A node in the ChangeLog stack */
{
   int type;                     /* type of change (insert, delete, modify) */
   unsigned long offset;         /* location of the change */
   unsigned char old;            /* the previous overwritten value */
   unsigned char new;            /* the new value */
   struct Change *p;             /* next change in the list */
};

struct ChangeLog
   /* Stack of changes to use with undo */
{
   int s;
   struct Change *base;
   struct Change *top;
} UndoStack;


struct FileNames
   /* node for linked list of filenames */
{
   char *filename;
   struct FileNames *p;
};


struct calcEntryBox
{
   char *tokens;
   char s[BOX_LEN + 1];
   int x;
   int pos_y;
   int pos_x;
   union
   {
      int i;
      unsigned int ui;
      long l;
      unsigned long ul;
   } value;
};


#endif
