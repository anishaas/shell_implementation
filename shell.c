#include <stdio.h>
// Error with include - "cannot open source file "sys.wait.h"
# include <sys/wait.h>

// READ: #define directive in C
// READ: macros in C
# define LSH_RL_BUFSIZE 1024
char *lsh_read_line(void) {
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    // READ: malloc
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character and store it as int - because EOF is an int
        // Take in a character of user input
        c = getchar();

        // Check for EOF, replace it with null character and return
        if (c == EOF || c == '\n') {
            // QUESTION: does \0 represent a null character?
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;
        
        // If exceeded buffer, reallocate
        if (position >= bufsize) {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprint(stderr, "lsh: allocation error\n");
                exit(EXIT_FEATURE);
            }
        }
    }

}


void lsh_loop(void){
    // steps to handle commands - read input, parse program/arguments, execute command
    // READ: C variable declaration - *, **, none
    char *line;
    char **args;
    int status;

    // READ: How to read user input in C?
    do {
        // print a prompt
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

    // READ: keyword 'free' in C, deallocating memory
        free(line);
        free(args);
    } while(status);
}

int main(int argc, char **argv) {
    // load config files

    // run command loop to read in commands
    lsh_loop();

    // shutdown/cleanup
    
    return EXIT_SUCCESS;
}