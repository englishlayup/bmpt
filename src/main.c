#include "lib.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc == 2) {
    struct Bitmap bitmap;
    read_bitmap(&bitmap, argv[1]);
    print_bitmap(bitmap);
    free_bitmap(&bitmap);
    return 0;
  }

  int opt;
  puts("Select an option: ");
  puts("1. A simple black square of 10x10");
  puts("2. A square gradient from black in the top left to pure red in the top "
       "right and pure blue in the bottom left.");
  printf("Selection: ");
  scanf("%d", &opt);

  switch (opt) {
  case 1: {
    struct Bitmap bitmap;
    bitmap = simple_black_square();
    print_bitmap(bitmap);
    free_bitmap(&bitmap);
    break;
  }
  case 2: {
    struct Bitmap bitmap;
    bitmap = gradient();
    print_bitmap(bitmap);
    free_bitmap(&bitmap);
    break;
  }
  case 3: {
    struct Bitmap bitmap;
    bitmap = test_img();
    bmp_info(bitmap);
    rotate_right_90(&bitmap);
    bmp_info(bitmap);
    free_bitmap(&bitmap);
    break;
  }
  default:
    fputs("Invalid selection.", stderr);
    exit(1);
  };

  return 0;
}
