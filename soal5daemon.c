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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
     //ambil waktu program sekarang
        time_t waktuwaktu;
        time(&waktuwaktu);

        struct tm *waktu;
        waktu = localtime(&waktuwaktu); //mengkonversi raw time di time_t biar jadi kepisah pisah hari, jam, bulan
        //1900 = 0 makanya + 1900 
        char fungsi_memisah[150]; //tm_sec dll didapat dari struct tm 
        sprintf(fungsi_memisah, "%d:%d:%d-%d:%d", waktu->tm_mday, waktu->tm_mon+1, waktu->tm_year+1900, waktu->tm_hour, waktu->tm_min); // untuk misah2 waktu disimpan di fungsi_memisah, cari struct tm
        //sprintf : nge print yang di print kemudian hasil print nya di simpan di yg kiri
        printf("%s", fungsi_memisah);
	char putri[150];
	sprintf(putri, "/home/putri/sisop/modul2_soalshift/%s", fungsi_memisah); //letak dimana mkdir ini akan dibuat
        mkdir(putri, 0777); //777 biar bisa akses semua (rwx)

        int incr;
        for(incr=1; incr<30; incr++)
        {
                FILE *file_syslog; //membuat file baru untuk syslog
                FILE *file_ygdibuat; //membuat file baru yang akan dibuat
                file_syslog = fopen("/var/log/syslog", "r");
                char nama_filebaru[255];
                sprintf(nama_filebaru, "/home/putri/sisop/modul2_soalshift/%s/log%d.log", fungsi_memisah, incr); //hasil print nya disimpan di nama_filebaru
                file_ygdibuat = fopen(nama_filebaru, "w");
		
		//pindah seluruh file ke log#.log
                char karakter; 
                while(fscanf(file_syslog, "%c", &karakter) != EOF)
                {
                        fprintf(file_ygdibuat, "%c", karakter);
                }
                fclose(file_syslog);
                fclose(file_ygdibuat);
                sleep(60);
        }

  }
  
  exit(EXIT_SUCCESS);
}
