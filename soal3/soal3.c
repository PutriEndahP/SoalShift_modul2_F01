#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id_satu, child_id_dua, child_id_tiga;
  int pipe_satu[2], pipe_dua[2];

  pipe(pipe_satu);
  pipe(pipe_dua);

  child_id_satu = fork();
  
  if (child_id_satu < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id_satu == 0) {
    // this is child
    
    char *zip[] = {"unzip", "/home/putri/sisop/modul2_soalshift/campur2.zip", NULL};
    execv(zip[], zip);
  } else {
 	while(wait(NULL) > 0);

  	child_id_dua = fork();

if (child_id_dua < 0) {
   exit(EXIT_FAILURE);
  }

if (child_id_dua == 0) 
{
	dup2(pipesatu[2], STDOUT_FILENO);
	close(pipesatu[2]);
	close(pipedua[2]);

	char *ls[] = {"ls", "/home/putri/sisop/modul2_soalshift/campur2", NULL};
	execvp(ls[0], ls);
}
else {
	while(wait(NULL) > 0);

	child_id_tiga == fork();

if(child_id_tiga < 0) {
   exit(EXIT_FAILURE);
}
if(child_id_tiga == 0) 
{
	
}
}
  }
}
