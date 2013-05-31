#ifndef _TURING_MACHINE_
#define _TURING_MACHINE_
#define MAX_SIZE 50
#define INITIAL_STATES 5

typedef enum {
  LEFT = -1,
  NONE,
  RIGHT
} movement_t;

typedef struct _transition {
  int to;
  char in_tape;
	char to_write;
  movement_t movement;
} transition;

typedef struct _state {
  int last_added;
  transition *transitions;
} state;

typedef struct _tape {
	char *elements;
	int size;
} tape;
#endif

int init_from_file(const char*, state**);
int init_from_stdin(state**);
int destroy_tm(state**);
int init_tape(tape**);
int destroy_tape(tape*);

int add_to_tm(state**, const int, const char, const int, const char, const int);
int run_step(const state*, tape*, int*, int*);
int run_all(const state*, tape*, int*, int*);
int read_tape(tape*);
void print_tape(tape, int);
