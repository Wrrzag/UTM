/** turing_machine.h
*
* Contains the definitions of the methods that can be used
* with a TM and the structures, enums and defines for it.
*
* Author: Marc Piñol <marcpinolpueyo@gmail.com>
* Version: 0.20
* License: GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
*
*/

#ifndef _TURING_MACHINE_
#define _TURING_MACHINE_
#define TAPE_INITIAL_SIZE 1
#define INITIAL_STATES 1
#define INITIAL_SYMBOL_NUM 3

typedef enum 
{
  ALLOC_ERROR = -1,
  OK = 0,
  TRANSITION_FOUND,
  TRANSITION_NOT_FOUND,
  END_STATE
} ret_t;


typedef enum 
{
  LEFT = -1,
  NONE,
  RIGHT
} movement_t;

typedef struct _transition 
{
  unsigned long to;
  char in_tape;
	char to_write;
  movement_t movement;
} transition;

typedef struct _state 
{
  unsigned long next_state_to_add;
  transition *transitions;
} state;

typedef struct _tape 
{
	char *elements;
	unsigned long size;
} tape;

typedef struct _turing_machine 
{
	state *states;
	tape *tape;
  char *symbols;
  unsigned long *final_states;
	unsigned long state_num;
  unsigned long symbol_num;

	unsigned long max_states;
  unsigned long final_state_count;
} turing_machine;

#endif

/** Initializes the TM getting its characteristics from a file.
* Allocates the needed memory of the TM structures and fills it
* with the files' data.
*
* Params: 
*		The file name where the TM's encoding is
*		The Turing Machine to initialize
*
*	Returns:
*		ALLOC_ERROR if there has been an error when allocating memory
* 	OK when the process is done
*
*/
ret_t init_from_file(const char*, turing_machine**);

/** Initializes the TM getting its characteristics from user input.
* Allocates the needed memory of the TM structures and fills it
* with the input's data.
*
* Params: 
*		The Turing Machine to initialize
*
*	Returns:
*		ALLOC_ERROR if there has been an error when allocating memory
* 	OK when the process is done
*
*/
ret_t init_from_stdin(turing_machine**);

/** Destroys the Turing Machine and frees its allocated memory.
* Frees all the inner structures of the TM and sets the pointers
* to null.
*
* Params: 
*		The Turing Machine to initialize
* 
* Returns: 
*		OK after the process is complete
*
*/
ret_t destroy_tm(turing_machine**);


/** Adds a transition to the Turring Machine.
* It manages the allocated memory to make sure there is enough space to
* add states (if needed) and the transition. 
*
* Params:
*		A pointer to the Turing Machine to modify
*		The state from which the transition goes
*		The index of the symbols data structure where the readed symbol is stored
*		The state where the transition goes
*		The index of the symbols array where the symbol to write on the tape 
*			is stored
*		The movement of the tape head
*
*	Returns:
* 	ALLOC_ERROR if there has been an error when allocating memory
* 	OK when the process is done
*
*/
ret_t add_to_tm(turing_machine*, const unsigned long, const unsigned long, const unsigned long, const unsigned long, const unsigned long);

/** Adds a symbol to the symbols data structure. The three first
* ones must be the equivalent to '0', '1' and 'blank'.
* It manages the allocated memory to make sure there is enough room for
* the symbol to be stored.
*
* Params:
*		A pointer to the Turing Machine to modify
*		The index of the data structure to store the symbol into
*		The symbol to be stored
*
*	Returns:
* 	ALLOC_ERROR if there has been an error when allocating memory
* 	OK when the process is done
*
*/
ret_t add_symbol(turing_machine*, const unsigned long, const char);

/** Adds a final state to be used by the TM. There can be no final
* states.
* It manages the allocated memory to make sure there is enough room for
* the state to be stored.
*
* Params:
*		A pointer to the Turing Machine to modify
*		The number of the state to add
*
*	Returns:
* 	ALLOC_ERROR if there has been an error when allocating memory
* 	OK when the process is done
*
*/
ret_t add_final_state(turing_machine*, const unsigned long);


/** Runs a step (makes a transition) of the TM, reading and writing on
* the tape. It manages the tape memory to make sure that what is 
* going to be written on it fits in it.
* It also moves the current state to the one that the transition
* is going to and moves the head of the tape to the position
* specified in the transition.
*
* Params:
*		A pointer to the Turing Machine to modify
*		A pointer to the position of the head of the tape
*		A pointer to the number of the current state
*
*	Returns:
* 	ALLOC_ERROR if there has been an error when allocating memory
* 	END_STATE if it has reached a final state and there are no more
*			valid transitions
*		TRANSITION_FOUND if the state has transitions and something has changed
*		TRANSITION_NOT_FOUND if the state has no more transitions and nothing
*			has changed
*
*/
ret_t run_step(const turing_machine*, unsigned long*, unsigned long*);

/** Runs the TM until the end (if there is any), reading and writing on
* the tape. It manages the tape memory to make sure that what is 
* going to be written on it fits in it.
* It also moves the current state to the one that the TM ended into
* and moves the head of the tape to the last position pointed to.
*
* Params:
*		A pointer to the Turing Machine to modify
*		A pointer to the position of the head of the tape
*		A pointer to the number of the current state
*
*	Returns:
* 	ALLOC_ERROR if there has been an error when allocating memory
* 	END_STATE if it has reached a final state and there are no more
*			valid transitions
*		TRANSITION_FOUND if the state has transitions and something has changed
*		TRANSITION_NOT_FOUND if the state has no more transitions and nothing
*			has changed
*
*/
ret_t run_all(const turing_machine*, unsigned long*, unsigned long*);


/** Reads the contents of the tape from the user. 
* It manages the tape memory to make sure the input fits in the allocated
* memory.
*
*	Params:
*		A pointer to the Turing Machine where the tape is stored
*
*	Returns:
* 	ALLOC_ERROR if there has been an error when allocating memory
* 	OK when the process is done
*		
*/
ret_t read_tape(turing_machine*);

/** Prints the TM's tape in the current state. It also shows the current
*	position the head is pointing to.
*
*	Params:
*		A pointer to the Turing Machine where the tape is stored
*
*/
void print_tape(turing_machine*, unsigned long);


/** Tells if the TM has any final state.
*
*	Params:
*		A pointer to the Turing Machine to know whether there are final
*	states or not
*
*	Returns:
*		1 if there are final states
*		0 if there is not any final state
*
*/
int has_final_states(turing_machine*);
