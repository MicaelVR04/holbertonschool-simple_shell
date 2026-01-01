#include "shell.h"

/**
 * env_fetch - Verifies tokens.
 * @args: String of arguments given.
 * @input: User input string.
 * @count: Command counter.
 *
 * Return: -1 on exit, 0 otherwise
 */
int env_fetch(char **args, char *input, int count)
{
	char **env = environ;
	int i = 0;

	(void)input;

	if (strcmp(args[0], "exit") == 0)
		return (-1);

	if (strcmp(args[0], "env") == 0)
	{
		while (env[i])
		{
			printf("%s\n", env[i]);
			i++;
		}
		return (0);
	}

	if (find_or_execute_command(args) == -1)
	{
		printf("./hsh: %d: %s: not found\n", count, args[0]);
	}

	return (0);
}

/**
 * _getenv - Finds the value of an environment variable.
 * @name: Name of the variable.
 *
 * Return: Pointer to value string, or NULL if not found
 */
char *_getenv(const char *name)
{
	size_t length = strlen(name);
	char **env = environ;

	while (*env != NULL)
	{
		if (strncmp(*env, name, length) == 0 && (*env)[length] == '=')
			return (&(*env)[length + 1]);
		env++;
	}
	return (NULL);
}

/**
 * parse_input - Tokenizes user input into arguments.
 * @input: User input string.
 *
 * Return: Array of strings (tokens)
 */
char **parse_input(char *input)
{
	char **arguments = malloc(MAX_ARGS * sizeof(char *));
	char *token;
	int number_of_arguments = 0;

	if (!arguments)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(input, " \n");
	while (token != NULL && number_of_arguments < MAX_ARGS - 1)
	{
		arguments[number_of_arguments] = token;
		token = strtok(NULL, " \n");
		number_of_arguments++;
	}
	arguments[number_of_arguments] = NULL;

	return (arguments);
}

/**
 * execute_command - Forks and executes a command.
 * @args: Array of arguments
 *
 * Return: -1 on fork failure, 0 otherwise
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
		return (-1);

	if (pid == 0)
	{
		execve(args[0], args, environ);
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&status);
	}
	return (0);
}

/* Prototype for helper in helpers.c */
int execute_in_path(char *dir, char **args, char *path_copy);

/**
 * find_or_execute_command - Checks PATH and executes command if found.
 * @args: Command and arguments array
 *
 * Return: 0 if executed, -1 if not found
 */
int find_or_execute_command(char **args)
{
	char *path, *path_copy, *dir;

	if (access(args[0], X_OK) == 0)
		return (execute_command(args));

	path = _getenv("PATH");
	if (!path)
		return (-1);

	path_copy = strdup(path);
	if (!path_copy)
		return (-1);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		if (execute_in_path(dir, args, path_copy) == 0)
			return (0);

		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (-1);
}
