#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef uint32_t color_t;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef long LONG;

struct Bitmap {
  unsigned int width;
  unsigned int height;
  color_t *pixels;
};

typedef struct tagBITMAPFILEHEADER // bmfh
{
  WORD bfType;        // signature word "BM" or 0x4D42
  DWORD bfSize;       // entire size of file
  WORD bfReserved1;   // must be zero
  WORD bfReserved2;   // must be zero
  DWORD bfOffsetBits; // offset in file of DIB pixel bits
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER // bmih
{
  DWORD biSize;          // size of the structure = 40
  DWORD biWidth;         // width of the image in pixels
  DWORD biHeight;        // height of the image in pixels
  WORD biPlanes;         // = 1
  WORD biBitCount;       // bits per pixel (1, 4, 8, 16, 24, or 32)
  DWORD biCompression;   // compression code
  DWORD biSizeImage;     // number of bytes in image
  DWORD biXPelsPerMeter; // horizontal resolution
  DWORD biYPelsPerMeter; // vertical resolution
  DWORD biClrUsed;       // number of colors used
  DWORD biClrImportant;  // number of important colors
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

struct Bitmap init_bitmap(int width, int height) {
  color_t *pixels = (color_t *)malloc(width * height * sizeof(color_t));
  struct Bitmap bitmap = {.width = width, .height = height, .pixels = pixels};
  return bitmap;
}

void free_bitmap(struct Bitmap *bitmap) {
  free(bitmap->pixels);
  bitmap->pixels = NULL;
}

void print_bytes(const struct Bitmap *bitmap) {
  int height = bitmap->height;
  int width = bitmap->width;
  color_t *pixels = bitmap->pixels;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      printf("0x%02X ", pixels[i * width + j]);
    }
    printf("\n");
  }
}

void bmp_info(const struct Bitmap *bitmap) {
  puts("----- Bitmap info -----");
  int height = bitmap->height;
  int width = bitmap->width;
  printf("Dimensions: %d x %d\n", width, height);
  print_bytes(bitmap);
}

void set_pixel(struct Bitmap *bitmap, int row, int col, color_t color) {
  if (row >= bitmap->height || col >= bitmap->width) {
    return;
  }
  bitmap->pixels[row * bitmap->width + col] = color;
}

int cprint(char buffer[], int buflen, uint8_t red, uint8_t green,
           uint8_t blue) {
  return snprintf(buffer, buflen, "\033[48;2;%d;%d;%dm \033[0m", red, green,
                  blue);
}

void print_bitmap(const struct Bitmap *bitmap) {
  for (int i = 0; i < bitmap->height; i++) {
    for (int j = 0; j < bitmap->width; j++) {
      color_t pixel = bitmap->pixels[i * bitmap->width + j];
      uint8_t red = (pixel & 0xFF0000) >> 16;
      uint8_t green = (pixel & 0xFF00) >> 8;
      uint8_t blue = (pixel & 0xFF);
      char buffer[30] = {};
      cprint(buffer, 30, red, green, blue);
      printf("%s", buffer);
      printf("%s", buffer);
    }
    printf("\n");
  }
}

void read_bitmap(struct Bitmap *bitmap, const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Error opening file: %s", filename);
    return;
  }
  WORD bfType, bfReserved1, bfReserved2, biPlanes, biBitCount;
  DWORD bfSize, bfOffBits, biSize, biWidth, biHeight, biCompression,
      biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant;
  fread(&bfType, 1, 2, file);
  fread(&bfSize, 1, 4, file);
  fread(&bfReserved1, 1, 2, file);
  fread(&bfReserved2, 1, 2, file);
  fread(&bfOffBits, 1, 4, file);
  fread(&biSize, 1, 4, file);
  fread(&biWidth, 1, 4, file);
  fread(&biHeight, 1, 4, file);
  fread(&biPlanes, 1, 2, file);
  fread(&biBitCount, 1, 2, file);
  fread(&biCompression, 1, 4, file);
  fread(&biSizeImage, 1, 4, file);
  fread(&biXPelsPerMeter, 1, 4, file);
  fread(&biYPelsPerMeter, 1, 4, file);
  fread(&biClrUsed, 1, 4, file);
  fread(&biClrImportant, 1, 4, file);

  assert(bfType == 0x4D42);
  assert(biXPelsPerMeter == 0x0);
  assert(biYPelsPerMeter == 0x0);
  assert(biClrUsed == 0x0);
  assert(biClrImportant == 0x0);
  assert(biBitCount == 0x18);

  bitmap->width = biWidth;
  bitmap->height = biHeight;
  bitmap->pixels =
      (color_t *)malloc(bitmap->width * bitmap->height * sizeof(color_t));
  long row_len = 4 * ((biWidth * biBitCount + 31) / 32);
  long padding_bytes = row_len - (biWidth * biBitCount) / 8;

  for (int i = bitmap->height - 1; i >= 0; i--) {
    for (int j = 0; j < bitmap->width; j++) {
      fread(&bitmap->pixels[i * bitmap->width + j], 1, 3, file);
    }
    fseek(file, padding_bytes, SEEK_CUR);
  }
  fclose(file);
}

