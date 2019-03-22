#include <sys/types.h>
	#include <sys/stat.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <unistd.h>
	#include <syslog.h>
	#include <string.h>
	#include <dirent.h>

	int main() {
		pid_t pid, sid;
		pid = fork();

		if (pid < 0){
			exit(EXIT_FAILURE);
		}

		if (pid > 0){
			exit(EXIT_SUCCESS);
		}

		umask(0);

		sid = setsid();

		if (sid < 0) {
			exit(EXIT_FAILURE);
		}

		if ((chdir("/")) < 0) {
			exit(EXIT_FAILURE);
		}

		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		while(1) {
		DIR *direktory;
		struct dirent *file;
		direktory = opendir("/home/gardanwm/Documents/gambarpng");
		if(direktory!=NULL){
		while(file = readdir(direktory)){

			if(strstr(&file->d_name[strlen(file->d_name)-5], ".png")){
			 char namafile[300]="",
				cur[300]="/home/gardanwm/Documents/gambarpng/",
				trg[300]="/home/gardanwm/Documents/gambar/";
			 strncpy(namafile, file->d_name, strlen(file->d_name)-4 );
			 strcat(namafile, "_grey.png");
			 rename(strcat(cur, file->d_name), strcat(trg, namafile));
		      }
		}
		closedir(direktory);
		      }
			sleep(10);
		}
		exit(EXIT_SUCCESS);

	}

