#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

int main()
{
	int total_length = 10;
	char line[total_length];
	FILE * command = popen("pidof soal5daemon","r");

	fgets(line,total_length,command);

	pid_t pid = strtoul(line,NULL,10);
	kill(pid,SIGKILL);
	pclose(command);
}
