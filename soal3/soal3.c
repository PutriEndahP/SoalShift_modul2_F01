#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
  pid_t child_id_satu, child_id_dua, child_id_tiga;
  int pipe_satu[2];
  int pipe_dua[2];

  pipe(pipe_satu);
  pipe(pipe_dua);

 // proses 1 -> unzip
  child_id_satu = fork();

  if (child_id_satu < 0)
    exit(EXIT_FAILURE);

  if (child_id_satu == 0) {

    char *argv[5] = {"unzip", "/home/putri/sisop/modul2_soalshift/campur2.zip", "-d", "/home/putri/sisop/modul2_soalshift/", NULL};
    execv("/usr/bin/unzip", argv);
  }
else {

        while(wait(NULL) > 0); // karena harus menunggu proses diatasnya selesai
 // proses 2 -> ls
  child_id_dua = fork();

if (child_id_dua < 0)
   exit(EXIT_FAILURE);

if (child_id_dua == 0)
{
        dup2(pipe_satu[1], STDOUT_FILENO); //agar output dari ls tdk keluar di luar, maka di masukkan ke pipe yang write (1) kalo read (0) 
        close(pipe_satu[1]);
        close(pipe_satu[0]);

        char *argv[4] = {"ls", "/home/putri/sisop/modul2_soalshift/campur2", NULL};
        execvp("/bin/ls", argv);
}
else {

        while(wait(NULL) > 0);
 // proses 3 -> grep
 child_id_tiga = fork();

if(child_id_tiga < 0)
   exit(EXIT_FAILURE);

if(child_id_tiga == 0)
{
        dup2(pipe_satu[0], STDIN_FILENO); //inputnya grep kan didapat dari proses sebelumnya, nah disimpan di pipesatu yang read(0)
        dup2(pipe_dua[1], STDOUT_FILENO); //outputnya grep disiman di pipekedua yang write(1)
        close(pipe_dua[0]);
        close(pipe_dua[1]);
        close(pipe_satu[0]);
        close(pipe_satu[1]);

        char *argv[4] = {"grep", ".txt$", NULL};
        execvp("/bin/grep", argv);
}
else {
        //while(wait(NULL) > 0);

        char jawaban[255];
        close(pipe_dua[1]);
        close(pipe_satu[0]);
        close(pipe_satu[1]);
        read(pipe_dua[0], jawaban, sizeof(jawaban));

        FILE *filenya = fopen("/home/putri/sisop/modul2_soalshift/daftar.txt", "w");
        fputs(jawaban, filenya);
        fclose(filenya);
      }
}
  }
        return 0;
}
