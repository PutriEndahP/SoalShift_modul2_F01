#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  int urutan_file=1;
  FILE *fp;
char file[256];
strcpy(file,"/home/putri/Documents/makanan/makan_enak.txt");
  while(1) {
        struct stat makan_enak;
        stat(file,&makan_enak);
        if(difftime(time(NULL),makan_enak.st_atime) <= 30)
        {
                char nfile[256];
                strcpy(nfile,"/home/putri/Documents/makanan/makan_sehat");
                char c[256];
                sprintf(c, "%d", urutan_file);
                strcat(nfile, c);
                strcat(nfile, ".txt");
                fp = fopen(nfile, "w");
                fclose(fp);
                urutan_file++;
        }
    sleep(5);
  }
  
  exit(EXIT_SUCCESS);
}


