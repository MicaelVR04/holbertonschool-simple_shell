#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 10

char *_getenv(const char *name);
char **parse_input(char *input);
int execute_command(char **args);
int find_or_execute_command(char **args);
int env_fetch(char **args, char *input, int count);
extern char **environ;

#endif /* SHELL_H */
