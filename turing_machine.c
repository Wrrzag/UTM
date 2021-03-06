/** turing_machine.c
*
* The TM methods implementation.
*	Here are implemented all the TM methods used to create, destroy
* and modify the TM.
*
* Author: Marc Piñol <marcpinolpueyo@gmail.com>
* Version: 0.20
* License: GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
*
*/

#include "turing_machine.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private declarations */
ret_t init_tape(tape**);
ret_t destroy_tape(turing_machine**);
ret_t destroy_states(turing_machine**);
ret_t destroy_symbols(turing_machine**);
ret_t destroy_final_states(turing_machine**);

ret_t parse_tm_file(const char*, turing_machine*);
ret_t alloc_and_init(turing_machine**);
ret_t manage_state_memory(turing_machine*, const unsigned long, const unsigned long);
ret_t manage_transitions_memory(turing_machine*, const unsigned long);

/* Function implementation */
ret_t 
init_from_file(const char* file_name, turing_machine **tm)
{
	if(alloc_and_init(tm) < 0)
  {
    return ALLOC_ERROR;
  }

	if(parse_tm_file(file_name, *tm) < 0) 
	{
		return ALLOC_ERROR;
	}

	return OK;
}

ret_t 
init_from_stdin(turing_machine **tm)
{
  if(alloc_and_init(tm) < 0)
  {
    return ALLOC_ERROR;
  }

  if(parse_file(stdin, *tm) < 0)
  {
    return ALLOC_ERROR;
  }

	return OK;
}

ret_t 
init_tape(tape **tape)
{
	if((*tape = malloc(sizeof(tape))) == NULL)
	{
		return ALLOC_ERROR;
	}
  if(((*tape)->elements = malloc(sizeof(char) * TAPE_INITIAL_SIZE)) == NULL)
  {
    return ALLOC_ERROR;
  }

  (*tape)->size = TAPE_INITIAL_SIZE;

	return OK;
}

ret_t 
destroy_tm(turing_machine **tm) 
{
  destroy_states(tm);
  destroy_symbols(tm);
  destroy_final_states(tm);
  destroy_tape(tm);

  return OK;
}

ret_t 
destroy_states(turing_machine **tm)
{
  free((*tm)->states->transitions);
  (*tm)->states->transitions = NULL;

  free((*tm)->states);
  (*tm)->states = NULL;

  return OK;
}

ret_t 
destroy_symbols(turing_machine **tm)
{
  free((*tm)->symbols);
  (*tm)->symbols = NULL;

  return OK;
}

ret_t 
destroy_final_states(turing_machine **tm)
{
  free((*tm)->final_states);
  (*tm)->final_states = NULL;

  return OK;
}

ret_t 
destroy_tape(turing_machine **tm) 
{
  free((*tm)->tape->elements);
  (*tm)->tape->elements = NULL;

  free((*tm)->tape);
  (*tm)->tape = NULL;

	return OK;
}

ret_t 
add_symbol(turing_machine *tm, const unsigned long sym_index, const char symbol)
{
  if(sym_index+1 > tm->symbol_num)
  {
    char *tmp_sym_ptr = NULL;
    if(( tmp_sym_ptr = malloc( sizeof(char)*(++tm->symbol_num) )) == NULL || 
        (tm->symbols = memcpy(tmp_sym_ptr, 
                              tm->symbols, 
                              sizeof(char)*( tm->symbol_num-1 )
                              )) == NULL)
    {
      return ALLOC_ERROR;
    }
  }

  tm->symbols[sym_index] = symbol;

  return OK;
}

ret_t 
add_final_state(turing_machine *tm, const unsigned long state)
{
  unsigned long *tmp_state_ptr = NULL;
  if(( tmp_state_ptr = malloc( sizeof(unsigned long)*(1+tm->final_state_count) )) == NULL)
  {
    return ALLOC_ERROR;
  }
  if(tm->final_state_count > 0)
  {
    if((tm->final_states = memcpy(tmp_state_ptr, 
                              tm->final_states, 
                              sizeof(unsigned long)*( tm->final_state_count )
                              )) == NULL)
    {
      return ALLOC_ERROR;
    }
  }
  else
  {
    tm->final_states = tmp_state_ptr;
  }

  tm->final_states[tm->final_state_count++] = state;

  return OK;
}

