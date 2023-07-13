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

    char fileIn[MAX_CHARS] = { '\0' };
    char fileOut[MAX_CHARS] = { '\0' };

    strcpy_s (fileIn, MAX_CHARS, argv[1]);
    strcpy_s (fileOut, MAX_CHARS, argv[2]);

    printf("%s - %s\n", fileIn, fileOut);

    FILE *fp = fopen (fileIn, "rb");

    if (fp == NULL)
    {
        printf ("Failed to open file!\n");
        return EXIT_FAILURE;
    }

    fclose (fp);

    return EXIT_SUCCESS;
}