/*
* ex2b.c
*
*  Created on: Nov 16, 2017
*      Author: Eliyho Tsuri, Dan Narkisi
*       Login: eliyhots, danna
*
*
* On the following program there are several modes:
*
* 1) First race condition when received large number 3
*    and then father and son continue running infinite loop
*    when father handler function is called Then the function prints
*    the number of process created and declared they were partners.
*
* 2) The son lottery number if the number is large than 3 called child
*    handler and prints the statement that several new ID generated is
*    a partner of the process that created the master plan.
*
* *) For 1-2 when we received a large number of first 3 declaring that
*    he an accomplice is the process that finishes first.
*
* 3) When you receive a small number of function summoned 3 son
*    and father called father handler and child handler print
*    the id process and end the program
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
#define MAX_TIME 10
// ------------------------ Prototype --------------------------
int set_number();
void do_son();
void do_dad();
void signal_handler();
void alrm(int signal);
pid_t pid;
// -------------------------- Main -----------------------------
int main()
{
	/* signal handler */
	signal(SIGUSR1, signal_handler);   // child signal
	signal(SIGALRM, alrm);			  // alarm signal

									  // create processing
	pid = fork();

	// infinite loop
	while (true)
	{
		if (pid < 0)
		{
			perror("fork() failed");
			exit(EXIT_FAILURE);
		}
		// son function
		if (pid == 0)
			do_son();
		// dad function
		if (pid > 1)
			do_dad();
	}
	return (EXIT_SUCCESS);
}
// ----------------------- set_number --------------------------
/*
* This function rand number (1 <= num < 10) and return to main.
*/
int set_number()
{
	srand(time(NULL));
	return (rand() % MAX_TIME - 1);
}
// -------------------------- do_son ---------------------------
/*
* This function getting number from the main and 2 cases
* 1) if number < 3 the function print process (pid of process)
* and end the process, 2) starting the alarm and
* call the child_handler and to print the partner pid.
*/
void do_son()
{
	int number;
	number = set_number();

	// if number < 3 the function end the process
	if (number < 3)
	{
		printf("process %d has a ends\n", getpid());
		exit(EXIT_SUCCESS);
	}
	else
	{
		kill(getppid(), SIGUSR1); // for send process
		alarm(MAX_TIME / 2);         // alarm on
		pause();
		alarm(0);		           // alarm off
	}
}
// -------------------------- do_dad ---------------------------
/*
* This function getting number from the main and 2 cases
* 1) if number < 3 the function print process (pid of process)
* and end the process, 2) starting the alarm and
* call the child_handler and to print the partner pid.
*/
void do_dad()
{
	int number;
	number = set_number();

	if (number < 3)
	{
		printf("process %d has a ends\n", getpid());
		exit(EXIT_SUCCESS);
	}
	else
	{
		kill(pid, SIGUSR1);
		alarm(MAX_TIME / 2);
		pause();
		alarm(0);
	}
}
// --------------------- signal_handler ------------------------
/*
* This function defined on the main function, this function is
* handler signal to child handler the function print the string
* process (pid) ... partner and return.
*/
void signal_handler()
{
	printf("process %d has a partner\n", getpid());
}
// ------------------------- alrm ------------------------------
/*
* This function defined on the main function, this function is
* handler alarm to child handler and father handler, the function
* print the string process (pid) ... quits and return.
*/
void alrm(int signal)
{
	printf("process %d was left alone, and quits\n", getpid());
	exit(EXIT_SUCCESS);
}
