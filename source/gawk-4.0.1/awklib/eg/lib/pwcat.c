/*
 * pwcat.c
 *
 * Generate a printable version of the password database
 */
/*
 * Arnold Robbins, arnold@skeeve.com, May 1993
 * Public Domain
 * December 2010, move to ANSI C definition for main().
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <pwd.h>

#if defined (STDC_HEADERS)
#include <stdlib.h>
#endif

int
main(int argc, char **argv)
{
#ifdef FRIG
    struct passwd *p;

    while ((p = getpwent()) != NULL)
#ifdef ZOS_USS
        printf("%s:%ld:%ld:%s:%s\n",
            p->pw_name, (long) p->pw_uid,
            (long) p->pw_gid, p->pw_dir, p->pw_shell);
#else
        printf("%s:%s:%ld:%ld:%s:%s:%s\n",
            p->pw_name, p->pw_passwd, (long) p->pw_uid,
            (long) p->pw_gid, "Android user", p->pw_dir, p->pw_shell);
            //(long) p->pw_gid, p->pw_gecos, p->pw_dir, p->pw_shell);
#endif

    endpwent();
#endif
    return 0;
}
