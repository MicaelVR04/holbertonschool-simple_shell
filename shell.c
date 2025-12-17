#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

/**
 * main - simple UNIX command line interpreter
 *
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    char *argv[2];

    while (1)
    {
        /* Display prompt */
        write(STDOUT_FILENO, "#cisfun$ ", 9);

        /* Read input */
        nread = getline(&line, &len, stdin);
        if (nread == -1) /* Ctrl+D (EOF) */
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* Remove newline */
        line[nread - 1] = '\0';

        /* Prepare arguments (one command, no args) */
        argv[0] = line;
        argv[1] = NULL;

        /* Create child process */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            /* Child: execute command */
            if (execve(argv[0], argv, environ) == -1)
            {
                perror("./shell");
                exit(1);
            }
        }
        else
        {
            /* Parent: wait for child */
            wait(NULL);
        }
    }

    free(line);
    return (0);
}
