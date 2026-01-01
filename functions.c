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
	unsigned int i = 0;

	if (strcmp(args[0], "exit") == 0)
	{
		free(input);
		return (-1);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		while (env[i] != NULL)
		{
			printf("%s\n", env[i]);
			i++;
		}
	}
	else if (find_or_execute_command(args) == -1)
		printf("./hsh: %d: %s: not found\n", count, args[0]);

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
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0) /* Child Process */
	{
		if (execve(args[0], args, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else /* Parent Process */
	{
		int status;

		if (wait(&status) == -1)
		{
			perror("wait");
			return (-1);
		}
		if (WIFEXITED(status)) /* Checks if child terminated normally */
		{
			return (WIFEXITED(status)); /* Return child process exit status */
		}
		else
		{
			return (-1);
		}
	}
	return (-1);
}
/**
 * find_or_execute_command - Checks the command input for execution.
 * @args: Array of tokens with args[0] being the command.
 *
 * Return: -1 if the command is found (Success), 0 if not found (Failure).
 */
int find_or_execute_command(char **args)
{
	/* Checks if the command is a full path */
	if (access(args[0], X_OK) == 0)
	{
		return (execute_command(args));
	}
	else
	{
		/* Checks if the command exists in PATH */
		char *path = _getenv("PATH");
		char *path_copy = strdup(path);
		char *dir = strtok(path_copy, ":");

		while (dir != NULL)
		{
			char *command_path = malloc(strlen(dir) + strlen(args[0]) + 2);

			sprintf(command_path, "%s/%s", dir, args[0]);
			if (access(command_path, X_OK) == 0)
			{
				args[0] = command_path; /* Updates the command with the full path */
				execute_command(args);
				free(command_path);
				free(path_copy);
				return (0); /* Command was found and executed successfully */
			}
			free(command_path);
			dir = strtok(NULL, ":");
		}
		free(path_copy);
		return (-1); /* The command was not found in PATH*/
	}
	return (0); /* The command was found and executed successfully */
}
