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

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK__DELIM " \t\r\n\a"
char **lsh_split_line(char *line) {
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
      fprintf(stdrr,"lsh: allocation error\n");
      exit(EXIT_FAILURE);
  }

  //tokenization - tokenize the string using whitespace as delimiters
  // use strtok function
  while (token != NULL) {
      tokens[position] = token;
      position++;

      if (position >= bufsize) {
          bufsize += LSH_TOK_BUFSIZE;
          tokens = realloc(tokens, bufsize * sizeof(char*));
          if(!tokens) {
              fprintf(stderr, "lsh: allocation error\n")
              exit(EXIT_FAILURE);
          }
      }

      token = strtok(NULL, LSH_TOK_DELIM);
    }
    // * What is the function of these lines?
    tokens[position] = NULL;
    return tokens;
  }

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int lsh_launch(char **args) {
    pid_t pid, wpid;
    int status;
    
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    } else {
    // Parent process
    do {
        wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
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