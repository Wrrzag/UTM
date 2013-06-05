#ifndef _TURING_MACHINE_
#define _TURING_MACHINE_
#define MAX_SIZE 50
#define INITIAL_STATES 1
#define INITIAL_SYMBOL_NUM 3

typedef enum {
  ALLOC_ERROR = -1,
  OK = 0,
  TRANSITION_FOUND,
  TRANSITION_NOT_FOUND,
  END_STATE
} ret_t;


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
  int *final_states;
	int state_num;
  int symbol_num;

	int max_states;
  int final_state_count;
} turing_machine;

#endif

ret_t init_from_file(const char*, turing_machine**);
ret_t init_from_stdin(turing_machine**);
ret_t destroy_tm(turing_machine**);
ret_t init_tape(tape**);
ret_t destroy_tape(tape*);

ret_t add_to_tm(turing_machine*, const int, const int, const int, const int, const int);
ret_t add_symbol(turing_machine*, const int, const char);
ret_t add_final_state(turing_machine*, const int);
ret_t run_step(const turing_machine*, int*, int*);
ret_t run_all(const turing_machine*, int*, int*);
ret_t read_tape(tape*);
void print_tape(turing_machine*, int);
