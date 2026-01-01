#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define PROMPT "#cisfun$ "

/**
 * execute_command - executes a command using execve
 * @command: command to execute
 *
 * Return: void
 */
void execute_command(char *command)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return;
	}

	if (pid == 0)
	{
		char *argv[] = {command, NULL};

		if (execve(command, argv, environ) == -1)
			perror(command);

		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

/**
 * main - entry point for simple shell
 *
 * Return: Always 0
 */
int main(void)
{
	char input[MAX_INPUT_SIZE];

	while (1)
	{
		printf("%s", PROMPT);

		if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			perror("Input error");
			continue;
		}

		/* Remove newline character */
		input[strcspn(input, "\n")] = 0;

		if (strlen(input) > 0)
			execute_command(input);
	}

	return (0);
}
