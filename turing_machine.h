#ifndef _TURING_MACHINE_
#define _TURING_MACHINE_
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

int init_from_file(const char*, state**);
int init_from_stdin(state**);
int destroy(state**);
int add_to_tm(state**, const int, const int, const int, const int, const int);
int run_step(const state*, char*, char*, int*);
int run_all(const state*, char*, char*, int*);
