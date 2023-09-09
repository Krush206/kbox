#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

// We will be called as /data/data/kevinboone.androidterm/lib/libkbox_shell.so

int main (int argc, char **argv)
  {
  char self[512];
  char root[512];
  char lib[512];
  char bin[512];
  char home[512];
  char user_home[512];
  char kbox[512];
  char kbox_bin[512];
  char cmd[512];
  char bb_source[512];
  char init_script[512];
  char bb_target[512];
  char *env[10];
  char *user_id;
  int i = 0;
  int uid = 0;

  int l = readlink ("/proc/self/exe", self, sizeof (self) - 1); 
  self[l] = 0;
  strcpy (lib, self);
  char *p = strrchr (lib, '/');
  if (p == NULL)
    strcpy (bin, ""); 
  else
    {
    *p = 0;
    }

  // lib = /data/data/kevinboone.androidterm/lib

  strcpy (root, lib);

  p = strrchr (root, '/');
  if (p == NULL)
    strcpy (root, ""); 
  else
    {
    *p = 0;
    }

  // root = /data/data/kevinboone.androidterm

  strcpy (kbox, root);
  strcat (kbox, "/");
  strcat (kbox, "kbox");
  mkdir (kbox, 0755);

  // kbox = /data/data/kevinboone.androidterm/kbox
  
  strcpy (kbox_bin, kbox);
  strcat (kbox_bin, "/");
  strcat (kbox_bin, "bin");
  mkdir (kbox_bin, 0755);

  // kbox_bin = /data/data/kevinboone.androidterm/kbox/bin
  
  strcpy (bb_target, kbox_bin);
  strcat (bb_target, "/");
  strcat (bb_target, "busybox");
   
  // bb_target = /data/data/kevinboone.androidterm/kbox/bin/busybox

  strcpy (bb_source, lib);
  strcat (bb_source, "/");
  strcat (bb_source, "libbusybox.so");
   
  // bb_source = /data/data/kevinboone.androidterm/lib/libbusybox.so

  strcpy (init_script, lib);
  strcat (init_script, "/");
  strcat (init_script, "libinitscript.so");
   
  // init_script = /data/data/kevinboone.androidterm/lib/libbusybox.so

  int f = open (bb_target, O_RDONLY);
  if (f > 0)
    {
    close (f);
    }
  else
    {
    printf ("kbox_shell: creating busybox for first run\n");

    sprintf (cmd, "cat %s > %s", bb_source, bb_target);
    system (cmd);

    sprintf (cmd, "chmod 755 %s", bb_target);
    system (cmd);
    }

  strcat (home, "/sdcard");
  strcat (home, "/");
  strcat (home, "kbox_home");
  mkdir (home, 0755);

  uid = getuid ();
  struct passwd *pw = getpwuid (uid);
  if (pw->pw_name) user_id = pw->pw_name; else user_id = "user";

  sprintf (user_home, "%s/%s", home, user_id);
  mkdir (user_home, 0755);

  char ss[1024];
  i = 0;
  sprintf (ss, "PATH=%s:/data/local/bin:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin:/system/bin:/bin:/usr/bin", bin);
  env[i] = strdup (ss);
  i++;
  sprintf (ss, "HOME=%s", user_home);
  env[i] = strdup (ss);
  i++;
  sprintf (ss, "USER=%s", user_id);
  env[i] = strdup (ss);
  i++;
  sprintf (ss, "USERNAME=%s", user_id);
  env[i] = strdup (ss);
  i++;
  sprintf (ss, "LOGNAME=%s", user_id);
  env[i] = strdup (ss);
  i++;
  sprintf (ss, "LD_LIBRARY_PATH=%s/lib:%s/usr/lib", kbox, kbox);
  env[i] = strdup (ss);
  i++;
  sprintf (ss, "KBOX=%s", kbox);
  env[i] = strdup (ss);
  i++;
  env[i] = 0;
 
  chdir (user_home);
  return execle (bb_target, bb_target, "ash", "-c", init_script,  NULL, env);
  }


