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
#include <cstring>
#include <iostream>

using namespace std;

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

    // main program here

	int urutan_file = 1;
	while(1)
	{
		struct stat makan_enak;
		stat("/home/putri/Documents/makanan/makan_enak.txt",&makan_enak);
//		cout << "diff " << difftime(time(NULL),makan_enak.st_atime) << endl; 

		//membandingkan waktu program berjalan dgn last time program
		//st.atime untuk waktu program terakhir dibuka(last time)
		if (difftime(time(NULL),makan_enak.st_atime) <= 30)//time null untuk waktu program saat itu berjalan
		{
//			cout << "-----------" << urutan_file << endl;
			string makan_sehat = "/home/putri/Documents/makanan/makan_sehat";
			makan_sehat = makan_sehat + to_string(urutan_file) + ".txt";

			//c_str digunakan meng konversi string ke array of char
			//awalnya kan dibaca berupa pointer terus dibaca isi pointernya
			//char sehat[100]={*(makan_sehat.c_str())};

			//membuat file makansehat agar bisa dilihat
			FILE *makansehat = fopen((char*)makan_sehat.c_str(),"w");
			//FILE *makansehat = fopen(sehat,"w");
			fclose(makansehat);
			urutan_file++;
		}
	sleep(5);
	}
 
  exit(EXIT_SUCCESS);
}
