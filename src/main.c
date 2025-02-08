#include "lib.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  struct Bitmap bitmap;
  int cont = 1;
  int opt;
  if (argc == 2) {
    read_bitmap(&bitmap, argv[1]);
    print_bitmap(&bitmap);
  } else {
    puts("Select an option: ");
    puts("1. A simple black square of 10x10");
    puts("2. A square gradient from black in the top left to pure red in the "
         "top "
         "right and pure blue in the bottom left");
    puts("3. Poland flag");
    puts("4. Ireland flag");
    printf("Selection: ");
    scanf("%d", &opt);
    switch (opt) {
    case 1:
      bitmap = simple_black_square();
      print_bitmap(&bitmap);
      break;
    case 2:
      bitmap = gradient();
      print_bitmap(&bitmap);
      break;
    case 3:
      bitmap = poland_flag();
      print_bitmap(&bitmap);
      break;
    case 4:
      bitmap = ireland_flag();
      print_bitmap(&bitmap);
      break;
    default:
      fputs("Invalid selection.", stderr);
      exit(1);
    };
  }

  while (cont) {
    puts("Select an option: ");
    puts("1. Save image to file");
    puts("2. Print pixels as binary data");
    puts("3. Rotate image right 90 degrees");
    puts("4. Monochrome bitmaps");
    puts("Otherwise, exit program");
    printf("Selection: ");
    scanf("%d", &opt);
    switch (opt) {
    case 1: {
      char filename[1024];
      printf("Enter filename: ");
      scanf("%s", filename);
      write_bitmap(bitmap, filename);
      break;
    }
    case 2:
      bmp_info(&bitmap);
      break;
    case 3:
      rotate_right_90(&bitmap);
      print_bitmap(&bitmap);
      break;
    case 4: {
      struct Bitmap red, green, blue;
      monochrome(&bitmap, &red, &green, &blue);
      puts("");
      print_bitmap(&red);
      puts("");
      print_bitmap(&green);
      puts("");
      print_bitmap(&blue);
      puts("");
      break;
    }
    default:
      cont = 0;
    }
  }

  free_bitmap(&bitmap);

  return 0;
}
