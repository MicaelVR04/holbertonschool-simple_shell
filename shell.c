#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT_SIZE 1024
#define PROMPT "#cisfun$ "

// Function to execute the command
void execute_command(char *command)
{
    pid_t pid;
    int status;

    // Fork the process to create a child
    pid = fork();
    if (pid == -1)
    {
        perror("Fork failed");
        return;
    }

    if (pid == 0)
    { // Child process
        // Use execve to run the command
        char *argv[] = {command, NULL};
        if (execve(command, argv, environ) == -1)
        {
            perror(command); // Print error if execve fails
        }
        exit(1); // Exit child process
    }
    else
    { // Parent process
        // Wait for the child to finish execution
        waitpid(pid, &status, 0);
    }
}

int main()
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        // Display the prompt
        printf("%s", PROMPT);

        // Read the user input
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
        {
            if (feof(stdin))
            {
                // Handle end of file (Ctrl+D)
                printf("\n");
                break;
            }
            perror("Input error");
            continue;
        }

        // Remove newline character from the input
        input[strcspn(input, "\n")] = 0;

        // Check for exit condition
        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        // If the input is not empty, execute the command
        if (strlen(input) > 0)
        {
            execute_command(input);
        }
    }

    return 0;
}
