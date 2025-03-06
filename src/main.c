#include "lib.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cmd_square(int argc, char **argv);
void cmd_gradient(int argc, char **argv);
void cmd_poland(int argc, char **argv);
void cmd_ireland(int argc, char **argv);

typedef struct {
  char *name;
  void (*func)(int, char **);
  char *desc;
} command_t;

command_t commands[] = {
    {"square", cmd_square, "A simple black square of 10x10"},
    {"gradient", cmd_gradient,
     "A square gradient from black in the top left to pure red in the top "
     "right and pure blue in the bottom left"},
    {"ireland_flag", cmd_ireland, "Ireland flag"},
    {"poland_flag", cmd_poland, "Poland flag"},
    {NULL, NULL, NULL},
};

void print_usage(const char *program_name) {
  printf("Usage: %s <command> [options]\n\n", program_name);
  printf("Available commands:\n");

  for (command_t *cmd = commands; cmd->name != NULL; cmd++) {
    printf("  %-10s %s\n", cmd->name, cmd->desc);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  for (command_t *cmd = commands; cmd->name != NULL; cmd++) {
    if (strcmp(argv[1], cmd->name) == 0) {
      cmd->func(argc - 1, &argv[1]);
      return 0;
    }
  }

  printf("Unknown command: %s\n", argv[1]);
  print_usage(argv[0]);
  return 0;
}

void cmd_square(int argc, char **argv) { simple_black_square(); }
void cmd_gradient(int argc, char **argv) { gradient(); }
void cmd_poland(int argc, char **argv) { poland_flag(); }
void cmd_ireland(int argc, char **argv) { ireland_flag(); }
