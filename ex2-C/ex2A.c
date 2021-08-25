/*
* ex21.c
*
*  Created on: Nov 14, 2017
*      Author: Eliyho Tsuri, Dan Narkisi
*       Login: eliyhots, danna
*
* The father plan establishes a single ,signal raised when they send two
* signals (signal1,signal2) and second signal SIGTERM The father begets
* a child and then each roll ten times in a loop, each one of them sleep
* 0-3 seconds a lottery random number 0,1 .if 0 sending the other process
* first signal, otherwise send the other process first signal, each sends
* output depending on the signal that is received,then each of them
* checks with the mass amount of signals received and when one of the
* masses coming to five it sends to other process signal SIGTERM and quit.
*
* Race modes :
*   when the child and the father processing then became
*   race and the winner is the process how get 5 signals.
* 	 if the child and the father the processing perceived
*
*/
// --------------------- Include Section -----------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>  // for wait()
#include <unistd.h>    // for sleep()
#include <sys/types.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
// ------------------------- Define ----------------------------
#define TEN 10
// ------------------------ Prototype --------------------------
void do_child();
void do_father();
void signal_handler(int signal);
void signal_term();
pid_t pid;     // for process
			   // -------------------------- Main -----------------------------
int main()
{
	int index = 0; // index for while
				   /* signals handler */
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	signal(SIGTERM, signal_term);


	pid = fork();
	while (index < TEN)
	{
		// if processing failed
		if (pid < 0)
		{
			perror("fork() failed");
			exit(EXIT_FAILURE);
		}
		// son function
		if (pid == 0)
			do_child();
		// dad function
		if (pid > 1)
			do_father();
		index++;
	}
	return (EXIT_SUCCESS);
}
// ----------------------- do_child ---------------------------
/*
* The child function sleep 1-3 second and lottery number
* if the number = 0 the function send to the second process
* SIGUSR1 if the number = 1 send to the second process SIGUSR2
*/
void do_child()
{
	int num;

	srand(time(NULL));
	sleep((rand() % 4));

	num = (rand() % 2);
	if (num == 0)
		kill(getppid(), SIGUSR1);
	else
		kill(getppid(), SIGUSR2);
}
// ---------------------- do_father ---------------------------
/*
* The father function sleep 1-3 second and lottery number
* if the number = 0 the function send to the second process
* SIGUSR1 if the number = 1 send to the second process SIGUSR2
*/
void do_father()
{
	int num;

	srand(time(NULL));
	sleep((rand() % 4));

	num = (rand() % 2);
	if (num == 0)
		kill(pid, SIGUSR1);
	else
		kill(pid, SIGUSR2);
}
// -------------------- signal_handler -------------------------
/*
* This signal called by father and the son, the signal check how
* is the signal on the argument and update his counter
* when some of the counter = 5 the signal called to signal term
*/
void signal_handler(int signal)
{
	static int count_sig1 = 0;
	static int count_sig2 = 0;

	printf("process %d was got signal %d\n", getpid(), signal);

	if (signal == SIGUSR1)
		count_sig1++;
	if (signal == SIGUSR2)
		count_sig2++;


	if (count_sig1 == 5 || count_sig2 == 5)
	{
		if (pid == 0)
		{
			printf("process %d surrender %d\n", getpid(), SIGUSR1);
			kill(getppid(), SIGTERM);
		}
		else
		{
			printf("process %d surrender %d\n", getpid(), SIGUSR2);
			kill(pid, SIGTERM);
		}
	}
}
// --------------------- signal_term ---------------------------
/*
* This signal print called by signal_handler the
* signal print the process winner and end the program
*/
void signal_term()
{
	printf("process %d win \n", getpid());
	exit(EXIT_SUCCESS);
}
