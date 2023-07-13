#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 20

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf ("Usage: <program> <input.bmp> <output.bmp>\n");
        return EXIT_FAILURE;
    }

    char file_in[MAX_CHARS] = { '\0' };
    char file_out[MAX_CHARS] = { '\0' };

    strcpy_s (file_in, MAX_CHARS, argv[1]);
    strcpy_s (file_out, MAX_CHARS, argv[2]);

    printf("%s - %s\n", file_in, file_out);

    return EXIT_SUCCESS;
}