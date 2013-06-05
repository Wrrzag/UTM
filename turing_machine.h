#ifndef _TURING_MACHINE_
#define _TURING_MACHINE_
#define MAX_SIZE 50
#define INITIAL_STATES 1
#define INITIAL_SYMBOL_NUM 3

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
  int next_state_to_add;
  transition *transitions;
} state;

typedef struct _tape {
	char *elements;
	int size;
} tape;

typedef struct _turing_machine {
	state *states;
	tape *tape;
  char *symbols;
	int state_num;
	int max_states;
  int symbol_num;
} turing_machine;

#endif

int init_from_file(const char*, turing_machine**);
int init_from_stdin(turing_machine**);
int destroy_tm(turing_machine**);
int init_tape(tape**);
int destroy_tape(tape*);

int add_to_tm(turing_machine*, const int, const int, const int, const int, const int);
int add_symbol(turing_machine*, const int, const char);
int run_step(const turing_machine*, int*, int*);
int run_all(const turing_machine*, int*, int*);
int read_tape(tape*);
void print_tape(turing_machine*, int);
