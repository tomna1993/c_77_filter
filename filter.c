#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 50

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
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;

int8_t ReadBmpHeader(
    const char file_in[MAX_CHARS], 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header);

void PrintBmpHeader( 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header);

int8_t ReadPixelArray(
    const char file_in[MAX_CHARS], 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array);

int8_t CreateBmp(
    const char file_out[MAX_CHARS], 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array);

void FilterGrayscale( 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array);

void FilterSepia( 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array);

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

  PrintBmpHeader(&bmp_file_header, &bmp_info_header);

  int32_t height_in_pixel = bmp_info_header.height_in_pixel;
  
  if (height_in_pixel < 0) height_in_pixel *= -1;

  Pixel *pixel_array = (Pixel *)calloc(
      bmp_info_header.width_in_pixel * height_in_pixel,
      sizeof(Pixel));
  
  is_error = ReadPixelArray(file_in, &bmp_file_header, &bmp_info_header, pixel_array);

  if (is_error == EXIT_FAILURE) return EXIT_FAILURE;

  // FilterGrayscale(&bmp_file_header, &bmp_info_header, pixel_array);

  FilterSepia(&bmp_file_header, &bmp_info_header, pixel_array);

  is_error = CreateBmp(file_out, &bmp_file_header, &bmp_info_header, pixel_array);

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

  fread(&bmp_file_header->bmp_size,
        sizeof(bmp_file_header->bmp_size), 1, fp);
  
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

void PrintBmpHeader( 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header) 
{
  printf("Header field: %x\n", bmp_file_header->header_field);
  printf("File size: %i\n", bmp_file_header->bmp_size);
  printf("Bitmap data address: %x\n", bmp_file_header->bitmap_data_address);

  printf("Header size: %i\n", bmp_info_header->header_size);
  printf("Width in pixel: %i\n", bmp_info_header->width_in_pixel);
  printf("Height in pixel: %i\n", bmp_info_header->height_in_pixel);
  printf("Color planes: %i\n", bmp_info_header->color_planes);
  printf("Bits per pixel: %i\n", bmp_info_header->bits_per_pixel);
  printf("Compression method: %i\n", bmp_info_header->compression_method);
  printf("Image size: %i\n", bmp_info_header->image_size);
  printf("Horizontal resolution: %i\n", bmp_info_header->horizontal_resolution);
  printf("Vertical resolution: %i\n", bmp_info_header->vertical_resolution);
  printf("Colors in palette: %i\n", bmp_info_header->colors_in_palette);
  printf("Important colors: %i\n", bmp_info_header->important_colors);
}

int8_t ReadPixelArray(
    const char file_in[MAX_CHARS], 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array) 
{
  FILE *fp = fopen(file_in, "rb");

  if (fp == NULL)
  {
    printf("Failed to open file!\n");
    return EXIT_FAILURE;
  }

  fseek(fp, bmp_file_header->bitmap_data_address, SEEK_SET);

  int32_t padding_bytes = (bmp_info_header->width_in_pixel * sizeof(Pixel)) % 4;

  if (padding_bytes > 0)
  {
    padding_bytes = 4 - padding_bytes;
  }

  int32_t height_in_pixel = bmp_info_header->height_in_pixel;
  
  if (height_in_pixel < 0) height_in_pixel *= -1;

  for (int row = 0; row < height_in_pixel; ++row)
  {
    if (row > 0) fseek(fp, padding_bytes, SEEK_CUR);

    for (int coll = 0; coll < bmp_info_header->width_in_pixel; ++coll)
    {
      fread(pixel_array + (row * bmp_info_header->width_in_pixel) + coll,
            sizeof(Pixel), 1, fp);
    }
  }

  fclose(fp);

  return EXIT_SUCCESS;  
}

