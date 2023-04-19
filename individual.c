#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigint_handler(int signal_num) {
	printf("Interrupt received. Exiting...\n");
   	exit(0);
}

int main() {
   	int pipefd[2];
    	pid_t child[5];
    	char message[100];
	signal(SIGINT,sigint_handler);

   	if (pipe(pipefd) == -1) {
        	perror("Failed to pipe");
        	exit(1);
    	}

    	for (int i = 0; i < 5; i++) {
        	pid_t pid = fork();
        	if (pid == -1) {
            	perror("Failed to fork");
            	exit(1);
        	} else if (pid == 0) {
            	close(pipefd[1]);
            	read(pipefd[0], message, sizeof(message));
            	printf("Child %d Message received: %s\n", i, message);
		close(pipefd[0]);
            	exit(0);
        	} else {
            	child[i] = pid;
        }
    }

    	printf("Enter a message to send to children: ");
    	fgets(message, sizeof(message), stdin);
    	close(pipefd[0]);
	for (int i = 0; i < 5; i++) {
		printf("Parent writing to child %d\n",i);
        	write(pipefd[1], message, sizeof(message));
   	}
	for(int i=0;i<5;i++){
		waitpid(child[i],NULL,0);
	}
   	close(pipefd[1]);
    	return 0;
}

