#include "shell.h"

/**
 * execute_in_path - Try to execute command in one PATH directory
 * @dir: Directory to check
 * @args: Command arguments
 * @path_copy: Duplicated PATH string
 *
 * Return: 0 if executed, 1 to continue loop
 */
int execute_in_path(char *dir, char **args, char *path_copy)
{
	char *cmd;
	size_t len;

	len = strlen(dir) + strlen(args[0]) + 2;
	cmd = malloc(len);
	if (!cmd)
		return (1);

	sprintf(cmd, "%s/%s", dir, args[0]);

	if (access(cmd, X_OK) == 0)
	{
		args[0] = cmd;
		execute_command(args);
		free(cmd);
		free(path_copy);
		return (0);
	}

	free(cmd);
	return (1);
}
