#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 20

typedef struct BMPFILEHEADER
{
    __int16 header_field;

    __int32 BMP_size;

    __int32 reserved;

    __int32 bitmap_data_address;
}
BMPFILEHEADER;

typedef struct BMPINFOHEADER
{
    __int32 header_size;

    __int32 width;

    __int32 height;

    __int16 color_planes;

    __int16 bits_per_pixel;

    __int32 compression_method;

    __int32 image_size;

    __int32 h_res;

    __int32 v_res;

    __int32 colors_in_palette;

    __int32 important_colors;
}
BMPINFOHEADER;

typedef struct PIXEL
{
    __int8 blue;
    __int8 green;
    __int8 red;
}
PIXEL;



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


    BMPFILEHEADER bmp_file_header = { 0 };
    BMPINFOHEADER bmp_info_header = { 0 };


    FILE *fp = fopen (fileIn, "rb");

    if (fp == NULL)
    {
        printf ("Failed to open file!\n");
        return EXIT_FAILURE;
    }

    fclose (fp);

    return EXIT_SUCCESS;
}