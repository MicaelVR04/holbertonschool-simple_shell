#include "shell.h"

int last_status = 0;

/**
	* env_fetch - Handle built-in commands or execute external
	* @args: Tokenized input
	* @input: Raw input line
	* @count: Command counter for errors
	*
	* Return: -1 if shell should exit, 0 otherwise
	*/
int env_fetch(char **args, char *input, int count)
{
	char **env = environ;
	int i = 0;

	(void)input;

	if (strcmp(args[0], "exit") == 0)
	{
		return (-1);
	}

	if (strcmp(args[0], "env") == 0)
	{
		while (env[i])
		{
			printf("%s\n", env[i]);
			i++;
		}
		return (0);
	}

	/* Execute external command if found in PATH or absolute/relative */
	if (find_or_execute_command(args) == -1)
	{
		fprintf(stderr, "./hsh: %d: %s: not found\n", count, args[0]);
		last_status = 127;
	}

	return (0);
}

/**
	* _getenv - Get value of environment variable
	* @name: Name of the environment variable
	*
	* Return: Pointer to value string, or NULL if not found
	*/
char *_getenv(const char *name)
{
	size_t len = strlen(name);
	char **env = environ;

	while (*env)
	{
		if (strncmp(*env, name, len) == 0 && (*env)[len] == '=')
			return (*env + len + 1);
		env++;
	}

	return (NULL);
}

/**
	* parse_input - Tokenize user input into arguments
	* @input: User input string
	*
	* Return: Array of string tokens
	*/
char **parse_input(char *input)
{
	char **args = malloc(MAX_ARGS * sizeof(char *));
	char *token;
	int i = 0;

	if (!args)
		exit(EXIT_FAILURE);

	token = strtok(input, " \n");
	while (token && i < MAX_ARGS - 1)
	{
		args[i++] = token;
		token = strtok(NULL, " \n");
	}
	args[i] = NULL;

	return (args);
}

/**
	* execute_command - Fork and execute a command
	* @args: Array of command arguments
	*
	* Return: -1 if fork fails, 0 otherwise
	*/
int execute_command(char **args)
{
	pid_t pid = fork();
	int status;

	if (pid == -1)
		return (-1);

	if (pid == 0)
	{
		execve(args[0], args, environ);
		exit(EXIT_FAILURE);
	}

	if (wait(&status) == -1)
		return (-1);

	if (WIFEXITED(status))
		last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		last_status = 128 + WTERMSIG(status);
	else
		last_status = status;

	return (0);
}

/**
	* find_or_execute_command - Execute command if it exists
	* @args: Tokenized input
	*
	* Return: 0 if executed, -1 if not found
	*/
int find_or_execute_command(char **args)
{
	char *path, *copy, *dir, *cmd;
	size_t len;

	/* Absolute or relative path: only treat as path if it contains '/' */
	if (strchr(args[0], '/') && access(args[0], X_OK) == 0)
		return (execute_command(args));

	path = _getenv("PATH");
	if (!path || path[0] == '\0')
		return (-1); /* Do NOT fork if PATH empty */

	copy = strdup(path);
	if (!copy)
		return (-1);

	for (dir = strtok(copy, ":"); dir; dir = strtok(NULL, ":"))
	{
		len = strlen(dir) + strlen(args[0]) + 2;
		cmd = malloc(len);
		if (!cmd)
			continue;

		sprintf(cmd, "%s/%s", dir, args[0]);
		if (access(cmd, X_OK) == 0)
		{
			args[0] = cmd;
			execute_command(args);
			free(cmd);
			free(copy);
			return (0);
		}
		free(cmd);
	}

	free(copy);
	return (-1);
}