ret_t 
add_to_tm(turing_machine *tm, const unsigned long from, const unsigned long trigger, const unsigned long to, const unsigned long to_write, const unsigned long mv_c)
{
  if(manage_state_memory(tm, from, to) < 0 ) /* There might be more states than memory allocated for them */
  {
    return ALLOC_ERROR;
  }

  if(manage_transitions_memory(tm, from) < 0) /* There might be more transitions than memory allocated for them */
  {
    return ALLOC_ERROR;
  }

  unsigned long *last = &(tm->states[from].next_state_to_add); /* It's tedious to use the complete name all the time */
  tm->states[from].transitions[*last].to = to;
  tm->states[from].transitions[*last].in_tape = tm->symbols[trigger];
  tm->states[from].transitions[*last].to_write = tm->symbols[to_write];
  tm->states[from].transitions[*last].movement = mv_c-1;
  (*last)++;

	return OK;
}

ret_t 
run_step(const turing_machine *tm, unsigned long *pos, unsigned long *q)
{
  int found = 0; /* Marks if the state is not found to stop the TM */
  unsigned long i;
 
  for(i = 0; tm->states[*q].transitions != NULL && i <= tm->states[*q].next_state_to_add && !found; i++) /* Check all transitions. Those uninitialized are states without transitions so we must avoid them to avoid segfault (it would try it for the first trans) */
  {  
    if(tm->tape->elements[*pos] == tm->states[*q].transitions[i].in_tape) /* What is in the tape is in a transition */
    {
			if(tm->tape->size <= (*pos)+1) /* The tape could be too small */
			{
				char *elems = NULL;
        if( (elems = malloc(++(tm->tape->size))) == NULL || (tm->tape->elements = memcpy(elems, tm->tape->elements, tm->tape->size)) == NULL )
        {
          return -1;
        }

				tm->tape->elements[(*pos)+1] = tm->symbols[2];
			}

      tm->tape->elements[*pos] = tm->states[*q].transitions[i].to_write; /* Write to tape */
      if((*pos += tm->states[*q].transitions[i].movement) < 0) (*pos)++; /* Move tape position and always put it to 0 if goes under it */
      *q = tm->states[*q].transitions[i].to; /* Change state */ 
      found = 1; 
    }
  }
  
  if(!found && tm->final_state_count > 0)
  {
    for(i = 0; i < tm->final_state_count; i++)
    {
      if(*q == tm->final_states[i])
      {
        return END_STATE;
      }
    }
  }

  return found ? TRANSITION_FOUND : TRANSITION_NOT_FOUND;
}

ret_t 
run_all(const turing_machine *tm, unsigned long *pos, unsigned long *q)
{
  ret_t ret_code;
	while((ret_code = run_step(tm, pos, q)) != TRANSITION_NOT_FOUND && ret_code != END_STATE);

	return ret_code;
}

ret_t 
read_tape(turing_machine *tm)
{
  tape *tape = tm->tape;

	char c;
	unsigned long count = 0;
	while((c = getchar()) != EOF && c != '\n')
	{
		tape->elements[count++] = c;
		
		if(count > tape->size)
		{
			tape->size++;

			if((tape->elements = realloc(tape->elements, tape->size+1)) == NULL)
			{
					return ALLOC_ERROR;
			}
		}
	}

	return OK;
}

void 
print_tape(turing_machine *tm, unsigned long pos)
{
  printf("\n[");
  unsigned long c;
  for( c = 0; c < tm->tape->size; c++)
  {
	  if(c == pos)
	  {
			if(tm->tape->elements[c] != tm->symbols[2])
	    	printf(" [%c] ", tm->tape->elements[c]);
			else
				printf(" [ ] ");
	  }
	  else
	  {
			if(tm->tape->elements[c] != tm->symbols[2])
	    	printf("  %c  ", tm->tape->elements[c]);
			else
				printf("     ");
	  }
  }
  printf("]\n");
}

int has_final_states(turing_machine *tm)
{
	return tm->final_state_count ? 1 : 0;
}