int8_t CreateBmp(
    const char file_out[MAX_CHARS], 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array) 
{
  FILE *fp = fopen(file_out, "wb");

  if (fp == NULL)
  {
    printf("Failed to open file!\n");
    return EXIT_FAILURE;
  }

  fwrite(&bmp_file_header->header_field,
        sizeof(bmp_file_header->header_field), 1, fp);

  fwrite(&bmp_file_header->bmp_size,
        sizeof(bmp_file_header->bmp_size), 1, fp);
  
  fwrite(&bmp_file_header->reserved,
        sizeof(bmp_file_header->reserved), 1, fp);

  fwrite(&bmp_file_header->bitmap_data_address,
        sizeof(bmp_file_header->bitmap_data_address), 1, fp);

  fwrite(&bmp_info_header->header_size,
        sizeof(bmp_info_header->header_size), 1, fp);

  fwrite(&bmp_info_header->width_in_pixel,
        sizeof(bmp_info_header->width_in_pixel), 1, fp);

  fwrite(&bmp_info_header->height_in_pixel,
        sizeof(bmp_info_header->height_in_pixel), 1, fp);

  fwrite(&bmp_info_header->color_planes,
        sizeof(bmp_info_header->color_planes), 1, fp);

  fwrite(&bmp_info_header->bits_per_pixel,
        sizeof(bmp_info_header->bits_per_pixel), 1, fp);

  fwrite(&bmp_info_header->compression_method,
        sizeof(bmp_info_header->compression_method), 1, fp);

  fwrite(&bmp_info_header->image_size,
        sizeof(bmp_info_header->image_size), 1, fp);

  fwrite(&bmp_info_header->horizontal_resolution, 
        sizeof(bmp_info_header->horizontal_resolution), 1, fp);

  fwrite(&bmp_info_header->vertical_resolution, 
        sizeof(bmp_info_header->vertical_resolution), 1, fp);

  fwrite(&bmp_info_header->colors_in_palette, 
        sizeof(bmp_info_header->colors_in_palette), 1, fp);

  fwrite(&bmp_info_header->important_colors, 
        sizeof(bmp_info_header->important_colors), 1, fp);

  int32_t padding_bytes = (bmp_info_header->width_in_pixel * sizeof(Pixel)) % 4;

  if (padding_bytes > 0) padding_bytes = 4 - padding_bytes;

  int8_t padding_value = 0;

  fseek(fp, bmp_file_header->bitmap_data_address, SEEK_SET);

  int32_t height_in_pixel = bmp_info_header->height_in_pixel;
  
  if (height_in_pixel < 0) height_in_pixel *= -1;

  for (int row = 0; row < height_in_pixel; ++row)
  {
    if (row > 0) fwrite(&padding_value, sizeof(int8_t), padding_bytes, fp);

    for (int coll = 0; coll < bmp_info_header->width_in_pixel; ++coll)
    {
      fwrite(pixel_array + (row * bmp_info_header->width_in_pixel) + coll,
            sizeof(Pixel), 1, fp);
    }
  }
    
  fwrite(&padding_value, sizeof(int8_t), padding_bytes, fp);

  fclose(fp);

  return EXIT_SUCCESS;  
}

void FilterGrayscale( 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array)
{
  Pixel *pixel_ptr = NULL;

  uint8_t average = 0;

  int32_t height_in_pixel = bmp_info_header->height_in_pixel;
  
  if (height_in_pixel < 0) height_in_pixel *= -1;

  for (int row = 0; row < height_in_pixel; ++row)
  {
    for (int coll = 0; coll < bmp_info_header->width_in_pixel; ++coll)
    {
      pixel_ptr = (pixel_array + (row * bmp_info_header->width_in_pixel) + coll);

      average = 0;
      average = (pixel_ptr->blue + pixel_ptr->green + pixel_ptr->red) / (int8_t)sizeof(Pixel);

      pixel_ptr->blue = pixel_ptr->green = pixel_ptr->red = average;
    }
  }

  pixel_ptr = NULL;
}

void FilterSepia( 
    BmpFileHeader *bmp_file_header, 
    BmpInfoHeader *bmp_info_header,
    Pixel *pixel_array)
{
  Pixel *pixel_ptr = NULL;

  uint16_t sepia_blue;
  uint16_t sepia_green;
  uint16_t sepia_red;

  int32_t height_in_pixel = bmp_info_header->height_in_pixel;
  
  if (height_in_pixel < 0) height_in_pixel *= -1;

  for (int row = 0; row < height_in_pixel; ++row)
  {
    for (int coll = 0; coll < bmp_info_header->width_in_pixel; ++coll)
    {
      pixel_ptr = (pixel_array + (row * bmp_info_header->width_in_pixel) + coll);

      sepia_blue = (uint16_t)((pixel_ptr->red * 0.272) +
                              (pixel_ptr->green * 0.534) +
                              (pixel_ptr->blue * 0.131));
      
      sepia_green = (uint16_t)((pixel_ptr->red * 0.349) +
                              (pixel_ptr->green * 0.686) +
                              (pixel_ptr->blue * 0.168));
      
      sepia_red = (uint16_t)((pixel_ptr->red * 0.393) +
                            (pixel_ptr->green * 0.769) +
                            (pixel_ptr->blue * 0.189));

      if (sepia_blue > 0xFF) sepia_blue = 0xFF;
      if (sepia_green > 0xFF) sepia_green = 0xFF;
      if (sepia_red > 0xFF) sepia_red = 0xFF;

      pixel_ptr->blue = sepia_blue;
      pixel_ptr->green = sepia_green;
      pixel_ptr->red = sepia_red;
    }
  }

  pixel_ptr = NULL;
}