uint calc_row_length(uint width) {
    // Doesn't cancel because of integer division
    return ((3 * width + 3) / 4) * 4;
}

unsigned int calc_img_size(int width, int height) {
  return calc_row_length(width) * height;
}

void write_file_header(int width, int height, FILE *file) {
  uint32_t bfSize = calc_img_size(width, height) + 14 + 40;
  uint16_t bfReserved1 = 0;
  uint16_t bfReserved2 = 0;
  uint32_t bfOffBits = 14 + 40;
  fputs("BM", file);
  fwrite(&bfSize, 4, 1, file);
  fwrite(&bfReserved1, 2, 1, file);
  fwrite(&bfReserved2, 2, 1, file);
  fwrite(&bfOffBits, 4, 1, file);
}

void write_info_header(int width, int height, FILE *file) {
  uint32_t biSize = 0x28;
  uint16_t biPlanes = 0x1;
  uint16_t biBitCount = 0x18;
  uint32_t biCompression = 0x0;
  uint32_t biSizeImage = calc_img_size(width, height);
  uint32_t biXPelsPerMeter = 0;
  uint32_t biYPelsPerMeter = 0;
  uint32_t biClrUsed = 0;
  uint32_t biClrImportant = 0;
  fwrite(&biSize, 4, 1, file);
  fwrite(&width, 4, 1, file);
  fwrite(&height, 4, 1, file);
  fwrite(&biPlanes, 2, 1, file);
  fwrite(&biBitCount, 2, 1, file);
  fwrite(&biCompression, 4, 1, file);
  fwrite(&biSizeImage, 4, 1, file);
  fwrite(&biXPelsPerMeter, 4, 1, file);
  fwrite(&biYPelsPerMeter, 4, 1, file);
  fwrite(&biClrUsed, 4, 1, file);
  fwrite(&biClrImportant, 4, 1, file);
}

void write_pixel_data(struct Bitmap bitmap, FILE *file) {
  int padding = calc_row_length(bitmap.width) - 3 * bitmap.width;
  for (int i = bitmap.height - 1; i >= 0; i--) {
    for (int j = 0; j < bitmap.width; j++) {
      color_t color = bitmap.pixels[i * bitmap.width + j];
      uint8_t red = (color & 0xFF0000) >> 16;
      uint8_t green = (color & 0xFF00) >> 8;
      uint8_t blue = (color & 0xFF);
      fwrite(&blue, 1, 1, file);
      fwrite(&green, 1, 1, file);
      fwrite(&red, 1, 1, file);
    }
    for (int k = 0; k < padding; k++) {
        fputc(0, file);
    }
  }
}

void write_bitmap(struct Bitmap bitmap, const char *filename) {
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    fprintf(stderr, "Error opening file: %s", filename);
    return;
  }
  write_file_header(bitmap.width, bitmap.height, file);
  write_info_header(bitmap.width, bitmap.height, file);
  write_pixel_data(bitmap, file);
  fclose(file);
}

color_t get_color(int red, int green, int blue) {
  return (red << 16) + (green << 8) + blue;
}

