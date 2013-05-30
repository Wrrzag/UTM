#include "parser.h"

int init_from_file(char*, state**);
int init_from_stdin(state**);
int destroy(state**);
int add_to_tm(state**, int, int, int, int, int);
