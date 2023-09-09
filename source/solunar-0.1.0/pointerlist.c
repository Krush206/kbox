#include <stdio.h>
#include <malloc.h>
#include "pointerlist.h"
#include "defs.h"

/*=======================================================================
PointerList_get_last
=======================================================================*/
PointerList *PointerList_get_last (PointerList *self)
  {
  if (!self) return NULL;
  PointerList *s = self;
  while (s->next)
    {
    s = s->next;
    }
  return s;
  }


/*=======================================================================
PointerList_append
=======================================================================*/
PointerList *PointerList_append (PointerList *self, void *pointer)
  {
  PointerList *newp = (PointerList *) malloc (sizeof (PointerList));
  newp->pointer = pointer;
  newp->next = NULL;

  if (self == NULL)
    {
    self = newp; 
    }
  else
    {
    PointerList *end = PointerList_get_last (self);
    end->next = newp;
    }

  return self;
  }


/*=======================================================================
PointerList_get_length
=======================================================================*/
int PointerList_get_length (const PointerList *self)
  {
  int l = 0;
  while (self)
    {
    l++;
    self = self->next;
    }
  return l;
  }

/*=======================================================================
PointerList_get_pointer
=======================================================================*/
void *PointerList_get_pointer (PointerList *self, int index)
  {
  int l = 0;
  while (self)
    {
    if (index == l) 
      {
      return self->pointer;
      }
    l++;
    self = self->next;
    }
  fprintf (stderr, 
    "PointerList index %d out of range in get_pointer()\n", index);
  return NULL;
  }

/*=======================================================================
PointerList_get_const_pointer
=======================================================================*/
const void *PointerList_get_const_pointer (const PointerList *self, int index)
  {
  return (const PointerList *) PointerList_get_pointer 
    ((PointerList *)self, index);
  }


/*=======================================================================
PointerList_concatenate
=======================================================================*/
PointerList *PointerList_concatenate (PointerList *in, PointerList *add)
  {
  int i, l = PointerList_get_length (add);
  for (i = 0; i < l; i++)
    in = PointerList_append (in, PointerList_get_pointer (add, i)); 
  return in;
  }

/*=======================================================================
PointerList_free
=======================================================================*/
void PointerList_free (PointerList *self, BOOL free_contents)
  {
  while (self)
    {
    if (free_contents)
      {
      if (self->pointer) free (self->pointer);
      }
    PointerList *next = self->next;
    free (self);
    self = next;
    }
  }

