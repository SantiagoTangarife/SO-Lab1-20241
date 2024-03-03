#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define INITIAL_CAPACITY 8

/* this solution was so beautiful but wont work if the input file doesnt have a new line at the end*/
void reverseLines(char ***lines, int count, FILE *output) {
    for (int i = count - 1; i >= 0; i--) {
        fprintf(output, "%s", (*lines)[i]);
        free((*lines)[i]); // Free each line after printing
    }
    free(*lines); // Free the array of lines
}


int main(int argc, char *argv[]) {
    FILE *input = stdin, *output = stdout;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char **lines = NULL;
    int capacity = INITIAL_CAPACITY, count = 0;
    struct stat input_stat, output_stat;

    // Parse command-line arguments
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    } else if (argc == 3) {
        if (stat(argv[1], &input_stat) == -1) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        // Attempt to stat the output file; not fatal if it fails since "w" mode will create the file
        if (stat(argv[2], &output_stat) != -1) {
            // Check if input and output files are the same
            if (input_stat.st_ino == output_stat.st_ino) {
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            }
        }

        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "reverse: cannot open input file '%s'\n", argv[1]);
            exit(1);
        }
        output = fopen(argv[2], "w");
        if (!output) {
            fprintf(stderr, "reverse: cannot create or open output file '%s'\n", argv[2]);
            exit(1); // It's crucial to exit here to avoid trying to write to a NULL file pointer.
        }


    } else if (argc == 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Initialize dynamic array for lines
    lines = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
    if (!lines) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Read lines into dynamic array
    while ((read = getline(&line, &len, input)) != -1) {
        if (count >= capacity) {
            capacity *= 2;
            lines = (char **)realloc(lines, capacity * sizeof(char *));
            if (!lines) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        lines[count++] = strdup(line);
    }

    // Write reversed lines to output
    reverseLines(&lines, count, output);

    // Cleanup
    free(line); // Free the getline buffer
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}
