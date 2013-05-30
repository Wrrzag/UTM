#ifndef _PARSER_
#define _PARSER_
#include <stdio.h>
typedef enum {
  LEFT,
  NONE,
  RIGHT
} movement_t;

typedef struct _transition {
  int to;
  movement_t movement;
  char to_write;
} transition;

typedef struct _state {
  int last_added;
  transition *transitions;
} state;
#endif

int parse_file(FILE*, state**);
