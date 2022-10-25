#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void handle_signal(int signum){
	printf("received signal %s\n",strsignal(signum));
	signal(signum,SIG_DFL);
	raise(signum);
}

int main(int argc, char **argv){
	signal(SIGCONT,handle_signal);
	signal(SIGINT,handle_signal);
	signal(SIGTSTP,handle_signal);
	while(1) {
		sleep(2);
	}

	return 0;
}