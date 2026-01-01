#include "shell.h"

/**
 * run_shell - Core loop of the shell
 * @mode: 1 if interactive, 0 if not
 *
 * Return: Always 0
 */
int run_shell(int mode)
{
	char **args;
	char *input = NULL;
	size_t buffer_size = 0;
	int count = 0;
	int comparator;

	while (1)
	{
		if (mode)
			printf("#cisfun$ ");

		if (getline(&input, &buffer_size, stdin) == -1)
			break;

		count++;

		if (input[strlen(input) - 1] == '\n')
			input[strlen(input) - 1] = '\0';

		args = parse_input(input);
		if (!args || !args[0])
		{
			free(args);
			continue;
		}

		comparator = env_fetch(args, input, count);
		free(args);

		if (comparator == -1)
		{
			free(input);
			exit(0);
		}
	}

	free(input);
	return (0);
}

/**
 * main - Entry point
 *
 * Return: Always 0
 */
int main(void)
{
	int mode = isatty(STDIN_FILENO);

	run_shell(mode);

	if (mode)
		printf("\n");

	return (0);
}
