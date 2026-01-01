#include "shell.h"
/**
 * env_fetch - Verifies tokens.
 * @args: String of arguments given.
 * @input: User input string.
 * @count: Fixed number.
 *
 * Return: -1 on (Failure), 0 on (Success).
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
 * _getenv - Finds the environment.
 * @name: Name of the environment variable.
 *
 * Return: The value after '=' (Success), NULL (Failure).
 */
char *_getenv(const char *name)
{
	size_t length = strlen(name);
	char **env = environ;

	while (*env != NULL)
	{
		if (strncmp(*env, name, length) == 0 && (*env)[length] == '=')
			return (&(*env)[length + 1]); /* Return the value after '=' */
		env++;
	}
	return (NULL); /* Environment variable not found */
}
/**
 * parse_input - Analyzes the user input and tokenizes it.
 * @input: User input string.
 *
 * Return: Array of tokens.
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
 * execute_command - Forks process, executes a command and waits if necessary.
 * @args: Array of tokens with command and arguments.
 *
 * Return: -1 on fork (Failure), 0 on (Success).
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
/**
 * find_or_execute_command - Checks the command input for execution.
 * @args: Array of tokens with args[0] being the command.
 *
 * Return: -1 if the command is found (Success), 0 if not found (Failure).
 */
int find_or_execute_command(char **args)
{
	char *path, *path_copy, *dir, *cmd;
	size_t len;

	if (access(args[0], X_OK) == 0)
	{
		execute_command(args);
		return (0);
	}

	path = _getenv("PATH");
	if (!path)
		return (-1);

	path_copy = strdup(path);
	if (!path_copy)
		return (-1);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		len = strlen(dir) + strlen(args[0]) + 2;
		cmd = malloc(len);
		if (!cmd)
			break;

		sprintf(cmd, "%s/%s", dir, args[0]);
		if (access(cmd, X_OK) == 0)
		{
			args[0] = cmd;
			execute_command(args);
			free(path_copy);
			return (0);
		}
		free(cmd);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (-1);
}