void fill(struct Bitmap *bitmap, size_t x1, size_t y1, size_t x2, size_t y2,
          color_t color) {
  if (x1 > x2) {
    size_t tmp = x1;
    x1 = x2;
    x2 = tmp;
  }
  if (y1 > y2) {
    size_t tmp = y1;
    y1 = y2;
    y2 = tmp;
  }
  for (size_t i = 0; i < x2 - x1; i++) {
    for (size_t j = 0; j < y2 - y1; j++) {
      set_pixel(bitmap, x1 + i, y1 + j, color);
    }
  }
}

void rotate_right_90(struct Bitmap *bitmap) {
  const int AREA = bitmap->width * bitmap->height;
  color_t tmp[AREA] = {};
  for (int i = 0; i < bitmap->height; i++) {
    for (int j = 0; j < bitmap->width; j++) {
      tmp[j * bitmap->height + (bitmap->height - 1 - i)] =
          bitmap->pixels[i * bitmap->width + j];
    }
  }
  memcpy(bitmap->pixels, tmp, AREA * sizeof(color_t));
  int width = bitmap->width;
  bitmap->width = bitmap->height;
  bitmap->height = width;
}

void monochrome(const struct Bitmap *bitmap, struct Bitmap *red_channel,
                struct Bitmap *green_channel, struct Bitmap *blue_channel) {
  *red_channel = init_bitmap(bitmap->width, bitmap->height);
  *green_channel = init_bitmap(bitmap->width, bitmap->height);
  *blue_channel = init_bitmap(bitmap->width, bitmap->height);
  for (int i = 0; i < bitmap->height; i++) {
    for (int j = 0; j < bitmap->width; j++) {
      color_t color = bitmap->pixels[i * bitmap->width + j];
      red_channel->pixels[i * bitmap->width + j] = color & 0xFF0000;
      green_channel->pixels[i * bitmap->width + j] = color & 0xFF00;
      blue_channel->pixels[i * bitmap->width + j] = color & 0xFF;
    }
  }
}

struct Bitmap test_img(size_t width, size_t height) {
  struct Bitmap bitmap = init_bitmap(width, height);
  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      set_pixel(&bitmap, i, j, i * width + j);
    }
  }
  return bitmap;
}

struct Bitmap poland_flag() {
  int width = 30;
  int height = 20;
  struct Bitmap bitmap = init_bitmap(width, height);
  fill(&bitmap, 0, 0, height / 2, width, 0xFFFFFF);
  fill(&bitmap, height / 2, 0, height, width, 0xDC143C);
  return bitmap;
}

struct Bitmap ireland_flag() {
  int width = 30;
  int height = 20;
  struct Bitmap bitmap = init_bitmap(width, height);
  fill(&bitmap, 0, 0, height, width / 3, 0x169B62);
  fill(&bitmap, 0, width / 3, height, 2 * width / 3, 0xFFFFFF);
  fill(&bitmap, 0, 2 * width / 3, height, 3 * width / 3, 0xFF883E);
  return bitmap;
}

struct Bitmap vietnam_flag() {
  int width = 30;
  int height = 20;
  struct Bitmap bitmap = init_bitmap(width, height);
  for (int i = 0; i < height / 2; i++) {
    for (int j = 0; j < width; j++) {
      set_pixel(&bitmap, i, j, 0xFFFFFF);
    }
  }
  for (int i = height / 2; i < height; i++) {
    for (int j = 0; j < width; j++) {
      color_t poland_red = get_color(0xDC, 0x14, 0x3C);
      set_pixel(&bitmap, i, j, poland_red);
    }
  }
  return bitmap;
}

struct Bitmap gradient() {
  struct Bitmap bitmap = init_bitmap(51, 51);
  for (int i = 0; i < bitmap.height; i++) {
    for (int j = 0; j < bitmap.width; j++) {
      color_t color = get_color(j * 5, 0, i * 5);
      set_pixel(&bitmap, i, j, color);
    }
  }
  return bitmap;
}

struct Bitmap simple_black_square() {
  struct Bitmap bitmap = init_bitmap(10, 10);
  for (int i = 0; i < bitmap.height; i++) {
    for (int j = 0; j < bitmap.width; j++) {
      set_pixel(&bitmap, i, j, 0);
    }
  }
  return bitmap;
}