/* PRIVATE */
ret_t 
parse_tm_file(const char *filename, turing_machine *tm)
{
		FILE *f;
		if((f = fopen(filename, "r")) == NULL)
		{
		  return ALLOC_ERROR;
		}
    if(parse_file(f, tm) < 0)
    {
  		fclose(f);
      return ALLOC_ERROR;
    }
		fclose(f);

		return OK;
}

ret_t 
alloc_and_init(turing_machine **tm)
{
  if((*tm = malloc(sizeof(turing_machine))) == NULL) /* TM alloc */
  {
    return ALLOC_ERROR;
  }

	(*tm)->states = NULL;
	(*tm)->tape = NULL;
  (*tm)->symbols = NULL;
  (*tm)->final_states = NULL;

	if(((*tm)->states = malloc(sizeof(state)*INITIAL_STATES)) == NULL) /* State alloc */
  {
    return ALLOC_ERROR;
  }
  unsigned long i;
  for(i=0; i<INITIAL_STATES; i++) /* Set internal ptrs to null */
  {
    (*tm)->states[i].transitions = NULL;
    (*tm)->states[i].next_state_to_add = 0;
  }

	if(init_tape(&((*tm)->tape)) < 0)
  {
    return ALLOC_ERROR;
  }

	if(((*tm)->symbols = malloc(sizeof(char)*INITIAL_SYMBOL_NUM)) == NULL) /* Symbols alloc */
  {
    return ALLOC_ERROR;
  }

	(*tm)->final_states = NULL; /* The space will be alloc'd if is needed */
  (*tm)->final_state_count = 0;
	(*tm)->state_num = 0;
	(*tm)->max_states = INITIAL_STATES;
  (*tm)->symbol_num = 0;

  return OK;
}

ret_t 
manage_state_memory(turing_machine *tm, const unsigned long from, const unsigned long to)
{
  unsigned long max_arg = from > to ? from : to;

  if(tm->state_num > tm->max_states || max_arg >= tm->max_states)
  {
    while((tm->max_states += INITIAL_STATES) <= max_arg);

		state *tmp_state_ptr = NULL;

    if((tmp_state_ptr = malloc(sizeof(state)*(tm->max_states))) == NULL)
    {
      return ALLOC_ERROR;
    }
    
    unsigned long i;
    for(i = 0; i < tm->max_states; i++) /* Set all new pointers to NULL */
    {
      tmp_state_ptr[i].transitions = NULL;
      tmp_state_ptr[i].next_state_to_add = 0;
    }
    for(i = 0; i < tm->state_num; i++)  /* Copy contents */
    {
      tmp_state_ptr[i].next_state_to_add = tm->states[i].next_state_to_add;

      if(tm->states[i].transitions != NULL) /* Don't alloc if we have no data */
      {
        tmp_state_ptr[i].transitions = malloc(sizeof(transition)*((tm->states[i].next_state_to_add)+1));

        unsigned long j;
        for(j = 0; j < tm->states[i].next_state_to_add; j++)
        {
          tmp_state_ptr[i].transitions[j] = tm->states[i].transitions[j];
        }
      }
    }

    free(tm->states->transitions);
    tm->states = tmp_state_ptr;
  }

  return OK;
}

ret_t 
manage_transitions_memory(turing_machine *tm, const unsigned long from)
{
  if(tm->states[from].transitions == NULL) /* Allocate memory for the first transition */
  {
    if((tm->states[from].transitions = malloc(sizeof(transition))) == NULL)
    {
      return ALLOC_ERROR;
    }

    tm->states[from].next_state_to_add = 0;
		tm->state_num++; /* It was never used before so we didn't count it. Now we do */
  }
  else /* Resize the transitions memory to put a new one */
  {
		transition *tmp_trans_ptr = NULL;
    if(( tmp_trans_ptr = malloc( sizeof(transition)*(( tm->states[from].next_state_to_add )+1) )) == NULL || 
        (tm->states[from].transitions = memcpy(tmp_trans_ptr, 
                                                tm->states[from].transitions, 
                                                sizeof(transition)*( tm->states[from].next_state_to_add )
                                              )) == NULL)
    {
      return ALLOC_ERROR;
    }
  }

  return OK;
}
