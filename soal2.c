#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

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

  while(1) {
    // main program here
	struct  stat info_file; //nama statnya adl info_file 
	char tempat[255] = "/home/putri/sisop/modul2_soalshift/hatiku/elen.ku";
	stat(tempat, &info_file); //mendapatkan informasi dari tempat di simpan di info_file 

	struct passwd *pwd = getpwuid(info_file.st_uid); //st.uid unt mendapatkan userID owner -> man 2 stat
	struct group *grup = getgrgid(info_file.st_gid); //st.gid unt mendapatkan group ID owner

	char own[100] = "www-data"; //string www-data disimpan di variabel own

	int us = strcmp(pwd->pw_name, own); //membandingkan yg ada di pwd dgn pw_name(membuka nama pw, dan own
	int grp = strcmp(grup->gr_name, own); //membandingkan yg ada di grp dgn gr_name(nama grup) dan own

	if(us==0 && grp==0) // jika us dan gr sama maka dihapus file nya
	{
		remove(tempat);
	}
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
  return 0;
}
