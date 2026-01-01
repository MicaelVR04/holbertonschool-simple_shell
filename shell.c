#include "shell.h"
/**
 * main - Entry point, command line interpreter
 *
 * Return: Always 0 on success
 */
int main(void)
{
	char **args;
	char *input = NULL;
	size_t buffer_size = 0;
	int count = 0, mode = isatty(STDIN_FILENO), comparator;

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
