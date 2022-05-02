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

// How shells start processes
// Starting processes is the main function of shells
// fork() system call - child process (duplicate of parent)
// exec() system call - replaces currently running program
// wait() system call
// fork() => exec()

int lsh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // child process
        // review function of these 2 lines of code 
        // execvp is a variant of exec() - expects program name and a vector
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
        // check for error in fork()
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    }  else {
        // parent process
        do {
            // review function of this do while 
            wpid = waitpid(pid, &status, WUNCTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}


// Built-in shell commands
// commands can only change shell's operation if implemented within shell process itself
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

// List of builtin commands and their corresponding functions.
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Builtin function implementations
int lsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
        if (chdir(args[1]) != 0) {
        perror("lsh");
    }
  }
  return 1;
}

int lsh_help(char **args) {
    int i;
    printf("Stephen Brennan's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    
    for (i = 0; i < lsh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }
    
    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lsh_exit(char **args) {
    return 0;
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