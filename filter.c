#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 20

#define BYTES_IN_PIXEL 3

typedef struct BmpFileHeader
{
  int16_t header_field;
  int32_t bmp_size;
  int32_t reserved;
  int32_t bitmap_data_address;
} BmpFileHeader;

typedef struct BmpInfoHeader
{
  int32_t header_size;
  int32_t width_in_pixel;
  int32_t height_in_pixel;
  int16_t color_planes;
  int16_t bits_per_pixel;
  int32_t compression_method;
  int32_t image_size;
  int32_t horizontal_resolution;
  int32_t vertical_resolution;
  int32_t colors_in_palette;
  int32_t important_colors;
} BmpInfoHeader;

typedef struct Pixel
{
    int8_t blue;
    int8_t green;
    int8_t red;
} Pixel;

int8_t ReadBmpHeader(
    const char file_in[MAX_CHARS], 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header);

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Usage: <program> <input.bmp> <output.bmp>\n");
    return EXIT_FAILURE;
  }

  char file_in[MAX_CHARS] = { '\0' };
  char file_out[MAX_CHARS] = { '\0' };

  strcpy_s(file_in, MAX_CHARS, argv[1]);
  strcpy_s(file_out, MAX_CHARS, argv[2]);

  BmpFileHeader bmp_file_header = { 0 };
  BmpInfoHeader bmp_info_header = { 0 };

  int8_t is_error = ReadBmpHeader(file_in, &bmp_file_header, &bmp_info_header);

  if (is_error == EXIT_FAILURE) return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

int8_t ReadBmpHeader(
    const char file_in[MAX_CHARS], 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header) 
{
  FILE *fp = fopen(file_in, "rb");

  if (fp == NULL)
  {
    printf("Failed to open file!\n");
    return EXIT_FAILURE;
  }

  fread(&bmp_file_header->header_field,
        sizeof(bmp_file_header->header_field), 1, fp);

  if (bmp_file_header->header_field != 0x4D42)
  {
    printf("File is not a BMP image!\n");
    return EXIT_FAILURE;
  }

  fread(&bmp_file_header->header_field,
        sizeof(bmp_file_header->header_field), 1, fp);

  fread(&bmp_file_header->bmp_size, sizeof(bmp_file_header->bmp_size), 1, fp);
  
  fread(&bmp_file_header->reserved,
        sizeof(bmp_file_header->reserved), 1, fp);

  fread(&bmp_file_header->bitmap_data_address,
        sizeof(bmp_file_header->bitmap_data_address), 1, fp);

  fread(&bmp_info_header->header_size,
        sizeof(bmp_info_header->header_size), 1, fp);

  fread(&bmp_info_header->width_in_pixel,
        sizeof(bmp_info_header->width_in_pixel), 1, fp);

  fread(&bmp_info_header->height_in_pixel,
        sizeof(bmp_info_header->height_in_pixel), 1, fp);

  fread(&bmp_info_header->color_planes,
        sizeof(bmp_info_header->color_planes), 1, fp);

  fread(&bmp_info_header->bits_per_pixel,
        sizeof(bmp_info_header->bits_per_pixel), 1, fp);

  fread(&bmp_info_header->compression_method,
        sizeof(bmp_info_header->compression_method), 1, fp);

  fread(&bmp_info_header->image_size,
        sizeof(bmp_info_header->image_size), 1, fp);

  fread(&bmp_info_header->horizontal_resolution, 
        sizeof(bmp_info_header->horizontal_resolution), 1, fp);

  fread(&bmp_info_header->vertical_resolution, 
        sizeof(bmp_info_header->vertical_resolution), 1, fp);

  fread(&bmp_info_header->colors_in_palette, 
        sizeof(bmp_info_header->colors_in_palette), 1, fp);

  fread(&bmp_info_header->important_colors, 
        sizeof(bmp_info_header->important_colors), 1, fp);

  fclose(fp);

  return EXIT_SUCCESS;  
}