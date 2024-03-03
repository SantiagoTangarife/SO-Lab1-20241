#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char *filename = "tests-out/test_write_access.txt";

    file = fopen(filename, "w"); // Open the file for writing
    if (file == NULL) {
        perror("Error opening file");
        return 1; // Return an error code if file opening fails
    }

    fprintf(file, "Testing write access to 'tests-out' directory.\n");
    fclose(file); // Close the file

    printf("File '%s' written successfully.\n", filename);
    return 0; // Success
}
